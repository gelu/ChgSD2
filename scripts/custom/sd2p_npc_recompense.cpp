/**
 *
 * @File : sd2p_npc_recompense.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.1
 *
 * @Synopsis : NPC proposant des recompenses par categorie en fonction du prix en nombre d'item.
 *
 **/


#include "precompiled.h"
#include "sd2p_sc_npc_recompense.h"
#include <sstream>

#define NEXT_PAGE  "[下一页] "
#define PREV_PAGE  "[上一页] "
#define MAIN_MENU  "[返回] "

using namespace std;
using namespace SD2P_NAMESPACE;
using namespace SD2P_NAMESPACE::SD2P_RECOMPENSE_NAMESPACE;


namespace
{
    enum
    {
        // npc_text
        NPC_TEXT_NOTHING        = 100100,
        NPC_TEXT_INVITEC        = 100101,
        NPC_TEXT_INVITER        = 100102,
        NPC_TEXT_VALID          = 100103,

        // Divers.
        GOSSIP_ACHAT            = GOSSIP_ACTION_INFO_DEF + 1,
        GOSSIP_NEXT_PAGEC       = GOSSIP_ACTION_INFO_DEF + 2,
        GOSSIP_PREV_PAGEC       = GOSSIP_ACTION_INFO_DEF + 3,
        GOSSIP_NEXT_PAGEI       = GOSSIP_ACTION_INFO_DEF + 4,
        GOSSIP_PREV_PAGEI       = GOSSIP_ACTION_INFO_DEF + 5,
        GOSSIP_SHOW_ITEMS       = GOSSIP_ACTION_INFO_DEF + 6,
        GOSSIP_CONFIRM          = GOSSIP_ACTION_INFO_DEF + 7,
        GOSSIP_QUIT             = GOSSIP_ACTION_INFO_DEF + 8,
        GOSSIP_MAIN_MENU        = GOSSIP_ACTION_INFO_DEF + 9,
    };

    ListPage PageCat, PageItem, CatSelect;


    // En cas d'erreur interne (produite uniquement en cas de reload
    // durant une transaction d'un joueur), reinitialisation de l'interface.
    void AffichErreur(Player * const pPlayer, Creature * const pCreature)
    {
        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_NOTHING, pCreature->GetGUID());
    }

    // Fonction permettant de faire un lancer de des en fonction d'une probabilite.
    bool GagneJet(const uint8 &SuccesProbability)
    {
        return urand(1, 100) <= SuccesProbability;
    }

    // Fonction d'affichage des categories
    void AffichCat(Player * const pPlayer, Creature * const pCreature)
    {
        const Vendor * pVendor = GetVendor(pCreature->GetEntry());
        if (!pVendor || pVendor->m_VCategories.empty())
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        Vendor::VCategorie_t VCat = pVendor->m_VCategories;
        Vendor::VCategorie_t::size_type i = PageCat[pPlayer] * NB_ITEM_PAGE;

        // Affichage Page Precedente.
        if (PageCat[pPlayer] > 0)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, PREV_PAGE, GOSSIP_PREV_PAGEC, 0);

        for ( ; i < VCat.size() && i < (NB_ITEM_PAGE * (PageCat[pPlayer] + 1)); ++i)
        {
            const Categorie * pCat = GetCategorie(VCat[i]);
            if (!pCat)
            {
                AffichErreur(pPlayer, pCreature);
                return;
            }

            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, pCat->GetName().c_str(), GOSSIP_SHOW_ITEMS, i);
        }

        // Affichage Page Suivante.
        if (i < VCat.size())
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, NEXT_PAGE, GOSSIP_NEXT_PAGEC, 0);

        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_INVITEC, pCreature->GetGUID());
    }

    // Fonction d'affichage des items d'une categorie
    void AffichItem(Player * const pPlayer, Creature * const pCreature)
    {
        const Vendor * pVendor = GetVendor(pCreature->GetEntry());
        if (!pVendor || pVendor->m_VCategories.empty())
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        Vendor::VCategorie_t VCat = pVendor->m_VCategories;
        Categorie::VIRecompense_t::size_type i = PageItem[pPlayer] * NB_ITEM_PAGE;

        const Categorie * pCat = GetCategorie(VCat[CatSelect[pPlayer]]);
        if (!pCat)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }
    
        // Affichage Page Precedente.
        if (PageItem[pPlayer] > 0)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, PREV_PAGE, GOSSIP_PREV_PAGEI, 0);

        for ( ; i < pCat->size() && i < (NB_ITEM_PAGE * (PageItem[pPlayer] + 1)); ++i)
        {
            const ItemRecompense * pRec = pCat->GetItem(i);
            if (!pRec)
            {
                AffichErreur(pPlayer, pCreature);
                return;
            }

            const ItemSD2P * pItem = GetItemSD2P(pRec->m_Entry);
            if (!pItem)
            {
                AffichErreur(pPlayer, pCreature);
                return;
            }

            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, pItem->m_Name.c_str(), GOSSIP_CONFIRM, i);
        }

        // Affichage Page Suivante.
        if (i < pCat->size())
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, NEXT_PAGE, GOSSIP_NEXT_PAGEI, 0);

        // Affichage lien vers menu Principal.
        if (VCat.size() > 1)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, MAIN_MENU, GOSSIP_MAIN_MENU, 0);

        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_INVITER, pCreature->GetGUID());
    }

    // Fonction de verification avant achat
    void ActionAchat(Player * const pPlayer, Creature * const pCreature, const uint32 &id)
    {
        const Vendor * pVendor = GetVendor(pCreature->GetEntry());
        if (!pVendor || pVendor->m_VCategories.empty())
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const Categorie * pCat = GetCategorie(pVendor->m_VCategories[CatSelect[pPlayer]]);
        if (!pCat)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const ItemRecompense * pRec  = pCat->GetItem(id);
        if (!pRec)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const ExtendedCost * pExtCost = GetExtendedCost(pRec->m_ExtendedCost);
        if (!pExtCost)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        ostringstream oss;

        // Verification si le joueur possede deja l'item.
        Item * pItem = Item::CreateItem(pRec->m_Entry, 1, NULL);
        if (!pItem)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }
        if (pPlayer->HasItemCount(pRec->m_Entry, pItem->GetMaxStackCount(), true))
        {
            oss << "　你已经拥有该物品　[" << GetItemName(pRec->m_Entry) << "]　";
            pCreature ->MonsterSay(oss.str().c_str(), pPlayer->GetGUID());
            return;
        }

        // Verification si le joueur peut obtenir l'item.
        for (ExtendedCost::VCost_t::const_iterator It = pExtCost->m_VCost.begin(); It != pExtCost->m_VCost.end(); ++It)
        {
            if (!pPlayer->HasItemCount(It->m_Entry, It->m_Count))
            {
                oss << "　你没有足够的兑换物品　[" << GetItemName(It->m_Entry) << "]　";
                pCreature->MonsterSay(oss.str().c_str(), pPlayer->GetGUID());
                return;
            }
        }

        // Ajout de l'item dans l'inventaire du joueur si le joueur gagne le jet
        // et si il a assez de place disponible.
        if (GagneJet(pRec->m_SuccesProbability))
        {
            if (!AddItem(pPlayer, pRec->m_Entry, 1))
            {
                pCreature->MonsterSay("　你已经不能再拿更多的这种物品了!　", pPlayer->GetGUID());
                return;
            }

			oss << "　恭喜你获得了物品　[" << GetItemName(pRec->m_Entry) << "]　";
            pCreature->MonsterSay(oss.str().c_str(), pPlayer->GetGUID());
        }
        else
        {
            pCreature->MonsterSay("　非常遗憾的告诉您，您输掉了！但请别灰心，加油合成，总有一天会成功的~~~!　", pPlayer->GetGUID());
        }

        // Retrait des items servant d'echange.
        for (ExtendedCost::VCost_t::const_iterator It = pExtCost->m_VCost.begin(); It != pExtCost->m_VCost.end(); ++It)
        {
            pPlayer->DestroyItemCount(It->m_Entry, It->m_Count, true);
        }
    }

    // Fonction de demande de confirmation d'un achat
    void Confirmation(Player * const pPlayer, Creature * const pCreature, const uint32 &id)
    {
        const Vendor * pVendor = GetVendor(pCreature->GetEntry());
        if (!pVendor || pVendor->m_VCategories.empty())
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const Categorie * pCat = GetCategorie(pVendor->m_VCategories[CatSelect[pPlayer]]);
        if (!pCat)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const ItemRecompense * pRec = pCat->GetItem(id);
        if (!pRec)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        const ExtendedCost * pExtCost = GetExtendedCost(pRec->m_ExtendedCost);
        if (!pExtCost)
        {
            AffichErreur(pPlayer, pCreature);
            return;
        }

        // Affichage de la proposition.
        ostringstream oss;
		oss << "[兑换物品] \n" << GetItemName(pRec->m_Entry) ;
        oss << "[" << uint32(pRec->m_SuccesProbability) << "% 成功率]\n ";
		oss << "\n[需求] \n";
        for (ExtendedCost::VCost_t::const_iterator It = pExtCost->m_VCost.begin(); It != pExtCost->m_VCost.end(); ++It)
			oss << GetItemName(It->m_Entry) << " [" << uint32(It->m_Count) << "个]\n";

        pPlayer->ADD_GOSSIP_ITEM(2, oss.str().c_str() , GOSSIP_ACHAT, id);
        pPlayer->ADD_GOSSIP_ITEM(2, "[退出] " , GOSSIP_QUIT, 0);
        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_VALID, pCreature->GetGUID());
    }
}

