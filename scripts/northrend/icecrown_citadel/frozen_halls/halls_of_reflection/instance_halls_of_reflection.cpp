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
#include "def_halls.h"

struct MANGOS_DLL_DECL instance_halls_of_reflection : public ScriptedInstance
{
    instance_halls_of_reflection(Map* pMap) : ScriptedInstance(pMap) 
    {
        Regular = pMap->IsRegularDifficulty();
        Initialize();
    }

    bool Regular;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiFalrynGUID;
    uint64 m_uiMarwynGUID;
    uint64 m_uiLichKingGUID;
    uint64 m_uiFrostGeneralGUID;
    uint64 m_uiCaptainsChestHordeGUID;
    uint64 m_uiCaptainsChestAllianceGUID;
    uint64 m_uiIcecrownDoorGUID;
    uint64 m_uiImpenetrableDoorGUID;
    uint64 m_uiFrostmourneGUID;
    uint64 m_uiFrostmourneAltarGUID;

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
    m_uiCaptainsChestHordeGUID = 0;
    m_uiCaptainsChestAllianceGUID = 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_FALRYN: 
                         m_uiFalrynGUID = pCreature->GetGUID();
                         break;
            case NPC_MARWYN: 
                          m_uiMarwynGUID = pCreature->GetGUID();
                          break;
            case NPC_LICH_KING: 
                          m_uiLichKingGUID = pCreature->GetGUID();
                          break;
            case NPC_FROST_GENERAL: 
                          m_uiFrostGeneralGUID = pCreature->GetGUID();
                          break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_CAPTAIN_CHEST_1:
                               if(Regular) m_uiCaptainsChestHordeGUID = pGo->GetGUID();
                                  break;
        case GO_CAPTAIN_CHEST_2:
                               if(Regular) m_uiCaptainsChestAllianceGUID = pGo->GetGUID();
                                  break;
        case GO_CAPTAIN_CHEST_3:
                               if(!Regular) m_uiCaptainsChestHordeGUID = pGo->GetGUID();
                                  break;
        case GO_CAPTAIN_CHEST_4:
                               if(!Regular) m_uiCaptainsChestAllianceGUID = pGo->GetGUID();
                                  break;
        case GO_ICECROWN_DOOR:     m_uiIcecrownDoorGUID = pGo->GetGUID(); break;
        case GO_IMPENETRABLE_DOOR: m_uiImpenetrableDoorGUID = pGo->GetGUID(); break;
        case GO_FROSTMOURNE:       m_uiFrostmourneGUID = pGo->GetGUID(); 
                                   pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                                   break;
        case GO_FROSTMOURNE_ALTAR: m_uiFrostmourneAltarGUID = pGo->GetGUID();
                                   pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                                   break;
        }
    }
    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_START_EVENT:   m_auiEncounter[0] = uiData;
                                     if (uiData == DONE) {
                                         CloseDoor(m_uiIcecrownDoorGUID);
                                     }; break;
            case TYPE_FALRYN:        m_auiEncounter[1] = uiData; break;
            case TYPE_MARWYN:        m_auiEncounter[2] = uiData;
                                     if (uiData == DONE) {
                                         OpenDoor(m_uiImpenetrableDoorGUID);
                                         OpenDoor(m_uiIcecrownDoorGUID);
                            if (m_uiCaptainsChestHordeGUID)
                                if (GameObject* pChest = instance->GetGameObject(m_uiCaptainsChestHordeGUID))
                                    if (pChest && !pChest->isSpawned()) pChest->SetRespawnTime(7*DAY);
                            if (m_uiCaptainsChestAllianceGUID)
                                if (GameObject* pChest = instance->GetGameObject(m_uiCaptainsChestAllianceGUID))
                                    if (pChest && !pChest->isSpawned()) pChest->SetRespawnTime(7*DAY);
                                     }; break;
            case TYPE_LICH_KING_1:   m_auiEncounter[3] = uiData; break;
            case TYPE_FROST_GENERAL: m_auiEncounter[4] = uiData; break;
            case TYPE_LICH_KING_BATTLE: m_auiEncounter[5] = uiData; break;
            case TYPE_ESCAPE:        m_auiEncounter[6] = uiData; break;
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
            case TYPE_START_EVENT:   return m_auiEncounter[0];
            case TYPE_FALRYN:        return m_auiEncounter[1];
            case TYPE_MARWYN:        return m_auiEncounter[2];
            case TYPE_LICH_KING_1:   return m_auiEncounter[3];
            case TYPE_FROST_GENERAL: return m_auiEncounter[4];
            case TYPE_LICH_KING_BATTLE: return m_auiEncounter[5];
            case TYPE_ESCAPE:        return m_auiEncounter[6];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_FALRYN: return  m_uiFalrynGUID;
            case NPC_MARWYN: return  m_uiMarwynGUID;
            case NPC_LICH_KING: return m_uiLichKingGUID;
            case NPC_FROST_GENERAL: return m_uiFrostGeneralGUID;
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

            if (m_auiEncounter[i] != DONE)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
        OpenDoor(m_uiIcecrownDoorGUID);
    }
};

InstanceData* GetInstanceData_instance_halls_of_reflection(Map* pMap)
{
    return new instance_halls_of_reflection(pMap);
}


bool GOHello_go_frostmourne_altar(Player *player, GameObject* pGo)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();
    if(!pInstance) return false;

    switch(pGo->GetEntry())
    {
        case GO_FROSTMOURNE_ALTAR: {
                              if (pInstance->GetData(TYPE_START_EVENT) != DONE) {
                                           pInstance->SetData(TYPE_START_EVENT, DONE);
                                           pInstance->SetData(TYPE_FALRYN, SPECIAL);
                                           pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                              };
                              if (pInstance->GetData(TYPE_START_EVENT) == DONE
                                  && pInstance->GetData(TYPE_FALRYN) != DONE) {
                                           pInstance->SetData(TYPE_START_EVENT, DONE);
                                           pInstance->SetData(TYPE_FALRYN, SPECIAL);
                                           pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                              };
                              }  break;
    }

    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    return true;
}

bool GOHello_go_frostmourne(Player *player, GameObject* pGo)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) pGo->GetInstanceData();
    if(!pInstance) return false;

    switch(pGo->GetEntry())
    {
        case GO_FROSTMOURNE:  {
                              if (pInstance->GetData(TYPE_START_EVENT) == DONE
                                  && pInstance->GetData(TYPE_FALRYN) == DONE
                                  && pInstance->GetData(TYPE_MARWYN) != DONE) {
                                           pInstance->SetData(TYPE_MARWYN, SPECIAL);
                                           pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
                              };
                              }  break;
    }


    return true;
}

void AddSC_instance_halls_of_reflection()
{
    Script* newscript;
    newscript = new Script;
    newscript->Name = "instance_halls_of_reflection";
    newscript->GetInstanceData = &GetInstanceData_instance_halls_of_reflection;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_frostmourne_altar";
    newscript->pGOHello = &GOHello_go_frostmourne_altar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_frostmourne";
    newscript->pGOHello = &GOHello_go_frostmourne;
    newscript->RegisterSelf();

}
