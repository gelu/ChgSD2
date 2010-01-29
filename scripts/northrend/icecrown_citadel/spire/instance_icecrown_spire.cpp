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
        Regular = pMap->IsRegularDifficulty();
        Initialize();
    }

    bool Regular;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
    uint64 m_uiMarrogwarGUID;
    uint64 m_uiDeathWhisperGUID;
    uint64 m_uiSaurfangGUID;

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
        m_uiMarrogwarGUID =0;
        m_uiDeathWhisperGUID =0;
        m_uiSaurfangGUID =0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_LORD_MARROGWAR: 
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
        }
    }
    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_MARROGWAR:
                if(uiData == IN_PROGRESS)
                {
                    needSave = true;
                }
                if(uiData == DONE) m_auiEncounter[0] = uiData; 
                break;
             case TYPE_DEATHWHISPER:
             case TYPE_SKULLS_PLATO:
             case TYPE_FLIGHT_WAR:
             case TYPE_SAURFANG:
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
             case TYPE_MARROGWAR:     return m_auiEncounter[0];
             case TYPE_DEATHWHISPER:   return m_auiEncounter[1];
             case TYPE_SKULLS_PLATO:  return m_auiEncounter[2];
             case TYPE_FLIGHT_WAR:    return m_auiEncounter[3];
             case TYPE_SAURFANG:      return m_auiEncounter[4];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_LORD_MARROGWAR: 
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

            if (m_auiEncounter[i] == IN_PROGRESS)
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