bool GossipHello_npc_recompense(Player * pPlayer, Creature * pCreature)
{
    // Re-initialisation des pages pour le joueur.
    PageCat[pPlayer] = PageItem[pPlayer] = CatSelect[pPlayer] = 0;

    // Verification si joueur en combat.
    if (pPlayer->isInCombat())
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->MonsterSay(" 　战斗中不能使用合成功能!", pPlayer->GetGUID());
        return true;
    }

    // Affichage des categories.
    AffichCat(pPlayer, pCreature);
    return true;
}

bool GossipSelect_npc_recompense(Player * pPlayer, Creature * pCreature, uint32 Sender, uint32 Param)
{
    switch(Sender) 
    {
      // Affichage des items d'une categorie.
      case GOSSIP_SHOW_ITEMS:
        CatSelect[pPlayer] = Param;
        AffichItem(pPlayer, pCreature);
        break;

      // Page de categories precedente.
      case GOSSIP_PREV_PAGEC:
        --PageCat[pPlayer];
        AffichCat(pPlayer, pCreature);
        break;

      // Page de categories suivante.
      case GOSSIP_NEXT_PAGEC:
        ++PageCat[pPlayer];
        AffichCat(pPlayer, pCreature);
        break;

      // Page d'items precedente.
      case GOSSIP_PREV_PAGEI:
        --PageItem[pPlayer];
        AffichItem(pPlayer, pCreature);
        break;

      // Page d'items suivante.
      case GOSSIP_NEXT_PAGEI:
        ++PageItem[pPlayer];
        AffichItem(pPlayer, pCreature);
        break;

      // Achat.
      case GOSSIP_ACHAT:
        pPlayer->CLOSE_GOSSIP_MENU();
        ActionAchat(pPlayer, pCreature, Param);
        break;

      // Demande de confirmation avant achat.
      case GOSSIP_CONFIRM:
        Confirmation(pPlayer, pCreature, Param);
        break;

      // Affichage Menu Principal.
      case GOSSIP_MAIN_MENU:
        GossipHello_npc_recompense(pPlayer, pCreature);
        break;

      // Fin de conversation avec le NPC.
      case GOSSIP_QUIT:
        pPlayer->CLOSE_GOSSIP_MENU();
        break;
    }
    return true;
}


void AddSC_npc_recompense(void)
{
    Script * newscript;

    newscript = new Script;
    newscript->Name="sd2p_npc_recompense";
    newscript->pGossipHello =  &GossipHello_npc_recompense;
    newscript->pGossipSelect = &GossipSelect_npc_recompense;
    newscript->RegisterSelf();
}
