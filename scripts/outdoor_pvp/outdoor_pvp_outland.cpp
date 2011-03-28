/* Copyright (C) 2006 - 2011 /dev/rsa for ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: outdoor_pvp_outland
SD%Complete: who know?
SDComment: map script selector by /dev/rsa
SDCategory: Outdoor PvP
EndScriptData */

#include "precompiled.h"
#include "outdoor_pvp_outland.h"

struct MANGOS_DLL_DECL outdoor_pvp_outland : public OutdoorPvP_ZoneSelector
{
    outdoor_pvp_outland(Map* pMap) : OutdoorPvP_ZoneSelector(pMap)
    {
        LoadZoneScripts(pMap, ZoneScriptList);
    };
};


InstanceData* GetInstanceData_outdoor_pvp_outland(Map* pMap)
{
    return new outdoor_pvp_outland(pMap);
}

void AddSC_outdoor_pvp_outland()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "outdoor_pvp_outland";
    newscript->GetInstanceData = &GetInstanceData_outdoor_pvp_outland;
    newscript->RegisterSelf();
}

