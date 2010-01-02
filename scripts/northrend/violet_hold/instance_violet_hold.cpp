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
SDName: Instance The Violet Hold
SD%Complete: 0%
SDComment:
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "def_violet_hold.h"

/* The Violet Hold encounters:
0 Whole Event
1 Rift
2 Erekem
3 Moragg
4 Ichoron
5 Xevozz
6 Lavanthor
7 Zuramat
*/
//inline uint32 RandRiftBoss() { return ((rand()%2) ? NPC_GUARDIAN : NPC_KEEPER); }

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations PortalLoc[]=
{
    {1888.271, 810.781, 38.441}, // 0 center
    {1857.125, 763.295, 38.654}, // 1 Lavanthor
    {1925.480, 849.981, 47.174}, // 2 Zuramat
    {1892.737, 744.589, 47.666}, // 3 Moragg
    {1878.198, 850.005, 43.333}, // 4 Portal in front of Erekem
    {1909.381, 806.796, 38.645}, // 5 Portal outside of Ichoron
    {1936.101, 802.950, 52.417}, // 6 at the highest platform

    {1876.100, 857.079, 43.333}, // 7 Erekem
    {1908.863, 785.647, 37.435}, // 8 Ichoron
    {1905.364, 840.607, 38.670}, // 9 Xevozz
};

struct MANGOS_DLL_DECL instance_violet_hold : public ScriptedInstance
{
    instance_violet_hold(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    bool bIsInBoss;

    uint8 m_uiLastBossID;
    uint8 m_uiRiftPortalCount;
    uint32 m_uiShieldPercent;
    int8 m_uiPortalTime;

    uint64 m_uiSinclariGUID;
    uint64 m_uiNPCSealDoorGUID;

    uint64 m_uiErekemGUID;
    uint64 m_uiMoraggGUID;
    uint64 m_uiIchoronGUID;
    uint64 m_uiXevozzGUID;
    uint64 m_uiLavanthorGUID;
    uint64 m_uiZuramatGUID;

    uint64 m_uiSealDoorGUID;
    uint64 m_uiErekemDoorGUID;
    uint64 m_uiErekemDoorLeftGUID;
    uint64 m_uiErekemDoorRightGUID;
    uint64 m_uiMoraggDoorGUID;
    uint64 m_uiIchoronDoorGUID;
    uint64 m_uiXevozzDoorGUID;
    uint64 m_uiLavanthorDoorGUID;
    uint64 m_uiZuramatDoorGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiSinclariGUID = 0;
        m_uiNPCSealDoorGUID = 0;

        m_uiErekemGUID      = 0;
        m_uiMoraggGUID      = 0;
        m_uiIchoronGUID     = 0;
        m_uiXevozzGUID      = 0;
        m_uiLavanthorGUID   = 0;
        m_uiZuramatGUID     = 0;

        m_uiSealDoorGUID        = 0;
        m_uiErekemDoorGUID      = 0;
        m_uiErekemDoorLeftGUID  = 0;
        m_uiErekemDoorRightGUID = 0;
        m_uiMoraggDoorGUID      = 0;
        m_uiIchoronDoorGUID     = 0;
        m_uiXevozzDoorGUID      = 0;
        m_uiLavanthorDoorGUID   = 0;
        m_uiZuramatDoorGUID     = 0;
        Clear();
    }

    void Clear(){
        bIsInBoss = false;

        m_uiLastBossID = 0;
        m_uiRiftPortalCount = 0;
        m_uiPortalTime = 0;
        m_uiShieldPercent = 100;
    }

    void InitWorldState(bool Enable = true)
    {
        DoUpdateWorldState(WORLD_STATE_VH,Enable ? 1 : 0);
        DoUpdateWorldState(WORLD_STATE_VH_PRISON,100);
        DoUpdateWorldState(WORLD_STATE_VH_PORTALS,0);
    }

