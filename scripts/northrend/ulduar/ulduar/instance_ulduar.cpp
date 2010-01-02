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
#include "def_ulduar.h"

#define ENCOUNTERS 14

//Ancient Gate of the Keepers (194255)
//Ice door after Hodir      (194441)
//Runed Stone Door after vezax (194750)

struct MANGOS_DLL_DECL instance_ulduar : public ScriptedInstance
{
    instance_ulduar(Map* pMap) : ScriptedInstance(pMap) 
    {
        Regular = pMap->IsRegularDifficulty();
        Initialize();
    }

    bool Regular;
    //Creatures GUID
    uint64 mKologarnGUID;
    uint64 mKologarnLeftArmGUID;
    uint64 mKologarnRightArmGUID;
    uint64 mSteelbreakerGUID;
    uint64 mMolgeimGUID;
    uint64 mBrundirGUID;
    //GO Guid (Caches)
    uint64 mKologarnCacheGUID;
    uint64 mHodirCacheGUID;
    uint64 mHodirRareCacheGUID;
    uint64 mThorimCacheGUID;
    uint64 mThorimHardCacheGUID;
    uint64 mMimironCacheGUID;
    uint64 mMimironHardCacheGUID;
    uint64 mAlgalonCacheGUID;
    uint64 mFreyaCacheGUID;
    uint64 mFreyaHard1CacheGUID;
    uint64 mFreyaHard2CacheGUID;
    uint64 mFreyaHard3CacheGUID;
    //Doors
    uint64 mXT002DoorGUID;
    uint64 mHodirFrozenDoorGUID;
    uint64 mAncientGateGUID;
    //
    uint32 mEncounter[ENCOUNTERS];
    uint32 mLeviathanReached;
    uint32 mXT002Reached;
    uint32 mMimironReached;
    std::string strSaveData;
    //misc
    bool KeepersKilled;
    bool KeepersKilledHM;
    
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
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            mEncounter[i] = NOT_STARTED;

        mKologarnGUID = 0;
        mKologarnLeftArmGUID = 0;
        mKologarnRightArmGUID = 0;
        mSteelbreakerGUID = 0;
        mMolgeimGUID = 0;
        mBrundirGUID = 0;

        mKologarnCacheGUID = 0;
        mHodirCacheGUID = 0;
        mHodirRareCacheGUID = 0;
        mThorimCacheGUID = 0;
        mThorimHardCacheGUID = 0;
        mMimironCacheGUID = 0;
        mMimironHardCacheGUID = 0;
        mAlgalonCacheGUID = 0;
        mFreyaCacheGUID = 0;
        mFreyaHard1CacheGUID = 0;
        mFreyaHard2CacheGUID = 0;
        mFreyaHard3CacheGUID = 0;

        mXT002DoorGUID = 0;
        mHodirFrozenDoorGUID = 0;
        mAncientGateGUID = 0;

        mLeviathanReached = 0;
        mXT002Reached = 0;
        mMimironReached = 0;

        KeepersKilled = false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case 32930: mKologarnGUID = pCreature->GetGUID();
            case 32933: mKologarnLeftArmGUID = pCreature->GetGUID();
            case 32934: mKologarnRightArmGUID = pCreature->GetGUID();
            case 32867: mSteelbreakerGUID = pCreature->GetGUID();
            case 32927: mMolgeimGUID = pCreature->GetGUID();
            case 32857: mBrundirGUID = pCreature->GetGUID();
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            //LOOT CACHES:
            //Cache of Living Stone (Kologarn)
            case 195046: if(Regular) mKologarnCacheGUID = pGo->GetGUID(); break;
            case 195047: if(!Regular) mKologarnCacheGUID = pGo->GetGUID(); break;
            //Cache of Winter (Hodir)
            case 194307: if(Regular) mHodirCacheGUID = pGo->GetGUID(); break;
            case 194308: if(!Regular) mHodirCacheGUID = pGo->GetGUID(); break;
            //Rare Cache of Winter (Hodir hard)
            case 194200: if(Regular) mHodirRareCacheGUID = pGo->GetGUID(); break;
            case 194201: if(!Regular) mHodirRareCacheGUID = pGo->GetGUID(); break;
            //Thorim's cache
            case 194312: if(Regular) mThorimCacheGUID = pGo->GetGUID(); break;
            case 194314: if(!Regular) mThorimCacheGUID = pGo->GetGUID(); break;
            //Thorim's cache (hard)
            case 194313: if(Regular) mThorimHardCacheGUID = pGo->GetGUID(); break;
            case 194315: if(!Regular) mThorimHardCacheGUID = pGo->GetGUID(); break;
            //Cache of Innovation (Mimiron)
            case 194789: if(Regular) mMimironCacheGUID = pGo->GetGUID(); break;
            case 194956: if(!Regular) mMimironCacheGUID = pGo->GetGUID(); break;
            //Cache of Innovation (Mimiron hard)
            //Gift of Freya
            case 194324: if(Regular) mFreyaCacheGUID = pGo->GetGUID(); break;
            case 194328: if(!Regular) mFreyaCacheGUID = pGo->GetGUID(); break;
            //Gift of the Observer
            case 194822: if(Regular) mAlgalonCacheGUID = pGo->GetGUID(); break;
            case 194823: if(!Regular) mAlgalonCacheGUID = pGo->GetGUID(); break;

