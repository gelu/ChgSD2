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

    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
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
    uint64 m_uiAnubarakGUID;

    uint64 m_uiCusader11Guid;
    uint64 m_uiCusader12Guid;
    uint64 m_uiCusader13Guid;
    uint64 m_uiCusader14Guid;
    uint64 m_uiCusader15Guid;
    uint64 m_uiCusader16Guid;
    uint64 m_uiCusader17Guid;
    uint64 m_uiCusader18Guid;
    uint64 m_uiCusader19Guid;
    uint64 m_uiCusader1aGuid;

    uint64 m_uiCusader21Guid;
    uint64 m_uiCusader22Guid;
    uint64 m_uiCusader23Guid;
    uint64 m_uiCusader24Guid;
    uint64 m_uiCusader25Guid;
    uint64 m_uiCusader26Guid;
    uint64 m_uiCusader27Guid;
    uint64 m_uiCusader28Guid;
    uint64 m_uiCusader29Guid;
    uint64 m_uiCusader2aGuid;

    uint64 m_uiCusader01Guid;
    uint64 m_uiCusader02Guid;


    uint64 m_uiCrusadersCacheGUID;

    void Initialize()
    {
    for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            m_auiEncounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
         case NPC_BARRENT: m_uiBarrentGUID = pCreature->GetGUID(); break;
         case NPC_GORMOK: m_uiGormokGUID = pCreature->GetGUID(); break;
         case NPC_ACIDMAW: m_uiAcidmawGUID = pCreature->GetGUID(); break;
         case NPC_DREADSCALE: m_uiDreadscaleGUID = pCreature->GetGUID(); break;
         case NPC_ICEHOWL: m_uiIcehowlGUID = pCreature->GetGUID(); break;
         case NPC_JARAXXUS: m_uiJaraxxusGUID = pCreature->GetGUID(); break;
         case NPC_DARKBANE: m_uiDarkbaneGUID = pCreature->GetGUID(); break;
         case NPC_LIGHTBANE: m_uiLightbaneGUID = pCreature->GetGUID(); break;
         case NPC_ANUBARAK: m_uiAnubarakGUID = pCreature->GetGUID(); break;

         case NPC_CRUSADER_1_1: m_uiCusader11Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_2: m_uiCusader12Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_3: m_uiCusader13Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_4: m_uiCusader14Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_5: m_uiCusader15Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_6: m_uiCusader16Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_7: m_uiCusader17Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_8: m_uiCusader18Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_9: m_uiCusader19Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_1_10: m_uiCusader1aGuid = pCreature->GetGUID(); break;

         case NPC_CRUSADER_2_1: m_uiCusader21Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_2: m_uiCusader22Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_3: m_uiCusader23Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_4: m_uiCusader24Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_5: m_uiCusader25Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_6: m_uiCusader26Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_7: m_uiCusader27Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_8: m_uiCusader28Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_9: m_uiCusader29Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_2_10: m_uiCusader2aGuid = pCreature->GetGUID(); break;

         case NPC_CRUSADER_0_1: m_uiCusader01Guid = pCreature->GetGUID(); break;
         case NPC_CRUSADER_0_2: m_uiCusader02Guid = pCreature->GetGUID(); break;
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
        case GO_CRUSADERS_CACHE_10_H:
                                  if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        case GO_CRUSADERS_CACHE_25_H:
                                  if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                                  m_uiCrusadersCacheGUID = pGo->GetGUID(); 
                                  break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case TYPE_STAGE:     m_auiEncounter[0] = uiData; break;
        case TYPE_BEASTS:    m_auiEncounter[1] = uiData; break;
        case TYPE_JARAXXUS:  m_auiEncounter[2] = uiData; break;
        case TYPE_CRUSADERS: m_auiEncounter[3] = uiData;
                                            if (uiData == DONE) {
                                                 if (GameObject* pChest = instance->GetGameObject(m_uiCrusadersCacheGUID))
                                                     if (pChest && !pChest->isSpawned()) {
                                                                                          pChest->SetRespawnTime(DAY);
                                                                                          pChest->SetGoState(GO_STATE_ACTIVE);
                                                                                      };
                                                                };
                                                         break;
        case TYPE_FROJA:     m_auiEncounter[4] = uiData; break;
        case TYPE_ANUBARAK:  m_auiEncounter[5] = uiData; break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
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
         case NPC_DREADSCALE: return m_uiDreadscaleGUID;
         case NPC_ICEHOWL: return m_uiIcehowlGUID;
         case NPC_JARAXXUS: return  m_uiJaraxxusGUID;
         case NPC_DARKBANE: return m_uiDarkbaneGUID;
         case NPC_LIGHTBANE: return m_uiLightbaneGUID;
         case NPC_ANUBARAK: return m_uiAnubarakGUID;

         case NPC_CRUSADER_1_1: return m_uiCusader11Guid;
         case NPC_CRUSADER_1_2: return m_uiCusader12Guid;
         case NPC_CRUSADER_1_3: return m_uiCusader13Guid;
         case NPC_CRUSADER_1_4: return m_uiCusader14Guid;
         case NPC_CRUSADER_1_5: return m_uiCusader15Guid;
         case NPC_CRUSADER_1_6: return m_uiCusader16Guid;
         case NPC_CRUSADER_1_7: return m_uiCusader17Guid;
         case NPC_CRUSADER_1_8: return m_uiCusader18Guid;
         case NPC_CRUSADER_1_9: return m_uiCusader19Guid;
         case NPC_CRUSADER_1_10: return m_uiCusader1aGuid;

         case NPC_CRUSADER_2_1: return m_uiCusader21Guid;
         case NPC_CRUSADER_2_2: return m_uiCusader22Guid;
         case NPC_CRUSADER_2_3: return m_uiCusader23Guid;
         case NPC_CRUSADER_2_4: return m_uiCusader24Guid;
         case NPC_CRUSADER_2_5: return m_uiCusader25Guid;
         case NPC_CRUSADER_2_6: return m_uiCusader26Guid;
         case NPC_CRUSADER_2_7: return m_uiCusader27Guid;
         case NPC_CRUSADER_2_8: return m_uiCusader28Guid;
         case NPC_CRUSADER_2_9: return m_uiCusader29Guid;
         case NPC_CRUSADER_2_10: return m_uiCusader2aGuid;

         case NPC_CRUSADER_0_1: return m_uiCusader01Guid;
         case NPC_CRUSADER_0_2: return m_uiCusader02Guid;

        }
        return 0;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_STAGE:  return m_auiEncounter[0];
            case TYPE_BEASTS: return m_auiEncounter[1];
            case TYPE_JARAXXUS: return m_auiEncounter[2];
            case TYPE_CRUSADERS: return m_auiEncounter[3];
            case TYPE_FROJA: return m_auiEncounter[4];
            case TYPE_ANUBARAK: return m_auiEncounter[5];
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

        for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
        {
            loadStream >> m_auiEncounter[i];

            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }
        m_auiEncounter[0] = 0;

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
