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
SD%Complete: 100%
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
    char const* name[2];
    float x, y, z;
    uint32 spellID;
    bool state;
    bool active;
    uint32 encounter;
};

static t_Locations PortalLoc[]=
{
{{"Hammer of the World","Молот света"},-17.1928, 2211.44, 30.1158,0,true,true,TYPE_TELEPORT}, //
{{"Chapel of Damned","Молельня проклятых"},-503.62, 2211.47, 62.8235,70856,false,true,TYPE_MARROWGAR},  //
{{"Skulls plato","Черепной вал"},-615.145, 2211.47, 199.972,70857,false,true,TYPE_DEATHWHISPER}, //
{{"The Rise of Deadly","Подъем смертоносного"},-549.131, 2211.29, 539.291,70858,false,true,TYPE_FLIGHT_WAR}, //
{{"Icecrown Citadel","Цитадель Ледяной Короны"},4198.42, 2769.22, 351.065,70859,false,true,TYPE_SAURFANG}, //
{{"Sanctuary of Blood","Святилище крови"},4490.205566, 2769.275635, 403.983765,0,false,true,TYPE_BLOOD_COUNCIL}, //
{{"Lair of the Queen of Ice","Логово Королевы льда"},4356.580078, 2565.75, 220.401993,70861,false,true,TYPE_VALITHRIA}, //
{{"Frozen Throne","Ледяной трон"},528.767273f, -2124.845947f, 1041.86f, 70860,false,true,TYPE_SINDRAGOSA}, //
};


bool GOGossipSelect_go_icecrown_teleporter(Player *pPlayer, GameObject* pGo, uint32 sender, uint32 action)
{
    int32 damage = 0;
    if(sender != GOSSIP_SENDER_MAIN) return false;

    if(!pPlayer->getAttackers().empty()) return false;

    if(action >= 0 && action <= PORTALS_COUNT)
    pPlayer->TeleportTo(MAP_NUM, PortalLoc[action].x, PortalLoc[action].y, PortalLoc[action].z, 0);
    if (PortalLoc[action].spellID !=0 ) 
           if (SpellEntry const* spell = (SpellEntry *)GetSpellStore()->LookupEntry(PortalLoc[action].spellID))
                  pPlayer->AddAura(new BossAura(spell, EFFECT_INDEX_2, &damage,(Unit*)pPlayer, (Unit*)pPlayer));

    pPlayer->CLOSE_GOSSIP_MENU();
    return true;
}

bool GOGossipHello_go_icecrown_teleporter(Player *pPlayer, GameObject* pGo)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();

    if (!pInstance || !pPlayer) return false;
    if (pPlayer->isInCombat()) return true;

    uint8 _locale;

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
                      _locale = 0;
                      break;
     case LOCALE_ruRU:
                      _locale = 1;
                      break;
     default:
                      _locale = 0;
                      break;
    };

    for(uint8 i = 0; i < PORTALS_COUNT; i++) {
    if (PortalLoc[i].active == true && (PortalLoc[i].state == true || pInstance->GetData(TYPE_TELEPORT) >= PortalLoc[i].encounter) || pPlayer->isGameMaster())
             pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, PortalLoc[i].name[_locale], GOSSIP_SENDER_MAIN, i);
    };
    pPlayer->SEND_GOSSIP_MENU(TELEPORT_GOSSIP_MESSAGE, pGo->GetGUID());
    return true;
}

bool GOGossipHello_go_plague_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_spire* pInstance = (instance_icecrown_spire*)pGo->GetInstanceData();
    if(!pInstance) return false;

    if (pInstance->GetData(TYPE_FESTERGUT) == DONE
        && pInstance->GetData(TYPE_ROTFACE) == DONE)
        {
            pInstance->OpenDoor(pInstance->GetData64(GO_SCIENTIST_DOOR_ORANGE));
            pInstance->OpenDoor(pInstance->GetData64(GO_SCIENTIST_DOOR_GREEN));
            pInstance->OpenDoor(pInstance->GetData64(GO_SCIENTIST_DOOR_COLLISION));
        };
    return true;
}

bool GOGossipHello_go_bloodwing_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_spire* pInstance = (instance_icecrown_spire*)pGo->GetInstanceData();
    if(!pInstance) return false;

    if (pInstance->GetData(TYPE_PUTRICIDE) == DONE)
            pInstance->OpenDoor(pInstance->GetData64(GO_BLOODWING_DOOR));

    return true;
}

bool GOGossipHello_go_frostwing_sigil(Player *player, GameObject* pGo)
{
    instance_icecrown_spire* pInstance = (instance_icecrown_spire*)pGo->GetInstanceData();
    if(!pInstance) return false;

    if (pInstance->GetData(TYPE_LANATHEL) == DONE)
        pInstance->OpenDoor(pInstance->GetData64(GO_FROSTWING_DOOR));

    return true;
}


void AddSC_icecrown_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "go_icecrown_teleporter";
    newscript->pGOGossipHello  = &GOGossipHello_go_icecrown_teleporter;
    newscript->pGOGossipSelect = &GOGossipSelect_go_icecrown_teleporter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_plague_sigil";
    newscript->pGOGossipHello  = &GOGossipHello_go_plague_sigil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_bloodwing_sigil";
    newscript->pGOGossipHello  = &GOGossipHello_go_bloodwing_sigil;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_frostwing_sigil";
    newscript->pGOGossipHello  = &GOGossipHello_go_frostwing_sigil;
    newscript->RegisterSelf();
}
