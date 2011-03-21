/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Netherspite
SD%Complete: 100%
SDComment: I think it works quite good. Maybe more testing!
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"
#include "GameObject.h"
#include "../../../game/TemporarySummon.h"

#define SPELL_NETHERBURN            30522
#define SPELL_VOID_ZONE             37063
#define SPELL_NETHERBREATH          38523
#define SPELL_EMPOWERMENT           38549
#define SPELL_NETHER_INFUSION       38688
//#define SPELL_NETHERSPITE_ROAR    38684

#define SPELL_BANISH_VISUAL         39833

//emotes
#define EMOTE_PHASE_PORTAL          -1532089
#define EMOTE_PHASE_BANISH          -1532090

enum Portals
{
    PORTAL_SERENITY = 0,
    PORTAL_DOMINANCE = 1,
    PORTAL_PERSEVERANCE = 2
};
#define MAX_PORTAL 3

static uint32 portalId[MAX_PORTAL]      = {17367, 17368, 17369};    // id's in creature_template

static uint32 portalVisual[MAX_PORTAL]  = {30490, 30491, 30487};    // spells for visual portal effects
static uint32 beamVisual[MAX_PORTAL]    = {30464, 30463, 30465};    // spell for beam visualization

static uint32 Beam_Debuff[MAX_PORTAL]   = {38638, 38639, 38637};    // Exsaustion debuffs
static uint32 beamBuff[MAX_PORTAL][2]   = {{30467, 30422},          // Buffs {Netherspite, Player}
                                           {30468, 30423},
                                           {30466, 30421}};

const float PortalSpawnCoords[3][4] =
{-11106.6f, -1601.8f, 280.0f, 3.96f,                // 1st spawn point
 -11195.4f, -1617.7f, 278.5f, 6.18f,                // 2nd spawn point
 -11142.4f, -1684.3f, 278.5f, 1.58f                 // 3rd spwan point
};

struct portalInfo
{
    uint64 GUID;
    bool active;
    uint64 targetGUID;
};


enum Phases
{
    NULL_PHASE = 0,
    BEAM_PHASE = 1,
    BANISH_PHASE = 2,
};

enum Events
{
    // events in all phases
    EVENT_SWITCH_PHASE          = 1,
    EVENT_ENRAGE                = 2,

    //portal phase events
    EVENT_EMPOWERMENT           = 3,
    EVENT_PORTALS_SPAWN         = 4,
    EVENT_PORTALS_START_BEAM    = 5,
    EVENT_PORTALS_UPDATE_BEAM   = 6,
    EVENT_VOID_ZONE             = 7,

    //banish phase events
    EVENT_NETHERBREATH          = 8,

    // WORKAROUND:
    EVENT_RECAST_BEAM_1         = 9,
    EVENT_RECAST_BEAM_2         = 10,
    EVENT_RECAST_BEAM_3         = 11
};


class NearestUnitForBeam
{
    public:
        NearestUnitForBeam(WorldObject const* obj,  Unit const* oCaster, float range, Portals beam)
            : i_obj(obj), i_origCaster(oCaster), i_range(range), i_beam(beam) {}
        bool operator()(Unit* u)
        {
            if( u->isTargetableForAttack() && i_origCaster->IsHostileTo(u))
            {
                float arc = M_PI_F/(2+i_obj->GetDistance2d(u)); // open the angle a bit, if target is more close
                if (i_obj->isInFront(u, i_range, arc) && u->isVisibleForOrDetect(i_origCaster, i_obj, false) && !u->HasAura(Beam_Debuff[i_beam]) )
                {
                    i_range = i_obj->GetDistance(u);            // use found unit range as new range limit for next check
                    return true;
                }
            }
            return false;
        }
        WorldObject const& GetFocusObject() const { return *i_obj; }
    private:
        WorldObject const* i_obj;
        Unit const* i_origCaster;
        float i_range;
        Portals i_beam;

        // prevent clone this object
        NearestUnitForBeam(NearestUnitForBeam const&);
};


