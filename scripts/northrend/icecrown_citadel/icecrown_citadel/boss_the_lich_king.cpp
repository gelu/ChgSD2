/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_the_lich_king
SD%Complete: 1%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
    SPELL_INFEST                     = 70541,
    SPELL_NECROTIC_PLAGUE            = 70337,
    SPELL_PLAGUE_SIPHON              = 74074,
    SPELL_SOUL_REAPER                = 69409,
    SPELL_DEFILE                     = 72754,
    SPELL_HARVEST_SOUL               = 68980,
    SPELL_HARVEST_SOUL_TELEPORT      = 71372,
//
    SPELL_CHANNEL_KING               = 71769,
    SPELL_BROKEN_FROSTMOURNE         = 72398,
    SPELL_BOOM_VISUAL                = 72726,
    SPELL_ICEBLOCK_TRIGGER           = 71614,
    SPELL_TIRION_LIGHT               = 71797,
    SPELL_FROSTMOURNE_TRIGGER        = 72405,
    SPELL_SUMMON_BROKEN_FROSTMOURNE  = 72406,
    SPELL_DISENGAGE                  = 61508,
    SPELL_FURY_OF_FROSTMOURNE        = 70063,
    SPELL_REVIVE_VISUAL              = 37755, //Override?
    SPELL_REVIVE                     = 51918,
    SPELL_CLONE_PLAYER               = 57507,
    SPELL_BERSERK                    = 47008,

//Transition phase
    SPELL_REMORSELESS_WINTER         = 68981,
    SPELL_PAIN_AND_SUFFERING         = 72133,
    SPELL_QUAKE                      = 72262,

//Raging spirit
    SPELL_SUMMON_RAGING_SPIRIT       = 69200,
    SPELL_SOUL_SHRIEK                = 69242,

//Ice sphere
    SPELL_SUMMON_ICE_SPHERE          = 69103,
    SPELL_ICE_PULSE                  = 69099,
    SPELL_ICE_BURST                  = 69108,
    SPELL_ICE_SPHERE_VISUAL          = 69090,

//Drudge ghouls
    SPELL_SUMMON_DRUDGE_GHOULS       = 70358,

//Shambling horror
    SPELL_SUMMON_SHAMBLING_HORROR    = 70372,
    SPELL_SHOCKWAVE                  = 72149,
    SPELL_HORROR_ENRAGE              = 72143,

//Vile spirits
    SPELL_SUMMON_VILE_SPIRITS        = 70498,
    SPELL_SPIRITS_BURST              = 70503,

//Valkyr
    SPELL_SUMMON_VALKYR              = 69037,
    NPC_VALKYR                       = 36609,
    SPELL_WINGS_OF_THE_DAMNED        = 74352,

//
    NPC_FROSTMOURNE_TRIGGER          = 38584,
    NPC_ICE_SPHERE                   = 36633,
    NPC_DEFILER                      = 38757,
    NPC_RAGING_SPIRIT                = 36701,

};

enum Yells
{
    SAY_INTRO_1_KING                =       -1631501,
    SAY_INTRO_2_TIRION              =       -1631552,
    SAY_INTRO_3_KING                =       -1631503,
    SAY_INTRO_4_TIRION              =       -1631554,
    SAY_INTRO_5_KING                =       -1631505,
    SAY_AGGRO_KING                  =       -1631506,
    SAY_REMORSELESS_WINTER          =       -1631507,
    SAY_RANDOM_1                    =       -1631508,
    SAY_RANDOM_2                    =       -1631509,
    SAY_KILL_1                      =       -1631510,
    SAY_KILL_2                      =       -1631511,
    SAY_BERSERK                     =       -1631512,
    SAY_ENDING_1_KING               =       -1631513,
    SAY_ENDING_2_KING               =       -1631514,
    SAY_ENDING_3_KING               =       -1631515,
    SAY_ENDING_4_KING               =       -1631516,
    SAY_ENDING_5_TIRION             =       -1631517,
    SAY_ENDING_6_KING               =       -1631518,
    SAY_ENDING_7_KING               =       -1631519,
    SAY_ENDING_8_TIRION             =       -1631520,
    SAY_ENDING_9_FATHER             =       -1631521,
    SAY_ENDING_10_TIRION            =       -1631522,
    SAY_ENDING_11_FATHER            =       -1631523,
    SAY_ENDING_12_KING              =       -1631524,
    SAY_DEATH_KING                  =       -1631525,
    SAY_ESCAPE_FROSTMOURNE          =       -1631526,
    SAY_HARVEST_SOUL                =       -1631527,
    SAY_DEVOURED_FROSTMOURNE        =       -1631528,
    SAY_SUMMON_VALKYR               =       -1631529,
    SAY_BROKEN_ARENA                =       -1631530,
    SAY_10_PROZENT                  =       -1631531,
};