            //DOORS:
            case 194631: mXT002DoorGUID = pGo->GetGUID(); break;
            case 194441: mHodirFrozenDoorGUID = pGo->GetGUID(); break;
            case 194255: mAncientGateGUID = pGo->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        bool needSave = false;
        switch(uiType)
        {
            case TYPE_FLAME_LEVIATHAN:
                if(uiData == IN_PROGRESS)
                {
                    mLeviathanReached = 1;
                    needSave = true;
                }
                mEncounter[0] = uiData; 
                break;
            case TYPE_IGNIS:            
                mEncounter[1] = uiData; break;
            case TYPE_RAZORSCALE:       
                mEncounter[2] = uiData; break;
            case TYPE_XT002: 
                if(uiData == IN_PROGRESS)
                {
                    mXT002Reached = 1;
                    needSave = true;
                    CloseDoor(mXT002DoorGUID);
                }
                else
                {
                    OpenDoor(mXT002DoorGUID);
                }
                mEncounter[3] = uiData; 
                break;
            case TYPE_IRON_COUNCIL:     
                mEncounter[4] = uiData; break;
            case TYPE_KOLOGARN:
                if(uiData == DONE) DoRespawnGameObject(mKologarnCacheGUID, DAY);
                mEncounter[5] = uiData; 
                break;
            case TYPE_AURIAYA:          
                mEncounter[6] = uiData; break;
            case TYPE_HODIR:
                if(uiData == DONE) 
                {
                    DoRespawnGameObject(mHodirCacheGUID, DAY);
                    OpenDoor(mHodirFrozenDoorGUID);

                    //Unlock Ancient Gate of the Keepers
                    //TODO: onscreen notification
                    KeepersKilled = mEncounter[7]==DONE && mEncounter[8]==DONE && mEncounter[9]==DONE && mEncounter[10]==DONE;
                    if(KeepersKilled) OpenDoor(mAncientGateGUID);
                }
                mEncounter[7] = uiData; 
                break;
            case TYPE_FREYA:
                if(uiData == DONE) 
                {
                    DoRespawnGameObject(mFreyaCacheGUID, DAY);
                    //Unlock Ancient Gate of the Keepers
                    //TODO: onscreen notification
                    KeepersKilled = mEncounter[7]==DONE && mEncounter[8]==DONE && mEncounter[9]==DONE && mEncounter[10]==DONE;
                    if(KeepersKilled) OpenDoor(mAncientGateGUID);;
                }
                mEncounter[8] = uiData; 
                break;
            case TYPE_THORIM:
                if(uiData == DONE)
                {
                    DoRespawnGameObject(mThorimCacheGUID, DAY);
                    //Unlock Ancient Gate of the Keepers
                    //TODO: onscreen notification
                    KeepersKilled = mEncounter[7]==DONE && mEncounter[8]==DONE && mEncounter[9]==DONE && mEncounter[10]==DONE;
                    if(KeepersKilled) OpenDoor(mAncientGateGUID);
                }
                mEncounter[9] = uiData; 
                break;
            case TYPE_MIMIRON:
                if(uiData == IN_PROGRESS)
                {
                    mMimironReached = 1;
                    needSave = true;
                }
                if(uiData == DONE)
                {
                    DoRespawnGameObject(mMimironCacheGUID, DAY);
                    //Unlock Ancient Gate of the Keepers
                    //TODO: onscreen notification
                    KeepersKilled = mEncounter[7]==DONE && mEncounter[8]==DONE && mEncounter[9]==DONE && mEncounter[10]==DONE;
                    if(KeepersKilled) OpenDoor(mAncientGateGUID);
                }
                mEncounter[10] = uiData; 
                break;
            case TYPE_VEZAX:            
                mEncounter[11] = uiData; break;
            case TYPE_YOGG_SARON:       
                mEncounter[12] = uiData; break;
            case TYPE_ALGALON:
                if(uiData == DONE) DoRespawnGameObject(mAlgalonCacheGUID, DAY);
                mEncounter[13] = uiData; 
                break;

            //Hard Modes
            case TYPE_HODIR_HARD:
                if(uiData == DONE) 
                {
                    DoRespawnGameObject(mHodirRareCacheGUID, DAY);
                    DoRespawnGameObject(mHodirCacheGUID, DAY);
                    OpenDoor(mHodirFrozenDoorGUID);
                    mEncounter[7] = uiData;
                }
                break;
            case TYPE_THORIM_HARD:
                if(uiData == DONE) DoRespawnGameObject(mThorimHardCacheGUID, DAY);
                mEncounter[9] = uiData;
                break;
        }

