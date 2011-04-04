/**
 *
 * @File : sd2_projects_util.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : Declaration de fonctions/classes/enumerations/variables communes aux projets.
 *
 **/

#ifndef SD2P_UTIL_H_
#define SD2P_UTIL_H_

#define SD2P_NAMESPACE       nsSD2P
#define SD2P_NAMESPACE_DEB   namespace SD2P_NAMESPACE {
#define SD2P_NAMESPACE_FIN   }

#include "ProgressBar.h"

SD2P_NAMESPACE_DEB


/***************
 * ENUMERATIONS
 ***************/

enum
{
    NB_ITEM_PAGE   = 10,   // Nombre de propositions par page (NPC Teleporteur/NPC Recompense).
};


/************************
 * STRUCTURES ET CLASSES
 ************************/

// Correspond a une destination de teleportation.
// Projet : NPC/Item/Go Teleporteur, NPC Gardien de Guilde
struct Destination
{
    uint32      m_Id;
    std::string m_Name;
    float       m_X, m_Y, m_Z, m_Orient;
    uint16      m_Map;
    uint8       m_Level;
    uint32      m_Cost;
};

// Classe permettant d'associer une variable type entier a un joueur.
// Necessaire pour eviter un conflit entre joueur avec le systeme de gestion de pages.
class ListPage
{
  protected:
    class Page
    {
      public:
        Page(const uint32 &NumPage = 0);

        Page & operator =  (const uint32 &Id);
        Page & operator =  (const Page &Page);
        Page & operator ++ (void);
        Page   operator ++ (int32);
        Page & operator -- (void);
        Page   operator -- (int32);
        operator int ();

        uint32 GetNumPage(void) const;

      private:
        uint32 m_NumPage;
    };

    typedef UNORDERED_MAP<uint64, Page *> MPlayer2Page_t;

  public:
    ListPage(void);
    ~ListPage(void);

    Page & operator [] (Player * const pPlayer);

  private:
    MPlayer2Page_t m_MPlayer2Page;
};


/************
 * FONCTIONS
 ************/

// Recuperation d'une destination.
const Destination * GetDestination(const uint32 &DestId);

// Permet l'ajout d'item(s) a un joueur.
bool AddItem(Player * pPlayer, uint32 IdItem, uint32 Count = 1);

// Permet de teleporter un joueur.
void Teleport(Player * const pPlayer, const uint16 &Map,
              const float &X, const float &Y, const float &Z, const float &Orient);

// Conversion "Objet T -> chaine de caracteres".
template <typename T> std::string ToString(const T &Val);

// Conversion "Valeur monetaire -> chaine de caracteres".
std::string ConvertMoney(const uint32 &Money);

// Chargement de la relation contenant les destinations.
void LoadDestination(void);


/******************
 * TYPE DE DONNEES
 ******************/

typedef UNORDERED_MAP<uint32, Destination> MDestination_t;


/*********************************
 * DECLARATION VARIABLES GLOBALES
 *********************************/

extern MDestination_t MDestination;


SD2P_NAMESPACE_FIN

#include "sd2p_util.hpp"

extern DatabaseType SD2Database;

#endif /* SD2P_UTIL_H_ */
