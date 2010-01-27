#include "precompiled.h"
#include "def_spire.h"

static Locations PortalLoc[]=
{
{-503.62, 2211.47, 62.8235},  //
{-17.1928, 2211.44, 30.1158}, //
{-549.131, 2211.29, 539.291}, //
{-615.145, 2211.47, 199.972}, //
{-210.223, 2217.72, 199.97},  
};

bool GOHello_icecrown_teleporter(Player *player, GameObject* pGo)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) creature->GetInstanceData();
    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

    if(!pInstance) return true;

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the 1", GOSSIP_SENDER_MAIN, BASE_CAMP);

    return true;
}

bool GOSelect_icecrown_teleporter(Player *player, GameObject* pGo, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!player->getAttackers().empty()) return true;

    switch(action)
    {
    case BASE_CAMP:
        player->TeleportTo(MAP_NUM, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    }
    return true;
}

void AddSC_icecrown_teleporter()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "icecrown_teleporter";
    newscript->pGOHello = &GOHello_icecrown_teleporter;
    newscript->pGOSelect = &GOSelect_icecrown_teleporter;
    newscript->RegisterSelf();
}