static Locations SpawnLoc[]=
{
    {459.93689f, -2124.638184f, 1040.860107f},    // 0 Lich King Intro
    {503.15652f, -2124.516602f, 1040.860107f},      // 1 Lich king move end
    {491.27118f, -2124.638184f, 1040.860107f},    // 2 Tirion 1
    {481.69797f, -2124.638184f, 1040.860107f},    // 3 Tirion 2
    {498.00448f, 2201.573486f, 1046.093872f},     // 4 Valkyrs?
    {959.996f, 212.576f, 193.843f},
    {932.537f, 231.813f, 193.838f},
    {958.675f, 254.767f, 193.822f},
    {946.955f, 201.316f, 192.535f},
    {944.294f, 149.676f, 197.551f},
    {930.548f, 284.888f, 193.367f},
    {965.997f, 278.398f, 195.777f},
};

struct MANGOS_DLL_DECL boss_the_lich_king_iccAI : public ScriptedAI
{
    boss_the_lich_king_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    uint32 nextEvent;
    uint32 nextPoint;
    uint32 UpdateTimer;
    bool movementstarted;
    bool battlestarted;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LICH_KING, NOT_STARTED);
        bsw->resetTimers();
        stage = 0;
        nextEvent = 0;
        nextPoint = 0;
        movementstarted = false;
        battlestarted = false;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !movementstarted) return;
        if (id == nextPoint) 
        {
            movementstarted = false;
            pInstance->SetData(TYPE_EVENT,nextEvent);
            m_creature->GetMotionMaster()->MovementExpired();
        }
    }

    void KilledUnit(Unit* pVictim)
    {
/*    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };*/
    }

    void JustReachedHome()
    {
        if (!pInstance) return;
        pInstance->SetData(TYPE_LICH_KING, FAIL);
    }

    void StartMovement(uint32 id, uint32 _nextEvent)
    {
        nextPoint = id;
        nextEvent = _nextEvent;
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
        pInstance->SetData(TYPE_EVENT,0);
        movementstarted = true;
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_LICH_KING, DONE);
    }

    void UpdateAI(const uint32 diff)
    {

        if (pInstance->GetData(TYPE_EVENT_NPC) == NPC_LICH_KING)
        {
            UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
            if (UpdateTimer <= diff)
            {
            debug_log("EventMGR: creature %u received signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
            switch (pInstance->GetData(TYPE_EVENT))
                {
                case 12000:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                          m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                          StartMovement(0,12020);
                          break;
                case 12020:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                          DoScriptText(SAY_INTRO_1_KING, m_creature);
                          UpdateTimer = 12000;
                          pInstance->SetData(TYPE_EVENT,12030);
                          break;
                case 12040:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                          DoScriptText(SAY_INTRO_3_KING, m_creature);
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12041);
                          break;
                case 12041:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_LAUGH);
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12042);
                          break;
                case 12042:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_ONESHOT_POINT_NOSHEATHE);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,12043);
                          break;
                case 12043:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_ONESHOT_NONE);
                          UpdateTimer = 10000;
                          pInstance->SetData(TYPE_EVENT,12050);
                          break;
                case 12060:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                          DoScriptText(SAY_INTRO_5_KING, m_creature);
                          UpdateTimer = 10000;
                          pInstance->SetData(TYPE_EVENT,12080);
                          break;
                case 12080:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_STATE_READY2H);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,12100);
                          break;
                case 12100:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE,EMOTE_ONESHOT_NONE);
                          UpdateTimer = 6000;
                          pInstance->SetData(TYPE_EVENT,12120);
                          break;
                case 12120:
                          m_creature->SetInCombatWithZone();
                          battlestarted = true;
                          pInstance->SetData(TYPE_EVENT,12999);
                          UpdateTimer = 6000;
                          break;
                case 13000:
                          break;
                default:
                          break;
                }
             } else UpdateTimer -= diff;
             pInstance->SetData(TYPE_EVENT_TIMER, UpdateTimer);
        }

        if (battlestarted && !m_creature->SelectHostileTarget())
        {
            battlestarted = false;
            pInstance->SetData(TYPE_LICH_KING, FAIL);
            EnterEvadeMode();
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_SHOCKWAVE, diff);

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_the_lich_king_icc(Creature* pCreature)
{
    return new boss_the_lich_king_iccAI(pCreature);
};

