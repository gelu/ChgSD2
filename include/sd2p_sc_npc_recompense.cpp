/**
 *
 * @File : sd2p_sc_npc_recompense.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.1
 *
 **/


#include "precompiled.h"
#include "sd2p_sc_npc_recompense.h"

SD2P_NAMESPACE_DEB
SD2P_RECOMPENSE_NAMESPACE_DEB


/********************************
 * DEFINITION VARIABLES GLOBALES
 ********************************/

MVendor_t       MVendor;
MCategorie_t    MCategorie;
MExtendedCost_t MExtendedCost;
MItemSD2P_t     MItemSD2P;


/************************
 * STRUCTURES ET CLASSES
 ************************/

ExtendedCost::ExtendedCost(const uint32 &IdExtendedCost)
    : m_IdExtendedCost(IdExtendedCost)
{
    m_VCost.reserve(NB_ITEMS_EXTENDED_COST);
}

#define CAT  Categorie

CAT::Categorie(const uint32 &Id, const std::string &Name)
    : m_Id(Id), m_Name(Name)
{
}

void CAT::AddItem(const ItemRecompense &Item)
{
    m_VIRecompense.push_back(Item);
}

ItemRecompense const * CAT::GetItem(const uint32 &Id) const
{
    return &m_VIRecompense[Id];
}

std::string CAT::GetName(void) const
{
    return m_Name;
}

uint32 CAT::GetCatId(void) const
{
    return m_Id;
}

uint32 CAT::size(void) const
{
    return m_VIRecompense.size();
}

#undef CAT


/************
 * FONCTIONS
 ************/

Categorie const * GetCategorie(const uint32 &Id)
{
    MCategorie_t::const_iterator It = MCategorie.find(Id);
    return (It != MCategorie.end() ? &It->second : NULL);
}

Vendor const * GetVendor(const uint32 &Entry)
{
    MVendor_t::const_iterator It = MVendor.find(Entry);
    return (It != MVendor.end() ? &It->second : NULL);
}

ItemSD2P const * GetItemSD2P(const uint32 &Entry)
{
    MItemSD2P_t::const_iterator It = MItemSD2P.find(Entry);
    return (It != MItemSD2P.end() ? &It->second : NULL);
}

ExtendedCost const * GetExtendedCost(const uint32 &Id)
{
    MExtendedCost_t::const_iterator It = MExtendedCost.find(Id);
    return (It != MExtendedCost.end() ? &It->second : NULL);
}

std::string GetItemName(const uint32 &Entry)
{
    const ItemSD2P * pItem = GetItemSD2P(Entry);
    if (pItem) return pItem->m_Name;
    outstring_log("SD2P >> [NPC Recompense] Item invalide (Entry: %u).", Entry);
    return "<Erreur>";
}

bool IsValidExtendedCost(const ExtendedCost & ExCost)
{
    if (ExCost.m_VCost.empty() || ExCost.m_VCost.size() > NB_ITEMS_EXTENDED_COST)
        return false;

    for (ExtendedCost::VCost_t::const_iterator It = ExCost.m_VCost.begin(); It < ExCost.m_VCost.end(); ++It)
    {
        if (!It->m_Count || !GetItemSD2P(It->m_Entry))
            return false;
    }
    return true;
}