    void OnPlayerEnter(Player* pPlayer)
    {
        if(m_auiEncounter[0] != NOT_STARTED)
            pPlayer->SendUpdateWorldState(WORLD_STATE_VH,1);
    }
    
    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_SINCLARI:
                m_uiSinclariGUID = pCreature->GetGUID();
                break;
            case NPC_DOOR_SEAL:
                m_uiNPCSealDoorGUID = pCreature->GetGUID();
                break;
            case NPC_EREKEM:
                m_uiErekemGUID = pCreature->GetGUID();
                break;
            case NPC_MORAGG:
                m_uiMoraggGUID = pCreature->GetGUID();
                break;
            case NPC_ICHORON:
                m_uiIchoronGUID = pCreature->GetGUID();
                break;
            case NPC_XEVOZZ:
                m_uiXevozzGUID = pCreature->GetGUID();
                break;
            case NPC_LAVANTHOR:
                m_uiLavanthorGUID = pCreature->GetGUID();
                break;
            case NPC_ZURAMAT:
                m_uiZuramatGUID = pCreature->GetGUID();
                break;
        }
    }

    void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_DOOR_SEAL:
                m_uiSealDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_EREKEM:
                m_uiErekemDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_EREKEM_LEFT:
                m_uiErekemDoorLeftGUID = pGo->GetGUID();
                break;
            case GO_DOOR_EREKEM_RIGHT:
                m_uiErekemDoorRightGUID = pGo->GetGUID();
                break;
            case GO_DOOR_MORAGG:
                m_uiMoraggDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_ICHORON:
                m_uiIchoronDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_XEVOZZ:
                m_uiXevozzDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_LAVANTHOR:
                m_uiLavanthorDoorGUID = pGo->GetGUID();
                break;
            case GO_DOOR_ZURAMAT:
                m_uiZuramatDoorGUID = pGo->GetGUID();
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_EVENT:
                if (uiData == IN_PROGRESS)
                {
                    Clear();
                    InitWorldState();
                }
                else if (uiData == FAIL)
                    DoUseDoorOrButton(m_uiSealDoorGUID);
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_EREKEM:
                m_auiEncounter[2] = uiData;
                break;
            case TYPE_MORAGG:
                m_auiEncounter[3] = uiData;
                break;
            case TYPE_ICHORON:
                m_auiEncounter[4] = uiData;
                break;
            case TYPE_XEVOZZ:
                m_auiEncounter[5] = uiData;
                break;
            case TYPE_LAVANTHOR:
                m_auiEncounter[6] = uiData;
                break;
            case TYPE_ZURAMAT:
                m_auiEncounter[7] = uiData;
                break;
            case TYPE_RIFT:
                if (uiData == SPECIAL){
                    ++m_uiRiftPortalCount;
                    DoUpdateWorldState(WORLD_STATE_VH_PORTALS, m_uiRiftPortalCount);
                }
                else if (uiData == IN_PROGRESS)
                    bIsInBoss = true;
                else
                    DoUseDoorOrButton(m_uiSealDoorGUID);

                m_auiEncounter[1] = uiData;
                break;
            case TYPE_DOOR:
                if (uiData == SPECIAL)
                {
                    --m_uiShieldPercent;
                    if(m_uiShieldPercent > 0)
                        DoUpdateWorldState(WORLD_STATE_VH_PRISON, m_uiShieldPercent);
                    else
                        m_auiEncounter[0] = FAIL;
                }
                break;
        }
        if (uiData == DONE)
            bIsInBoss = false;
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_EVENT:
                return m_auiEncounter[0];
            case TYPE_EREKEM:
                return m_auiEncounter[2];
            case TYPE_MORAGG:
                return m_auiEncounter[3];
            case TYPE_ICHORON:
                return m_auiEncounter[4];
            case TYPE_XEVOZZ:
                return m_auiEncounter[5];
            case TYPE_LAVANTHOR:
                return m_auiEncounter[6];
            case TYPE_ZURAMAT:
                return m_auiEncounter[7];
            case TYPE_RIFT:
                return m_uiRiftPortalCount;
            case TYPE_LASTBOSS:
            {
                if (!m_uiLastBossID)
                    m_uiLastBossID = urand(1, 5);
                else
                {
                    uint8 uiBossID = urand(1, 5);
                    if (uiBossID == m_uiLastBossID)
                        --m_uiLastBossID;
                    else
                        m_uiLastBossID = uiBossID;
                }

                return m_uiLastBossID;
            }
            case DATA_BOSSTIME:
                return bIsInBoss;
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_EREKEM:
                return m_uiErekemGUID;
            case DATA_MORAGG:
                return m_uiMoraggGUID;
            case DATA_ICHORON:
                return m_uiIchoronGUID;
            case DATA_XEVOZZ:
                return m_uiXevozzGUID;
            case DATA_LAVANTHOR:
                return m_uiLavanthorGUID;
            case DATA_ZURAMAT:
                return m_uiZuramatGUID;
            case DATA_SINCLARI:
                return m_uiSinclariGUID;
            case DATA_NPC_SEAL_DOOR:
                return m_uiNPCSealDoorGUID;
            case DATA_SEAL_DOOR:
                return m_uiSealDoorGUID;
            case DATA_EREKEM_DOOR:
                return m_uiErekemDoorGUID;
            case DATA_MORAGG_DOOR:
                return m_uiMoraggDoorGUID;
            case DATA_ICHORON_DOOR:
                return m_uiIchoronDoorGUID;
            case DATA_XEVOZZ_DOOR:
                return m_uiXevozzDoorGUID;
            case DATA_LAVANTHOR_DOOR:
                return m_uiLavanthorDoorGUID;
            case DATA_ZURAMAT_DOOR:
                return m_uiZuramatDoorGUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_violet_hold";
    newscript->GetInstanceData = &GetInstanceData_instance_violet_hold;
    newscript->RegisterSelf();
}
