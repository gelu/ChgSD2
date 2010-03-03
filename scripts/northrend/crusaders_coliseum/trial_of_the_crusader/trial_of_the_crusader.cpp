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
SD%Complete: 30%
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
{"Вы готовы драться с Анубараком?",GOSSIP_ACTION_INFO_DEF+6,false,TYPE_LICH_KING}, //
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
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    bool flag25;
    uint32 DelayTimer;
    uint32 substage;

    void Reset()
    {
    pInstance->SetData(TYPE_STAGE,0);
    flag25 = true;
    DelayTimer = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance) return;

    if(DelayTimer < diff) {
        switch (pInstance->GetData(TYPE_STAGE)) {
        case 0: break;
        case 1: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_GORMOK)))
                        if (!pTemp->isAlive()) {
                         pInstance->SetData(TYPE_STAGE,2);
                         pInstance->SetData(TYPE_EVENT,20);
                         }
                 break;
                 };
        case 2: {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_DREADSCALE));
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ACIDMAW));
                if (pTemp && pTemp1){
                 if (!pTemp->isAlive() && !pTemp1->isAlive()) {
                 pInstance->SetData(TYPE_STAGE,3);
                 pInstance->SetData(TYPE_EVENT,30);
                 }
                 }
                 else {
                        m_creature->SummonCreature(NPC_DREADSCALE, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                        m_creature->SummonCreature(NPC_ACIDMAW, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);

                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_DREADSCALE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ACIDMAW))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 }
        case 3: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ICEHOWL))) {
                 if (!pTemp->isAlive()) {
                        pInstance->SetData(TYPE_STAGE,0);
                        pInstance->SetData(TYPE_BEASTS,DONE);
                        pInstance->SetData(TYPE_EVENT,40);
                        }
                 }
                 else {
                        m_creature->SummonCreature(NPC_ICEHOWL, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                              if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ICEHOWL))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 };
        case 4: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_JARAXXUS))) {
                 if (!pTemp->isAlive()) {
                        pInstance->SetData(TYPE_STAGE,0);
                        pInstance->SetData(TYPE_JARAXXUS,DONE);
                        }
                 }
                 else {
                        m_creature->SummonCreature(NPC_JARAXXUS, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                              if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_JARAXXUS))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                        }
                 break;
                 };

        case 5: {
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_1));
                Creature* pTemp2 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_2));
                Creature* pTemp3 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_3));
                Creature* pTemp4 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_4));
                Creature* pTemp5 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_5));
                Creature* pTemp6 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_6));
                          if (pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                 Creature* pTemp7 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_7));
                                 Creature* pTemp8 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_8));
                                 Creature* pTemp9 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_9));
                                 Creature* pTemp10 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_1_10));
                                 Creature* pTemp11 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_0_1));
                                 Creature* pTemp12 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_0_1));
                                 if (pTemp7 && pTemp8 && pTemp9 && pTemp10 && pTemp11 && pTemp12) {
                                    if (!pTemp7->isAlive() && !pTemp8->isAlive() && !pTemp9->isAlive() && !pTemp10->isAlive() && !pTemp11->isAlive() && !pTemp12->isAlive())
                                        flag25 = true;
                                        else flag25 = false;
                                 }
                          } else flag25 = true;

                if (pTemp1 && pTemp2 && pTemp3 && pTemp4 && pTemp5 && pTemp6) {
                    if (!pTemp1->isAlive() && !pTemp2->isAlive() && !pTemp3->isAlive() && !pTemp4->isAlive() && !pTemp5->isAlive() && !pTemp6->isAlive() && flag25) {
                        pInstance->SetData(TYPE_STAGE,0);
                        pInstance->SetData(TYPE_CRUSADERS,DONE);
                        }
                    };
                break;
                };

        case 6: {
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_1));
                Creature* pTemp2 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_2));
                Creature* pTemp3 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_3));
                Creature* pTemp4 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_4));
                Creature* pTemp5 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_5));
                Creature* pTemp6 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_6));
                          if (pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                 Creature* pTemp7 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_7));
                                 Creature* pTemp8 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_8));
                                 Creature* pTemp9 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_9));
                                 Creature* pTemp10 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_2_10));
                                 Creature* pTemp11 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_0_1));
                                 Creature* pTemp12 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CRUSADER_0_1));
                                 if (pTemp7 && pTemp8 && pTemp9 && pTemp10 && pTemp11 && pTemp12) {
                                    if (!pTemp7->isAlive() && !pTemp8->isAlive() && !pTemp9->isAlive() && !pTemp10->isAlive() && !pTemp11->isAlive() && !pTemp12->isAlive())
                                        flag25 = true;
                                        else flag25 = false;
                                 }
                          } else flag25 = true;

                if (pTemp1 && pTemp2 && pTemp3 && pTemp4 && pTemp5 && pTemp6) {
                    if (!pTemp1->isAlive() && !pTemp2->isAlive() && !pTemp3->isAlive() && !pTemp4->isAlive() && !pTemp5->isAlive() && !pTemp6->isAlive() && flag25) {
                        pInstance->SetData(TYPE_STAGE,0);
                        pInstance->SetData(TYPE_CRUSADERS,DONE);
                        }
                    };
                break;
                };

        case 7: {
                Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_DARKBANE));
                Creature* pTemp1 = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_LIGHTBANE));
                if (pTemp && pTemp1){
                 if (!pTemp->isAlive() && !pTemp1->isAlive()) {
                                 pInstance->SetData(TYPE_STAGE,0);
                                 pInstance->SetData(TYPE_FROJA,DONE);
                                 }
                 }
                break;
                };
        case 8: {
               if  (pInstance->GetData(TYPE_LICH_KING) == DONE){
                     pInstance->SetData(TYPE_ANUBARAK,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ANUBARAK)))
                        pTemp->Respawn();
                    else {
                          m_creature->SummonCreature(NPC_ANUBARAK, SpawnLoc[19].x, SpawnLoc[19].y, SpawnLoc[19].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ANUBARAK))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[20].x, SpawnLoc[20].y, SpawnLoc[20].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                pInstance->SetData(TYPE_STAGE,9);
                                m_creature->SetVisibility(VISIBILITY_OFF);
                                }
                          }
                     }
                 break;
                 };
        case 9: {
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ANUBARAK)))
                        if (!pTemp->isAlive())  {
                                 pInstance->SetData(TYPE_STAGE,0);
                                 pInstance->SetData(TYPE_ANUBARAK,DONE);
                                 }
                 break;
                 };

          }
        } else DelayTimer -= diff;
    }
};

