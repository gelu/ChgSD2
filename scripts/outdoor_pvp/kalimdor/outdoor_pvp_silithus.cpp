/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: outdoor_pvp_silithus
SD%Complete:
SDComment:
SDCategory: Outdoor PvP
EndScriptData */

#include "precompiled.h"
#include "outdoor_pvp_silithus.h"

outdoor_pvp_silithus::outdoor_pvp_silithus(Map* pMap) : OutdoorPvP(pMap),
    m_uiResourcesAly(0),
    m_uiResourcesHorde(0),
    m_uiLastControllerFaction(0){ }

void outdoor_pvp_silithus::OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId)
{
    if (uiZoneId == ZONE_ID_SILITHUS)
    {
        if(pPlayer->GetTeam() == m_uiLastControllerFaction)
            pPlayer->CastSpell(pPlayer, SPELL_CENARION_FAVOR, false);

        // add to the player set
        sSilithusPlayers.insert(pPlayer->GetGUID());

        // send actual world states
        SendPlayerWorldState(pPlayer);
    }
    else
    {
        if (pPlayer->HasAura(SPELL_CENARION_FAVOR))
            pPlayer->RemoveAurasDueToSpell(SPELL_CENARION_FAVOR);

        // remove from the player set
        if (sSilithusPlayers.find(pPlayer->GetGUID()) != sSilithusPlayers.end())
            sSilithusPlayers.erase(pPlayer->GetGUID());
    }
}

void outdoor_pvp_silithus::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ALLIANCE_SILITHYSTS:
            if (uiData)
                m_uiResourcesAly += uiData;
            else
                m_uiResourcesAly = uiData;

            if (GetData(TYPE_ALLIANCE_SILITHYSTS) == MAX_SILITHYST)
            {
                SetData(TYPE_CONTROLLER_FACTION, ALLIANCE);
                DoApplyTeamBuff(sSilithusPlayers, ALLIANCE, SPELL_CENARION_FAVOR);

                // send zone emote
                //sWorld.SendZoneText(ZONE_ID_SILITHUS, ZONE_EMOTE_ALY_CAPTURE);

                // reset counters
                SetData(TYPE_ALLIANCE_SILITHYSTS, 0);
                SetData(TYPE_HORDE_SILITHYSTS, 0);
            }
            break;
        case TYPE_HORDE_SILITHYSTS:
            if (uiData)
                m_uiResourcesHorde += uiData;
            else
                m_uiResourcesHorde = uiData;

            if (GetData(TYPE_HORDE_SILITHYSTS) == MAX_SILITHYST)
            {
                SetData(TYPE_CONTROLLER_FACTION, HORDE);
                DoApplyTeamBuff(sSilithusPlayers, HORDE, SPELL_CENARION_FAVOR);

                // send zone emote
                //sWorld.SendZoneText(ZONE_ID_SILITHUS, ZONE_EMOTE_HORDE_CAPTURE);

                // reset counters
                SetData(TYPE_ALLIANCE_SILITHYSTS, 0);
                SetData(TYPE_HORDE_SILITHYSTS, 0);
            }
            break;
        case TYPE_CONTROLLER_FACTION:
            m_uiLastControllerFaction = uiData;
            break;
    }

    // update states
    UpdateZoneWorldState();

    if (uiData)
    {
        OUT_SAVE_PVP_DATA;

        std::ostringstream saveStream;
        saveStream << m_uiResourcesAly << " " << m_uiResourcesHorde << " " << m_uiLastControllerFaction;

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_PVP_DATA_COMPLETE;
    }
}

uint32 outdoor_pvp_silithus::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ALLIANCE_SILITHYSTS:
            return m_uiResourcesAly;
        case TYPE_HORDE_SILITHYSTS:
            return m_uiResourcesHorde;
        case TYPE_CONTROLLER_FACTION:
            return m_uiLastControllerFaction;
    }
    return 0;
}

void outdoor_pvp_silithus::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_PVP_DATA_FAIL;
        return;
    }

    OUT_LOAD_PVP_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_uiResourcesAly >> m_uiResourcesHorde >> m_uiLastControllerFaction;

    OUT_LOAD_PVP_DATA_COMPLETE;
}

void outdoor_pvp_silithus::UpdateZoneWorldState()
{
    DoUpdateWorldState(sSilithusPlayers, WORLD_STATE_SI_GATHERED_A, m_uiResourcesAly);
    DoUpdateWorldState(sSilithusPlayers, WORLD_STATE_SI_GATHERED_H, m_uiResourcesHorde);
}

