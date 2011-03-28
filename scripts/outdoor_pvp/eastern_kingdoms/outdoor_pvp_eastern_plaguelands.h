/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef OUTDOOR_PVP_EASTERN_PLAGUELANDS_H
#define OUTDOOR_PVP_EASTERN_PLAGUELANDS_H
#include "precompiled.h"

enum
{
    MAX_VARIABLES                           = 4,

    TYPE_CROWNGUARD_CONTROLLER              = 1,
    TYPE_EASTWALL_CONTROLLER                = 2,
    TYPE_NORTHPASS_CONTROLLER               = 3,
    TYPE_PLAGUEWOOD_CONTROLLER              = 4,

    // spells
    // aly
    SPELL_ECOES_OF_LORDAERON_ALY_1          = 11413,
    SPELL_ECOES_OF_LORDAERON_ALY_2          = 11414,
    SPELL_ECOES_OF_LORDAERON_ALY_3          = 11415,
    SPELL_ECOES_OF_LORDAERON_ALY_4          = 1386,

    // horde
    SPELL_ECOES_OF_LORDAERON_HORDE_1        = 30880,
    SPELL_ECOES_OF_LORDAERON_HORDE_2        = 30683,
    SPELL_ECOES_OF_LORDAERON_HORDE_3        = 30682,
    SPELL_ECOES_OF_LORDAERON_HORDE_4        = 29520,

    // zone ids
    ZONE_ID_EASTERN_PLAGUELANDS             = 139,
    ZONE_ID_STRATHOLME                      = 2017,
    ZONE_ID_SCHOLOMANCE                     = 2057,

    // graveyards
    GRAVEYARD_ZONE_EASTERN_PLAGUE           = 139,
    GRAVEYARD_ID_EASTERN_PLAGUE             = 927,

    // taxi nodes
    TAXI_NODE_PLAGUEWOOD_TOWER              = 84,
    TAXI_NODE_NORTHPASS_TOWER               = 85,
    TAXI_NODE_EASTWALL_TOWER                = 86,
    TAXI_NODE_CROWNGUARD_TOWER              = 87,

    // npcs
    NPC_SPECTRAL_FLIGHTMASTER               = 17209,

    // quest
    NPC_CROWNGUARD_TOWER_QUEST_DOODAD       = 17689,
    NPC_EASTWALL_TOWER_QUEST_DOODAD         = 17690,
    NPC_NORTHPASS_TOWER_QUEST_DOODAD        = 17696,
    NPC_PLAGUEWOOD_TOWER_QUEST_DOODAD       = 17698,

    // aly
    NPC_LORDAERON_COMMANDER                 = 17635,
    NPC_LORDAERON_SOLDIER                   = 17647,

    // horde
    NPC_LORDAERON_VETERAN                   = 17995,
    NPC_LORDAERON_FIGHTER                   = 17996,

    // gameobjects
    GO_LORDAERON_SHRINE                     = 181682,
    GO_TOWER_FLAG                           = 182106,

    // capture points
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_1     = 181899,
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_2     = 182096,
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_3     = 182097,
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_4     = 182098,

    // world states
    WORLD_STATE_TOWER_COUNT_ALY             = 2327,
    WORLD_STATE_TOWER_COUNT_HORDE           = 2328,
    WORLD_STATE_TOWER_SLIDER_DISPLAY        = 2426,
    WORLD_STATE_TOWER_SLIDER_POS            = 2427,
    WORLD_STATE_TOWER_SLIDER_NEUTRAL        = 2428,

    // plaguewood tower
    WORLD_STATE_PLAGUEWOOD_CONQ_ALY         = 2366,
    WORLD_STATE_PLAGUEWOOD_CONQ_HORDE       = 2367,
    WORLD_STATE_PLAGUEWOOD_PROG_ALY         = 2368,
    WORLD_STATE_PLAGUEWOOD_PROG_HORDE       = 2369,
    WORLD_STATE_PLAGUEWOOD_ALY              = 2370,
    WORLD_STATE_PLAGUEWOOD_HORDE            = 2371,
    WORLD_STATE_PLAGUEWOOD_NEUTRAL          = 2353,

    // northpass tower
    WORLD_STATE_NORTHPASS_CONQ_ALY          = 2362,
    WORLD_STATE_NORTHPASS_CONQ_HORDE        = 2363,
    WORLD_STATE_NORTHPASS_PROG_ALY          = 2364,
    WORLD_STATE_NORTHPASS_PROG_HORDE        = 2365,
    WORLD_STATE_NORTHPASS_ALY               = 2372,
    WORLD_STATE_NORTHPASS_HORDE             = 2373,
    WORLD_STATE_NORTHPASS_NEUTRAL           = 2352,

