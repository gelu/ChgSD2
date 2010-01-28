#include "precompiled.h"
#include "def_spire.h"
enum 
{
PORTALS_COUNT = 5
};

struct Locations
{
    char const* name;
    float x, y, z;
    uint32 id;
    bool state;
};

static Locations PortalLoc[]=
{
{"Молот света",-17.1928, 2211.44, 30.1158,1,true}, //
{"Шпиль",-503.62, 2211.47, 62.8235,0,true},  //
{"Черепной вал",-615.145, 2211.47, 199.972,3,true}, //
{"Воздушное сражение",-209.5, 2211.91, 199.97,4,true}, //
{"Подъем смертоносного",-549.131, 2211.29, 539.291,2,true}, //
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
//    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    bool m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

//    if(!pInstance) return true;

    for(uint8 i = 0; i < PORTALS_COUNT; i++) {
    if (PortalLoc[i].state == true)
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, PortalLoc[i].name, GOSSIP_SENDER_MAIN, i);
    };
    player->SEND_GOSSIP_MENU(TELEPORT_GOSSIP_MESSAGE, pCreature->GetGUID());
    return true;
}


bool GOHello_go_icecrown_teleporter(Player *player, GameObject* pGo)
{

//    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();
    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

//    if(!pInstance) return true;

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the 1", GOSSIP_SENDER_MAIN, BASE_CAMP);
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
