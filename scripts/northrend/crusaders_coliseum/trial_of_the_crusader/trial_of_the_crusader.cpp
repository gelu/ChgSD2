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
{"Вы готовы драться с крестоносцами фракций?",GOSSIP_ACTION_INFO_DEF+3,false,TYPE_CRUSADERS}, //
{"Вы готовы драться с Близнецами?",GOSSIP_ACTION_INFO_DEF+4,false,TYPE_FROJA}, //
{"Вы готовы драться с Ануб`Араком?",GOSSIP_ACTION_INFO_DEF+5,false,TYPE_ANUBARAK}, //
{"Не надо сюда тыкать. На сегодня арена закрыта.",GOSSIP_ACTION_INFO_DEF+6,true,TYPE_ANUBARAK}, //
};
#define NUM_MERSSAGES 6

struct MANGOS_DLL_DECL npc_toc_announcerAI : public ScriptedAI
{
    npc_toc5_announcerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance) return;
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
    if (!_GossipMessage[i].state && (m_pInstance->GetData(_GossipMessage[i].encounter) == NOT_STARTED || m_pInstance->GetData(_GossipMessage[i].encounter) == IN_PROGRESS))
        {pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
        break;
        }
    if (_GossipMessage[i].state && m_pInstance->GetData(_GossipMessage[i].encounter) == DONE)
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, _GossipMessage[i].name, GOSSIP_SENDER_MAIN,_GossipMessage[i].id);
    };
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_toc5_announcer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

pPlayer->CLOSE_GOSSIP_MENU();

switch(uiAction) {
    case GOSSIP_ACTION_INFO_DEF+1: {
    };

    case GOSSIP_ACTION_INFO_DEF+2: {

    };

    case GOSSIP_ACTION_INFO_DEF+3: {
    };

    case GOSSIP_ACTION_INFO_DEF+4: {
    };

    case GOSSIP_ACTION_INFO_DEF+5: {
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