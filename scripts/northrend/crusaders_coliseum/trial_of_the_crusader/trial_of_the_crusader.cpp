/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Trial Of the crusader
SD%Complete: 10%
SDComment: event script by /dev/rsa
SDCategory: trial_of_the_crusader
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

struct _Messages
{
    char const* name;
    uint32 id;
    bool state;
    uint32 encounter;
};

static _Messages _GossipMessage[]=
{
{"Вы готовы драться с бестиями Нортренда?",GOSSIP_ACTION_INFO_DEF+1,false,TYPE_BEASTS}, //
{"Вы готовы драться с лордом Джараксусом?",GOSSIP_ACTION_INFO_DEF+2,false,TYPE_JARAXXUS},  //
{"Вы готовы драться с крестоносцами альянса?",GOSSIP_ACTION_INFO_DEF+3,false,TYPE_CRUSADERS}, //
{"Вы готовы драться с крестоносцами орды?",GOSSIP_ACTION_INFO_DEF+4,false,TYPE_CRUSADERS}, //
{"Вы готовы драться с Близнецами?",GOSSIP_ACTION_INFO_DEF+5,false,TYPE_FROJA}, //
{"Вы готовы драться с Анубараком?",GOSSIP_ACTION_INFO_DEF+6,false,TYPE_ANUBARAK}, //
{"Не надо сюда тыкать. На сегодня арена закрыта.",GOSSIP_ACTION_INFO_DEF+7,true,TYPE_ANUBARAK}, //
};
enum
{
         NUM_MESSAGES = 7,
};


struct MANGOS_DLL_DECL npc_toc_announcerAI : public ScriptedAI
{
    npc_toc_announcerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool flag25;

    void Reset()
    {
    m_pInstance->SetData(TYPE_STAGE,0);
    flag25 = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance) return;