CreatureAI* GetAI_npc_toc_announcer(Creature* pCreature)
{
    return new npc_toc_announcerAI(pCreature);
}

bool GossipHello_npc_toc_announcer(Player* pPlayer, Creature* pCreature)
{
 
    ScriptedInstance* pInstance;
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!pInstance) return false;

    if(!pPlayer->getAttackers().empty()) return true;

    for(uint8 i = 0; i < NUM_MESSAGES; i++) {
    if (!_GossipMessage[i].state && (pInstance->GetData(_GossipMessage[i].encounter) == NOT_STARTED || pInstance->GetData(_GossipMessage[i].encounter) == IN_PROGRESS)) {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
        if (_GossipMessage[i].encounter == TYPE_CRUSADERS) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[3].name, GOSSIP_SENDER_MAIN,_GossipMessage[3].id);
        break;
        }
    if (_GossipMessage[i].state && pInstance->GetData(_GossipMessage[i].encounter) == DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
    };
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_toc_announcer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* pInstance;
    pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

pPlayer->CLOSE_GOSSIP_MENU();

switch(uiAction) {
    case GOSSIP_ACTION_INFO_DEF+1: {
    if (pInstance->GetData(TYPE_BEASTS) == NOT_STARTED ||
        pInstance->GetData(TYPE_BEASTS) == FAIL) 
           pInstance->SetData(TYPE_EVENT,11);
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+2: {
    pInstance->SetData(TYPE_STAGE,4);
    pInstance->SetData(TYPE_JARAXXUS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_JARAXXUS)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_JARAXXUS, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_JARAXXUS))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+3: {
    pInstance->SetData(TYPE_STAGE,5);
    pInstance->SetData(TYPE_CRUSADERS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_1)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_1, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_1))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_2)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_2, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_2))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_3)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_3, SpawnLoc[5].x, SpawnLoc[5].y, SpawnLoc[5].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_3))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_4)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_4, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_4))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_5)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_5, SpawnLoc[7].x, SpawnLoc[7].y, SpawnLoc[7].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_5))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_6)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_1_6, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_6))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
                          if (pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_7)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_7, SpawnLoc[9].x, SpawnLoc[9].y, SpawnLoc[9].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_7))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_8)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_8, SpawnLoc[10].x, SpawnLoc[10].y, SpawnLoc[10].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_8))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_9)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_9, SpawnLoc[11].x, SpawnLoc[11].y, SpawnLoc[11].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_9))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_10)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_1_10, SpawnLoc[12].x, SpawnLoc[12].y, SpawnLoc[12].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_1_10))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_1)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_1, SpawnLoc[13].x, SpawnLoc[13].y, SpawnLoc[13].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_1))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_2)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_2, SpawnLoc[14].x, SpawnLoc[14].y, SpawnLoc[14].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_2))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                            }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+4: {
    pInstance->SetData(TYPE_STAGE,6);
    pInstance->SetData(TYPE_CRUSADERS,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_1)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_1, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_1))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_2)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_2, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_2))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_3)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_3, SpawnLoc[5].x, SpawnLoc[5].y, SpawnLoc[5].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_3))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_4)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_4, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_4))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_5)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_5, SpawnLoc[7].x, SpawnLoc[7].y, SpawnLoc[7].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_5))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_6)))
                        pTemp->Respawn();
                    else {
                          pCreature->SummonCreature(NPC_CRUSADER_2_6, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_6))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
                          if (pInstance->GetData(TYPE_DIFFICULTY) == 25) {
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_7)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_7, SpawnLoc[9].x, SpawnLoc[9].y, SpawnLoc[9].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_7))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_8)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_8, SpawnLoc[10].x, SpawnLoc[10].y, SpawnLoc[10].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_8))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_9)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_9, SpawnLoc[11].x, SpawnLoc[11].y, SpawnLoc[11].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_9))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_10)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_2_10, SpawnLoc[12].x, SpawnLoc[12].y, SpawnLoc[12].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_2_10))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_1)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_1, SpawnLoc[13].x, SpawnLoc[13].y, SpawnLoc[13].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_1))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                                   if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_2)))
                                       pTemp->Respawn();
                                   else {
                                       pCreature->SummonCreature(NPC_CRUSADER_0_2, SpawnLoc[14].x, SpawnLoc[14].y, SpawnLoc[14].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                                           if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_CRUSADER_0_2))) {
                                           pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                           pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                           pTemp->SetInCombatWithZone();
                                           }
                                         }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+5: {
    pInstance->SetData(TYPE_STAGE,7);
    pInstance->SetData(TYPE_FROJA,IN_PROGRESS);
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_LIGHTBANE)))
                        { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          pCreature->SummonCreature(NPC_LIGHTBANE, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_LIGHTBANE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
            if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_DARKBANE)))
                        { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          pCreature->SummonCreature(NPC_DARKBANE, SpawnLoc[4].x, SpawnLoc[4].y, SpawnLoc[4].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*pCreature),pInstance->GetData64(NPC_DARKBANE))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+6: {
    pInstance->SetData(TYPE_STAGE,8);
    pCreature->SummonCreature(NPC_LICH_KING_1, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_MANUAL_DESPAWN, 0);
    pInstance->SetData(TYPE_EVENT,500);
    break;
    };

    case GOSSIP_ACTION_INFO_DEF+7: {
    pInstance->SetData(TYPE_STAGE,9);

    break;
    };

  };

