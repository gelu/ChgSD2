#include "precompiled.h"
#include "sc_creature.h"
#include "sc_gossip.h"
//#include "Player.h"

#define GOSSIP_ITEM_ARENA_TO_HONOR "Change 50 Arena to 1000 Honor"
#define GOSSIP_ITEM_HONOR_TO_ARENA "Change 1000 Honor to 50 Arena"

bool GossipHello_npc_arena_honor(Player *player, Creature *_Creature)
{
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_HONOR_TO_ARENA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(0, GOSSIP_ITEM_ARENA_TO_HONOR, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->SEND_GOSSIP_MENU(3961,_Creature->GetGUID());
	return true;
}

bool GossipSelect_npc_arena_honor(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (action == GOSSIP_ACTION_INFO_DEF + 1)
    {
		if (player->GetHonorPoints() >= 1000)
        {
			player->ModifyHonorPoints(-1000); 
			player->ModifyArenaPoints(+50);
        }
    }
    else if (action == GOSSIP_ACTION_INFO_DEF + 2)
    {
        if (player->GetArenaPoints() >= 50 && player->GetHonorPoints() <= 74000)
        {
            player->ModifyArenaPoints(-50); 
            player->ModifyHonorPoints(+1000);
        }
    }
    player->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_npc_arena_honor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_arena_honor";
    newscript->pGossipHello =  &GossipHello_npc_arena_honor;
    newscript->pGossipSelect = &GossipSelect_npc_arena_honor;
    newscript->RegisterSelf();
}