void LoadDatabase(void)
{
    // Chargement des categories.
    QueryResult * pResult = SD2Database.PQuery("SELECT catid, name FROM sd2p_recompense_category");
    MCategorie.clear();

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_recompense_category\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 NbCat = pResult->GetRowCount();

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            Categorie Cat (pFields[0].GetUInt32(), pFields[1].GetCppString());
            MCategorie.insert(MCategorie_t::value_type(pFields[0].GetUInt32(), Cat));

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u recompense_categorie charge(s).", NbCat);
    } else outstring_log("ATTENTION >> 0 recompense_categorie charge.");


    // Chargement des differents vendeur.
    pResult = SD2Database.PQuery("SELECT creature_id, catid FROM sd2p_recompense_vendor");
    MVendor.clear();

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_recompense_vendor\"...");
        barGoLink Bar(pResult->GetRowCount());

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (!GetCategorie(pFields[1].GetUInt32()))
            {
                outstring_log("SD2P >> Categorie introuvable (CatId: %u).", pFields[1].GetUInt32());
                continue;
            }

            Vendor * pVendor = const_cast<Vendor*>(GetVendor(pFields[0].GetUInt32()));
            if (!pVendor)
            {
                Vendor Vendeur;
                Vendeur.m_Entry = pFields[0].GetUInt32();
                pVendor = &MVendor.insert(MVendor_t::value_type(pFields[0].GetUInt32(), Vendeur)).first->second;
            }

            pVendor->m_VCategories.push_back(pFields[1].GetUInt32());

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u recompense_vendor charge(s).", MVendor.size());
    } else outstring_log("ATTENTION >> 0 recompense_vendor charge.");


    // Chargement des items.
    pResult = SD2Database.PQuery("SELECT item_entry, name FROM sd2p_recompense_item");
    MItemSD2P.clear();

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_recompense_item\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 NbItem = pResult->GetRowCount();

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            Item * pItem = Item::CreateItem(pFields[0].GetUInt32(), 1, NULL);
            if (!pItem)
            {
                outstring_log("SD2P >> Item inexistant (Entry: %u).", pFields[0].GetUInt32());
                --NbItem;
                continue;
            }

            ItemSD2P ItemS = { pFields[0].GetUInt32(), pFields[1].GetCppString() };
            MItemSD2P.insert(MItemSD2P_t::value_type(pFields[0].GetUInt32(), ItemS));

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u recompense_item charge(s).", NbItem);
    } else outstring_log("ATTENTION >> 0 recompense_item charge.");


    // Chargement des extended_cost.
    pResult = SD2Database.PQuery(
        "SELECT id_extended_cost, item_entry1, item_count1, item_entry2, item_count2, "
        "       item_entry3, item_count3, item_entry4, item_count4 "
        "FROM sd2p_recompense_extended_cost");
    MExtendedCost.clear();

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_recompense_extended_cost\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 NbExtendedCost = pResult->GetRowCount();

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            ExtendedCost ExCost(pFields[0].GetUInt32());
            for (uint8 i = 0; i < NB_ITEMS_EXTENDED_COST; ++i)
            {
                ExtendedCost::Cost ItCost =
                {
                    pFields[i * 2 + 1].GetUInt32(),   // item_entry
                    pFields[i * 2 + 2].GetUInt32(),   // item_cost
                };

                if (!ItCost.m_Count && !ItCost.m_Entry)
                    continue;

                ExCost.m_VCost.push_back(ItCost);
            }

            if (!IsValidExtendedCost(ExCost))
            {
                outstring_log("SD2P >> ExtendedCost invalide (ExCostId: %u).", pFields[0].GetUInt32());
                --NbExtendedCost;
                continue;
            }

            MExtendedCost.insert(MExtendedCost_t::value_type(pFields[0].GetUInt32(), ExCost));

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u recompense_extended_cost charge(s).", NbExtendedCost);
    } else outstring_log("ATTENTION >> 0 recompense_extended_cost charge.");


    // Chargement des recompenses
    pResult = SD2Database.PQuery(
        "SELECT entry_recompense, extended_cost, succes_probability, catid "
        "FROM sd2p_recompense_content");

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_recompense_content\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 NbItem = pResult->GetRowCount();

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (0 > pFields[2].GetUInt32() || pFields[2].GetUInt32() > 100)
            {
                outstring_log("SD2P >> Probabilite invalide (CatId: %u / ItemID: %u).", pFields[3].GetUInt32(), pFields[0].GetUInt32());
                --NbItem;
                continue;
            }

            if (!GetExtendedCost(pFields[1].GetUInt32()))
            {
                outstring_log("SD2P >> ExtendedCost invalide (CatId: %u / ItemID: %u).", pFields[3].GetUInt32(), pFields[0].GetUInt32());
                --NbItem;
                continue;
            }

            Categorie * pCat = const_cast<Categorie*>(GetCategorie(pFields[3].GetUInt32()));
            if (!pCat)
            {
                outstring_log("SD2P >> Categorie inexistante (CatId: %u / ItemID: %u).", pFields[3].GetUInt32(), pFields[0].GetUInt32());
                --NbItem;
                continue;
            }

            if (!GetItemSD2P(pFields[0].GetUInt32()))
            {
                outstring_log("SD2P >> Item inexistant (CatId: %u / ItemID: %u).", pFields[3].GetUInt32(), pFields[0].GetUInt32());
                --NbItem;
                continue;
            }

            ItemRecompense IRec =
            {
                pFields[0].GetUInt32(),     // Entry
                pFields[1].GetUInt32(),     // ExtendedCost
                pFields[2].GetUInt32(),     // SuccesProbability
            };
            pCat->AddItem(IRec);

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u recompense_content charge(s).", NbItem);
    } else outstring_log("ATTENTION >> 0 recompense_content charge.");
}


SD2P_RECOMPENSE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN
