/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VIOLET_HOLD_H
#define DEF_VIOLET_HOLD_H

enum
{
    MAX_ENCOUNTER           = 9,

    TYPE_EVENT              = 0,
    TYPE_RIFT               = 1,
    TYPE_EREKEM             = 2,
    TYPE_MORAGG             = 3,
    TYPE_ICHORON            = 4,
    TYPE_XEVOZZ             = 5,
    TYPE_LAVANTHOR          = 6,
    TYPE_ZURAMAT            = 7,
    TYPE_CYANIGOSA          = 8,


    WORLD_STATE_VH          = 3816,
    WORLD_STATE_VH_PRISON   = 3815,
    WORLD_STATE_VH_PORTALS  = 3810,

    TYPE_LASTBOSS           = 19,
    TYPE_DOOR               = 21,
    TYPE_PORTAL_TIME        = 22,

    TYPE_DISRUPTIONS        = 101,
    TYPE_LASTBOSS_ID        = 102,

    DATA_EREKEM             = 23,
    DATA_MORAGG             = 24,
    DATA_ICHORON            = 25,
    DATA_XEVOZZ             = 26,
    DATA_LAVANTHOR          = 27,
    DATA_ZURAMAT            = 28,
    DATA_SINCLARI           = 29,
    DATA_BOSSTIME           = 30,
    DATA_NPC_SEAL_DOOR      = 31,

    DATA_SEAL_DOOR          = 32,
    DATA_EREKEM_DOOR        = 33,
    DATA_MORAGG_DOOR        = 34,
    DATA_ICHORON_DOOR       = 35,
    DATA_XEVOZZ_DOOR        = 36,
    DATA_LAVANTHOR_DOOR     = 37,
    DATA_ZURAMAT_DOOR       = 38,
    DATA_EREKEM_DOOR_L      = 39,
    DATA_EREKEM_DOOR_R      = 40,

    NPC_EREKEM             = 29315,
    NPC_EREKEM_GUARD       = 29395,
    NPC_MORAGG             = 29316,
    NPC_ICHORON            = 29313,
    NPC_XEVOZZ             = 29266,
    NPC_LAVANTHOR          = 29312,
    NPC_ZURAMAT            = 29314,
    NPC_CYANIGOSA          = 31134,

    NPC_AZURE_SABOTEUR     = 31079, // Open boss's cell

    NPC_AZURE_CAPTAIN      = 30666, //Melee, 40k - 60k hp, 3 for 1-11, 4 for 13-17
    NPC_AZURE_RAIDER       = 30668, //Melee, 40k - 60k hp
    NPC_AZURE_SORCEROR     = 30667, //Caster, 40k - 60k hp
    NPC_AZURE_STALKER      = 32191, //Melee, 40k - 60k hp
    NPC_GUARDIAN           = 30660, 
    NPC_KEEPER             = 30695, 
    NPC_AZURE_BINDER       = 30663, //Caster, 7k - 10k hp
    NPC_AZURE_INVADER      = 30661, //Melee, 8k - 12k hp
    NPC_AZURE_MAGE_SLAYER  = 30664, //Melee, 10k - 15k hp
    NPC_AZURE_SPELLBREAKER = 30662, //Caster, 10k - 15k hp

    NPC_SINCLARI           = 30658,
    NPC_GUARD              = 30659,
    NPC_PORTAL             = 31011,
    NPC_DOOR_SEAL          = 30896,

    GO_DOOR_SEAL           = 191723,
    GO_DOOR_EREKEM         = 191564,
    GO_DOOR_EREKEM_RIGHT   = 191563,
    GO_DOOR_EREKEM_LEFT    = 191562,
    GO_DOOR_MORAGG         = 191606,
    GO_DOOR_ICHORON        = 191722,
    GO_DOOR_XEVOZZ         = 191556,
    GO_DOOR_LAVANTHOR      = 191566,
    GO_DOOR_ZURAMAT        = 191565,

    SPELL_PORTAL_CHANNEL   = 58012,
    SPELL_CORRUPT          = 58040
};

struct Locations
{
    float x, y, z;
    uint32 id;
};
struct WayPoints
{
    WayPoints(uint32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    uint32 id;
    float x, y, z;
};

static Locations PortalLoc[]=
{
    {1888.271, 810.781, 38.441}, // 0 center
    {1857.125, 763.295, 38.654}, // 1 Lavanthor
    {1925.480, 849.981, 47.174}, // 2 Zuramat
    {1892.737, 744.589, 47.666}, // 3 Moragg
    {1878.198, 850.005, 43.333}, // 4 Portal in front of Erekem
    {1909.381, 806.796, 38.645}, // 5 Portal outside of Ichoron
    {1936.101, 802.950, 52.417}, // 6 at the highest platform
};

static Locations BossLoc[]=
{
    {0,0,0},
    {0,0,0},
    {1876.100, 857.079, 43.333}, // Erekem
    {1892.737, 744.589, 47.666}, // Moragg
    {1908.863, 785.647, 37.435}, // Ichoron
    {1905.364, 840.607, 38.670}, // Xevozz
    {1857.125, 763.295, 38.654}, // Lavanthor
    {1925.480, 849.981, 47.174}, // Zuramat
};
static Locations DragonsWP[]=
{
    //center, ichoron
    {1869.393, 803.902, 38.768}, // 0 
    {1859.843, 804.222, 44.008}, // 1 
    {1827.960, 804.208, 44.364}, // 2 

    //From left side (lavanthor)
    {1861.016, 789.717, 38.908}, // 3 
    {1856.217, 796.705, 44.008}, // 4 
    {1827.960, 804.208, 44.364}, // 5 

    //From Zuramat
    {1931.446, 826.734, 47.556}, // 6 
    {1913.049, 823.930, 38.792}, // 7 
    {1827.960, 804.208, 44.364}, // 8 
    {1869.393, 803.902, 38.768}, // 9 
    {1859.843, 804.222, 44.008}, // 10 
    {1827.960, 804.208, 44.364}, // 11 

    //From Morag
    {1887.500, 763.096, 47.666}, // 12 
    {1880.837, 775.769, 38.796}, // 13 
    {1861.016, 789.717, 38.908}, // 14 
    {1856.217, 796.705, 44.008}, // 15 
    {1827.960, 804.208, 44.364}, // 16 

    //From erekem
    {1878.280, 843.267, 43.333}, // 17 
    {1872.311, 835.531, 38.780}, // 18 
    {1861.997, 818.766, 38.650}, // 19 
    {1857.348, 811.230, 44.008}, // 20
    {1827.960, 804.208, 44.364}, // 21 

    //From Highest platform
    {1937.298, 824.557, 52.332}, // 22
    {1913.049, 823.930, 38.792}, // 23
    {1869.393, 803.902, 38.768}, // 24
    {1859.843, 804.222, 44.008}, // 25
    {1827.960, 804.208, 44.364}, // 26
};

#endif