struct MANGOS_DLL_DECL boss_netherspiteAI : public ScriptedAI
{
    boss_netherspiteAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Phases activePhase;
    typedef std::map<Events, uint32> timerMap;
    timerMap timers;
    portalInfo pPortals[MAX_PORTAL];

    void Reset()
    {
        if (m_pInstance)
        {
            //door opens
            m_pInstance->SetData(TYPE_NETHERSPITE, NOT_STARTED);
            //if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
            //    pDoor->SetGoState(GO_STATE_ACTIVE);
        }

        activePhase = NULL_PHASE;
        timers.clear();
        for (uint8 i=0;i<MAX_PORTAL;i++)
        {
            pPortals[i].GUID = 0;
            pPortals[i].active = false;
            pPortals[i].targetGUID = 0;
        }
        m_creature->RemoveAllAuras();
    }

    void EnterEvadeMode()
    {
        DespawnPortals();
        ScriptedAI::EnterEvadeMode();
    }

    void Aggro(Unit *who)
    {
        if (m_pInstance)
        {
            // set encounter in progres
            m_pInstance->SetData(TYPE_NETHERSPITE, IN_PROGRESS);
            // close door
            //if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
            //    pDoor->SetGoState(GO_STATE_READY);
        }
        NextPhase();
        DoMeleeAttackIfReady();
    }

    void JustDied(Unit* Killer)
    {
        DespawnPortals();
        if (m_pInstance)
        {
            // encounter is done :-)
            m_pInstance->SetData(TYPE_NETHERSPITE, DONE);
            // open door
            // if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
            //    pDoor->SetGoState(GO_STATE_ACTIVE);
        }
    }

    void NextPhase()
    {
        switch(activePhase)
        {
            case NULL_PHASE:
            {
                timers[EVENT_ENRAGE]        = 9*60000;                  // 9min, from start of fight
                // NO break!
            }
            case BANISH_PHASE:
            {
                // BEAM_PHASE
                activePhase = BEAM_PHASE;
                DoResetThreat();
                SetCombatMovement(true);
                DoStartMovement(m_creature->getVictim(), 0);            // we need this, if we switch from BANISH_PHASE
                DoCast(m_creature,SPELL_NETHERBURN, true);              // Netherburn active in this phase
                m_creature->RemoveAurasDueToSpell(SPELL_BANISH_VISUAL);

                // init timers for phase
                timers[EVENT_PORTALS_SPAWN] = 4000;
                timers[EVENT_VOID_ZONE]     = urand(10000,20000);
                timers[EVENT_SWITCH_PHASE]  = 60000;
                timers.erase(EVENT_NETHERBREATH);
                break;
            }
            //enter banish phase
            case BEAM_PHASE:
            {
                // BANISH_PHASE
                activePhase = BANISH_PHASE;
                m_creature->RemoveAurasDueToSpell(SPELL_NETHERBURN);    // no Netherburn in this phase
                m_creature->RemoveAurasDueToSpell(SPELL_EMPOWERMENT);   // no Empowerment
                DoCast(m_creature, SPELL_BANISH_VISUAL, true);
                DoScriptText(EMOTE_PHASE_BANISH,m_creature);
                DespawnPortals();
                DoResetThreat();
                SetCombatMovement(false);
                DoStartNoMovement(m_creature->getVictim());

                // init timers for phase
                timers[EVENT_SWITCH_PHASE] = 30000;
                timers[EVENT_NETHERBREATH] = 8000;
                for (uint8 i = EVENT_EMPOWERMENT; i <= EVENT_VOID_ZONE; i++)
                    timers.erase(Events(i));
                break;
            }
            default:
                break;
        }
    }

