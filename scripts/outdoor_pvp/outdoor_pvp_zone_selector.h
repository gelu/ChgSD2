/* Copyright (C) 2006 - 2011 /dev/rsa for ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

/*
Usage:
    - write and register script as always, but not assign scriptname in mangos DB;
    - put script name in ZoneScriptList structure in map header file
    - enjoy

    TODO: make save/load expression for map
*/
#ifndef OUTDOOR_PVP_ZONE_SELECTOR_H
#define OUTDOOR_PVP_ZONE_SELECTOR_H
#include "precompiled.h"

struct ZoneScriptEntry
{
    public:
        bool           active;
        int            zone;
        std::string    scriptName;
};

typedef UNORDERED_MAP<uint32 /*zoneId*/, InstanceData* > ZoneScriptsMap;
typedef std::set< InstanceData* > ZoneScriptsSet;

class OutdoorPvP_ZoneSelector : public OutdoorPvP
{
    public:
        OutdoorPvP_ZoneSelector(Map* pMap) : OutdoorPvP(pMap)
        {};

        ~OutdoorPvP_ZoneSelector()
        {
            for (ZoneScriptsSet::iterator itr = m_scriptsSet.begin(); itr != m_scriptsSet.end();)
                delete (*itr);
        }

        ZoneScriptsMap m_scriptsMap;
        ZoneScriptsSet m_scriptsSet;
        std::string saveData;

        void LoadZoneScripts(Map* pMap, ZoneScriptEntry ZoneScriptList[])
        {
            m_scriptsMap.clear();
            m_scriptsSet.clear();

            for(uint16 i = 0; ZoneScriptList[i].zone != 0; ++i)
            {
                if (!ZoneScriptList[i].active)
                    continue;

                Script* pScript = GetScriptByName(ZoneScriptList[i].scriptName);

                if (!pScript || !pScript->GetInstanceData)
                    continue;

                InstanceData* pData = pScript->GetInstanceData(pMap);

                if (!pData)
                    continue;

                m_scriptsMap.insert(std::make_pair(ZoneScriptList[i].zone, pData));
                m_scriptsSet.insert(pData);
            }
        };

        void Initialize()
        {
            for(ZoneScriptsSet::const_iterator itr = m_scriptsSet.begin(); itr != m_scriptsSet.end(); ++itr )
               (*itr)->Initialize();
        };

        void Load(const char* data)
        {
            // need parser for data
            for(ZoneScriptsSet::const_iterator itr = m_scriptsSet.begin(); itr != m_scriptsSet.end(); ++itr )
               (*itr)->Load(data);
        };

        void Update(uint32 diff)
        {
            for(ZoneScriptsSet::const_iterator itr = m_scriptsSet.begin(); itr != m_scriptsSet.end(); ++itr )
                   (*itr)->Update(diff);
        };

        bool IsEncounterInProgress() const { return false; };

        void OnPlayerEnter(Player* player)
        {
            if (!player)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(player->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnPlayerEnter(player);
        };

        void OnPlayerDeath(Player* player)
        {
            if (!player)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(player->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnPlayerDeath(player);
        };

        void OnPlayerLeave(Player* player)
        {
            if (!player)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(player->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnPlayerLeave(player);
        };

        void OnPlayerEnterZone(Player* player, uint32 uiNewZoneId, uint32 uiNewAreaId)
        {
            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(uiNewZoneId);
            if (itr != m_scriptsMap.end())
               itr->second->OnPlayerEnterZone(player, uiNewZoneId, uiNewAreaId);
        };

        void OnPlayerLeaveZone(Player* player, uint32 uiOldZoneId)
        {
            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(uiOldZoneId);
            if (itr != m_scriptsMap.end())
               itr->second->OnPlayerLeaveZone(player, uiOldZoneId);
        };

        void OnPlayerDroppedFlag(Player* player, uint32 uiSpellId)
        {
            if (!player)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(player->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnPlayerDroppedFlag(player, uiSpellId);
        };

        void OnObjectCreate(GameObject* pGO)
        {
            if (!pGO)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(pGO->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnObjectCreate(pGO);
        };

        void OnCreatureCreate(Creature* creature)
        {
            if (!creature)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(creature->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnCreatureCreate(creature);
        };

        void OnCreatureEnterCombat(Creature* creature)
        {
            if (!creature)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(creature->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnCreatureCreate(creature);
        };

        void OnCreatureEvade(Creature* creature)
        {
            if (!creature)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(creature->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnCreatureEvade(creature);
        };

        void OnCreatureDeath(Creature* creature)
        {
            if (!creature)
                return;

            ZoneScriptsMap::const_iterator itr = m_scriptsMap.find(creature->GetZoneId());

            if (itr != m_scriptsMap.end())
                itr->second->OnCreatureDeath(creature);
        };

        bool CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* source, Unit const* target = NULL, uint32 miscvalue1 = 0) { return false; };

        bool CheckConditionCriteriaMeet(Player const* source, uint32 map_id, uint32 instance_condition_id) { return false; };

};

#endif