void outdoor_pvp_silithus::SendPlayerWorldState(Player* pPlayer)
{
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_GATHERED_A, m_uiResourcesAly);
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_GATHERED_H, m_uiResourcesHorde);
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_SILITHYST_MAX, MAX_SILITHYST);
}

void outdoor_pvp_silithus::OnPlayerDroppedFlag(Player* pPlayer, uint32 uiSpellId)
{
    if (uiSpellId != SPELL_SILITHYST)
        return;

    // ToDo
    // * make it drop by damage - core issue
    // * exclude the case when it's dropped near the area trigger

    //switch(pPlayer->GetTeam())
    //{
    //    case ALLIANCE:
    //        if (AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(AREATRIGGER_SILITHUS_ALY))
    //        {
    //            // 5.0f is safe-distance; else drop the flag
    //            if (pPlayer->GetDistance(atEntry->x, atEntry->y, atEntry->z) < 5.0f + atEntry->radius)
    //                return;
    //        }
    //        break;
    //    case HORDE:
    //        if (AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(AREATRIGGER_SILITHUS_HORDE))
    //        {
    //            // 5.0f is safe-distance; else drop the flag
    //            if (pPlayer->GetDistance(atEntry->x, atEntry->y, atEntry->z) < 5.0f + atEntry->radius)
    //                return;
    //        }
    //        break;
    //}

    // drop the flag if conditions are ok
    pPlayer->CastSpell(pPlayer, SPELL_SILITHYST_FLAG_DROP, true);
}

bool AreaTrigger_at_silithus(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isGameMaster() || pPlayer->isDead())
        return false;

    outdoor_pvp_silithus* pOutdoorPvp = (outdoor_pvp_silithus*)pPlayer->GetInstanceData();

    if (!pOutdoorPvp)
        return false;

    if (pAt->id == AREATRIGGER_SILITHUS_ALY)
    {
        if(pPlayer->GetTeam() == ALLIANCE && pPlayer->HasAura(SPELL_SILITHYST))
        {
            // remove aura
            pPlayer->RemoveAurasDueToSpell(SPELL_SILITHYST);
            pOutdoorPvp->SetData(TYPE_ALLIANCE_SILITHYSTS, 1);

            // reward the player
            pPlayer->CastSpell(pPlayer, SPELL_TRACES_OF_SILITHYST, false);
            pPlayer->RewardHonor(NULL, 1, HONOR_REWARD_SILITHYST);
            //pPlayer->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(FACTION_CENARION_CIRCLE), REPUTATION_REWARD_SILITHYST);

            // complete quest
            if (pPlayer->GetQuestStatus(QUEST_SCOURING_DESERT_ALY) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(NPC_SILITHUS_DUST_QUEST_ALY);
        }
    }
    else if (pAt->id == AREATRIGGER_SILITHUS_HORDE)
    {
        if(pPlayer->GetTeam() == HORDE && pPlayer->HasAura(SPELL_SILITHYST))
        {
            // remove aura
            pPlayer->RemoveAurasDueToSpell(SPELL_SILITHYST);
            pOutdoorPvp->SetData(TYPE_HORDE_SILITHYSTS, 1);

            // reward the player
            pPlayer->CastSpell(pPlayer, SPELL_TRACES_OF_SILITHYST, false);
            pPlayer->RewardHonor(NULL, 1, HONOR_REWARD_SILITHYST);
            //pPlayer->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(FACTION_CENARION_CIRCLE), REPUTATION_REWARD_SILITHYST);

            // complete quest
            if (pPlayer->GetQuestStatus(QUEST_SCOURING_DESERT_HORDE) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(NPC_SILITHUS_DUST_QUEST_HORDE);
        }
    }

    return false;
}

bool GOUse_go_silithyst(Player* pPlayer, GameObject* pGo)
{
    // ToDo - in the old DBCs this GO has a spell, so it doesn't need script
    pPlayer->CastSpell(pPlayer, SPELL_SILITHYST, false);
    pGo->Delete();

    return true;
}

InstanceData* GetInstanceData_outdoor_pvp_silithus(Map* pMap)
{
    return new outdoor_pvp_silithus(pMap);
}

void AddSC_outdoor_pvp_silithus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "at_silithus";
    pNewScript->pAreaTrigger = &AreaTrigger_at_silithus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_silithyst";
    pNewScript->pGOUse = &GOUse_go_silithyst;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "outdoor_pvp_silithus";
    pNewScript->GetInstanceData = &GetInstanceData_outdoor_pvp_silithus;
    pNewScript->RegisterSelf();
}