struct MANGOS_DLL_DECL boss_tirion_iccAI : public ScriptedAI
{
    boss_tirion_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 UpdateTimer;
    uint32 nextEvent;
    uint32 nextPoint;
    bool movementstarted;

    void Reset()
    {
        if(!pInstance) return;
        movementstarted = false;
        m_creature->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
    }

    void StartMovement(uint32 id, uint32 _nextEvent)
    {
        nextPoint = id;
        nextEvent = _nextEvent;
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
        pInstance->SetData(TYPE_EVENT,0);
        movementstarted = true;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !movementstarted) return;
        if (id == nextPoint) 
        {
            movementstarted = false;
            pInstance->SetData(TYPE_EVENT,nextEvent);
            m_creature->GetMotionMaster()->MovementExpired();
        }
    }

    void UpdateAI(const uint32 diff)
    {

        if (pInstance->GetData(TYPE_LICH_KING) == FAIL && m_creature->HasAura(SPELL_ICEBLOCK_TRIGGER)) 
        {
            m_creature->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
            m_creature->GetMotionMaster()->MoveTargetedHome();
            Reset();
        }

        if (pInstance->GetData(TYPE_EVENT_NPC) == NPC_TIRION)
        {
            UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
            if (UpdateTimer <= diff)
            {
            debug_log("EventMGR: creature %u received signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
            switch (pInstance->GetData(TYPE_EVENT))
                {
                case 12030:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                          DoScriptText(SAY_INTRO_2_TIRION, m_creature);
                          UpdateTimer = 9000;
                          pInstance->SetData(TYPE_EVENT,12040);
                          break;
                case 12050:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                          DoScriptText(SAY_INTRO_4_TIRION, m_creature);
                          UpdateTimer = 5000;
                          pInstance->SetData(TYPE_EVENT,12051);
                          break;
                case 12051:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12052);
                          break;
                case 12052:
                          StartMovement(3,12053);
                          break;
                case 12053:
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12060);
                          m_creature->CastSpell(m_creature, SPELL_ICEBLOCK_TRIGGER, false);
                          break;
                case 13010:
                          break;
                default:
                          break;
                }
             } else UpdateTimer -= diff;
             pInstance->SetData(TYPE_EVENT_TIMER, UpdateTimer);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

//        DoMeleeAttackIfReady();
    }


};

bool GossipHello_boss_tirion_icc(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance;
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (pInstance->GetData(TYPE_LICH_KING) != NOT_STARTED) return false;

    char const* _message;

    switch (LocaleConstant currentlocale = pPlayer->GetSession()->GetSessionDbcLocale())
    {
     case LOCALE_enUS:
     case LOCALE_koKR:
     case LOCALE_frFR:
     case LOCALE_deDE:
     case LOCALE_zhCN:
     case LOCALE_zhTW:
     case LOCALE_esES:
     case LOCALE_esMX:
                      _message = "We are ready, Tirion!";
                      break;
     case LOCALE_ruRU:
                      _message = "Всегда готовы, дедуля!";
                      break;
     default:
                      _message = "We are ready, Tirion!";
                      break;
    };

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _message, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    pPlayer->PlayerTalkClass->SendGossipMenu(907, pCreature->GetGUID());
    return true;
};

bool GossipSelect_boss_tirion_icc(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* pInstance;
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    if (!pInstance) return false;

    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
        pInstance->SetData(TYPE_EVENT,12000);
        return true;
    } else return false;

};

CreatureAI* GetAI_boss_tirion_icc(Creature* pCreature)
{
    return new boss_tirion_iccAI(pCreature);
};

struct MANGOS_DLL_DECL  mob_ice_sphere_iccAI : public ScriptedAI
{
    mob_ice_sphere_iccAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;

    void Reset()
    {
       bsw->doCast(SPELL_ICE_SPHERE_VISUAL);
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) 
         return;

        bsw->timedCast(SPELL_ICE_PULSE, uiDiff);

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 3.0f))
            bsw->timedCast(SPELL_ICE_BURST,uiDiff);
    }
};

CreatureAI* GetAI_mob_ice_sphere_icc(Creature* pCreature)
{
    return new mob_ice_sphere_iccAI(pCreature);
};

void AddSC_boss_lich_king_icc()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_the_lich_king_icc";
    newscript->GetAI = &GetAI_boss_the_lich_king_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_tirion_icc";
    newscript->GetAI = &GetAI_boss_tirion_icc;
    newscript->pGossipHello = &GossipHello_boss_tirion_icc;
    newscript->pGossipSelect = &GossipSelect_boss_tirion_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ice_sphere_icc";
    newscript->GetAI = &GetAI_mob_ice_sphere_icc;
    newscript->RegisterSelf();

};