return true;
}

struct MANGOS_DLL_DECL boss_lich_king_tocAI : public ScriptedAI
{
    boss_lich_king_tocAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        WayPointList.clear();
        JustRespawned();
    }

    ScriptedInstance* pInstance;
    uint32 UpdateTimer;
    uint32 event_state_lich_king;
    bool Event;
    bool MovementStarted;
    std::list<WayPoints> WayPointList;
    std::list<WayPoints>::iterator WayPoint;
    uint32 WalkTimer;
    bool IsWalking;

    void Reset()
    {
        UpdateTimer = 0;
        event_state_lich_king = 0;
        Event = false;
        MovementStarted = false;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void JustRespawned()
    {
        Reset();
    }

    void MoveInLineOfSight(Unit *who)
    {
    }

    void StartMovement()
    {
        if(!WayPointList.empty() || MovementStarted)
            return;

        AddWaypoint(0, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z);
        AddWaypoint(1, SpawnLoc[17].x, SpawnLoc[17].y, SpawnLoc[17].z);
        AddWaypoint(2, SpawnLoc[18].x, SpawnLoc[18].y, SpawnLoc[18].z);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        WayPoint = WayPointList.begin();
        MovementStarted = true;
        IsWalking = true;
        WalkTimer = 200;
        event_state_lich_king = 1;
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
    }

    void AddWaypoint(uint32 id, float x, float y, float z)
    {
        WayPoints wp(id, x, y, z);
        WayPointList.push_back(wp);
    }

    void UpdateAI(const uint32 diff)
    {
    if (!MovementStarted) StartMovement();

        if (IsWalking && WalkTimer)
        {
            if (WalkTimer <= diff)
            {
                if (WayPoint != WayPointList.end())
                {
                    m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y,WayPoint->z);
                    WalkTimer = 0;
                }
            }else WalkTimer -= diff;
        }

        if (UpdateTimer <= diff)
        {
        switch (pInstance->GetData(TYPE_EVENT))
        {
        case 501:
               DoScriptText(-1713550,m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 2500);
               pInstance->SetData(TYPE_EVENT,502);
               break;
        case 503:
               DoScriptText(-1713552,m_creature);
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
               pInstance->SetData(TYPE_EVENT_TIMER, 13000);
               pInstance->SetData(TYPE_EVENT,504);
               break;
        case 504:
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
               pInstance->SetData(TYPE_EVENT_TIMER, 1000);
               pInstance->SetData(TYPE_EVENT,505);
               break;
        case 505:
               m_creature->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
               pInstance->SetData(TYPE_EVENT_TIMER, 3000);
               pInstance->SetData(TYPE_EVENT,506);
               break;
        case 506: if (Event) {
               DoScriptText(-1713553,m_creature);
               m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
               pInstance->SetData(TYPE_EVENT_TIMER, 1300);
               pInstance->SetData(TYPE_EVENT,507);
               }
               break;
        case 507:
               m_creature->CastSpell(m_creature,68198,false);
               pInstance->SetData(TYPE_EVENT_TIMER, 1300);
               pInstance->SetData(TYPE_EVENT,508);
               break;
        case 508:
               if (GameObject* pGoFloor = pInstance->instance->GetGameObject(pInstance->GetData64(GO_ARGENT_COLISEUM_FLOOR)))
                  {
                   pGoFloor->SetUInt32Value(GAMEOBJECT_DISPLAYID,9060);
                   pGoFloor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK_10 | GO_FLAG_NODESPAWN);
                   pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1,8449);
                   }
                m_creature->CastSpell(m_creature,69016,false);
                if(pInstance) pInstance->SetData(TYPE_LICH_KING,DONE);

               Event=false;
               m_creature->ForcedDespawn();
               pInstance->SetData(TYPE_EVENT,509);
               break;
        }
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
        } else UpdateTimer -= diff;

    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(pInstance)
        {
            if(id == 2)
            {
                Event = true;
            }
        }
        if(type != POINT_MOTION_TYPE) return;
        if(WayPoint->id != id) return;
        ++WayPoint;
        WalkTimer = 200;
    }
};

