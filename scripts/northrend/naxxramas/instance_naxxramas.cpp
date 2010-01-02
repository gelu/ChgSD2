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

/* Originally based on BroodWyrm scripts. Modified by danbst. Rewrited by Lutik.*/

#include "precompiled.h"
#include "naxxramas.h"


struct MANGOS_DLL_DECL instance_naxxramas : public ScriptedInstance
{
    instance_naxxramas(Map *Map) : ScriptedInstance(Map)
    {
        Regular = Map->IsRegularDifficulty();
        Initialize();
    };

    std::string str_data;

    uint32 mEncounter[ENCOUNTERS];
    uint32 mHorsemen[4];

    bool Regular;
    //Bosses and other NPC's
    uint64 mFaerlinaGUID;
    //Doors and other GO's
    uint64 mAnubRoomDoorGUID;
    uint64 mNothEnterDoorGUID;
    uint64 mNothExitDoorGUID;
    uint64 mGothikEnterDoorGUID;
    uint64 mGothikCombatDoorGUID;
    uint64 mGothikExitDoorGUID;
    uint64 mGluthDoorGUID;
    uint64 mHorsemenDoorGUID;

    uint64 mHorsemenChestGUID;


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
    
    void CheckHorsemen()
    {
        if(mHorsemen[0]==DONE && mHorsemen[1]==DONE && mHorsemen[2]==DONE && mHorsemen[3]==DONE)
            SetData(TYPE_FOURHORSEMEN, DONE);
        if(mHorsemen[0]==NOT_STARTED && mHorsemen[1]==NOT_STARTED && mHorsemen[2]==NOT_STARTED && mHorsemen[3]==NOT_STARTED)
            SetData(TYPE_FOURHORSEMEN, NOT_STARTED);
    }

    void Initialize()
    {
        //Bosses and other NPC's
        mFaerlinaGUID = 0;
        //Doors and other GO's
        uint64 mAnubRoomDoorGUID = 0;
        mNothEnterDoorGUID = 0;
        mNothExitDoorGUID = 0;
        mGothikEnterDoorGUID = 0;
        mGothikCombatDoorGUID = 0;
        mGothikExitDoorGUID = 0;
        mGluthDoorGUID = 0;
        mHorsemenChestGUID = 0;
        mHorsemenDoorGUID = 0;

        for(uint8 i = 0; i < ENCOUNTERS; i++)
            mEncounter[i] = NOT_STARTED;
            
        for(uint8 i = 0; i < 4; i++)
            mHorsemen[i] = NOT_STARTED;
    }

    void OnCreatureCreate(Creature *pCreature, uint32 entry)
    {
        switch(entry)
        {
            //Spider Quarter
            case 15953: mFaerlinaGUID = pCreature->GetGUID();
            //Military Quarter
            //Plague Quarter
            //Construct Quarter
            //Frostwyrm Lair
        }
    }

