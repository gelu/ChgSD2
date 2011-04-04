/**
 *
 * @File : sd2p_sc_object_teleport.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.0
 *
 **/


#ifndef SC_OBJECT_TELEPORT_H
#define SC_OBJECT_TELEPORT_H

#include <vector>
#include "sd2p_util.h"

#define SD2P_OBJECT_TELE_NAMESPACE       nsObjectTele
#define SD2P_OBJECT_TELE_NAMESPACE_DEB   namespace SD2P_OBJECT_TELE_NAMESPACE {
#define SD2P_OBJECT_TELE_NAMESPACE_FIN   }

SD2P_NAMESPACE_DEB
SD2P_OBJECT_TELE_NAMESPACE_DEB


/***************
 * ENUMERATIONS
 ***************/

// Definition des relations de la DB (les tables)
enum Relation
{
    RELATION_00    = 0,
    RELATION_01    = 1,
    MAX_RELATION,
};

// Definition des types de permissions pour la relation0
enum Relation00_Flag
{
    FLAG_ISPLAYER   = 0,
    FLAG_GMLEVEL    = 1,
    FLAG_ISGM       = 2,
    FLAG_LEVEL      = 3,
    FLAG_ITEM       = 4,
    FLAG_QUEST      = 5,
    FLAG_REPUTATION = 6,
    MAX_RELATION00_FLAG,
};

// Definition des types de permissions pour la relation1
enum Relation01_Flag
{
    FLAG_TEAM       = 0,
    FLAG_GUILD      = 1,
    FLAG_ACCOUNT    = 2,
    FLAG_GENDER     = 3,
    FLAG_RACE       = 4,
    FLAG_CLASS      = 5,
    FLAG_PLAYER     = 6,
    MAX_RELATION01_FLAG,
};

// Differents parametres de FLAG_TEAM
enum
{
    TEAM_ALLIANCE   = 1,
    TEAM_HORDE      = 2,
    MAX_FLAG_TEAM,
};


/******************
 * TYPE DE DONNEES
 ******************/

class  DefinitionObject;
struct DestObjectTele;

typedef UNORDERED_MAP<uint32, DefinitionObject> MDefObject_t;
typedef std::vector<DestObjectTele>             VDestObjectTele_t;


/************************
 * STRUCTURES ET CLASSES
 ************************/

struct DestObjectTele
{
    uint32 m_DestId;
    uint64 m_Data0;
    uint32 m_Data1;

    bool IsValid(const DefinitionObject &DefObject, Player * const pPlayer) const;
};

class DefinitionObject
{
  public:
    DefinitionObject(const uint32 &Entry, const Relation &Relation, const uint8 &Flag);

    void  AddDest(const DestObjectTele &Dest);
    const Destination * GetDest(const uint32 &DestId) const;
    Relation GetRelation(void) const;
    uint8  GetFlag(void) const;
    uint32 GetEntry(void) const;
    uint32 size(void) const;
    const Destination * GetAllowedDestination(Player * const pPlayer) const;


  private:
    uint32          m_Entry;
    Relation        m_Relation;
    uint8           m_Flag;
    VDestObjectTele_t m_VDestObjectTele;
};


/************
 * FONCTIONS
 ************/

bool IsValidData(const uint32 &Entry, const Relation &Table, const uint8 &Flag,
                 const uint64 &Data0, const uint32 &Data1);
DefinitionObject const * GetDefinitionObject(const uint32 &Entry);
void LoadDatabase(void);


/*********************************
 * DECLARATION VARIABLES GLOBALES
 *********************************/

extern MDefObject_t MDefObject;


SD2P_OBJECT_TELE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN

#endif /* SC_OBJECT_TELEPORT_H */
