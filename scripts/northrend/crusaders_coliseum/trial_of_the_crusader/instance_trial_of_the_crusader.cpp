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
SDName: Instance_Trial_Of_the_crusader
SD%Complete: 10%
SDComment: 
SDCategory: Trial Of the crusader
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

struct MANGOS_DLL_DECL instance_trial_of_the_crusader : public ScriptedInstance
{
    instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap) { 
    Difficulty = pMap->GetDifficulty();
    Initialize(); 
    }

    uint32 m_auiEncounter[MAX_ENCOUNTER+1];
    uint8 Difficulty;
    std::string m_strInstData;

    uint64 m_uiBarrentGUID;

    uint64 m_uiGormokGUID;
    uint64 m_uiAcidmawGUID;
    uint64 m_uiDreadscaleGUID;
    uint64 m_uiIcehowlGUID;
    uint64 m_uiJaraxxusGUID;
    uint64 m_uiDarkbaneGUID;
    uint64 m_uiLightbaneGUID;

    uint64 m_uiCrusadersCacheGUID;

    void Initialize()
    {
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_BARRENT: m_uiBarrentGUID = pCreature->GetGUID(); break;
            case NPC_GORMOK: m_uiGormokGUID = pCreature->GetGUID(); break;
            case NPC_ACIDMAW: m_uiAcidmawGUID = pCreature->GetGUID(); break;
            case NPC_DREADSCALE: m_uiDreascaleGUID = pCreature->GetGUID(); break;
            case NPC_ICEHOWL: m_uiIcehowlGUID = pCreature->GetGUID(); break;
            case NPC_JARAXXUS: m_uiJaraxxusGUID = pCreature->GetGUID(); break;
            case NPC_DARKBANE: m_uiDarkbaneGUID = pCreature->GetGUID(); break;
            case NPC_LIGHTBANE: m_uiLightbaneGUID = pCreature->GetGUID(); break;
            case NPC_ANUBARAK: m_uiBarrentGUID = pCreature->GetGUID(); break;
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_CRUSADERS_CACHE_10:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        case GO_CRUSADERS_CACHE_25:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        case GO_CRUSADERS_CACHE_10H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        case GO_CRUSADERS_CACHE_25H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case TYPE_BEASTS:    m_auiEncounter[0] = uiData; break;
        case TYPE_JARAXXUS:  m_auiEncounter[1] = uiData; break;
        case TYPE_CRUSADERS: m_auiEncounter[2] = uiData; break;
        case TYPE_FROJA:     m_auiEncounter[3] = uiData; break;
        case TYPE_ANUBARAK:  m_auiEncounter[4] = uiData; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                saveStream << m_auiEncounter[i] << " ";

            m_strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
         case NPC_BARRENT: return m_uiBarrentGUID;
         case NPC_GORMOK: return m_uiGormokGUID;
         case NPC_ACIDMAW: return m_uiAcidmawGUID;
         case NPC_DREADSCALE: return m_uiDreascaleGUID;
         case NPC_ICEHOWL: return m_uiIcehowlGUID;
         case NPC_JARAXXUS: return  m_uiJaraxxusGUID;
         case NPC_DARKBANE: return m_uiDarkbaneGUID;
         case NPC_LIGHTBANE: return m_uiLightbaneGUID;
         case NPC_ANUBARAK: return m_uiBarrentGUID;
        }
        return 0;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_BEASTS: return m_auiEncounter[0];
            case TYPE_JARAXXUS: return m_auiEncounter[1];
            case TYPE_CRUSADERS: return m_auiEncounter[2];
            case TYPE_FROJA: return m_auiEncounter[3];
            case TYPE_ANUBARAK: return m_auiEncounter[4];
        }

        return 0;
    }

    const char* Save()
    {
        return m_strInstData.c_str();
    }

    void Load(const char* strIn)
    {
        if (!strIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(strIn);

        std::istringstream loadStream(strIn);

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_trial_of_the_crusader(Map* pMap)
{
    return new instance_trial_of_the_crusader(pMap);
}

void AddSC_instance_trial_of_the_crusader()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_trial_of_the_crusader";
    newscript->GetInstanceData = &GetInstanceData_instance_trial_of_the_crusader;
    newscript->RegisterSelf();
}
