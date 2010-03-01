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
SDName: icecrown_teleport
SD%Complete: 30%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
#include "precompiled.h"
#include "def_spire.h"

enum 
{
PORTALS_COUNT = 8
};

struct t_Locations
{
    char const* name;
    float x, y, z;
    uint32 id;
    bool state;
    bool active;
    uint32 encounter;
};

static t_Locations PortalLoc[]=
{
{"Молот света",-17.1928, 2211.44, 30.1158,0,true,true,TYPE_TELEPORT}, //
{"Шпиль",-503.62, 2211.47, 62.8235,1,false,true,TYPE_MARROWGAR},  //
{"Черепной вал",-615.145, 2211.47, 199.972,2,false,true,TYPE_DEATHWHISPER}, //
{"Воздушное сражение",-209.5, 2211.91, 199.97,3,false,true,TYPE_SKULLS_PLATO}, //
{"Подъем смертоносного",-549.131, 2211.29, 539.291,4,false,true,TYPE_FLIGHT_WAR}, //
{"Цитадель Ледяной Короны",4198.42, 2769.22, 351.065,5,false,false,TYPE_SAURFANG}, //
{"Святилище крови",4490.205566, 2769.275635, 403.983765,6,false,false,TYPE_BLOOD_COUNCIL}, //
{"Логово Королевы льда",4356.236816, 2402.573242, 220.462540,7,false,false,TYPE_VALITHRIA}, //
};


bool GossipSelect_icecrown_teleporter(Player *player, Creature* pCreature, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!player->getAttackers().empty()) return true;
    if(action >= 0 && action <= PORTALS_COUNT)
        player->TeleportTo(MAP_NUM, PortalLoc[action].x, PortalLoc[action].y, PortalLoc[action].z, 0);
        player->CLOSE_GOSSIP_MENU();
    return true;
}

bool GossipHello_icecrown_teleporter(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    bool m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

    for(uint8 i = 0; i < PORTALS_COUNT; i++) {
    if (PortalLoc[i].active == true && (PortalLoc[i].state == true || pInstance->GetData(TYPE_TELEPORT) >= PortalLoc[i].encounter))
             player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, PortalLoc[i].name, GOSSIP_SENDER_MAIN, i);
    };
    player->SEND_GOSSIP_MENU(TELEPORT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}


bool GOHello_go_icecrown_teleporter(Player *player, GameObject* pGo)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();
    if(!pInstance) return true;

    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "No message", GOSSIP_SENDER_MAIN, 0);
    player->SEND_GOSSIP_MENU(GO_TELEPORT_GOSSIP_MESSAGE, pGo->GetGUID());
    return true;
}


void AddSC_icecrown_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "icecrown_teleporter";
    newscript->pGossipHello = &GossipHello_icecrown_teleporter;
    newscript->pGossipSelect = &GossipSelect_icecrown_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_icecrown_teleporter";
    newscript->pGOHello = &GOHello_go_icecrown_teleporter;
    newscript->RegisterSelf();
}
