/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"
#include "BSW_instance.h"

void BSWScriptedInstance::DoCompleteAchievement(uint32 uiAchievmentId)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                pPlayer->CompletedAchievement(uiAchievmentId);
        }
    }
    else
        debug_log("SD2: DoCompleteAchievement attempt set data, but no players in map.");
}

void BSWScriptedInstance::DoOpenDoor(uint64 guid)
{
    if (!guid)
        return;

    GameObject* pGo = instance->GetGameObject(guid);

    if (pGo)
        pGo->SetGoState(GO_STATE_ACTIVE);
    else
        debug_log("SD2: DoOpenDoor attempt set data to object %u, but no this object", guid);
}

void BSWScriptedInstance::DoCloseDoor(uint64 guid)
{
    if (!guid)
        return;

    GameObject* pGo = instance->GetGameObject(guid);

    if (pGo)
        pGo->SetGoState(GO_STATE_READY);
    else
        debug_log("SD2: DoCloseDoor attempt set data to object %u, but no this object", guid);
}

