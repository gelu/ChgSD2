/**
 *
 * @File : sd2p_sc_npc_recompense.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 2.1
 *
 * @Synopsis : Definition de la classe Recompense permettant d'aider au developpement du script npc_recompense.cpp.
 *
 **/


#ifndef SC_RECOMPENSE_H
#define SC_RECOMPENSE_H

#include <vector>
#include "sd2p_util.h"

#define SD2P_RECOMPENSE_NAMESPACE       nsRecompense
#define SD2P_RECOMPENSE_NAMESPACE_DEB   namespace SD2P_RECOMPENSE_NAMESPACE {
#define SD2P_RECOMPENSE_NAMESPACE_FIN   }

SD2P_NAMESPACE_DEB
SD2P_RECOMPENSE_NAMESPACE_DEB


/***************
 * ENUMERATIONS
 ***************/

enum
{
    NB_ITEMS_EXTENDED_COST = 4,
};


/************************
 * STRUCTURES ET CLASSES
 ************************/

struct Vendor
{
    typedef std::vector<uint32> VCategorie_t;

    uint32       m_Entry;
    VCategorie_t m_VCategories;
};

struct ItemSD2P
{
    uint32      m_Entry;
    std::string m_Name;
};

struct ExtendedCost
{
    struct Cost
    {
        uint32 m_Entry;
        uint8  m_Count;
    };
    
    typedef std::vector<Cost> VCost_t;

    ExtendedCost(const uint32 &IdExtendedCost);

    uint32  m_IdExtendedCost;
    VCost_t m_VCost;
};

struct ItemRecompense
{
    uint32 m_Entry;
    uint32 m_ExtendedCost;
    uint8  m_SuccesProbability;
};

class Categorie
{
  public:
    Categorie(const uint32 &Id, const std::string &Name);

    void AddItem(const ItemRecompense &Item);
    ItemRecompense const * GetItem(const uint32 &Id) const;
    std::string GetName(void) const;
    uint32 GetCatId(void) const;
    uint32 size(void) const;

    typedef std::vector<ItemRecompense> VIRecompense_t;

  private:
    uint32         m_Id;
    std::string    m_Name;
    VIRecompense_t m_VIRecompense;
};


/************
 * FONCTIONS
 ************/

Categorie const * GetCategorie(const uint32 &Id);
Vendor const * GetVendor(const uint32 &Entry);
ItemSD2P const * GetItemSD2P(const uint32 &Entry);
ExtendedCost const * GetExtendedCost(const uint32 &Id);
std::string GetItemName(const uint32 &Entry);
bool IsValidExtendedCost(const ExtendedCost & ExCost);
void LoadDatabase(void);


/******************
 * TYPE DE DONNEES
 ******************/

typedef UNORDERED_MAP<uint32, Vendor>       MVendor_t;
typedef UNORDERED_MAP<uint32, Categorie>    MCategorie_t;
typedef UNORDERED_MAP<uint32, ExtendedCost> MExtendedCost_t;
typedef UNORDERED_MAP<uint32, ItemSD2P>     MItemSD2P_t;


/*********************************
 * DECLARATION VARIABLES GLOBALES
 *********************************/

extern MVendor_t       MVendor;
extern MCategorie_t    MCategorie;
extern MExtendedCost_t MExtendedCost;
extern MItemSD2P_t     MItemSD2P;


SD2P_RECOMPENSE_NAMESPACE_FIN
SD2P_NAMESPACE_FIN

#endif /* SC_RECOMPENSE_H */
