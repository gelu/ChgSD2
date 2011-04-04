/**
 *
 * @File : sd2p_sc_npc_teleport.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.0
 *
 **/


#include "precompiled.h"
#include "sd2p_sc_npc_teleport.h"

SD2P_NAMESPACE_DEB
SD2P_NPC_TELE_NAMESPACE_DEB


/********************************
 * DEFINITION VARIABLES GLOBALES
 ********************************/

VCategorie_t VCategorie;


/************************
 * STRUCTURES ET CLASSES
 ************************/

#define CAT  Categorie

CAT::Categorie(const uint32 &Id, const std::string &Name,
               const Flag_t &Flag, const uint64 &Data0, const uint32 &Data1)
    : m_Id(Id), m_Name(Name), m_Flag(Flag), m_Data0(Data0), m_Data1(Data1)
{
}

void CAT::AddDest(const uint32 &DestId)
{
    m_VDest.push_back(DestId);
}

const Destination * CAT::GetDest(const uint32 &DestId) const
{
    return GetDestination(m_VDest[DestId]);
}

uint32 CAT::size(void) const
{
    return m_VDest.size();
}

std::string CAT::GetName(const bool IsGM /*= false*/) const
{
    if (!IsGM || m_Flag != FLAG_TEAM)
        return m_Name;

    switch (m_Data0)
    {
      case TEAM_HORDE:      return std::string(m_Name + " (H)");
      case TEAM_ALLIANCE:   return std::string(m_Name + " (A)");
    }
    return m_Name;
}

bool CAT::IsAllowedToTeleport(Player * const pPlayer) const
{
    if (pPlayer->isGameMaster())
    {
        if (m_Flag == FLAG_GMLEVEL)
            return pPlayer->GetSession()->GetSecurity() >= m_Data0;
        return true;
    }

    switch (m_Flag)
    {
      case FLAG_TEAM:
        switch (m_Data0)
        {
          case TEAM_HORDE:      return pPlayer->GetTeam() == HORDE;
          case TEAM_ALLIANCE:   return pPlayer->GetTeam() == ALLIANCE;
          case TEAM_ALL:        return true;
        }

      case FLAG_GUILD:
        return pPlayer->GetGuildId() == m_Data0;

      case FLAG_GMLEVEL:
        return pPlayer->GetSession()->GetSecurity() >= m_Data0;

      case FLAG_ISGM:
        return pPlayer->isGameMaster();

      case FLAG_ACCOUNT:
        return pPlayer->GetSession()->GetAccountId() == m_Data0;

      case FLAG_LEVEL:
        return pPlayer->getLevel() >= m_Data0;

      case FLAG_ITEM:
        return pPlayer->HasItemCount(m_Data0, m_Data1, true);

      case FLAG_QUEST:
        if (m_Data1 < MAX_QUEST_STATUS)
            return pPlayer->GetQuestStatus(m_Data0) == m_Data1;
        return pPlayer->GetQuestRewardStatus(m_Data0);

      case FLAG_GENDER:
        return pPlayer->getGender() == m_Data0;

      case FLAG_RACE:
        return pPlayer->getRace() == m_Data0;

      case FLAG_CLASS:
        return pPlayer->getClass() == m_Data0;

      case FLAG_REPUTATION:
        return pPlayer->GetReputationRank(m_Data0) >= m_Data1;

      case FLAG_PLAYER:
        return pPlayer->GetGUID() == m_Data0;
    }

    error_log("SD2P >> [NPC Tele] Flag invalide (Flag: %u / Categorie: %u).", m_Flag, m_Id);
    return false;
}

#undef CAT


/************
 * FONCTIONS
 ************/

uint32 CountOfCategoryAllowedBy(Player * const pPlayer)
{
    uint32 count = 0;
    for (VCategorie_t::const_iterator It = VCategorie.begin(); It != VCategorie.end(); ++It)
    {   
        if (It->IsAllowedToTeleport(pPlayer))
            ++count;
    }
    return count;
}

