/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "precompiled.h"
#include "def_spire.h"

struct MANGOS_DLL_DECL instance_icecrown_spire : public ScriptedInstance
{
    instance_icecrown_spire(Map* pMap) : ScriptedInstance(pMap) 
    {
        Difficulty = pMap->GetDifficulty();
        Initialize();
    }

    uint8 Difficulty;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiMarrogwarGUID;
    uint64 m_uiDeathWhisperGUID;
    uint64 m_uiSaurfangGUID;

    uint64 m_uiIcewall1GUID;
    uint64 m_uiIcewall2GUID;
    uint64 m_uiSaurfangDoorGUID;
    uint64 m_uiSaurfangCacheGUID;
    uint64 m_uiGunshipArmoryGUID;

    void OpenDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_ACTIVE);
    }

    void CloseDoor(uint64 guid)
    {
        if(!guid) return;
        GameObject* pGo = instance->GetGameObject(guid);
        if(pGo) pGo->SetGoState(GO_STATE_READY);
    }

    void Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;

        m_auiEncounter[0] = 0;

        m_uiMarrogwarGUID =0;
        m_uiDeathWhisperGUID =0;
        m_uiSaurfangGUID = 0;
        m_uiSaurfangCacheGUID = 0;
        m_uiGunshipArmoryGUID = 0;

    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LORD_MARROWGAR: 
                         m_uiMarrogwarGUID = pCreature->GetGUID();
                         break;
            case NPC_LADY_DEATHWHISPER: 
                          m_uiDeathWhisperGUID = pCreature->GetGUID();
                          break;
            case NPC_DEATHBRINGER_SAURFANG: 
                          m_uiSaurfangGUID = pCreature->GetGUID();
                          break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ICEWALL_1: 
                         m_uiIcewall1GUID = pGo->GetGUID();
                         break;
            case GO_ICEWALL_2: 
                         m_uiIcewall2GUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_DOOR: 
                         m_uiSaurfangDoorGUID = pGo->GetGUID();
                         break;
            case GO_SAURFANG_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID(); 
                                  break;
            case GO_SAURFANG_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_SAURFANG_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiSaurfangCacheGUID = pGo->GetGUID();
                                  break;
            case GO_GUNSHIP_ARMORY_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiGunshipArmoryGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiGunshipArmoryGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiGunshipArmoryGUID = pGo->GetGUID(); 
                                  break;
            case GO_GUNSHIP_ARMORY_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiGunshipArmoryGUID = pGo->GetGUID(); 
                                  break;
        }
    }
    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_TELEPORT:
                if (uiData > m_auiEncounter[0]) m_auiEncounter[0] = uiData;
                uiData = NOT_STARTED;
                break;
            case TYPE_MARROWGAR:
                m_auiEncounter[1] = uiData; 
                if (uiData == DONE) {
                OpenDoor(m_uiIcewall1GUID);
                OpenDoor(m_uiIcewall1GUID);
                m_auiEncounter[0] = TYPE_MARROWGAR;
                }
                break;
             case TYPE_DEATHWHISPER:
                m_auiEncounter[2] = uiData; 
                if (uiData == DONE) {
                m_auiEncounter[0] = TYPE_DEATHWHISPER; 
                }
                break;
             case TYPE_SKULLS_PLATO:
                m_auiEncounter[3] = uiData; 
                if (uiData == DONE) {
                m_auiEncounter[0] = TYPE_SKULLS_PLATO; 
                }
                break;
             case TYPE_FLIGHT_WAR:
                m_auiEncounter[4] = uiData; 
                if (uiData == DONE) {
                                 if (GameObject* pChest = instance->GetGameObject(m_uiGunshipArmoryGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(DAY);
//                                        pChest->SetGoState(GO_STATE_ACTIVE);
                                      };
                                };
                m_auiEncounter[0] = TYPE_FLIGHT_WAR; 
                break;
             case TYPE_FROSTWIRM_COUNT:
                m_auiEncounter[15] = uiData;
                uiData = NOT_STARTED;
                break;
             case TYPE_SAURFANG:
                m_auiEncounter[5] = uiData; 
                if (uiData == DONE) {
//                OpenDoor(m_uiSaurfangDoorGUID);
                                 if (GameObject* pChest = instance->GetGameObject(m_uiSaurfangCacheGUID))
                                     if (pChest && !pChest->isSpawned()) {
                                          pChest->SetRespawnTime(DAY);
//                                        pChest->SetGoState(GO_STATE_ACTIVE);
                                      };
                                };
//                m_auiEncounter[0] = TYPE_SAURFANG;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
                saveStream << m_auiEncounter[i] << " ";

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
             case TYPE_TELEPORT:      return m_auiEncounter[0];
             case TYPE_MARROWGAR:     return m_auiEncounter[1];
             case TYPE_DEATHWHISPER:  return m_auiEncounter[2];
             case TYPE_SKULLS_PLATO:  return m_auiEncounter[3];
             case TYPE_FLIGHT_WAR:    return m_auiEncounter[4];
             case TYPE_SAURFANG:      return m_auiEncounter[5];
             case TYPE_FROSTWIRM_COUNT:      return m_auiEncounter[15];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_LORD_MARROWGAR: 
                                     return m_uiMarrogwarGUID;
            case NPC_LADY_DEATHWHISPER:
                                     return m_uiDeathWhisperGUID;
            case NPC_DEATHBRINGER_SAURFANG: 
                                     return m_uiSaurfangGUID;
        }
        return 0;
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS && i >= 1)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_icecrown_spire(Map* pMap)
{
    return new instance_icecrown_spire(pMap);
}


void AddSC_instance_icecrown_spire()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_icecrown_spire";
    pNewScript->GetInstanceData = &GetInstanceData_instance_icecrown_spire;
    pNewScript->RegisterSelf();
}