        switch (m_pInstance->GetData(TYPE_STAGE)) {
        case 0: break;
        case 1: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_GORMOK)))
                        if (!pTemp->isAlive())  m_pInstance->SetData(TYPE_STAGE,2);
                 break;
                 };
        case 2: {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_DREADSCALE));
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ACIDMAW));
                if (pTemp && pTemp1){
                 if (!pTemp->isAlive() && !pTemp1->isAlive()) m_pInstance->SetData(TYPE_STAGE,3);
                 }
                 else {
                        m_creature->SummonCreature(NPC_DREADSCALE, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                        m_creature->SummonCreature(NPC_ACIDMAW, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);

                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_DREADSCALE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ACIDMAW))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 }
        case 3: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ICEHOWL))) {
                 if (!pTemp->isAlive()) {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_BEASTS,DONE);
                        }
                 }
                 else {
                        m_creature->SummonCreature(NPC_ICEHOWL, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                              if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ICEHOWL))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 };
        case 4: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JARAXXUS))) {
                 if (!pTemp->isAlive()) {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_JARAXXUS,DONE);
                        }
                 }
                 else {
                        m_creature->SummonCreature(NPC_JARAXXUS, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                              if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_JARAXXUS))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 };

        case 5: {
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_1));
                Creature* pTemp2 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_2));
                Creature* pTemp3 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_3));
                Creature* pTemp4 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_4));
                Creature* pTemp5 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_5));
                Creature* pTemp6 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_6));
                          if (m_pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                 Creature* pTemp7 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_7));
                                 Creature* pTemp8 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_8));
                                 Creature* pTemp9 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_9));
                                 Creature* pTemp10 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_1_10));
                                 Creature* pTemp11 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1));
                                 Creature* pTemp12 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1));
                                 if (pTemp7 && pTemp8 && pTemp9 && pTemp10 && pTemp11 && pTemp12) {
                                    if (!pTemp7->isAlive() && !pTemp8->isAlive() && !pTemp9->isAlive() && !pTemp10->isAlive() && !pTemp11->isAlive() && !pTemp12->isAlive())
                                        flag25 = true;
                                        else flag25 = false;
                                 }
                          } else flag25 = true;

                if (pTemp1 && pTemp2 && pTemp3 && pTemp4 && pTemp5 && pTemp6) {
                    if (!pTemp1->isAlive() && !pTemp2->isAlive() && !pTemp3->isAlive() && !pTemp4->isAlive() && !pTemp5->isAlive() && !pTemp6->isAlive() && flag25) {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_CRUSADERS,DONE);
                        }
                    };
                break;
                };

        case 6: {
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_1));
                Creature* pTemp2 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_2));
                Creature* pTemp3 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_3));
                Creature* pTemp4 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_4));
                Creature* pTemp5 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_5));
                Creature* pTemp6 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_6));
                          if (m_pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                 Creature* pTemp7 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_7));
                                 Creature* pTemp8 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_8));
                                 Creature* pTemp9 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_9));
                                 Creature* pTemp10 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_2_10));
                                 Creature* pTemp11 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1));
                                 Creature* pTemp12 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_CRUSADER_0_1));
                                 if (pTemp7 && pTemp8 && pTemp9 && pTemp10 && pTemp11 && pTemp12) {
                                    if (!pTemp7->isAlive() && !pTemp8->isAlive() && !pTemp9->isAlive() && !pTemp10->isAlive() && !pTemp11->isAlive() && !pTemp12->isAlive())
                                        flag25 = true;
                                        else flag25 = false;
                                 }
                          } else flag25 = true;

                if (pTemp1 && pTemp2 && pTemp3 && pTemp4 && pTemp5 && pTemp6) {
                    if (!pTemp1->isAlive() && !pTemp2->isAlive() && !pTemp3->isAlive() && !pTemp4->isAlive() && !pTemp5->isAlive() && !pTemp6->isAlive() && flag25) {
                        m_pInstance->SetData(TYPE_STAGE,0);
                        m_pInstance->SetData(TYPE_CRUSADERS,DONE);
                        }
                    };
                break;
                };

        case 7: {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_DARKBANE));
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LIGHTBANE));
                if (pTemp && pTemp1){
                 if (!pTemp->isAlive() && !pTemp1->isAlive()) {
                                 m_pInstance->SetData(TYPE_STAGE,0);
                                 m_pInstance->SetData(TYPE_FROJA,DONE);
                                 }
                 }
                break;
                };
        case 8: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ANUBARAK)))
                        if (!pTemp->isAlive())  {
                                 m_pInstance->SetData(TYPE_STAGE,0);
                                 m_pInstance->SetData(TYPE_ANUBARAK,DONE);
                                 }
                 break;
                 };

        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;
    }
};

CreatureAI* GetAI_npc_toc_announcer(Creature* pCreature)
{
    return new npc_toc_announcerAI(pCreature);
}

bool GossipHello_npc_toc_announcer(Player* pPlayer, Creature* pCreature)
{
 
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!m_pInstance) return false;

    if(!pPlayer->getAttackers().empty()) return true;

    for(uint8 i = 0; i < NUM_MESSAGES; i++) {
    if (!_GossipMessage[i].state && (m_pInstance->GetData(_GossipMessage[i].encounter) == NOT_STARTED || m_pInstance->GetData(_GossipMessage[i].encounter) == IN_PROGRESS)) {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
        if (_GossipMessage[i].encounter == TYPE_CRUSADERS) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[3].name, GOSSIP_SENDER_MAIN,_GossipMessage[3].id);
        break;
        }
    if (_GossipMessage[i].state && m_pInstance->GetData(_GossipMessage[i].encounter) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
    };
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_toc_announcer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

pPlayer->CLOSE_GOSSIP_MENU();

