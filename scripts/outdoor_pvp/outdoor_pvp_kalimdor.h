/* Copyright (C) 2006 - 2011 /dev/rsa for ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef OUTDOOR_PVP_KALIMDOR_H
#define OUTDOOR_PVP_KALIMDOR_H

#include "precompiled.h"
#include "outdoor_pvp_zone_selector.h"

enum Zones
{
    // zone ids
    ZONE_ID_SILITHUS                = 1377,
    ZONE_ID_TEMPLE_OF_AQ            = 3428,
    ZONE_ID_RUINS_OF_AQ             = 3429,
};

static ZoneScriptEntry ZoneScriptList[] =
{
    { true,  ZONE_ID_SILITHUS,            "outdoor_pvp_silithus"          },
    { true,  ZONE_ID_TEMPLE_OF_AQ,        "outdoor_pvp_silithus"          },
    { true,  ZONE_ID_RUINS_OF_AQ,         "outdoor_pvp_silithus"          },
    { false, 0, "" }
};

#endif
