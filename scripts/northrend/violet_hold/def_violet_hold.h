/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VIOLET_HOLD_H
#define DEF_VIOLET_HOLD_H

enum
{
    MAX_ENCOUNTER           = 2,

    WORLD_STATE_VH          = 3816,
    //WORLD_STATE_VH_RIFT     = 2784,

    TYPE_EREKEM             = 11,
    TYPE_MORAGG             = 12,
    TYPE_ICHORON            = 13,
    TYPE_XEVOZZ             = 14,
    TYPE_LAVANTHOR          = 15,
    TYPE_ZURAMAT            = 16,
    TYPE_CYANIGOSA          = 17,
    TYPE_RIFT               = 18,
    TYPE_LASTBOSS           = 19,

    DATA_EREKEM             = 20,
    DATA_MORAGG             = 21,
    DATA_ICHORON            = 22,
    DATA_XEVOZZ             = 23,
    DATA_LAVANTHOR          = 24,
    DATA_ZURAMAT            = 25,
    DATA_SINCLARI           = 26,
    DATA_BOSSTIME           = 27,

    DATA_SEAL_DOOR          = 30,
    DATA_EREKEM_DOOR        = 31,
    DATA_MORAGG_DOOR        = 32,
    DATA_ICHORON_DOOR       = 33,
    DATA_XEVOZZ_DOOR        = 34,
    DATA_LAVANTHOR_DOOR     = 35,
    DATA_ZURAMAT_DOOR       = 36,

    NPC_EREKEM             = 29315,
    NPC_EREKEM_GUARD       = 29395,
    NPC_MORAGG             = 29316,
    NPC_ICHORON            = 29313,
    NPC_XEVOZZ             = 29266,
    NPC_LAVANTHOR          = 29312,
    NPC_ZURAMAT            = 29314,
    NPC_CYANIGOSA          = 31134,

    NPC_AZURE_SABOTEUR     = 31079,
    NPC_AZURE_CAPTAIN      = 30666, // 3 for 1-11, 4 for 13-17
    NPC_AZURE_RAIDER       = 30668,
    NPC_AZURE_SORCEROR     = 30667,
    NPC_AZURE_STALKER      = 32191,
    NPC_GUARDIAN           = 30660,
    NPC_KEEPER             = 30695,

    NPC_SINCLARI           = 30658,
    NPC_GUARD              = 30659,
    NPC_PORTAL             = 31011,

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
};

#endif
