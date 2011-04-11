/**
 *
 * @File : sd2p_sc_npc_guild_guard.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 2.1
 *
 **/

 
#include "precompiled.h"
#include "sd2p_sc_npc_guild_guard.h"

SD2P_NAMESPACE_DEB
SD2P_GUILD_GUARD_NAMESPACE_DEB


/********************************
 * DEFINITION VARIABLES GLOBALES
 ********************************/

MGuard_t     MGuard;
VGuardDest_t VGuardDest;


/************
 * FONCTIONS
 ************/

Guard const * GetGuard(const uint32 &Entry)
{
    MGuard_t::const_iterator It = MGuard.find(Entry);
    return (It != MGuard.end() ? &It->second : NULL);
}

void LoadDatabase(void)
{
    // Chargement des Gardiens de Guilde.
    QueryResult * pResult = SD2Database.PQuery("SELECT entry, guildid FROM sd2p_npc_guild_guard");
    MGuard.clear();

    if (pResult)
    {
        outstring_log( "SD2P: Chargement \"sd2p_npc_guild_guard\"...");
        barGoLink Bar(pResult->GetRowCount());

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (!pFields[0].GetUInt32() || !pFields[1].GetUInt32())
            {
                outstring_log("½Å±¾¿â£º ERREUR >> Garde invalide (Entry: %u).", pFields[0].GetUInt32());
                continue;
            }
            
            Guard * pGarde = const_cast<Guard*>(GetGuard(pFields[0].GetUInt32()));
            if (!pGarde)
            {
                Guard Garde;
                Garde.m_Entry = pFields[0].GetUInt32();
                pGarde = &MGuard.insert(MGuard_t::value_type(Garde.m_Entry, Garde)).first->second;
            }

            pGarde->m_lGuilds.push_back(pFields[1].GetUInt32());

        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u npc_guild_guard charge(s).", MGuard.size());
    } else outstring_log("ATTENTION >> 0 npc_guild_guard charge.");


    // Chargement des destinations de Gardien de Guilde.
    pResult = SD2Database.PQuery("SELECT DestId FROM sd2p_npc_guild_guard_dest");
    VGuardDest.clear();

    if (pResult)
    {
        outstring_log( "SD2P: Chargement \"sd2p_npc_guild_guard_dest\"...");
        barGoLink Bar(pResult->GetRowCount());

        do
        {
            Bar.step();
            Field * pFields = pResult->Fetch();

            if (!GetDestination(pFields[0].GetUInt32()))
            {
                outstring_log("SD2P >> Destination introuvable (DestID: %u).", pFields[0].GetUInt32());
                continue;
            }
            
            VGuardDest.push_back(pFields[0].GetUInt32());
        } while (pResult->NextRow());

        delete pResult;
        outstring_log("");
        outstring_log(">> %u npc_guild_guard_dest charge(s).", VGuardDest.size());
    } else outstring_log("ATTENTION >> 0 npc_guild_guard_dest charge.");
}


SD2P_GUILD_GUARD_NAMESPACE_FIN
SD2P_NAMESPACE_FIN
