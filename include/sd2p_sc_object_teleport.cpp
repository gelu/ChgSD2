/**
 *
 * @File : sd2p_sc_object_teleport.cpp
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

SD2P_NAMESPACE_DEB
SD2P_OBJECT_TELE_NAMESPACE_DEB


/********************************
 * DEFINITION VARIABLES GLOBALES
 ********************************/

MDefObject_t MDefObject;


/************************
 * STRUCTURES ET CLASSES
 ************************/

#define DEFIT  DefinitionObject

DEFIT::DefinitionObject(const uint32 &Entry, const Relation &Relation, const uint8 &Flag)
    : m_Entry(Entry), m_Relation(Relation), m_Flag(Flag)
{
}

void DEFIT::AddDest(const DestObjectTele &Dest)
{
    m_VDestObjectTele.push_back(Dest);
}

const Destination * DEFIT::GetDest(const uint32 &DestId) const
{
    return GetDestination(m_VDestObjectTele[DestId].m_DestId);
}

uint8 DEFIT::GetFlag(void) const
{
    return m_Flag;
}

uint32 DEFIT::GetEntry (void) const
{
    return m_Entry;
}

uint32 DEFIT::size(void) const
{
    return m_VDestObjectTele.size();
}

Relation DEFIT::GetRelation(void) const
{
    return m_Relation;
}

const Destination * DEFIT::GetAllowedDestination(Player * const pPlayer) const
{
    for (VDestObjectTele_t::const_iterator It = m_VDestObjectTele.begin(); It != m_VDestObjectTele.end(); ++It)
    {
        if (It->IsValid(*this, pPlayer))
        {
            const Destination * pDest = GetDestination(It->m_DestId);
            if (pDest) return pDest;
            outstring_log("SD2P >> [Object Tele] Destination invalide (Id: %u).",It->m_DestId);
        }
    }
    return NULL;
}

#undef DEFIT

bool DestObjectTele::IsValid(const DefinitionObject &DefIt, Player * const pPlayer) const
{
    switch (DefIt.GetRelation())
    {
      case RELATION_00:
        switch (DefIt.GetFlag())
        {
          case FLAG_ISPLAYER:
            return true;

          case FLAG_GMLEVEL:
            return pPlayer->GetSession()->GetSecurity() >= m_Data0;

          case FLAG_ISGM:
            return pPlayer->isGameMaster();

          case FLAG_LEVEL:
            return pPlayer->getLevel() >= m_Data0;

          case FLAG_ITEM:
            return pPlayer->HasItemCount(m_Data0, m_Data1, true);

          case FLAG_QUEST:
            if (m_Data1 < MAX_QUEST_STATUS)
                return pPlayer->GetQuestStatus(m_Data0) == m_Data1;
            return pPlayer->GetQuestRewardStatus(m_Data0);

          case FLAG_REPUTATION:
            return pPlayer->GetReputationRank(m_Data0) >= m_Data1;
        }
        return false;

      case RELATION_01:
        switch (DefIt.GetFlag())
        {
          case FLAG_TEAM:
            switch (m_Data0)
            {
              case TEAM_HORDE:      return pPlayer->GetTeam() == HORDE;
              case TEAM_ALLIANCE:   return pPlayer->GetTeam() == ALLIANCE;
            }
            return false;

          case FLAG_GUILD:
            return pPlayer->GetGuildId() == m_Data0;

          case FLAG_ACCOUNT:
            return pPlayer->GetSession()->GetAccountId() == m_Data0;

          case FLAG_GENDER:
            return pPlayer->getGender() == m_Data0;

          case FLAG_RACE:
            return pPlayer->getRace() == m_Data0;

          case FLAG_CLASS:
            return pPlayer->getClass() == m_Data0;

          case FLAG_PLAYER:
            return pPlayer->GetGUID() == m_Data0;
        }
        return false;
    }
    return false;
}

DefinitionObject const * GetDefinitionObject(const uint32 &Entry)
{
    MDefObject_t::const_iterator It = MDefObject.find(Entry);
    return (It != MDefObject.end() ? &It->second : NULL);
}