        if (uiData == DONE || needSave)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << mEncounter[0] << " " << mEncounter[1] << " " << mEncounter[2] << " "
                << mEncounter[3] << " " << mEncounter[4] << " " << mEncounter[5] << " "
                << mEncounter[6] << " " << mEncounter[7] << " " << mEncounter[8] << " "
                << mEncounter[9] << " " << mEncounter[10] << " " << mEncounter[11] << " "
                << mEncounter[12] << " " << mEncounter[13] << " " << mEncounter[14] << " "
                << mLeviathanReached << " " << mXT002Reached << " " << mMimironReached;

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
            case TYPE_FLAME_LEVIATHAN:  return mEncounter[0];
            case TYPE_IGNIS:            return mEncounter[1];
            case TYPE_RAZORSCALE:       return mEncounter[2];
            case TYPE_XT002:            return mEncounter[3];
            case TYPE_IRON_COUNCIL:     return mEncounter[4];
            case TYPE_KOLOGARN:         return mEncounter[5]; 
            case TYPE_AURIAYA:          return mEncounter[6];
            case TYPE_HODIR:            return mEncounter[7];
            case TYPE_FREYA:            return mEncounter[8];
            case TYPE_THORIM:           return mEncounter[9];
            case TYPE_MIMIRON:          return mEncounter[10];
            case TYPE_VEZAX:            return mEncounter[11];
            case TYPE_YOGG_SARON:       return mEncounter[12];
            case TYPE_ALGALON:          return mEncounter[13];

            case TYPE_LEVIATHAN_TP:     return mLeviathanReached;
            case TYPE_XT002_TP:         return mXT002Reached;
            case TYPE_MIMIRON_TP:       return mMimironReached;

            case TYPE_KEEPERS:          return KeepersKilled ? 1 : 0;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_KOLOGARN:              return mKologarnGUID;
            case NPC_KOLOGARN_LEFT_ARM:     return mKologarnLeftArmGUID;
            case NPC_KOLOGARN_RIGHT_ARM:    return mKologarnRightArmGUID;
            case NPC_STEELBREAKER:          return mSteelbreakerGUID;
            case NPC_MOLGEIM:               return mMolgeimGUID;
            case NPC_BRUNDIR:               return mBrundirGUID;
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

        loadStream >> mEncounter[0] >> mEncounter[1] >> mEncounter[2] >> mEncounter[3]
            >> mEncounter[4] >> mEncounter[5] >> mEncounter[6] >> mEncounter[7]
            >> mEncounter[8] >> mEncounter[9] >> mEncounter[10] >> mEncounter[11]
            >> mEncounter[12] >> mEncounter[13] >> mEncounter[14] >> mLeviathanReached
            >> mXT002Reached >> mMimironReached;

        for(uint8 i = 0; i < ENCOUNTERS; ++i)
            if (mEncounter[i] == IN_PROGRESS)
                mEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            if (mEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }
};

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script* pNewScript;
    pNewScript = new Script;
    pNewScript->Name = "instance_ulduar";
    pNewScript->GetInstanceData = &GetInstanceData_instance_ulduar;
    pNewScript->RegisterSelf();
}