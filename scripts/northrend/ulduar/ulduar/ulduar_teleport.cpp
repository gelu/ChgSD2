
#include "precompiled.h"
#include "def_ulduar.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
- Spark of Imagination
- Prison of Yogg-Saron
*/

#define BASE_CAMP    200
#define GROUNDS      201
#define FORGE        202
#define SCRAPYARD    203
#define ANTECHAMBER  204
#define WALKWAY      205
#define CONSERVATORY 206
#define SPARK        207
#define PRISON       208

bool GossipHello_ulduar_teleporter(Player *player, Creature *creature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) creature->GetInstanceData();
    if(!pInstance) return true;

    player->ADD_GOSSIP_ITEM(0, "Teleport to the Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);
    if(pInstance->GetData(TYPE_LEVIATHAN_TP))
    {
        player->ADD_GOSSIP_ITEM(0, "Teleport to the Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);
        if(pInstance->GetData(TYPE_FLAME_LEVIATHAN) == DONE)
        {
            player->ADD_GOSSIP_ITEM(0, "Teleport to the Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);
            if(pInstance->GetData(TYPE_XT002_TP))
            {
                player->ADD_GOSSIP_ITEM(0, "Teleport to the Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);
                if(pInstance->GetData(TYPE_XT002) == DONE)
                {
                    player->ADD_GOSSIP_ITEM(0, "Teleport to the Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
                    if(pInstance->GetData(TYPE_KOLOGARN) == DONE)
                    {
                        player->ADD_GOSSIP_ITEM(0, "Teleport to the Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);
                        if(pInstance->GetData(TYPE_AURIAYA) == DONE)
                        {
                            player->ADD_GOSSIP_ITEM(0, "Teleport to the Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);
                            if(pInstance->GetData(TYPE_MIMIRON_TP))
                            {
                                player->ADD_GOSSIP_ITEM(0, "Teleport to the Spark of Imagination", GOSSIP_SENDER_MAIN, SPARK);
                                if(pInstance->GetData(TYPE_VEZAX) == DONE)
                                    player->ADD_GOSSIP_ITEM(0, "Teleport to the Prison of Yogg-Saron", GOSSIP_SENDER_MAIN, PRISON);
                            }
                        }
                    }
                }
            }
        }
    }
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

    return true;
}

bool GossipSelect_ulduar_teleporter(Player *player, Creature *creature, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!player->getAttackers().empty()) return true;

    switch(action)
    {
    case BASE_CAMP:
        player->TeleportTo(603, -706.122, -92.6024, 429.876, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case GROUNDS:
        player->TeleportTo(603, 131.248, -35.3802, 409.804, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case FORGE:
        player->TeleportTo(603, 553.233, -12.3247, 409.679, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case SCRAPYARD:
        player->TeleportTo(603, 926.292, -11.4635, 418.595, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case ANTECHAMBER:
        player->TeleportTo(603, 1498.09, -24.246, 420.967, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case WALKWAY:
        player->TeleportTo(603, 1859.45, -24.1, 448.9, 0); 
        player->CLOSE_GOSSIP_MENU(); break;
    case CONSERVATORY:
        player->TeleportTo(603, 2086.27, -24.3134, 421.239, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case SPARK:
        player->TeleportTo(603, 2536.87, 2569.15, 412.304, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case PRISON:
        //player->TeleportTo(603, 2536.87, 2569.15, 412.304, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    }

    return true;
}

void AddSC_ulduar_teleport()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "ulduar_teleporter";
    newscript->pGossipHello = &GossipHello_ulduar_teleporter;
    newscript->pGossipSelect = &GossipSelect_ulduar_teleporter;
    newscript->RegisterSelf();
}
