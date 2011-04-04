/**
 *
 * @File : sd2p_sc_npc_teleport.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.0
 *
 **/


#ifndef SC_NPC_TELEPORT_H
#define SC_NPC_TELEPORT_H

#include <vector>
#include "sd2p_util.h"

#define SD2P_NPC_TELE_NAMESPACE       nsNpcTele
#define SD2P_NPC_TELE_NAMESPACE_DEB   namespace SD2P_NPC_TELE_NAMESPACE {
#define SD2P_NPC_TELE_NAMESPACE_FIN   }

SD2P_NAMESPACE_DEB
SD2P_NPC_TELE_NAMESPACE_DEB


/***************
 * ENUMERATIONS
 ***************/

// Differents types de permissions
enum Flag_t
{
    FLAG_TEAM       = 0,
    FLAG_GUILD      = 1,
    FLAG_GMLEVEL    = 2,
    FLAG_ISGM       = 3,
    FLAG_ACCOUNT    = 4,
    FLAG_LEVEL      = 5,
    FLAG_ITEM       = 6,
    FLAG_QUEST      = 7,
    FLAG_GENDER     = 8,
    FLAG_RACE       = 9,
    FLAG_CLASS      = 10,
    FLAG_REPUTATION = 11,
    FLAG_PLAYER     = 12,
    MAX_FLAG,
};

// Differents parametres de FLAG_TEAM
enum
{
    TEAM_ALL        = 0,
    TEAM_ALLIANCE   = 1,
    TEAM_HORDE      = 2,
    MAX_FLAG_TEAM,
};


/************************
 * STRUCTURES ET CLASSES
 ************************/

class Categorie
{
  public:
    Categorie(const uint32 &Id, const std::string &Name,
              const Flag_t &Flag, const uint64 &Data0, const uint32 &Data1);

    void AddDest(const uint32 &DestId);
    const Destination * GetDest(const uint32 &DestId) const;
    std::string GetName(const bool IsGM = false) const;
    uint32 size(void) const;
    bool IsAllowedToTeleport(Player * const pPlayer) const;

  private:
    uint32              m_Id;
    std::string         m_Name;
    Flag_t              m_Flag;
    uint64              m_Data0;
    uint32              m_Data1;
    std::vector<uint32> m_VDest;
};


/************
 * FONCTIONS
 ************/

bool IsValidData(const uint32 &CatId, const Flag_t &Flag, const uint64 &Data0, const uint32 &Data1);
uint32 CountOfCategoryAllowedBy(Player * const pPlayer);
void LoadDatabase(void);


/******************
 * TYPE DE DONNEES
 ******************/

typedef std::vector<Categorie> VCategorie_t;


/*********************************
 * DECLARATION VARIABLES GLOBALES
 *********************************/

extern VCategorie_t VCategorie;


SD2P_NPC_TELE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN

#endif /* SC_NPC_TELEPORT_H */
