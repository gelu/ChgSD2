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
SD%Complete: 5%
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
    SPELL_SPAWN_DEFILE               = 72762,
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
    SPELL_SUMMON_BROKEN_FROSTMOURNE_2 = 73017,
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

//Defile
    SPELL_DEFILE                     = 72743,

//
    NPC_FROSTMOURNE_TRIGGER          = 38584,
    NPC_ICE_SPHERE                   = 36633,
    NPC_DEFILER                      = 38757,
    NPC_RAGING_SPIRIT                = 36701,
    NPC_VILE_SPIRIT                  = 37799,
    NPC_STRANGULATE_VEHICLE          = 36598,

};

enum Common
{
     FINAL_BATTLE_MOVIE             = 14,
};

static Locations SpawnLoc[]=
{
    {459.93689f, -2124.638184f, 1040.860107f},    // 0 Lich King Intro
    {503.15652f, -2124.516602f, 1040.860107f},    // 1 Lich king move end
    {491.27118f, -2124.638184f, 1040.860107f},    // 2 Tirion 1
    {481.69797f, -2124.638184f, 1040.860107f},    // 3 Tirion 2
    {498.00448f, -2201.573486f, 1046.093872f},    // 4 Valkyrs?
    {517.48291f, -2124.905762f, 1040.861328f},    // 5 Tirion?
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
    Creature* pTirion;
    Creature* pFrostmourne;

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
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631519,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631517,m_creature,pVictim);
               break;
        };
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
        m_creature->GetMotionMaster()->Clear();
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
                          DoScriptText(-1631501, m_creature);
                          UpdateTimer = 12000;
                          pInstance->SetData(TYPE_EVENT,12030);
                          break;
                case 12040:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                          DoScriptText(-1631503, m_creature);
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
                          DoScriptText(-1631505, m_creature);
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
                          pInstance->SetData(TYPE_EVENT,12200);
                          UpdateTimer = 10000;
                          break;
                case 12200:
                          DoScriptText(-1631506, m_creature);
                          UpdateTimer = 5000;
                          pInstance->SetData(TYPE_EVENT,12220);
                          break;
                case 13000:
                          DoScriptText(-1631507, m_creature);
                          UpdateTimer = 10000;
                          pInstance->SetData(TYPE_EVENT,13020);
                          break;
                case 13020:
                          DoScriptText(-1631508, m_creature);
                          UpdateTimer = 10000;
                          pInstance->SetData(TYPE_EVENT,13060);
                          break;
                case 13060:
                          DoScriptText(-1631509, m_creature);
                          UpdateTimer = 10000;
                          pInstance->SetData(TYPE_EVENT,13100);
                          break;
                case 13100:
                          DoScriptText(-1631510, m_creature);
                          UpdateTimer = 8000;
                          pInstance->SetData(TYPE_EVENT,13110);
                          bsw->doCast(SPELL_CHANNEL_KING);
                          break;
                case 13120:
                          DoScriptText(-1631511, m_creature);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13130);
                          break;
                case 13140:
                          UpdateTimer = 2000;
                          bsw->doRemove(SPELL_CHANNEL_KING);
                          pInstance->SetData(TYPE_EVENT,13150);
                          m_creature->CastSpell(m_creature, SPELL_SUMMON_BROKEN_FROSTMOURNE, false);
                          break;
                case 13160:
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13170);
                          m_creature->CastSpell(m_creature, SPELL_SUMMON_BROKEN_FROSTMOURNE_2, false);
                          break;
                case 13180:
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13190);
                          pFrostmourne = m_creature->SummonCreature(NPC_FROSTMOURNE, m_creature->GetPositionX()+2, m_creature->GetPositionY()+2, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
                          pFrostmourne->CastSpell(pFrostmourne, SPELL_BROKEN_FROSTMOURNE, false);
                          pFrostmourne->CastSpell(pFrostmourne, SPELL_FROSTMOURNE_TRIGGER, false);
                          pFrostmourne->GetMotionMaster()->MoveChase(m_creature);
                          m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
                          break;
                case 13200:
                          DoScriptText(-1631512, m_creature);
                          m_creature->RemoveAurasDueToSpell(SPELL_SUMMON_BROKEN_FROSTMOURNE);
                          m_creature->RemoveAllAuras();
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13210);
                          break;
                case 13280:
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,132900);
                          stage = 12;
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

        switch(stage)
        {
            case 0:           // Phase 1
//                    bsw->timedCast(SPELL_SHOCKWAVE, diff);
                    bsw->timedCast(SPELL_INFEST, diff);
                    bsw->timedCast(SPELL_SUMMON_DRUDGE_GHOULS, diff);
                    bsw->timedCast(SPELL_PLAGUE_SIPHON, diff);
                    bsw->timedCast(SPELL_SUMMON_SHAMBLING_HORROR, diff);
                    bsw->timedCast(SPELL_NECROTIC_PLAGUE, diff);

                    DoMeleeAttackIfReady();
                    if (bsw->timedQuery(SPELL_BERSERK, diff))
                    {
                        bsw->doCast(SPELL_BERSERK);
                        DoScriptText(-1631518,m_creature);
                    };

                    if (m_creature->GetHealthPercent() < 70.0f) 
                       {
                            stage = 1;
                            DoScriptText(-1631515,m_creature);
                       }
                    break;
            case 1:             // Go in transition phase
                    m_creature->AttackStop();
                    SetCombatMovement(false);
                    StartMovement(1,0);
                    stage = 2;
                    break;
            case 2:
                    if (movementstarted) return;
                    bsw->doCast(SPELL_REMORSELESS_WINTER);
                    stage = 3;
                    return;
            case 3:
                    bsw->timedCast(SPELL_SUMMON_RAGING_SPIRIT, diff);
                    bsw->timedCast(SPELL_SUMMON_ICE_SPHERE, diff);
                    bsw->timedCast(SPELL_PAIN_AND_SUFFERING, diff);

                    if (bsw->timedQuery(SPELL_BERSERK, diff))
                    {
                        bsw->doCast(SPELL_BERSERK);
                        DoScriptText(-1631518,m_creature);
                    };

                    if (bsw->timedQuery(SPELL_REMORSELESS_WINTER, diff))
                       {
                            bsw->doCast(SPELL_QUAKE);
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(GO_ICESHARD_1));
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(GO_ICESHARD_2));
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(GO_ICESHARD_3));
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(GO_ICESHARD_4));
                            stage = 4;
                            DoScriptText(-1631524, m_creature);
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            SetCombatMovement(true);
                       };

                    break;
            case 4:           // Phase 2

                    if (bsw->timedQuery(SPELL_SPAWN_DEFILE, diff)) 
                       {
                            bsw->doCast(SPELL_SPAWN_DEFILE);
//                            DoScriptText(-1631527,m_creature);
                       }
                    if (bsw->timedQuery(SPELL_SUMMON_VALKYR, diff)) 
                       {
                            bsw->doCast(SPELL_SUMMON_VALKYR);
                            DoScriptText(-1631527,m_creature);
                       }

                    bsw->timedCast(SPELL_SOUL_REAPER, diff);
                    bsw->timedCast(SPELL_INFEST, diff);

                    DoMeleeAttackIfReady();

                    if (bsw->timedQuery(SPELL_BERSERK, diff))
                    {
                        bsw->doCast(SPELL_BERSERK);
                        DoScriptText(-1631518,m_creature);
                    };

                    if (m_creature->GetHealthPercent() < 40.0f) 
                       {
                            stage = 5;
                            DoScriptText(-1631523,m_creature);
                       }
                    break;
            case 5:           // Go in transition phase
                    m_creature->AttackStop();
                    SetCombatMovement(false);
                    StartMovement(1,0);
                    stage = 6;
                    break;
            case 6:
                    if (movementstarted) return;
                    bsw->doCast(SPELL_REMORSELESS_WINTER);
                    stage = 7;
                    return;
            case 7:
                    bsw->timedCast(SPELL_SUMMON_RAGING_SPIRIT, diff);
                    bsw->timedCast(SPELL_SUMMON_ICE_SPHERE, diff);
                    bsw->timedCast(SPELL_PAIN_AND_SUFFERING, diff);

                    if (bsw->timedQuery(SPELL_BERSERK, diff))
                    {
                        bsw->doCast(SPELL_BERSERK);
                        DoScriptText(-1631518,m_creature);
                    };

                    if (bsw->timedQuery(SPELL_REMORSELESS_WINTER, diff))
                       {
                            stage = 8;
                            DoScriptText(-1631524, m_creature);
                            m_creature->GetMotionMaster()->Clear();
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            SetCombatMovement(true);

                            bsw->doCast(SPELL_SUMMON_VILE_SPIRITS);
                            for (uint8 i = 0; i < 11; ++i)
                                bsw->doCast(NPC_VILE_SPIRIT);

                            bsw->doCast(SPELL_QUAKE);
                       };

                    break;
            case 8:           // Phase 3
                    if (bsw->timedQuery(SPELL_SPAWN_DEFILE, diff)) 
                       {
                            bsw->doCast(SPELL_SPAWN_DEFILE);
//                            DoScriptText(-1631527,m_creature);
                       }
                    bsw->timedCast(SPELL_SOUL_REAPER, diff);

                    if (bsw->timedQuery(SPELL_HARVEST_SOUL, diff)) 
                       {
                            bsw->doCast(SPELL_HARVEST_SOUL);
                            DoScriptText(-1631520,m_creature);
                       }

                    bsw->timedCast(SPELL_SOUL_REAPER, diff);
                    bsw->timedCast(SPELL_INFEST, diff);

                    DoMeleeAttackIfReady();

                    if (m_creature->GetHealthPercent() < 10.0f) 
                       {
                            stage = 9;
                            DoScriptText(-1631513,m_creature);
                       }
                    break;
            case 9:           // Ending Phase start
                    m_creature->AttackStop();
                    if (pTirion = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_TIRION))) {
                                m_creature->SetInCombatWith(pTirion);
                                pTirion->AddThreat(m_creature, 1000.0f);
                                }
                    bsw->doCast(SPELL_FURY_OF_FROSTMOURNE);
                    SetCombatMovement(false);
                    StartMovement(1,13000);
                    stage = 10;
                    break;
            case 10:
                    break;
            case 11:
                    break;
            case 12:
                    DoMeleeAttackIfReady();
                    break;
        }
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
    Creature* pMenethil;

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

    void doRevivePlayers()
    {
        Map::PlayerList const &PlList = pMenethil->GetMap()->GetPlayers();
        if (PlList.isEmpty()) return;
        for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
            {
               if (Player* pPlayer = i->getSource())
               {
                   if (pPlayer && !pPlayer->isAlive())
                      {
                          pMenethil->CastSpell(pPlayer, 26687, true);
                          pPlayer->ResurrectPlayer(100, false);
                       }
                }
             };
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
                          DoScriptText(-1631552, m_creature);
                          UpdateTimer = 9000;
                          pInstance->SetData(TYPE_EVENT,12040);
                          break;
                case 12050:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                          DoScriptText(-1631554, m_creature);
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12051);
                          break;
                case 12051:
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                          UpdateTimer = 1000;
                          pInstance->SetData(TYPE_EVENT,12052);
                          break;
                case 12052:
                          m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                          StartMovement(3,12053);
                          break;
                case 12053:
                          UpdateTimer = 3000;
                          pInstance->SetData(TYPE_EVENT,12060);
                          m_creature->CastSpell(m_creature, SPELL_ICEBLOCK_TRIGGER, true);
                          break;
                case 13110:
                          DoScriptText(-1631555, m_creature);
                          UpdateTimer = 2000;
                          m_creature->CastSpell(m_creature, SPELL_TIRION_LIGHT, false);
                          pInstance->SetData(TYPE_EVENT,13120);
                          break;
                case 13130:
                          SetCombatMovement(false);
                          m_creature->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13131);
                          break;
                case 13131:
                          m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                          StartMovement(2,13132);
                          break;
                case 13132:
                          StartMovement(5,13140);
                          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                          break;
                case 13150:
                          UpdateTimer = 1000;
                          pInstance->SetData(TYPE_EVENT,13160);
                          break;
                case 13170:
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,13180);
                          break;
                case 13190:
                          UpdateTimer = 500;
                          pInstance->SetData(TYPE_EVENT,13200);
                          break;
                case 13210:
                          UpdateTimer = 6000;
                          DoScriptText(-1631556, m_creature);
                          pInstance->SetData(TYPE_EVENT,13230);
                          break;
                case 13230:
                          UpdateTimer = 6000;
                          pMenethil = m_creature->SummonCreature(NPC_MENETHIL, m_creature->GetPositionX()+5, m_creature->GetPositionY()+5, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
                          pInstance->SetData(TYPE_EVENT,13250);
                          DoScriptText(-1631560, pMenethil);
                          break;
                case 13250:
                          UpdateTimer = 6000;
                          pInstance->SetData(TYPE_EVENT,13270);
                          DoScriptText(-1631561, pMenethil);
                          pMenethil->CastSpell(pMenethil, SPELL_REVIVE_VISUAL, false);
                          doRevivePlayers();
                          break;

                case 13270:
                          UpdateTimer = 6000;
                          pInstance->SetData(TYPE_EVENT,13280);
                          if (Creature* pLichKing = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_LICH_KING)))
                          {
                              m_creature->AI()->AttackStart(pLichKing);
                              pMenethil->AI()->AttackStart(pLichKing);
                          };