switch(uiAction) {
    case GOSSIP_ACTION_INFO_DEF+1: {
    if (m_pInstance->GetData(TYPE_BEASTS) == NOT_STARTED) m_pInstance->SetData(TYPE_STAGE,1);
    m_pInstance->SetData(TYPE_BEASTS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_GORMOK)))
                        { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          pCreature->SummonCreature(NPC_GORMOK, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_GORMOK))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+2: {
    m_pInstance->SetData(TYPE_STAGE,4);
    m_pInstance->SetData(TYPE_JARAXXUS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_JARAXXUS)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_JARAXXUS, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_JARAXXUS))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+3: {
    m_pInstance->SetData(TYPE_STAGE,5);
    m_pInstance->SetData(TYPE_CRUSADERS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_1)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_1, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_1))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_2)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_2, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_2))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_3)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_3, SpawnLoc[5].x, SpawnLoc[5].y, SpawnLoc[5].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_3))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_4)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_4, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_4))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_5)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_5, SpawnLoc[7].x, SpawnLoc[7].y, SpawnLoc[7].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_5))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_6)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_6, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_6))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
                          if (m_pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_7)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_7, SpawnLoc[9].x, SpawnLoc[9].y, SpawnLoc[9].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_7))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_8)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_8, SpawnLoc[10].x, SpawnLoc[10].y, SpawnLoc[10].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_8))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_9)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_9, SpawnLoc[11].x, SpawnLoc[11].y, SpawnLoc[11].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_9))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_10)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_10, SpawnLoc[12].x, SpawnLoc[12].y, SpawnLoc[12].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_1_10))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_1, SpawnLoc[13].x, SpawnLoc[13].y, SpawnLoc[13].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_1))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_2, SpawnLoc[14].x, SpawnLoc[14].y, SpawnLoc[14].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_2))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                            }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+4: {
    m_pInstance->SetData(TYPE_STAGE,6);
    m_pInstance->SetData(TYPE_CRUSADERS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_1)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_1, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_1))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_2)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_2, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_2))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_3)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_3, SpawnLoc[5].x, SpawnLoc[5].y, SpawnLoc[5].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_3))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_4)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_4, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_4))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_5)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_5, SpawnLoc[7].x, SpawnLoc[7].y, SpawnLoc[7].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_5))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_6)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_6, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_6))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
                          if (m_pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_7)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_7, SpawnLoc[9].x, SpawnLoc[9].y, SpawnLoc[9].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_7))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_8)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_8, SpawnLoc[10].x, SpawnLoc[10].y, SpawnLoc[10].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_8))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_9)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_9, SpawnLoc[11].x, SpawnLoc[11].y, SpawnLoc[11].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_9))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_10)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_10, SpawnLoc[12].x, SpawnLoc[12].y, SpawnLoc[12].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_2_10))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_1)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_1, SpawnLoc[13].x, SpawnLoc[13].y, SpawnLoc[13].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_1))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_2)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_2, SpawnLoc[14].x, SpawnLoc[14].y, SpawnLoc[14].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_CRUSADER_0_2))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+5: {
    m_pInstance->SetData(TYPE_STAGE,7);
    m_pInstance->SetData(TYPE_FROJA,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_LIGHTBANE)))
                        { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          pCreature->SummonCreature(NPC_LIGHTBANE, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_LIGHTBANE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_DARKBANE)))
                        { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          pCreature->SummonCreature(NPC_DARKBANE, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_DARKBANE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+6: {
    m_pInstance->SetData(TYPE_STAGE,8);
    m_pInstance->SetData(TYPE_ANUBARAK,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_ANUBARAK)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_ANUBARAK, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),m_pInstance->GetData64(NPC_ANUBARAK))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };
    
    case GOSSIP_ACTION_INFO_DEF+7: {
    m_pInstance->SetData(TYPE_STAGE,9);

    break;
    };

  };

return true;
}

void AddSC_trial_of_the_crusader()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_toc_announcer";
    NewScript->GetAI = &GetAI_npc_toc_announcer;
    NewScript->pGossipHello = &GossipHello_npc_toc_announcer;
    NewScript->pGossipSelect = &GossipSelect_npc_toc_announcer;
    NewScript->RegisterSelf();
}