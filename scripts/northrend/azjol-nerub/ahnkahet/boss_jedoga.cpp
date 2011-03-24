/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Jedoga
SD%Complete: 90%
SDAuthor: Tassadar, modded/fixed by kelthuzad
SDComment: Correct Timers
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SAY_AGGRO                           = -1619017,
    SAY_CALL_SACRIFICE_1                = -1619018,
    SAY_CALL_SACRIFICE_2                = -1619019,
    SAY_SACRIFICE_1                     = -1619020,
    SAY_SACRIFICE_2                     = -1619021,
    SAY_SLAY_1                          = -1619022,
    SAY_SLAY_2                          = -1619023,
    SAY_SLAY_3                          = -1619024,
    SAY_DEATH                           = -1619025,
    SAY_PREACHING_1                     = -1619026,
    SAY_PREACHING_2                     = -1619027,
    SAY_PREACHING_3                     = -1619028,
    SAY_PREACHING_4                     = -1619029,
    SAY_PREACHING_5                     = -1619030,

    SAY_VOLUNTEER_1                     = -1619031,         //said by the volunteer image
    SAY_VOLUNTEER_2                     = -1619032,

    NPC_VOLUNTEER						= 30385,
    NPC_TWILIGHT_INITIATE               = 30114,
    NPC_VISUAL_TRIGGER					= 38667,

    FAC_FRIENDLY						= 35,
    FAC_HOSTILE							= 16, 

    SPELL_SPHERE_VISUAL					= 56075,
    SPELL_SACRIFICE_VISUAL				= 56133,
    SPELL_DARK_BEAM						= 46016,
    SPELL_GIFT_OF_THE_HERALD			= 56219,
    
    SPELL_LIGHTING_BALL					= 56891,
    SPELL_LIGHTING_BALL_H				= 60032,

    SPELL_THUNDERSHOCK					= 56926,
    SPELL_THUNDERSHOCK_H				= 60029,

    SPELL_CYCLONE_STRIKE				= 56855,
    SPELL_CYCLONE_STRIKE_H				= 60030,

    ACHIEVEMENT_VOLUNTEER_WORK          = 2056
};

const float volunteerPos[7][4] =
{
    {362.002197f, -729.438904f, -16.179300f, 1.125480f},
    {372.852570f, -730.883850f, -16.179300f, 1.688610f},
    {379.525360f, -726.276672f, -16.179300f, 2.160635f},
    {385.052338f, -718.627014f, -16.179300f, 2.408821f},
    {391.472870f, -710.442200f, -16.082842f, 3.012007f},
    {394.789246f, -701.645203f, -16.179674f, 3.290038f},
    {393.005707f, -694.984816f, -16.179674f, 3.748711f}
};

#define MAX_VOLUNTEER	7

#define CORD_CENTER_X	372.330994f
#define CORD_CENTER_Y	-705.278015f
#define CORD_CENTER_Z	-16.179701f

#define CORD_ABOVE_Z	-0.624178f

#define START_X         372.33f
#define START_Y         -705.28f
#define START_Z         -8.904f
#define START_O			5.427970f

/*######
## boss_jedoga
######*/

struct MANGOS_DLL_DECL boss_jedogaAI : public ScriptedAI
{
    boss_jedogaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool volunteerPhase;
    bool getsAchievement;

    std::list<uint64> volunteerGUIDList;

    Creature* pChosenVolunteer;
    Creature* pVisualTrigger;

    uint32 volunteerDeathTimer;
    uint32 volunteerPhaseTimer;
    uint32 volunteerReachedTimer;
    uint32 lightingBallTimer;
    uint32 thundershockTimer;
    uint32 cycloneStrikeTimer;

    uint8 victimCounter;