CreatureAI* GetAI_boss_lich_king_toc(Creature *_Creature)
{
    boss_lich_king_tocAI* newAI = new boss_lich_king_tocAI(_Creature); 
    return newAI;
}

struct MANGOS_DLL_DECL npc_fizzlebang_tocAI : public ScriptedAI
{
    npc_fizzlebang_tocAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        Reset();
    }

    InstanceData* pInstance;
    uint32 UpdateTimer;

    void Reset()
    {
    }

    void UpdateAI(const uint32 diff)
    {
      if(!pInstance) return;


        if(UpdateTimer <= diff)
        {
        switch(pInstance->GetData(TYPE_EVENT))
              {
               case 111:
                    pInstance->SetData(TYPE_EVENT, 112);
                    pInstance->SetData(TYPE_EVENT_TIMER, 11000);
                    break;
               case 112:
                    DoScriptText(-1999937, m_creature);
                    pInstance->SetData(TYPE_EVENT, 113);
                    m_creature->GetMotionMaster()->MovePoint(0, 558.94f, 138.02f, 394.16f);
                    pInstance->SetData(TYPE_EVENT_TIMER, 18000);
                    break;
               case 113:
                    DoScriptText(-1999938, m_creature);
                    pInstance->SetData(TYPE_EVENT, 114);
                    pInstance->SetData(TYPE_EVENT_TIMER, 12000);
                    break;
               case 114:
                    DoScriptText(-1999939, m_creature);
                    pInstance->SetData(TYPE_EVENT, 115);
//                    Creature* boss = m_creature->SummonCreature(NPC_JARAXXUS, 568.00f, 141.43f, 394.16f, 3.61f);
                    //boss->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    pInstance->SetData(TYPE_EVENT_TIMER, 11000);
                    break;
              }
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
        } else UpdateTimer -= diff;
    }
};

CreatureAI* GetAI_npc_fizzlebang_toc(Creature* pCreature)
{
    return new npc_fizzlebang_tocAI(pCreature);
}

