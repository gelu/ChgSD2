/**
 *
 * @File : sd2_projects_util.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : Definitions de fonctions/classes/enumerations/variables communes aux projets.
 *
 **/

#include "precompiled.h"
#include "sd2p_util.h"

SD2P_NAMESPACE_DEB


/***************
 * ENUMERATIONS
 ***************/

enum
{
    SPELL_VISUAL_TELEPORT  = 35517,     // Simple effet visuel de teleportation.
};


/********************************
 * DEFINITION VARIABLES GLOBALES
 ********************************/
 
MDestination_t MDestination;


/************************
 * STRUCTURES ET CLASSES
 ************************/

#define LPAGE   ListPage
#define PAGE    LPAGE::Page

LPAGE::ListPage(void)
{
}

LPAGE::~ListPage(void)
{
    for (MPlayer2Page_t::iterator It = m_MPlayer2Page.begin(); It != m_MPlayer2Page.end(); ++It)
        delete It->second;
}

PAGE & LPAGE::operator [] (Player * const pPlayer)
{
    MPlayer2Page_t::const_iterator It = m_MPlayer2Page.find(pPlayer->GetGUID());
    if (It != m_MPlayer2Page.end()) return *It->second;
    return *m_MPlayer2Page.insert(MPlayer2Page_t::value_type(pPlayer->GetGUID(), new Page())).first->second;
}

PAGE::Page(const uint32 &NumPage /*= 0*/)
    : m_NumPage(NumPage)
{
}

uint32 PAGE::GetNumPage(void) const
{
    return m_NumPage;
}

PAGE & PAGE::operator = (const uint32 &Id)
{
    m_NumPage = Id;
    return *this;
}

PAGE & PAGE::operator = (const PAGE & Page)
{
    m_NumPage = Page.m_NumPage;
    return *this;
}

PAGE & PAGE::operator ++ (void)
{
    ++m_NumPage;
    return *this;
}

PAGE PAGE::operator ++ (int32)
{
    Page Tmp (*this);
    ++m_NumPage;
    return Tmp;
}

PAGE & PAGE::operator -- (void)
{
    --m_NumPage;
    return *this;
}

PAGE PAGE::operator -- (int32)
{
    Page Tmp (*this);
    --m_NumPage;
    return Tmp;
}

PAGE::operator int (void)
{
    return m_NumPage;
}

#undef PAGE
#undef LPAGE


/************
 * FONCTIONS
 ************/

Destination const * GetDestination(const uint32 &DestId)
{
    MDestination_t::const_iterator It = MDestination.find(DestId);
    return (It != MDestination.end() ? &It->second : NULL);
}

bool AddItem(Player * pPlayer, uint32 IdItem, uint32 Count /*= 1*/)
{
    if (Item * pItem = pPlayer->StoreNewItemInInventorySlot(IdItem, Count))
    {
        pPlayer->SendNewItem(pItem, Count, true, false, false);
        return true;
    }
    return false;
}

void Teleport(Player * const pPlayer, const uint16 &Map,
              const float &X, const float &Y, const float &Z, const float &Orient)
{
    pPlayer->Unmount();
    pPlayer->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
    pPlayer->CastSpell(pPlayer, SPELL_VISUAL_TELEPORT, true);
    pPlayer->TeleportTo(Map, X, Y, Z, Orient);
}

std::string ConvertMoney(const uint32 &Money)
{
    std::string Str = ToString<uint32>(Money);
    uint32 SizeStr  = Str.length();

    if (SizeStr > 4) Str = Str.insert(Str.length()-4, "po");
    if (SizeStr > 2) Str = Str.insert(Str.length()-2, "pa");
    Str += "pc";

    return Str;
}

void LoadDestination(void)
{
    QueryResult * pResult = SD2Database.PQuery(
        "SELECT id, name, pos_X, pos_Y, pos_Z, orient, mapid, required_level, cost FROM sd2p_destinations");
    MDestination.clear();

    if (pResult)
    {
        outstring_log( "SD2P: Chargement \"sd2p_destinations\"...");
        barGoLink Bar(pResult->GetRowCount());

        uint32 NbDest = pResult->GetRowCount();

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            Destination Dest =
            {
                pFields[0].GetUInt32(),      // Id
                pFields[1].GetCppString(),   // Name
                pFields[2].GetFloat(),       // X
                pFields[3].GetFloat(),       // Y
                pFields[4].GetFloat(),       // Z
                pFields[5].GetFloat(),       // Orientation
                pFields[6].GetUInt16(),      // Map
                pFields[7].GetUInt8(),       // Level
                pFields[8].GetUInt32(),      // Cost
            };

            MDestination.insert(MDestination_t::value_type(pFields[0].GetUInt32(), Dest));
        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u destinations charges.", NbDest);
    } else outstring_log("ATTENTION >> 0 sd2p_destinations charges.");
}


SD2P_NAMESPACE_FIN
