/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: instance_halls_of_reflection
SD%Complete: 70%
SDComment:
SDErrors:
SDCategory: instance script
SDAuthor: /dev/rsa, modified by MaxXx2021 aka Mioka
EndScriptData */

#include "precompiled.h"
#include "def_halls.h"

struct MANGOS_DLL_DECL instance_halls_of_reflection : public ScriptedInstance
{
    instance_halls_of_reflection(Map* pMap) : ScriptedInstance(pMap) 
    {
        Regular = pMap->IsRegularDifficulty();
        Initialize();
    }

    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint32 m_auiLider;
    std::string strSaveData;

    bool Regular;

    uint64 m_uiFalricGUID;
    uint64 m_uiMarwynGUID;
    uint64 m_uiLichKingGUID;
    uint64 m_uiLiderGUID;

    uint64 m_uiMainGateGUID;
    uint64 m_uiExitGateGUID;

    uint64 m_uiFrostGeneralGUID;
    uint64 m_uiCaptainsChestHordeGUID;
    uint64 m_uiCaptainsChestAllianceGUID;
    uint64 m_uiFrostmourneGUID;
    uint64 m_uiFrostmourneAltarGUID;

    void Initialize()
    {
        for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;
        m_uiMainGateGUID = 0;
        m_uiFrostmourneGUID = 0;
        m_uiFalricGUID = 0;
        m_uiLiderGUID = 0;
        m_uiLichKingGUID = 0;
        m_uiExitGateGUID = 0;
    }

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

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_FALRIC:
                   m_uiFalricGUID = pCreature->GetGUID();
                   break;
            case NPC_MARWYN:
                   m_uiMarwynGUID = pCreature->GetGUID();
                   break;
            case BOSS_LICH_KING:
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
            case GO_IMPENETRABLE_DOOR: m_uiMainGateGUID = pGo->GetGUID(); break;
            case GO_FROSTMOURNE: m_uiFrostmourneGUID = pGo->GetGUID(); break;
            case GO_ICECROWN_DOOR: m_uiExitGateGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_PHASE:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_EVENT:
                m_auiEncounter[1] = uiData;
                uiData = NOT_STARTED;
                break;
            case TYPE_FALRIC:
                m_auiEncounter[2] = uiData;
                if(uiData == SPECIAL)
                    CloseDoor(m_uiExitGateGUID);
                break;
            case TYPE_MARWYN:
                m_auiEncounter[3] = uiData;
                if(uiData == DONE)
                {
                    OpenDoor(m_uiMainGateGUID);
                    OpenDoor(m_uiExitGateGUID);
                }
                break;
            case TYPE_LICH_KING:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_ICE_WALL_01:
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_ICE_WALL_02:
                m_auiEncounter[6] = uiData;
                break;
            case TYPE_ICE_WALL_03:
                m_auiEncounter[7] = uiData;
                break;
            case TYPE_ICE_WALL_04:
                m_auiEncounter[8] = uiData;
                break;
            case TYPE_HALLS:
                m_auiEncounter[9] = uiData;
                break;
            case DATA_LIDER:
                m_auiLider = uiData;
                uiData = NOT_STARTED;
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
            case TYPE_PHASE:
                return m_auiEncounter[0];
            case TYPE_EVENT:
                return m_auiEncounter[1];
            case TYPE_FALRIC:
                return m_auiEncounter[2];
            case TYPE_MARWYN:
                return m_auiEncounter[3];
            case TYPE_LICH_KING:
                return m_auiEncounter[4];
            case TYPE_ICE_WALL_01:
                return m_auiEncounter[5];
            case TYPE_ICE_WALL_02:
                return m_auiEncounter[6];
            case TYPE_ICE_WALL_03:
                return m_auiEncounter[7];
            case TYPE_ICE_WALL_04:
                return m_auiEncounter[8];
            case TYPE_HALLS:
                return m_auiEncounter[9];
            case DATA_LIDER:
                return m_auiLider;
        }
        return 0;
    }

    void SetData64(uint32 uiData, uint64 uiGuid)
    {
        switch(uiData)
        {
            case DATA_ESCAPE_LIDER:
                   m_uiLiderGUID = uiGuid;
                   break;
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case GO_IMPENETRABLE_DOOR: return m_uiMainGateGUID;
            case GO_FROSTMOURNE: return m_uiFrostmourneGUID;
            case NPC_FALRIC: return m_uiFalricGUID;
            case NPC_MARWYN: return m_uiMarwynGUID;
            case BOSS_LICH_KING: return m_uiLichKingGUID;
            case DATA_ESCAPE_LIDER: return m_uiLiderGUID;
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

            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }

};

InstanceData* GetInstanceData_instance_halls_of_reflection(Map* pMap)
{
    return new instance_halls_of_reflection(pMap);
}

void AddSC_instance_halls_of_reflection()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_halls_of_reflection";
    newscript->GetInstanceData = &GetInstanceData_instance_halls_of_reflection;
    newscript->RegisterSelf();
}