    bool handleEvent(Events ev)
    {
        switch(ev)
        {
            case EVENT_SWITCH_PHASE:
                NextPhase();
                return true;
            case EVENT_ENRAGE:
                DoCast(m_creature, SPELL_NETHER_INFUSION);
                timers.erase(EVENT_ENRAGE);
                return false;
            case EVENT_EMPOWERMENT:
                DoCast(m_creature, SPELL_EMPOWERMENT);
                timers.erase(EVENT_EMPOWERMENT);
                return false;
            case EVENT_PORTALS_SPAWN:
                SpawnPortals();
                timers.erase(EVENT_PORTALS_SPAWN);
                // first beam starts after 4s
                timers[EVENT_PORTALS_START_BEAM] = 4000;
                // "...a few seconds after beams come up..."
                timers[EVENT_EMPOWERMENT] = urand(2000,6000);
                return false;
            case EVENT_PORTALS_START_BEAM:
            {
                // start a random beam
                while(1)
                {
                    uint8 beam = urand(0,MAX_PORTAL-1);
                    if (pPortals[beam].active)
                        continue;
                    pPortals[beam].active = true;
                    break;
                }

                // check if all beams are active
                bool allActive = true;
                for (uint8 i=0; i<MAX_PORTAL; i++)
                    if (!pPortals[i].active)
                    {
                        allActive = false;
                        break;
                    }

               // delete timer, if all portals active
               if (!allActive)
                   timers[EVENT_PORTALS_START_BEAM] = urand(500, 2000);
               else
                    timers.erase(EVENT_PORTALS_START_BEAM);

               if (timers.find(EVENT_PORTALS_UPDATE_BEAM) != timers.end())
                   return false;
               // no break here, if no beam update timer present
            }
            case EVENT_PORTALS_UPDATE_BEAM:
            {
                for (uint8 i=0; i<MAX_PORTAL; i++)
                    if (pPortals[i].active)
                        updateBeam(Portals(i));
                timers[EVENT_PORTALS_UPDATE_BEAM] = 1000;
                return false;
            }
            case EVENT_VOID_ZONE:
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),SPELL_VOID_ZONE,true);
                timers[EVENT_VOID_ZONE] = urand(10000,20000);
                return false;
            case EVENT_NETHERBREATH:
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),SPELL_NETHERBREATH);
                timers[EVENT_NETHERBREATH] = 5000;
                return false;
            case EVENT_RECAST_BEAM_1:
            case EVENT_RECAST_BEAM_2:
            case EVENT_RECAST_BEAM_3:
            {
                Unit* portal = m_creature->GetMap()->GetUnit(pPortals[ev-9].GUID);
                if (!portal)
                {
                    // portal lost!?
                    pPortals[ev-9].GUID         = 0;
                    pPortals[ev-9].active       = false;
                    pPortals[ev-9].targetGUID   = 0;
                    return false;
                }

                Unit* target = m_creature->GetMap()->GetUnit(pPortals[ev-9].targetGUID);

                if (!target)
                {
                    pPortals[ev-9].targetGUID   = m_creature->GetGUID();
                    target = m_creature;
                }

                portal->CastSpell(target, beamVisual[ev-9], false);
                timers.erase(ev);
                return false;
            }
            default:
                break;
        }
        return false;
    }

    void SpawnPortals()
    {
        DoScriptText(EMOTE_PHASE_PORTAL,m_creature);

        // permutate spawnpoints
        uint8 permutation[MAX_PORTAL] = {0,0,0};
        for (uint8 i=0;i<(MAX_PORTAL);i++)
        {
            // TODO: better algorithm!?
            while(1)
            {
                uint32 rand = urand(1,MAX_PORTAL);
                bool alreadyIn = false;
                for (uint8 j=0; j<MAX_PORTAL;j++)
                    if (permutation[j] == rand)
                        alreadyIn = true;
                if (!alreadyIn)
                {
                    permutation[i] = rand;
                    break;
                }
            }
        }
        for (uint8 i=0;i<(MAX_PORTAL);i++)
            permutation[i]--;

        // spawn
        for (uint8 i=0;i<MAX_PORTAL;i++)
        {
            Unit* portal = m_creature->SummonCreature(portalId[i], PortalSpawnCoords[permutation[i]][0], PortalSpawnCoords[permutation[i]][1], PortalSpawnCoords[permutation[i]][2], PortalSpawnCoords[permutation[i]][3] ,TEMPSUMMON_MANUAL_DESPAWN , 0);
            if (portal)
                pPortals[i].GUID = portal->GetGUID();
            // we cannot add portal visuals to creature_template_addon, because one spell has no aura defined :-(
            portal->CastSpell(portal, portalVisual[i], true, NULL, NULL, m_creature->GetGUID());
            portal->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);
        }
    }

    void DespawnPortals()
    {
        for (uint8 i=0;i<(MAX_PORTAL);i++)
        {
            Unit* portal = m_creature->GetMap()->GetUnit(pPortals[i].GUID);
            // portals are always TemporarySummons
            if (portal)
                ((TemporarySummon*)portal)->UnSummon();
            pPortals[i].GUID = 0;
            pPortals[i].active = false;
            pPortals[i].targetGUID = 0;
        }
        timers.erase(EVENT_PORTALS_START_BEAM);
        timers.erase(EVENT_PORTALS_UPDATE_BEAM);
    }

    void updateBeam(Portals beam)
    {
        Unit* portal = m_creature->GetMap()->GetUnit(pPortals[beam].GUID);
        if (!portal)
        {
            // portal lost!?
            pPortals[beam].GUID         = 0;
            pPortals[beam].active       = false;
            pPortals[beam].targetGUID   = 0;
            return;
        }
        portal->SetInFront(m_creature);

        Unit* target = NULL;
        // add half objectsize as radius, to include also player standing "in" netherspite
        float radius = portal->GetDistance2d(m_creature) + m_creature->GetObjectBoundingRadius()/2;
        {
            CellPair p(MaNGOS::ComputeCellPair(portal->GetPositionX(), portal->GetPositionY()));
            Cell cell(p);
            cell.SetNoCreate();

            NearestUnitForBeam u_check(portal, m_creature, radius, beam);
            MaNGOS::UnitLastSearcher<NearestUnitForBeam> checker(target, u_check);

            TypeContainerVisitor<MaNGOS::UnitLastSearcher<NearestUnitForBeam>, GridTypeMapContainer > grid_object_checker(checker);
            TypeContainerVisitor<MaNGOS::UnitLastSearcher<NearestUnitForBeam>, WorldTypeMapContainer > world_object_checker(checker);
            cell.Visit(p, grid_object_checker, *portal->GetMap(), *portal, radius);
            cell.Visit(p, world_object_checker, *portal->GetMap(), *portal, radius);
        }

        // take netherspite, if we did not find a target
        if (!target)
            target = m_creature;

        // recast beam visual if tagret changed
        if (target->GetGUID() != pPortals[beam].targetGUID)
        {
            portal->InterruptSpell(CURRENT_CHANNELED_SPELL);
            // WORKAROUND: cast one tick later
            timers[Events(EVENT_RECAST_BEAM_1 + beam)] = 1;
            //portal->CastSpell(target, beamVisual[beam], false);
        }

        // add buff, needs to be casted by target!!
        target->CastSpell(target, beamBuff[beam][(target != m_creature)], true, NULL, NULL, m_creature->GetGUID());

        pPortals[beam].active       = true;
        pPortals[beam].targetGUID   = target->GetGUID();
    }

    void UpdateAI(const uint32 diff)
    {
        // ignore aggrotable etc. if in BEAM_PAHSE and somebody is standing in the perseverance beam
        if (activePhase == BEAM_PHASE && pPortals[PORTAL_PERSEVERANCE].targetGUID && pPortals[PORTAL_PERSEVERANCE].targetGUID != m_creature->GetGUID())
        {
            AttackStart(m_creature->GetMap()->GetUnit(pPortals[PORTAL_PERSEVERANCE].targetGUID));
        }
        else if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        std::list<Events> eventList;
        // caclulate all timers
        for (timerMap::iterator itr = timers.begin(); itr != timers.end(); itr++)
        {
            if ((*itr).second <= diff)
                eventList.push_back((*itr).first);
            else (*itr).second -= diff;
        }
        // execute events
        for (std::list<Events>::iterator itr = eventList.begin(); itr != eventList.end(); itr++)
        {
            bool skipOtherEvents = handleEvent((*itr));
            if (skipOtherEvents)
                break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_netherspite(Creature* pCreature)
{
    return new boss_netherspiteAI(pCreature);
}

void AddSC_boss_netherspite()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_netherspite";
    newscript->GetAI = &GetAI_boss_netherspite;
    newscript->RegisterSelf();
}