    // eastwall tower
    WORLD_STATE_EASTWALL_CONQ_ALY           = 2359,
    WORLD_STATE_EASTWALL_CONQ_HORDE         = 2360,
    WORLD_STATE_EASTWALL_PROG_ALY           = 2357,
    WORLD_STATE_EASTWALL_PROG_HORDE         = 2358,
    WORLD_STATE_EASTWALL_ALY                = 2354,
    WORLD_STATE_EASTWALL_HORDE              = 2356,
    WORLD_STATE_EASTWALL_NEUTRAL            = 2361,

    // crownguard tower
    WORLD_STATE_CROWNGUARD_CONQ_ALY         = 2374,
    WORLD_STATE_CROWNGUARD_CONQ_HORDE       = 2375,
    WORLD_STATE_CROWNGUARD_PROG_ALY         = 2376,
    WORLD_STATE_CROWNGUARD_PROG_HORDE       = 2377,
    WORLD_STATE_CROWNGUARD_ALY              = 2378,
    WORLD_STATE_CROWNGUARD_HORDE            = 2379,
    WORLD_STATE_CROWNGUARD_NEUTRAL          = 2355,
};

/*
const uint32 EPTowerPlayerEnterEvents[EP_TOWER_NUM] = {10691,10699,10701,10705};

const uint32 EPTowerPlayerLeaveEvents[EP_TOWER_NUM] = {10692,10698,10700,10704};

/*
enum EP_Summons {
    EP_EWT_COMMANDER = 0,
    EP_EWT_SOLDIER1,
    EP_EWT_SOLDIER2,
    EP_EWT_SOLDIER3,
    EP_EWT_SOLDIER4,
    EP_PWT_FLIGHTMASTER,
};

enum EP_GoSummons {
    EP_NPT_BUFF = 0,
    EP_NPT_FLAGS,
    EP_EWT_FLAGS,
    EP_CGT_FLAGS,
    EP_PWT_FLAGS
};

enum EP_Towers {
    EP_EWT = 0, // plaguelands 03
    EP_NPT,// plaguelands 01
    EP_PWT,// plaguelands 04
    EP_CGT,// plaguelands 02
    EP_TOWER_NUM
};

const go_type EPCapturePoints[EP_TOWER_NUM] = {
    {182097,0,2574.51f,-4794.89f,144.704f,-1.45003f,-0.097056f,0.095578f,-0.656229f,0.742165f},
    {181899,0,3181.08f,-4379.36f,174.123f,-2.03472f,-0.065392f,0.119494f,-0.842275f,0.521553f},
    {182098,0,2962.71f,-3042.31f,154.789f,2.08426f,-0.074807f,-0.113837f,0.855928f,0.49883f},
    {182096,0,1860.85f,-3731.23f,196.716f,-2.53214f,0.033967f,-0.131914f,0.944741f,-0.298177f}
};

const go_type EPTowerFlags[EP_TOWER_NUM] = {
    {182106,0,2569.60f,-4772.93f,115.399f,2.72271f,0,0,0.978148f,0.207912f},
    {182106,0,3148.17f,-4365.51f,145.029f,1.53589f,0,0,0.694658f,0.71934f},
    {182106,0,2992.63f,-3022.95f,125.593f,3.03687f,0,0,0.99863f,0.052336f},
    {182106,0,1838.42f,-3703.56f,167.713f,0.890118f,0,0,0.430511f,0.902585f}
};

const uint32 EP_NUM_CREATURES = 6;
const uint32 EP_EWT_NUM_CREATURES = 5;

// one lordaeron commander, 4 soldiers
// should be spawned at EWT and follow a path, but trans-grid pathing isn't safe, so summon them directly at NPT
const creature_type EP_EWT_Summons_A[EP_EWT_NUM_CREATURES] = {
    {17635,469,0, 3167.61f,-4352.09f,138.20f,4.5811f},
    {17647,469,0, 3172.74f,-4352.99f,139.14f,4.9873f},
    {17647,469,0, 3165.89f,-4354.46f,138.67f,3.7244f},
    {17647,469,0, 3164.65f,-4350.26f,138.22f,2.4794f},
    {17647,469,0, 3169.91f,-4349.68f,138.37f,0.7444f}
};

const creature_type EP_EWT_Summons_H[EP_EWT_NUM_CREATURES] = {
    {17995,67,0, 3167.61f,-4352.09f,138.20f,4.5811f},
    {17996,67,0, 3172.74f,-4352.99f,139.14f,4.9873f},
    {17996,67,0, 3165.89f,-4354.46f,138.67f,3.7244f},
    {17996,67,0, 3164.65f,-4350.26f,138.22f,2.4794f},
    {17996,67,0, 3169.91f,-4349.68f,138.37f,0.7444f}
};

enum EP_TowerStates {
    EP_TS_N = 1,
    EP_TS_N_A = 2,
    EP_TS_N_H = 4,
    EP_TS_A_P = 8,
    EP_TS_H_P = 16,
    EP_TS_A = 32,
    EP_TS_H = 64
};

// when spawning, pay attention at setting the faction manually!
const creature_type EP_PWT_FlightMaster = {17209,0,0,2987.5f,-3049.11f,120.126f,5.75959f};

// after spawning, modify the faction so that only the controller will be able to use it with SetUInt32Value(GAMEOBJECT_FACTION, faction_id);
const go_type EP_NPT_LordaeronShrine = {181682,0,3167.72f,-4355.91f,138.785f,1.69297f,0,0,0.748956f,0.66262f};
*/

class MANGOS_DLL_DECL outdoor_pvp_eastern_plaguelands : public OutdoorPvP
{
    public:
        outdoor_pvp_eastern_plaguelands(Map* pMap);
        ~outdoor_pvp_eastern_plaguelands() {}

        void OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId, uint32 uiAreaId);
        void OnPlayerLeaveZone(Player* pPlayer, uint32 uiZoneId);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        void UpdateZoneWorldState();
        void SendPlayerWorldState(Player* pPlayer);

        std::string strInstData;

        uint32 m_uiPlaguewoodController;
        uint32 m_uiEastwallController;
        uint32 m_uiNorthpassController;
        uint32 m_uiCrownguardController;
        uint32 m_uiTowersAly;
        uint32 m_uiTowersHorde;

        PlayerSet sPlaguelandsPlayers;
};

/*
class OPvPCapturePointEP_EWT : public OPvPCapturePoint
{
friend class OutdoorPvPEP;
public:
    OPvPCapturePointEP_EWT(OutdoorPvP * pvp);
    void ChangeState();
    void SendChangePhase();
    void FillInitialWorldStates(WorldPacket& data, uint32& count);
    // used when player is activated/inactivated in the area
    bool HandlePlayerEnter(Player * plr);
    void HandlePlayerLeave(Player * plr);
protected:
    void SummonSupportUnitAtNorthpassTower(uint32 team);
    void UpdateTowerState();
protected:
    uint32 m_TowerState;
    uint32 m_UnitsSummonedSide;
};

class OPvPCapturePointEP_NPT : public OPvPCapturePoint
{
friend class OutdoorPvPEP;
public:
    OPvPCapturePointEP_NPT(OutdoorPvP * pvp);
    void ChangeState();
    void SendChangePhase();
    void FillInitialWorldStates(WorldPacket& data, uint32& count);
    // used when player is activated/inactivated in the area
    bool HandlePlayerEnter(Player * plr);
    void HandlePlayerLeave(Player * plr);
protected:
    void SummonGO(uint32 team);
    void UpdateTowerState();
protected:
    uint32 m_TowerState;
    uint32 m_SummonedGOSide;
};

class OPvPCapturePointEP_CGT : public OPvPCapturePoint
{
friend class OutdoorPvPEP;
public:
    OPvPCapturePointEP_CGT(OutdoorPvP * pvp);
    void ChangeState();
    void SendChangePhase();
    void FillInitialWorldStates(WorldPacket& data, uint32& count);
    // used when player is activated/inactivated in the area
    bool HandlePlayerEnter(Player * plr);
    void HandlePlayerLeave(Player * plr);
protected:
    void LinkGraveYard(Team team);
    void UpdateTowerState();
protected:
    uint32 m_TowerState;
    uint32 m_GraveyardSide;
};

class OPvPCapturePointEP_PWT : public OPvPCapturePoint
{
friend class OutdoorPvPEP;
public:
    OPvPCapturePointEP_PWT(OutdoorPvP * pvp);
    void ChangeState();
    void SendChangePhase();
    void FillInitialWorldStates(WorldPacket& data, uint32& count);
    // used when player is activated/inactivated in the area
    bool HandlePlayerEnter(Player * plr);
    void HandlePlayerLeave(Player * plr);
protected:
    void SummonFlightMaster(uint32 team);
    void UpdateTowerState();
protected:
    uint32 m_FlightMasterSpawned;
    uint32 m_TowerState;
};

class OutdoorPvPEP : public OutdoorPvP
{
friend class OPvPCapturePointEP_EWT;
friend class OPvPCapturePointEP_NPT;
friend class OPvPCapturePointEP_PWT;
friend class OPvPCapturePointEP_CGT;
public:
    OutdoorPvPEP();
    bool SetupOutdoorPvP();
    void HandlePlayerEnterZone(Player *plr, uint32 zone);
    void HandlePlayerLeaveZone(Player *plr, uint32 zone);
    bool Update(uint32 diff);
    void FillInitialWorldStates(WorldPacket& data, uint32& count);
    void SendRemoveWorldStates(Player * plr);
    void BuffTeams();
private:
    // how many towers are controlled
    uint32 EP_Controls[EP_TOWER_NUM];
    uint32 m_AllianceTowersControlled;
    uint32 m_HordeTowersControlled;
};
*/
#endif