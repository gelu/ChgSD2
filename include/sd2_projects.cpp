/**
 *
 * @File : sd2_projects.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : Coeur du module SD2 Projects.
 *
 **/

#include "precompiled.h"
#include "sd2_projects.h"
#include "sd2p_util.h"

void nsSD2P::LoadDatabase(void)
{
    outstring_log("");
    outstring_log(" ____  ____ ____    ____            _           _           ");
    outstring_log("/ ___||  _ \\___ \\  |  _ \\ _ __ ___ (_) ___  ___| |_ ___  ");
    outstring_log("\\___ \\| | | |__) | | |_) | '__/ _ \\| |/ _ \\/ __| __/ __|");
    outstring_log(" ___) | |_| / __/  |  __/| | | (_) | |  __/ (__| |_\\__ \\  ");
    outstring_log("|____/|____/_____| |_|   |_|  \\___// |\\___|\\___|\\__|___/");
    outstring_log("                                 |__/   DB by Chglove       ");
    outstring_log("                                        Auteur: Wilibald09  ");
    outstring_log("");
    outstring_log("");

    if (NbProjects <= 0)
    {
        outstring_log("*** AUCUN PROJET INSTALLE ***");
        outstring_log("");
        return;
    }

    bool DestChargee = false;
    for (uint16 i = 0; i < NbProjects; ++i)
    {
        const Projects * pProjet = &TabProjects[i];
        if (!pProjet || !pProjet->pLoadDBFunction) continue;

        outstring_log("*** Chargement projet \"%s\" ***", pProjet->Nom.c_str());
        outstring_log("");
        if (!DestChargee && pProjet->RequiertDestination)
        {
            LoadDestination();
            DestChargee = true;
        }
        pProjet->pLoadDBFunction();
    }
}

void nsSD2P::LoadScripts(void)
{
    for (uint16 i = 0; i < NbProjects; ++i)
    {
        const Projects * pProjet = &TabProjects[i];
        if (!pProjet || !pProjet->pLoadSCFunction) continue;

        pProjet->pLoadSCFunction();
    }
}
