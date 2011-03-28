/* Copyright (C) 2006 - 2011 /dev/rsa for ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef OUTDOOR_PVP_EASTERN_KINGDOMS_H
#define OUTDOOR_PVP_EASTERN_KINGDOMS_H

#include "precompiled.h"
#include "outdoor_pvp_zone_selector.h"

enum Zones
{
    ZONE_ID_EASTERN_PLAGUELANDS             = 139,
    ZONE_ID_STRATHOLME                      = 2017,
    ZONE_ID_SCHOLOMANCE                     = 2057
};

static ZoneScriptEntry ZoneScriptList[] =
{
    { true,  ZONE_ID_EASTERN_PLAGUELANDS, "outdoor_pvp_eastern_plaguelands"          },
    { true,  ZONE_ID_STRATHOLME,          "outdoor_pvp_eastern_plaguelands"          },
    { true,  ZONE_ID_SCHOLOMANCE,         "outdoor_pvp_eastern_plaguelands"          },
    { false, 0, "" }
};

#endif