    void OnObjectCreate(GameObject *pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_ARAC_ANUB_DOOR: mAnubRoomDoorGUID = pGo->GetGUID(); break;
            case GO_PLAG_NOTH_ENTRY_DOOR: mNothEnterDoorGUID = pGo->GetGUID(); break;
            case GO_PLAG_NOTH_EXIT_DOOR: mNothExitDoorGUID = pGo->GetGUID(); break;
            case GO_MILI_GOTH_ENTRY_GATE: mGothikEnterDoorGUID = pGo->GetGUID(); break;
            case GO_MILI_GOTH_EXIT_GATE: mGothikExitDoorGUID = pGo->GetGUID(); break;
            case GO_MILI_GOTH_COMBAT_GATE: mGothikCombatDoorGUID = pGo->GetGUID(); break;
            case GO_CONS_GLUT_EXIT_DOOR: mGluthDoorGUID = pGo->GetGUID(); break;
            case GO_CHEST_HORSEMEN_NORM: if(Regular) mHorsemenChestGUID = pGo->GetGUID(); break;
            case GO_CHEST_HORSEMEN_HERO: if(!Regular) mHorsemenChestGUID = pGo->GetGUID(); break;
            case GO_MILI_HORSEMEN_DOOR: mHorsemenDoorGUID = pGo->GetGUID(); break;
        }
    }

    uint64 GetData64(uint32 type)
    {
        switch (type)
        {
            case GUID_FAERLINA: return mFaerlinaGUID;
        }
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        //todo: rewrite door system
        switch(type)
        {
            //Spider Quarter
            case TYPE_ANUBREKHAN:
                mEncounter[0] = data;
                if(data == IN_PROGRESS)
                    CloseDoor(mAnubRoomDoorGUID);
                else
                    OpenDoor(mAnubRoomDoorGUID);
                break;
            case TYPE_FAERLINA:
                mEncounter[1] = data;
                break;
            case TYPE_MAEXXNA:
                mEncounter[2] = data;
                break;
            //Construct Quarter
            case TYPE_PATCHWERK:
                mEncounter[3] = data;
                break;
            case TYPE_GROBBULUS:
                mEncounter[4] = data;
                break;
            case TYPE_GLUTH:
                mEncounter[5] = data;
                if(data == IN_PROGRESS)
                    CloseDoor(mGluthDoorGUID);
                else
                    OpenDoor(mGluthDoorGUID);
                break;
            case TYPE_THADDIUS:
                mEncounter[6] = data;
                break;
            //Military Quarter
            case TYPE_RAZUVIOUS:
                mEncounter[7] = data;
                break;
            case TYPE_GOTHIK:
                mEncounter[8] = data;
                if(data == IN_PROGRESS)
                {
                    CloseDoor(mGothikEnterDoorGUID);
                    CloseDoor(mGothikExitDoorGUID);
                    CloseDoor(mGothikCombatDoorGUID);
                }
                else if(data == SPECIAL)
                {
                    OpenDoor(mGothikCombatDoorGUID);
                }
                else //DONE, NOT_STARTED
                {
                    OpenDoor(mGothikEnterDoorGUID);
                    OpenDoor(mGothikExitDoorGUID);
                    OpenDoor(mGothikCombatDoorGUID);
                };
                break;
            case TYPE_FOURHORSEMEN:
                mEncounter[9] = data;
                if(data == DONE)
                {
                    DoRespawnGameObject(mHorsemenChestGUID, DAY);
                };
                if(data == IN_PROGRESS)
                    CloseDoor(mHorsemenDoorGUID);
                else
                    OpenDoor(mHorsemenDoorGUID);
                break;
            //Plague Quarter
            case TYPE_NOTH:
                mEncounter[10] = data;
                if(data == IN_PROGRESS)
                {
                    CloseDoor(mNothEnterDoorGUID);
                    CloseDoor(mNothExitDoorGUID);
                }
                else
                {
                    OpenDoor(mNothEnterDoorGUID);
                    OpenDoor(mNothExitDoorGUID);
                }
                break;
            case TYPE_HEIGAN:
                mEncounter[11] = data;
                break;
            case TYPE_LOATHEB:
                mEncounter[12] = data;
                break;
            //Frostwyrm Lair
            case TYPE_SAPPHIRON:
                mEncounter[13] = data;
                break;
            case TYPE_KELTHUZAD:
                mEncounter[14] = data;
                break;
                
            //Four Horsemen Chest
            case TYPE_BLAUMEAUX: 
                mHorsemen[0] = data; CheckHorsemen(); break;
            case TYPE_RIVENDARE:
                mHorsemen[1] = data; CheckHorsemen(); break;
            case TYPE_KORTHAZZ:
                mHorsemen[2] = data; CheckHorsemen(); break;
            case TYPE_ZELIEK:
                mHorsemen[3] = data; CheckHorsemen(); break;
                
            
            
            
        }

        if (data == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << mEncounter[0] << " " << mEncounter[1] << " " << mEncounter[2] << " "
                        << mEncounter[3] << " " << mEncounter[4] << " " << mEncounter[5] << " "
                        << mEncounter[6] << " " << mEncounter[7] << " " << mEncounter[8] << " "
                        << mEncounter[9] << " " << mEncounter[10] << " " << mEncounter[11] << " "
                        << mEncounter[12] << " " << mEncounter[13] << " " << mEncounter[14];

            str_data = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    uint32 GetData(uint32 type)
    {
        switch (type)
        {
            //Arachnid Quarter
            case TYPE_ANUBREKHAN:   return mEncounter[0];
            case TYPE_FAERLINA:     return mEncounter[1];
            case TYPE_MAEXXNA:      return mEncounter[2];
            //Construct Quarter
            case TYPE_PATCHWERK:    return mEncounter[3];
            case TYPE_GROBBULUS:    return mEncounter[4];
            case TYPE_GLUTH:        return mEncounter[5];
            case TYPE_THADDIUS:     return mEncounter[6];
            //Military Quarter
            case TYPE_RAZUVIOUS:    return mEncounter[7];
            case TYPE_GOTHIK:       return mEncounter[8];
            case TYPE_FOURHORSEMEN: return mEncounter[9];
            //Plague Quarter
            case TYPE_NOTH:         return mEncounter[10];
            case TYPE_HEIGAN:       return mEncounter[11];
            case TYPE_LOATHEB:      return mEncounter[12];
            //Frostwyrm Lair
            case TYPE_SAPPHIRON:    return mEncounter[13];
            case TYPE_KELTHUZAD:    return mEncounter[14];
        }
        return 0;
    }
    
    const char* Save()
    {
        return str_data.c_str();
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> mEncounter[0] >> mEncounter[1] >> mEncounter[2]
                    >> mEncounter[3] >> mEncounter[4] >> mEncounter[5]
                    >> mEncounter[6] >> mEncounter[7] >> mEncounter[8]
                    >> mEncounter[9] >> mEncounter[10] >> mEncounter[11]
                    >> mEncounter[12] >> mEncounter[13] >> mEncounter[14];
        for(uint32 i = 0; i < ENCOUNTERS; i++)
        {
            if (mEncounter[i] == IN_PROGRESS)               // Do not load an encounter as "In Progress" - reset it instead.
                mEncounter[i] = NOT_STARTED;
            SetData(i,mEncounter[i]);
        }
        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_naxxramas(Map* map)
{
    return new instance_naxxramas(map);
}

void AddSC_instance_naxxramas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_naxxramas";
    newscript->GetInstanceData = &GetInstanceData_naxxramas;
    newscript->RegisterSelf();
}