bool IsValidData(const uint32 &CatId, const Flag_t &Flag,
                 const uint64 &Data0, const uint32 &Data1)
{
    switch(Flag)
    {
      case FLAG_TEAM:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (Data0 < MAX_FLAG_TEAM) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Team) (Categorie: %u).", CatId);
        return false;

      case FLAG_GUILD:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (Data0) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Guild) (Categorie: %u).", CatId);
        return false;

      case FLAG_GMLEVEL:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (0 < Data0 && Data0 < 256) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (GmLevel) (Categorie: %u).", CatId);
        return false;

      case FLAG_ISGM:
        if (Data0) error_log("SD2P >> [NPC Tele] Data0 invalide (Categorie: %u).", CatId);
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        return true;

      case FLAG_ACCOUNT:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (Data0) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Account) (Categorie: %u).", CatId);
        return false;

      case FLAG_LEVEL:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (0 < Data0 && Data0 < 256) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Level) (Categorie: %u).", CatId);
        return false;

      case FLAG_ITEM:
        if (!Data0)
        {
            error_log("SD2P >> [NPC Tele] Data0 invalide (Categorie: %u).", CatId);
            return false;
        }
        if (Data1) return true;
        error_log("SD2P >> [NPC Tele] Data1 invalide (ItemCount) (Categorie: %u).", CatId);
        return false;

      case FLAG_QUEST:
        if (!Data0)
        {
            error_log("SD2P >> [NPC Tele] Data0 invalide (QuestID) (Categorie: %u).", CatId);
            return false;
        }
        if (Data1 < MAX_QUEST_STATUS + 1) return true;
        error_log("SD2P >> [NPC Tele] Data1 invalide (Quest Statut) (Categorie: %u).", CatId);
        return false;

      case FLAG_GENDER:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (Data0 < GENDER_NONE) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Gender) (Categorie: %u).", CatId);
        return false;

      case FLAG_RACE:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (0 < Data0 && Data0 < MAX_RACES) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Race) (Categorie: %u).", CatId);
        return false;

      case FLAG_CLASS:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (0 < Data0 && Data0 < MAX_CLASSES) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Class) (Categorie: %u).", CatId);
        return false;

      case FLAG_REPUTATION:
        if (!Data0)
        {
            error_log("SD2P >> [NPC Tele] Data0 invalide (Faction/Reputation) (Categorie: %u).", CatId);
            return false;
        }
        if (Data1 <= REP_EXALTED) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (Faction/Reputation) (Categorie: %u).", CatId);
        return false;

      case FLAG_PLAYER:
        if (Data1) error_log("SD2P >> [NPC Tele] Data1 invalide (Categorie: %u).", CatId);
        if (Data0) return true;
        error_log("SD2P >> [NPC Tele] Data0 invalide (PlayerGuid) (Categorie: %u).", CatId);
        return false;
    }

    error_log("SD2P >> [NPC Tele] Flag invalide (Categorie: %u).", CatId);
    return false;
}

void LoadDatabase(void)
{
    QueryResult * pResult = SD2Database.PQuery(
        "SELECT flag, data0, data1, CatId, C.name, destid "
        "FROM sd2p_npc_tele_category C, sd2p_npc_tele_association A "
        "WHERE C.id = CatId ORDER BY C.name, CatId");
    VCategorie.clear();

    if (pResult)
    {
        outstring_log("SD2P: Chargement \"sd2p_npc_tele_category\" et \"sd2p_npc_tele_association\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 CatId  = 0;
        uint32 NbDest = 0;
        bool IsValidCat = true;
        bool FirstTime  = true;

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (!GetDestination(pFields[5].GetUInt32()))
            {
                outstring_log("SD2P >> Destination introuvable (DestID: %u).", pFields[5].GetUInt32());
                continue;
            }

            if (!IsValidCat && CatId == pFields[3].GetUInt32() && !FirstTime)
                continue;

            IsValidCat = true;
            FirstTime = false;

            if (!IsValidData(pFields[3].GetUInt32(), Flag_t(pFields[0].GetUInt8()),
                             pFields[1].GetUInt64(), pFields[2].GetUInt32()))
            {
                IsValidCat = false;
                CatId = pFields[3].GetUInt32();
                continue;
            }

            if (CatId != pFields[3].GetUInt32())
            {
                CatId = pFields[3].GetUInt32();
                Categorie Cat (CatId, pFields[4].GetCppString(), Flag_t(pFields[0].GetUInt8()),
                               pFields[1].GetUInt64(), pFields[2].GetUInt32());
                VCategorie.push_back(Cat);
            }

            VCategorie.back().AddDest(pFields[5].GetUInt32());
            ++NbDest;
        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u npc_teleport charge(s).", NbDest);
    } else outstring_log("WARNING >> 0 npc_teleport charge.");
}


SD2P_NPC_TELE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN
