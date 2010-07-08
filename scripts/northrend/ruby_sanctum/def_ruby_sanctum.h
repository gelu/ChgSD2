/* Copyright (C) 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RUBY_SANCTUM_H
#define DEF_RUBY_SANCTUM_H
#include "sc_boss_spell_worker.h"

enum
{
    MAX_ENCOUNTERS                 = 4,

    TYPE_BALTHARUS                 = 0,
    TYPE_ZARITHIAN                 = 1,
    TYPE_RAGEFIRE                  = 2,
    TYPE_HALION                    = 3,

    TYPE_HALION_LOCK               = 4, //lock inside the fire ring

    NPC_HALION_P                   = 39863, // Halion Physical Realm NPC
    NPC_HALION_T                   = 40142, // Halion Twilight Realm NPC

    NPC_BALTHARUS                  = 39751,
    NPC_CLONE                      = 39899,
    NPC_ZARITHIAN                  = 39746,
    NPC_RAGEFIRE                   = 39747,

    GO_HALION_FIRE_RING            = 202800, //9482
    GO_HALION_PORTAL_0             = 193988, //1327 ENTRY
    GO_HALION_PORTAL_1             = 202794, //1327 ENTRY
    GO_HALION_PORTAL_2             = 202795, //1327 EXIT
    GO_HALION_PORTAL_3             = 202797, //1327 EXIT

    DATA_HEALTH_BALTHARUS          = 201,
    DATA_HEALTH_CLONE              = 202,
    DATA_HEALTH_HALION_P           = 203,
    DATA_HEALTH_HALION_T           = 204,
};

enum uiWorldStates
{
    UPDATE_STATE_UI_SHOW            = 5049,
    UPDATE_STATE_UI_COUNT           = 5050,
};

static Locations SpawnLoc[]=
{
    {0.0f, 0.0f, 0.0f}, // 0 HALION
    {0.0f, 0.0f, 0.0f}, // 1 PULSAR0
    {0.0f, 0.0f, 0.0f}, // 2 PULSAR1
};

#endif
