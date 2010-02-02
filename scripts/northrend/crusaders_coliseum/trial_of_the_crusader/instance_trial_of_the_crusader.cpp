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
    instance_trial_of_the_crusader(Map* pMap) : ScriptedInstance(pMap) { Initialize(); }

    uint32 m_auiEncounter[MAX_ENCOUNTER+1];

    std::string m_strInstData;

    uint64 m_uiJacobGUID;
    uint64 m_uiAmbroseGUID;
    uint64 m_uiColososGUID;
    uint64 m_uiJaelyneGUID;
    uint64 m_uiLanaGUID;
    uint64 m_uiMokraGUID;
    uint64 m_uiEresseaGUID;
    uint64 m_uiRunokGUID;
    uint64 m_uiZultoreGUID;
    uint64 m_uiVisceriGUID;
    uint64 m_uicrusadersLootGUID;
    uint64 m_uiEadricGUID;
    uint64 m_uiEadricLootGUID;
    uint64 m_uiPaletressGUID;
    uint64 m_uiPaletressLootGUID;
    uint64 m_uiBlackKnightGUID;

    void Initialize()
    {
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            m_auiEncounter[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_JACOB:
                m_uiJacobGUID = pCreature->GetGUID();
                break;
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
        case GO_crusaderS_LOOT: m_uicrusadersLootGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
        case DATA_TOC_ANNOUNCER: m_uiAnnouncerGUID = uiData; break;
        case TYPE_GRAND_crusaderS: m_auiEncounter[0] = uiData;
                if (uiData == DONE)
                {
                    if (GameObject* pChest = instance->GetGameObject(m_uicrusadersLootGUID))
                        if (pChest && !pChest->isSpawned())
                            pChest->SetRespawnTime(350000000);
                }
                if (uiData == FAIL)
                {
                      m_auiEncounter[0] = NOT_STARTED;
                }
                break;
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
         case DATA_crusader_1: return m_uicrusader1;
        }
        return 0;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case DATA_crusaderID_1: return m_uicrusaderId1;
            case TYPE_GRAND_crusaderS: return m_auiEncounter[0];
            case TYPE_ARGENT_CHALLENGE: return m_auiEncounter[1];
            case TYPE_BLACK_KNIGHT: return m_auiEncounter[2];
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
