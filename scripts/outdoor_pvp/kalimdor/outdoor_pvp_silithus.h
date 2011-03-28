/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef OUTDOOR_PVP_SILITHUS_H
#define OUTDOOR_PVP_SILITHUS_H

enum
{
    TYPE_ALLIANCE_SILITHYSTS        = 1,
    TYPE_HORDE_SILITHYSTS           = 2,
    TYPE_CONTROLLER_FACTION         = 3,

    // npcs
    NPC_SILITHUS_DUST_QUEST_ALY     = 17090,        // dummy npcs for quest credit
    NPC_SILITHUS_DUST_QUEST_HORDE   = 18199,

    // game objects
    GO_SILITHYST_MOUND              = 181597,       // created when a player drops the flag
    GO_SILITHYST_GEYSER             = 181598,       // spawn on the map by default

    // spells
    SPELL_SILITHYST_OBJECT          = 29518,        // unk, related to the GO
    SPELL_SILITHYST                 = 29519,        // buff recieved when you are carrying a silithyst
    SPELL_TRACES_OF_SILITHYST       = 29534,        // individual buff recieved when succesfully delivered a silithyst
    SPELL_CENARION_FAVOR            = 30754,        // zone buff recieved when a faction gathers 200 silithysts
    SPELL_SILITHYST_FLAG_DROP       = 29533,

    // quests
    QUEST_SCOURING_DESERT_ALY       = 9419,
    QUEST_SCOURING_DESERT_HORDE     = 9422,

    // zone ids
    ZONE_ID_SILITHUS                = 1377,
    ZONE_ID_TEMPLE_OF_AQ            = 3428,         // ToDo - research
    ZONE_ID_RUINS_OF_AQ             = 3429,         // don't know yet how to handle the buff inside the instances

    // area triggers
    AREATRIGGER_SILITHUS_ALY        = 4162,         // areatriggers ids
    AREATRIGGER_SILITHUS_HORDE      = 4168,

    // zone emotes
    ZONE_EMOTE_HORDE_CAPTURE        = -1001000,
    ZONE_EMOTE_ALY_CAPTURE          = -1001001,

    FACTION_CENARION_CIRCLE         = 609,
    HONOR_REWARD_SILITHYST          = 19,
    REPUTATION_REWARD_SILITHYST     = 20,
    MAX_SILITHYST                   = 200,

    // world states
    WORLD_STATE_SI_GATHERED_A       = 2313,         // world state ids
    WORLD_STATE_SI_GATHERED_H       = 2314,
    WORLD_STATE_SI_SILITHYST_MAX    = 2317,
};

class MANGOS_DLL_DECL outdoor_pvp_silithus : public OutdoorPvP
{
    public:
        outdoor_pvp_silithus(Map* pMap);
        ~outdoor_pvp_silithus() {}

        void OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId);
        void OnPlayerDroppedFlag(Player* pPlayer, uint32 uiSpellId);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void UpdateZoneWorldState();
        void SendPlayerWorldState(Player* pPlayer);

        std::string strInstData;

        uint32 m_uiResourcesAly;
        uint32 m_uiResourcesHorde;
        uint32 m_uiLastControllerFaction;

        PlayerSet sSilithusPlayers;
};

#endif