/**
 *
 * @File : sd2_projects.h
 *
 * @Authors : Wilibald09
 *
 * @Date : 14/06/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : Coeur du module SD2 Projects.
 *             Il suffit de commenter/decommenter les trois "etapes" d'un projet
 *             pour le desactiver/activer.
 *
 **/

#ifndef SD2P_H_
#define SD2P_H_

#include "sd2p_util.h"


/****************************************************
 *** ETAPE 1 - Inclusion de l'en-tete des projets ***
 ****************************************************/

#include "sd2p_sc_npc_recompense.h"
#include "sd2p_sc_npc_guild_guard.h"
#include "sd2p_sc_npc_teleport.h"
#include "sd2p_sc_object_teleport.h"


/********************************************************
 *** ETAPE 2 - Declaration des Scripts IA des projets ***
 ********************************************************/

extern void AddSC_npc_recompense();
extern void AddSC_npc_guild_guard();
extern void AddSC_npc_teleport();
extern void AddSC_object_teleport();
extern void AddSC_npc_resurrect();


namespace SD2P_NAMESPACE
{
    struct Projects
    {
        std::string Nom;
        void (*pLoadSCFunction) (void);
        void (*pLoadDBFunction) (void);
        bool RequiertDestination;
    };


    /******************************************
     *** ETAPE 3 - Installation des projets ***
     ******************************************/

    const Projects TabProjects[] = 
    {
        { "NPC Recompense V2.1",          &AddSC_npc_recompense,  &SD2P_RECOMPENSE_NAMESPACE::LoadDatabase,   false },
        { "NPC Gardien de Guilde V2.0",   &AddSC_npc_guild_guard, &SD2P_GUILD_GUARD_NAMESPACE::LoadDatabase,  true  },
        { "NPC Teleporteur V2.0",         &AddSC_npc_teleport,    &SD2P_NPC_TELE_NAMESPACE::LoadDatabase,     true  },
        { "Object Teleporteur V2.0",      &AddSC_object_teleport, &SD2P_OBJECT_TELE_NAMESPACE::LoadDatabase,  true  },
        { "NPC Resurrect V1.0",           &AddSC_npc_resurrect,   NULL,                                       false },

        { "", NULL, NULL, false },
    };

    const int16 NbProjects = sizeof(TabProjects) / sizeof(TabProjects[0]) - 1;


    // Cette fonction permet de charger les relations des projets.
    // Il est necessaire d'inclure la ligne suivante dans le fichier "ScriptMgr.cpp" de SD2 :
    //      nsSD2P::LoadDatabase();
    void LoadDatabase(void);

    // Cette fonction permet de charger scripts d'IA des projets.
    // Il est necessaire d'inclure la ligne suivante dans le fichier "ScriptMgr.cpp" de SD2 :
    //      nsSD2P::LoadScripts();
    void LoadScripts(void);
}


#endif /* SD2P_H_ */