bool IsValidData(const uint32 &Entry, const Relation &Table, const uint8 &Flag,
                 const uint64 &Data0, const uint32 &Data1)
{
    if (!Entry || 0 > Table || Table >= MAX_RELATION)
        return false;

    switch (Table)
    {
      case RELATION_00:
        switch (Flag)
        {
          case FLAG_ISPLAYER:
          case FLAG_ISGM:
            if (Data0)
                error_log("SD2P >> [Object Tele] Data0 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            return true;

          case FLAG_GMLEVEL:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (0 < Data0 && Data0 < 256)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (GmLevel) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_LEVEL:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (0 < Data0 && Data0 < 256)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (Level) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_ITEM:
            if (!Data0)
            {
                error_log("SD2P >> [Object Tele] Data0 invalide (ObjectID) (Object: %u / Relation: %u).", Entry, Table);
                return false;
            }
            if (Data1)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (ObjectCount) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_QUEST:
            if (!Data0)
            {
                error_log("SD2P >> [Object Tele] Data0 invalide (QuestId) (Object: %u / Relation: %u).", Entry, Table);
                return false;
            }
            if (Data1 < MAX_QUEST_STATUS + 1)
                return true;
            error_log("SD2P >> [Object Tele] Data1 invalide (QuestStatut) (Object: %u / Relation: %u).", Entry, Table);
            return false;
        }
        error_log("SD2P >> [Object Tele] Flag invalide (Object: %u / Flag: %u).", Entry, Flag);
        return false;

      case RELATION_01:
        switch (Flag)
        {
          case FLAG_TEAM:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (0 < Data0 && Data0 < MAX_FLAG_TEAM)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (Team) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_GUILD:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (Data0)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (GuildID) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_ACCOUNT:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (Data0)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (AccountID) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_GENDER:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (Data0 < GENDER_NONE)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (Gender) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_RACE:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (0 < Data0 && Data0 < MAX_RACES)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (Race) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_CLASS:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (0 < Data0 && Data0 < MAX_CLASSES)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (Class) (Object: %u / Relation: %u).", Entry, Table);
            return false;

          case FLAG_PLAYER:
            if (Data1)
                error_log("SD2P >> [Object Tele] Data1 invalide (Object: %u / Relation: %u).", Entry, Table);
            if (Data0)
                return true;
            error_log("SD2P >> [Object Tele] Data0 invalide (PlayerGUID) (Object: %u / Relation: %u).", Entry, Table);
            return false;
        }
        error_log("SD2P >> [Object Tele] Flag invalide (Object: %u / Flag: %u).", Entry, Flag);
        return false;
    }
    error_log("SD2P >> [Object Tele] Relation invalide (Object: %u / Relation: %u).", Entry, Table);
    return false;
}

void LoadDatabase(void)
{
    // Verification integrite des donnees de la DB
    QueryResult * pResult = SD2Database.PQuery(
    "  SELECT 0 NumRel, T.Entry Entry"
    "  FROM sd2p_object_tele_relation0 T, sd2p_object_tele_definition D "
    "  WHERE T.Entry = D.Entry "
    "    AND relation_id <> 0 "
    "UNION "
    "  SELECT 1 NumRel, T.Entry Entry"
    "  FROM sd2p_object_tele_relation1 T, sd2p_object_tele_definition D "
    "  WHERE T.Entry = D.Entry "
    "    AND relation_id <> 1 "
    "ORDER BY NumRel, Entry");

    if (pResult)
    {
        outstring_log("SD2: ERREUR >> %u objects incorrectement configures...", pResult->GetRowCount());
        do
        {
            Field * pFields = pResult->Fetch();
            outstring_log("\tIdObject : %u (Relation invalide : %u)",
                          pFields[0].GetUInt32(), pFields[1].GetUInt8());
        } while (pResult->NextRow());

        delete pResult;
    }

    // Recuperation des donnees de la DB
    pResult = SD2Database.PQuery(
    "  SELECT relation_id, flag, Data0, Data1, T.Entry Entry, DestId "
    "  FROM sd2p_object_tele_definition D, sd2p_object_tele_relation0 T "
    "  WHERE D.Entry = T.Entry "
    "    AND relation_id = 0 "
    "UNION "
    "  SELECT relation_id, flag, Data0, Data1, T.Entry Entry, DestId "
    "  FROM sd2p_object_tele_definition D, sd2p_object_tele_relation1 T "
    "  WHERE D.Entry = T.Entry "
    "    AND relation_id = 1 "
    "ORDER BY relation_id, Entry, flag, Data0 DESC ");
    MDefObject.clear();

    if (pResult)
    {
        outstring_log( "SD2P: Chargement \"sd2p_object_tele_definition\" et \"sd2p_object_tele_relation0\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 IdObject = 0;
        uint32 NbDest = 0;
        DefinitionObject * pLastDefIt;

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (!GetDestination(pFields[5].GetUInt32()))
            {
                outstring_log("SD2P >> Destination introuvable (DestID: %u).", pFields[0].GetUInt32());
                continue;
            }

            if (!IsValidData(pFields[4].GetUInt32(), (Relation)pFields[0].GetUInt8(),
                             pFields[1].GetUInt32(), pFields[2].GetUInt64(), pFields[3].GetUInt32()))
                continue;

            if (IdObject != pFields[4].GetUInt32())
            {
                IdObject = pFields[4].GetUInt32();
                DefinitionObject DefIt(IdObject, (Relation)pFields[0].GetUInt8(), pFields[1].GetUInt8());
                pLastDefIt = &MDefObject.insert(MDefObject_t::value_type(IdObject, DefIt)).first->second;
            }

            DestObjectTele Dest =
            {
                pFields[5].GetUInt32(),      // DestId
                pFields[2].GetUInt64(),      // Data0
                pFields[3].GetUInt32(),      // Data1
            };

            pLastDefIt->AddDest(Dest);
            ++NbDest;
        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u object_teleport charge(s).", NbDest);
    } else outstring_log("ATTENTION >> 0 object_teleport charge.");
}


SD2P_OBJECT_TELE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN
