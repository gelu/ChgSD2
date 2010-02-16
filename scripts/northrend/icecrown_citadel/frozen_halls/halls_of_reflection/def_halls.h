
#ifndef DEF_ICECROWN_HALLS_H
#define DEF_ICECROWN_HALLS_H

enum
{
    MAX_ENCOUNTERS              = 8,

    TYPE_START_EVENT            = 0,
    TYPE_MOBS_WAVE_1            = 1,
    TYPE_FALRYN                 = 2,
    TYPE_MOBS_WAVE_2            = 3,
    TYPE_MARWYN                 = 4,
    TYPE_LICH_KING_1            = 5,
    TYPE_FROST_GENERAL          = 6,
    TYPE_LICH_KING_BATTLE       = 7,
    TYPE_ESCAPE                 = 8,

    NPC_FALRYN                   = 33112,
    NPC_MARWYN                   = 38113,
    NPC_LICH_KING                = 37226,

    NPC_PRAUDMUR_0               = 37221,
    NPC_PRAUDMUR_1               = 36955,
    NPC_PRAUDMUR_2               = 36955,
    NPC_SYLVANA_0                = 37223,
    NPC_SYLVANA_1                = 37554,
    NPC_SYLVANA_2                = 37554,

    NPC_DARK_1                   = 38177,
    NPC_DARK_2                   = 38176,
    NPC_DARK_3                   = 38173,
    NPC_DARK_4                   = 38172,
    NPC_DARK_5                   = 38567,
    NPC_DARK_6                   = 38175,

    NPC_FROST_GENERAL            = 36723,


    GO_ICECROWN_DOOR             = 201976, //72802
    GO_IMPENETRABLE_DOOR         = 197341, //72801
    GO_FROSTMOURNE_ALTAR         = 202236, //3551
    GO_FROSTMOURNE               = 202302, //364

    GO_CAPTAIN_CHEST_1           = 202212, //3145
    GO_CAPTAIN_CHEST_2           = 201710, //30357
    GO_CAPTAIN_CHEST_3           = 202337, //3246
    GO_CAPTAIN_CHEST_4           = 202336, //3333

};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations SpawnLoc[]=
{
    {5325.330078, 1976.609985, 707.695007},  // 0 Марвин
    {5274.630859, 2040.283813, 709.319824},  // 1 Фалрик
    {5346.934570, 2044.740234, 707.695801},  // 2 Выход
};

#endif