    void Reset()
    {
        DepawnVolunteers();
        victimCounter = 0;
        volunteerPhase = false;
        volunteerDeathTimer = 9999999;
        volunteerPhaseTimer = 20000;
        volunteerReachedTimer = 9999999;
        lightingBallTimer = 4000;
        thundershockTimer = 6000;
        cycloneStrikeTimer = 8000;
        getsAchievement = true;

        volunteerGUIDList.clear();

        if(m_pInstance)
            m_pInstance->SetData(TYPE_JEDOGA,NOT_STARTED);

        m_creature->NearTeleportTo(START_X,START_Y,START_Z,START_O);
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->CastSpell(m_creature,SPELL_SPHERE_VISUAL,true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PASSIVE);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        SpawnVolunteers();
        if(m_pInstance)
            m_pInstance->SetData(TYPE_JEDOGA,IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_JEDOGA,DONE);
        DoScriptText(SAY_DEATH, m_creature);
        DepawnVolunteers();

        if(!m_bIsRegularMode && getsAchievement)
        {
            Map* pMap = m_creature->GetMap();
            if (pMap && pMap->IsDungeon())
            {
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    itr->getSource()->CompletedAchievement(ACHIEVEMENT_VOLUNTEER_WORK);
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
                
    }

    void SpawnVolunteers()
    {
        for (int i = 0; i < MAX_VOLUNTEER; i++)
        {
            if (Creature* pVolunteer = m_creature->SummonCreature(NPC_VOLUNTEER,
                volunteerPos[i][0], volunteerPos[i][1], volunteerPos[i][2],volunteerPos[i][3], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000))
            {
                pVolunteer->setFaction(FAC_FRIENDLY);
                pVolunteer->DeleteThreatList();
                pVolunteer->CastSpell(pVolunteer, SPELL_SPHERE_VISUAL, true);
                pVolunteer->HandleEmoteCommand(EMOTE_STATE_KNEEL);
                volunteerGUIDList.push_back(pVolunteer->GetGUID());
            } 
        }
    }

    void DepawnVolunteers()
    {
        if (!volunteerGUIDList.empty() && m_pInstance)
        {
            for(std::list<uint64>::iterator itr = volunteerGUIDList.begin(); itr != volunteerGUIDList.end(); ++itr)
            {
                if (Creature* pVolunteer = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pVolunteer->isAlive())
                        pVolunteer->ForcedDespawn();
                }
            }
        }
    }

    void MoveVolunteer()
    {
        if (Creature* pVolunteer = SelectRandomCreatureOfEntryInRange(NPC_VOLUNTEER, 100.0f))
        {
            if (pVolunteer->isAlive())
            {
                pVolunteer->GetMotionMaster()->Clear();
                pVolunteer->GetMotionMaster()->MovePoint(0, CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_Z);
                pVolunteer->setFaction(FAC_HOSTILE);
                pVolunteer->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                pChosenVolunteer = pVolunteer;
                switch(urand(0, 1))
                {
                    case 0: DoScriptText(SAY_CALL_SACRIFICE_1, pVolunteer); break;
                    case 1: DoScriptText(SAY_CALL_SACRIFICE_2, pVolunteer); break;
                }

                if (pVisualTrigger = m_creature->SummonCreature(NPC_VISUAL_TRIGGER, CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_X, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
                {
                    pVisualTrigger->GetMotionMaster()->Clear();
                    pVisualTrigger->GetMotionMaster()->MoveIdle();
                    pVisualTrigger->SetVisibility(VISIBILITY_ON);
                    pVisualTrigger->CastSpell(pVisualTrigger, SPELL_SACRIFICE_VISUAL, true);
                }
            }
            else
            {
                MoveVolunteer();
                if (pVisualTrigger)
                    pVisualTrigger->ForcedDespawn();
            }
        }
    }

    Creature* SelectRandomCreatureOfEntryInRange(uint32 uiEntry, float fRange)
    {
        std::list<Creature* > lCreatureList;
        GetCreatureListWithEntryInGrid(lCreatureList, m_creature, uiEntry, fRange);

        if (lCreatureList.empty())
            return NULL;

        std::list<Creature* >::iterator iter = lCreatureList.begin();
        advance(iter, urand(0, lCreatureList.size()-1));

        return *iter;
    }

    void MovementInform(uint32 mtype, uint32 id)
    {
        if (m_creature->GetPositionZ() > CORD_CENTER_Z + 10.0f)
        {
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->CastSpell(m_creature, SPELL_SPHERE_VISUAL, true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
            
    }

    bool allStartMobsDead()
    {
        std::list<Creature* > lCreatureList;
        GetCreatureListWithEntryInGrid(lCreatureList, m_creature, NPC_TWILIGHT_INITIATE, 40.);

        if (!lCreatureList.empty())
            for(std::list<Creature*>::iterator itr = lCreatureList.begin(); itr != lCreatureList.end(); ++itr)
                if ((*itr)->isAlive())
                    return false;

        return true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_JEDOGA) == NOT_STARTED)
            if(allStartMobsDead())
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PASSIVE);
                m_creature->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                m_creature->SetInCombatWithZone();
            }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (volunteerPhase)
        {
            if (volunteerReachedTimer < uiDiff)
            {
                if (pChosenVolunteer && pChosenVolunteer->isAlive())
                {
                    m_creature->CastSpell(m_creature, SPELL_GIFT_OF_THE_HERALD, true);
                    m_creature->CastSpell(pChosenVolunteer, SPELL_DARK_BEAM, true);
                    pChosenVolunteer->GetMotionMaster()->MoveIdle();
                    pChosenVolunteer->RemoveAllAuras();
                    pChosenVolunteer->setFaction(FAC_FRIENDLY);
                    pChosenVolunteer->SetHealth(m_creature->GetMaxHealth());
                    switch(urand(0, 1))
                    {
                        case 0: DoScriptText(SAY_SACRIFICE_1, pChosenVolunteer); break;
                        case 1: DoScriptText(SAY_SACRIFICE_2, pChosenVolunteer); break;
                    }
                }
                volunteerReachedTimer = 9999999;
            }else volunteerReachedTimer -= uiDiff;

            if (volunteerDeathTimer < uiDiff)
            {
                if (pChosenVolunteer)
                    volunteerGUIDList.remove(pChosenVolunteer->GetGUID());

                if (pChosenVolunteer && pChosenVolunteer->isAlive())
                    pChosenVolunteer->DealDamage(pChosenVolunteer, pChosenVolunteer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                if (volunteerGUIDList.empty() || victimCounter >= 6)
                {
                    DepawnVolunteers();
                    SpawnVolunteers();
                    victimCounter = 0;
                }

                m_creature->NearTeleportTo(CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_Z, START_O);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->GetMotionMaster()->MoveChase(pTarget);
                    m_creature->Attack(pTarget, true);
                }

                if (pVisualTrigger)
                    pVisualTrigger->ForcedDespawn();
                
                victimCounter++;
                volunteerPhase = false;
                volunteerDeathTimer = 9999999;
            }else volunteerDeathTimer -= uiDiff;
        }
        else
        {
            if (volunteerPhaseTimer	< uiDiff)
            {
                MoveVolunteer();
                m_creature->GetMotionMaster()->Clear();
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->NearTeleportTo(CORD_CENTER_X, CORD_CENTER_Y, CORD_ABOVE_Z, START_O);
                volunteerPhase = true;
                volunteerPhaseTimer = 32000;
                volunteerDeathTimer = 16000;
                volunteerReachedTimer = 14500;
            }else volunteerPhaseTimer -= uiDiff;

            if (lightingBallTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTING_BALL : SPELL_LIGHTING_BALL_H); 
                lightingBallTimer = urand(4000, 6000);
            }else lightingBallTimer -= uiDiff;
        
            if (thundershockTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_THUNDERSHOCK : SPELL_THUNDERSHOCK_H); 
                thundershockTimer = urand(12000, 18000);
            }else thundershockTimer -= uiDiff;

            if (cycloneStrikeTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CYCLONE_STRIKE : SPELL_CYCLONE_STRIKE_H); 
                cycloneStrikeTimer = urand(14000, 20000);
            }else cycloneStrikeTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jedoga(Creature* pCreature)
{
    return new boss_jedogaAI(pCreature);
}

/*######
## npc_twilight_volunteer
######*/

struct MANGOS_DLL_DECL npc_twilight_volunteerAI : public ScriptedAI
{
    npc_twilight_volunteerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *m_pInstance;                          // the instance

    void Reset() 
    { 
        SetCombatMovement(false);
    }

    void MovementInform(uint32 mtype, uint32 id)
    {

    }

    void JustDied(Unit* pKiller)
    {
        if((pKiller->GetTypeId() == TYPEID_PLAYER) || (pKiller->GetOwner()) && (pKiller->GetOwner()->GetTypeId() == TYPEID_PLAYER))
            if (m_pInstance)
                if (Creature* pJedoga = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_JEDOGA_SHADOWSEEKER)))
                    if (boss_jedogaAI* pJedogaAI = dynamic_cast<boss_jedogaAI*>(pJedoga->AI()))
                    {
                        pJedogaAI->getsAchievement = false;
                    }
    }

    void UpdateAI(const uint32 uiDiff) 
    { 

    }
};

CreatureAI* GetAI_npc_twilight_volunteer(Creature* pCreature)
{
    return new npc_twilight_volunteerAI(pCreature);
}

void AddSC_boss_jedoga()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_jedoga";
    newscript->GetAI = &GetAI_boss_jedoga;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_twilight_volunteer";
    newscript->GetAI = &GetAI_npc_twilight_volunteer;
    newscript->RegisterSelf();
}