struct MANGOS_DLL_DECL npc_tirion_tocAI : public ScriptedAI
{
    npc_tirion_tocAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        Reset();
    }

    InstanceData* pInstance;
    uint32 UpdateTimer;

    void Reset()
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance) return;

        if (UpdateTimer <= diff)
        {
        switch (pInstance->GetData(TYPE_EVENT))
        {
        case 11:
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
               DoScriptText(-1713500, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,12);
               break;
        case 14:
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
               DoScriptText(-1713501, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,15);
               break;
        case 15:
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                if (pInstance->GetData(TYPE_BEASTS) == NOT_STARTED || 
                     pInstance->GetData(TYPE_BEASTS) == FAIL) 
                         pInstance->SetData(TYPE_STAGE,1);
                pInstance->SetData(TYPE_BEASTS,IN_PROGRESS);
                if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_GORMOK)))
                    { if(!pTemp->isAlive()) pTemp->Respawn(); }
                    else {
                          m_creature->SummonCreature(NPC_GORMOK, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 5, TEMPSUMMON_CORPSE_TIMED_DESPAWN, DESPAWN_TIME);
                          if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_GORMOK))) {
                                pTemp->GetMotionMaster()->MovePoint(0, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                                pTemp->AddSplineFlag(SPLINEFLAG_WALKMODE);
                                pTemp->SetInCombatWithZone();
                                }
                          }
                pInstance->SetData(TYPE_EVENT_TIMER, 10000);
                pInstance->SetData(TYPE_EVENT,16);
                break;

        case 20:
               DoScriptText(-1713503, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,21);
               break;
        case 30:
               DoScriptText(-1713505, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,31);
               break;
        case 40:
               DoScriptText(-1713509, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,41);
               break;

        case 500:
               DoScriptText(-1713549, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 3000);
               pInstance->SetData(TYPE_EVENT,501);
               break;
        case 502:
               DoScriptText(-1713551, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,503);
               break;
        }
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
        } else UpdateTimer -= diff;
    }
};

CreatureAI* GetAI_npc_tirion_toc(Creature* pCreature)
{
    return new npc_tirion_tocAI(pCreature);
}

struct MANGOS_DLL_DECL npc_garrosh_tocAI : public ScriptedAI
{
    npc_garrosh_tocAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        Reset();
    }

    InstanceData* pInstance;
    uint32 UpdateTimer;

    void Reset()
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance) return;

        if (UpdateTimer <= diff)
        {
        switch (pInstance->GetData(TYPE_EVENT))
        {
        case 12:
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
               DoScriptText(-1713702, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,13);
               break;
        }
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
        } else UpdateTimer -= diff;
    }
};

CreatureAI* GetAI_npc_garrosh_toc(Creature* pCreature)
{
    return new npc_garrosh_tocAI(pCreature);
};

struct MANGOS_DLL_DECL npc_rinn_tocAI : public ScriptedAI
{
    npc_rinn_tocAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)m_creature->GetInstanceData();
        Reset();
    }

    InstanceData* pInstance;
    uint32 UpdateTimer;

    void Reset()
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance) return;
        if (UpdateTimer <= diff)
        {
        switch (pInstance->GetData(TYPE_EVENT))
        {
        case 13:
               m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
               DoScriptText(-1713502, m_creature);
               pInstance->SetData(TYPE_EVENT_TIMER, 10000);
               pInstance->SetData(TYPE_EVENT,14);
               break;
        }
        UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
        } else UpdateTimer -= diff;
    }
};

CreatureAI* GetAI_npc_rinn_toc(Creature* pCreature)
{
    return new npc_rinn_tocAI(pCreature);
};

void AddSC_trial_of_the_crusader()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "npc_toc_announcer";
    NewScript->GetAI = &GetAI_npc_toc_announcer;
    NewScript->pGossipHello = &GossipHello_npc_toc_announcer;
    NewScript->pGossipSelect = &GossipSelect_npc_toc_announcer;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_lich_king_toc";
    NewScript->GetAI = &GetAI_boss_lich_king_toc;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_fizzlebang_toc";
    NewScript->GetAI = &GetAI_npc_fizzlebang_toc;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_tirion_toc";
    NewScript->GetAI = &GetAI_npc_tirion_toc;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_garrosh_toc";
    NewScript->GetAI = &GetAI_npc_garrosh_toc;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_rinn_toc";
    NewScript->GetAI = &GetAI_npc_rinn_toc;
    NewScript->RegisterSelf();
}