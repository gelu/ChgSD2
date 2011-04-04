/**
 *
 * @File : sd2p_object_teleport.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.0
 *
 **/


#include "precompiled.h"
#include "sd2p_sc_object_teleport.h"

using namespace SD2P_NAMESPACE;
using namespace SD2P_NAMESPACE::SD2P_OBJECT_TELE_NAMESPACE;


namespace
{
    bool ObjectTeleport(Player * pPlayer, Object * pObject)
    {
        if (const DefinitionObject * pDefObj = GetDefinitionObject(pObject->GetEntry()))
        {
            if (const Destination * pDest = pDefObj->GetAllowedDestination(pPlayer))
            {
                if (pPlayer->isGameMaster())
                {
                    Teleport(pPlayer, pDest->m_Map, pDest->m_X, pDest->m_Y, pDest->m_Z, pDest->m_Orient);
                    return false;
                }

                if (pPlayer->getLevel() < pDest->m_Level)
                {
                    pPlayer->GetSession()->SendNotification("Vous n'avez pas le niveau requis. Cette destination requiert le niveau %u.", pDest->m_Level);
                    return true;
                }

                if (pPlayer->GetMoney() < pDest->m_Cost)
                {
                    pPlayer->GetSession()->SendNotification("Vous n'avez pas assez d'argent. Le prix de la teleportation est de %s.", ConvertMoney(pDest->m_Cost).c_str());
                    return true;
                }

                if (pDest->m_Cost)
                    pPlayer->ModifyMoney(-1 * pDest->m_Cost);

                Teleport(pPlayer, pDest->m_Map, pDest->m_X, pDest->m_Y, pDest->m_Z, pDest->m_Orient);
                return false;
            }
            pPlayer->GetSession()->SendNotification("Vous n'etes pas autorise a employer cet objet.");
            return true;
        }
        pPlayer->GetSession()->SendNotification("Cet objet n'est associe a aucune destination.");
        outstring_log("SD2P >> [Object Tele] Aucune destination associee (Object: %u).", pObject->GetEntry());
        return true;
    }
}

bool ItemUse_item_teleport(Player * pPlayer, Item * pItem, SpellCastTargets const &targets)
{
    return ObjectTeleport(pPlayer, pItem);
}

bool GOHello_go_teleport(Player * pPlayer, GameObject * pGO)
{
    return ObjectTeleport(pPlayer, pGO);
}


void AddSC_object_teleport(void)
{
    Script * newscript;

    newscript = new Script;
    newscript->Name="sd2p_item_teleport";
    newscript->pItemUse = ItemUse_item_teleport;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="sd2p_go_teleport";
    newscript->pGossipHelloGO = &GOHello_go_teleport;
    newscript->RegisterSelf();
}