//                          DoScriptText(-1631560, m_creature);
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

    if (pInstance->GetData(TYPE_LICH_KING) != NOT_STARTED &&
        pInstance->GetData(TYPE_LICH_KING) != FAIL ) return false;

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
    pPlayer->PlayerTalkClass->SendGossipMenu(721001, pCreature->GetGUID());
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
       if (Unit* pTarget = bsw->SelectRandomPlayerAtRange(120.0f))
           {
               m_creature->SetInCombatWith(pTarget);
               m_creature->AddThreat(pTarget,100.0f);
           }
       bsw->doCast(SPELL_ICE_SPHERE_VISUAL);
       m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) 
            return;

        bsw->timedCast(SPELL_ICE_PULSE, uiDiff);

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 1.0f))
            bsw->timedCast(SPELL_ICE_BURST,uiDiff);
    }
};

CreatureAI* GetAI_mob_ice_sphere_icc(Creature* pCreature)
{
    return new mob_ice_sphere_iccAI(pCreature);
};

struct MANGOS_DLL_DECL mob_defiler_iccAI : public ScriptedAI
{
    mob_defiler_iccAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 life_timer;

    void Reset()
    {
        SetCombatMovement(false);
        life_timer = 30000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->CastSpell(m_creature, SPELL_DEFILE, true);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (life_timer <= uiDiff)
            m_creature->ForcedDespawn();
        else life_timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_defiler_icc(Creature* pCreature)
{
    return new mob_defiler_iccAI(pCreature);
}

struct MANGOS_DLL_DECL mob_strangulate_vehicleAI : public ScriptedAI
{
    mob_strangulate_vehicleAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
       m_creature->ForcedDespawn();
    }

};

CreatureAI* GetAI_mob_strangulate_vehicle(Creature* pCreature)
{
    return new mob_strangulate_vehicleAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_vile_spiritAI : public ScriptedAI
{
    mob_vile_spiritAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    bool movementstarted;

    void Reset()
    {
       if (Unit* pTarget = bsw->SelectRandomPlayerAtRange(120.0f))
           {
               m_creature->SetInCombatWith(pTarget);
               m_creature->AddThreat(pTarget,1000.0f);
           }
       SetCombatMovement(false);
       movementstarted = false;
       m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bsw->timedQuery(SPELL_SPIRITS_BURST, uiDiff) && !movementstarted)
        {
           SetCombatMovement(true);
           m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
           movementstarted = true;
        }

        if (m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER) 
            return;

        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 1.0f))
        {
            bsw->doCast(SPELL_SPIRITS_BURST);
            m_creature->ForcedDespawn();
        };
    }
};

CreatureAI* GetAI_mob_vile_spirit(Creature* pCreature)
{
    return new mob_vile_spiritAI(pCreature);
}

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

    newscript = new Script;
    newscript->Name = "mob_defiler_icc";
    newscript->GetAI = &GetAI_mob_defiler_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_strangulate_vehicle";
    newscript->GetAI = &GetAI_mob_strangulate_vehicle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vile_spirit";
    newscript->GetAI = &GetAI_mob_vile_spirit;
    newscript->RegisterSelf();

};
