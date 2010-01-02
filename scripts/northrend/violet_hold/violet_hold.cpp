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
SDName: violet_hold
SDAuthor: ckegg
SD%Complete: 0
SDComment: 
SDCategory: The Violet Hold
EndScriptData */

#include "precompiled.h"
#include "def_violet_hold.h"

struct Locations
{
    float x, y, z;
    uint32 id;
};
struct WayPoints
{
    WayPoints(uint32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    uint32 id;
    float x, y, z;
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
};
static Locations BossLoc[]=
{
    {1857.125, 763.295, 38.654}, // Lavanthor
    {1925.480, 849.981, 47.174}, // Zuramat
    {1892.737, 744.589, 47.666}, // Moragg
    {1876.100, 857.079, 43.333}, // Erekem
    {1908.863, 785.647, 37.435}, // Ichoron
    {1905.364, 840.607, 38.670}, // Xevozz
};
static Locations DragonsWP[]=
{
    //center, ichoron
    {1869.393, 803.902, 38.768}, // 0 
    {1859.843, 804.222, 44.008}, // 1 
    {1827.960, 804.208, 44.364}, // 2 

    //From left side (lavanthor)
    {1861.016, 789.717, 38.908}, // 3 
    {1856.217, 796.705, 44.008}, // 4 
    {1827.960, 804.208, 44.364}, // 5 

    //From Zuramat
    {1931.446, 826.734, 47.556}, // 6 
    {1913.049, 823.930, 38.792}, // 7 
    {1827.960, 804.208, 44.364}, // 8 
    {1869.393, 803.902, 38.768}, // 9 
    {1859.843, 804.222, 44.008}, // 10 
    {1827.960, 804.208, 44.364}, // 11 

    //From Morag
    {1887.500, 763.096, 47.666}, // 12 
    {1880.837, 775.769, 38.796}, // 13 
    {1861.016, 789.717, 38.908}, // 14 
    {1856.217, 796.705, 44.008}, // 15 
    {1827.960, 804.208, 44.364}, // 16 

    //From erekem
    {1878.280, 843.267, 43.333}, // 17 
    {1872.311, 835.531, 38.780}, // 18 
    {1861.997, 818.766, 38.650}, // 19 
    {1857.348, 811.230, 44.008}, // 20
    {1827.960, 804.208, 44.364}, // 21 

    //From Highest platform
    {1937.298, 824.557, 52.332}, // 22
    {1913.049, 823.930, 38.792}, // 23
    {1869.393, 803.902, 38.768}, // 24
    {1859.843, 804.222, 44.008}, // 25
    {1827.960, 804.208, 44.364}, // 26

};
enum
{
    SPELL_TELEPORT_INSIDE                 = 62139,
    SPELL_SHIELD_DISRUPTION               = 58291,

    //DRAGONS SPELLS
    //Azure Captain
    SPELL_MORTAL_STRIKE                   = 32736,
    SPELL_WHIRLWIND                       = 41057,

    //Azure Raider
    SPELL_CONCUSSION_BLOW                 = 52719,
    SPELL_MAGIC_REFLECTION                = 60158,

    //Azure Sorceror
    SPELL_ARCANE_STREAM                   = 60181,
    SPELL_ARCANE_STREAM_H                 = 60204,
    SPELL_MANA_DETONATION                 = 60182,
    SPELL_MANA_DETONATION_H               = 60205,

    //Azure stalker
    SPELL_BACKSTAB                        = 58471,
    SPELL_TACTICAL_BLINK                  = 58470,
};

uint32 m_uiNextPortal_Timer;
/*######
## mob_vh_dragons
## This script is for ALL mobs which are spawned from portals,
## they have to be scripted in SD2 because in EventAI you cant
## check for distance from door seal :/
## (Intro not implented yet)
######*/
struct MANGOS_DLL_DECL mob_vh_dragonsAI : public ScriptedAI
{
    mob_vh_dragonsAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegular = pCreature->GetMap()->IsRegularDifficulty();
        WayPointList.clear();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegular;
    uint32 creatureEntry;
    uint32 motherPortalID;
    uint32 WalkTimer;
    int8 portalLoc;
    bool IsWalking;
    bool MovementStarted;
    Creature* pDoorSeal;

    std::list<WayPoints> WayPointList;
    std::list<WayPoints>::iterator WayPoint;

    //Azure Captain
    uint32 m_uiMortalStrike_Timer;
    uint32 m_uiWhirlwind_Timer;

    //Azure Raider
    uint32 m_uiConcussionBlow_Timer;
    uint32 m_uiMagicReflection_Timer;

    //Azure Sorceror
    uint32 m_uiArcaneStream_Timer;
    uint32 m_uiManaDetonation_Timer;

    //Azure Stalker
    uint32 m_uiBackstab_Timer;
    uint32 m_uiBlink_Timer;

    void Reset(){
        creatureEntry = m_creature->GetEntry();
        motherPortalID = 0;
        WalkTimer = 200;
        portalLoc = -1;
        IsWalking = false;
        MovementStarted = false;
        pDoorSeal = GetClosestCreatureWithEntry(m_creature, NPC_DOOR_SEAL, 150.0f);
       
        //Azure Captain
        m_uiMortalStrike_Timer = 3000;
        m_uiWhirlwind_Timer = 5000;

        //Azure Raider
        m_uiConcussionBlow_Timer = 3000;
        m_uiMagicReflection_Timer = 10000;

        //Azure Sorceror
        m_uiArcaneStream_Timer = 5000;
        m_uiManaDetonation_Timer = 3000;

        //Azure Stalker
        m_uiBackstab_Timer = 7100;
        m_uiBlink_Timer = 7000;
    }
    void StartMovement()
    {
        if(!WayPointList.empty() || MovementStarted)
            return;

        uint8 start = 0;
        uint8 end = 0;
        switch(portalLoc)
        {
            case -1:
                return;
            //center & ichoron
            case 0:
            case 5:
                start = 0;
                end = 2;
                break;
            //From lavanthor
            case 1:
                start = 3;
                end = 5;
                break;
            // From Zuramat
            case 2:
                start = 6;
                end = 11;
                break;
            //From Moragg
            case 3:
                start = 12;
                end = 16;
                break;
            //From Erekem
            case 4:
                start = 17;
                end = 21;
                break;
            //From highest platform
            case 6:
                start = 22;
                end = 26;
                break;
        }
        uint8 wpId = 0;
        for(uint8 i = start; i <= end; ++i){
            error_log("AddWP: %u", i);
            AddWaypoint(wpId, DragonsWP[i].x, DragonsWP[i].y, DragonsWP[i].z);
            wpId++;
        }

        WayPoint = WayPointList.begin();
        m_creature->AddMonsterMoveFlag(MONSTER_MOVE_WALK);
        IsWalking = true;
        MovementStarted = true;
    }
    void AddWaypoint(uint32 id, float x, float y, float z)
    {
        WayPoints DWP(id, x, y, z);
        WayPointList.push_back(DWP);
    }
    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE || creatureEntry == NPC_GUARDIAN || creatureEntry == NPC_KEEPER)
            return;

        if(WayPoint->id != uiPointId)
            return;

        ++WayPoint;
        WalkTimer = 200;
    }
    void UpdateAI(const uint32 uiDiff){
        if(portalLoc != -1)
            StartMovement();

        if (IsWalking && WalkTimer)
        {
            if (WalkTimer <= uiDiff)
            {
                if (WayPoint != WayPointList.end())
                {
                    m_creature->GetMotionMaster()->MovePoint(WayPoint->id, WayPoint->x, WayPoint->y,WayPoint->z);
                    WalkTimer = 0;
                }
            }else WalkTimer -= uiDiff;
        }

        //Corrupt Seal
        if(pDoorSeal){
            if(m_creature->IsWithinDist(pDoorSeal, 20.0f, false) && !m_creature->IsNonMeleeSpellCasted(false))
            {
                IsWalking = false;
                WayPointList.clear();
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);
                DoCast(pDoorSeal, SPELL_CORRUPT);
            }
        }

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_creature->getVictim())
            if(m_creature->getVictim()->GetEntry() == NPC_DOOR_SEAL)
                return;
        switch(creatureEntry)
        {
            case NPC_AZURE_CAPTAIN:
                AzureCaptain_UpdateAI(uiDiff);
                break;
            case NPC_AZURE_RAIDER:
                AzureRaider_UpdateAI(uiDiff);
                break;
            case NPC_AZURE_SORCEROR:
                AzureSorceror_UpdateAI(uiDiff);
                break;
            case NPC_AZURE_STALKER:
                AzureStalker_UpdateAI(uiDiff);
                break;
            case NPC_GUARDIAN:
            case NPC_KEEPER:
            case NPC_AZURE_BINDER:
            case NPC_AZURE_INVADER:
            case NPC_AZURE_MAGE_SLAYER:
            case NPC_AZURE_SPELLBREAKER:
                break;
            default:
                debug_log("SD2: The Violet Hold: Unhandled dragon entry %u!", m_creature->GetEntry());
                break;
        }
        DoMeleeAttackIfReady();
    }
    //Azure Captain
    void AzureCaptain_UpdateAI(const uint32 uiDiff)
    {
        //Mortal Strike
        if (m_uiMortalStrike_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
            m_uiMortalStrike_Timer = 6000;
        }else m_uiMortalStrike_Timer -= uiDiff;

        //Whirlwind
        if (m_uiWhirlwind_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_WHIRLWIND);
            m_uiWhirlwind_Timer = 15000;
        }else m_uiWhirlwind_Timer -= uiDiff;
    }
    //Azure Raider
    void AzureRaider_UpdateAI(const uint32 uiDiff)
    {
        //Concusion Blow
        if (m_uiConcussionBlow_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_CONCUSSION_BLOW);
            m_uiMortalStrike_Timer = 7000;
        }else m_uiConcussionBlow_Timer -= uiDiff;

        //Magic reflection
        if (m_uiMagicReflection_Timer <= uiDiff)
        {
            DoCast(m_creature, SPELL_MAGIC_REFLECTION);
            m_uiMagicReflection_Timer = 30000;
        }else m_uiMagicReflection_Timer -= uiDiff;
    }
    //Azure Sorceror
    void AzureSorceror_UpdateAI(const uint32 uiDiff)
    {
        //Arcane Stream
        if (m_uiArcaneStream_Timer <= uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, m_bIsRegular ? SPELL_ARCANE_STREAM : SPELL_ARCANE_STREAM_H);
            m_uiArcaneStream_Timer = 7000;
        }else m_uiArcaneStream_Timer -= uiDiff;

        //Mana Detonation
        if (m_uiManaDetonation_Timer <= uiDiff)
        {
            DoCast(m_creature, m_bIsRegular ? SPELL_MANA_DETONATION : SPELL_MANA_DETONATION_H);
            m_uiManaDetonation_Timer = 18000;
        }else m_uiManaDetonation_Timer -= uiDiff;
    }
    //Azure Stalker
    void AzureStalker_UpdateAI(const uint32 uiDiff)
    {
        //Backstab
        if (m_uiBackstab_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_BACKSTAB);
            m_uiBackstab_Timer = 15100;
        }else m_uiBackstab_Timer -= uiDiff;

        //Tactical blink
        if (m_uiBlink_Timer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_TACTICAL_BLINK);
            m_uiBlink_Timer = 15000;
        }else m_uiBlink_Timer -= uiDiff;
    }
};

/*######
## npc_violet_portal
######*/
struct MANGOS_DLL_DECL npc_violet_portalAI : public ScriptedAI
{
    npc_violet_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_uiGroupSpawned;
    uint8 portalType; // 0 = nothing, 1 = Guard & spawns, 2 = Group of elites
    uint32 portalID; // To identify portal...
    int8 portalLoc;

    uint32 TimeRiftWave_Timer;
    uint32 Check_Timer;

    void Reset()
    {
        m_uiGroupSpawned = false;
        portalType = 0;
        portalID = 0;
        portalLoc = -1;
        
        TimeRiftWave_Timer = 15000;
        Check_Timer = 5000;
    }
    uint32 SelectRandSummon()
    {
        uint32 entry = 0;
        if(portalType == 1)
        {
            switch (urand(0, 3))
            {
                case 0: entry = NPC_AZURE_BINDER; break;
                case 1: entry = NPC_AZURE_INVADER; break;
                case 2: entry = NPC_AZURE_MAGE_SLAYER; break;
                case 3: entry = NPC_AZURE_SPELLBREAKER; break;
            }      
        }else{
            switch (urand(0, 3))
            {
                case 0: entry = NPC_AZURE_CAPTAIN; break;
                case 1: entry = NPC_AZURE_RAIDER; break;
                case 2: entry = NPC_AZURE_SORCEROR; break;
                case 3: entry = NPC_AZURE_STALKER; break;
            }    
        }
        return entry;
    }
    void SpawnGroup()
    {
        if(portalType == 0)
            return;

        uint8 uiSpawnCount = (m_pInstance->GetData(TYPE_RIFT) < 12) ? 3 : 4;
        for(uint8 i = 0; i < uiSpawnCount; i++)
        {
            uint32 uiSpawnEntry = SelectRandSummon();
            if(Creature* pSummoned = m_creature->SummonCreature(uiSpawnEntry, m_creature->GetPositionX()-5+rand()%10, m_creature->GetPositionY()-5+rand()%10, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                error_log("Spawnuje NPC %u, motherPortalID %u, portalLoc %u", uiSpawnEntry, portalID, portalLoc);
                ((mob_vh_dragonsAI*)pSummoned->AI())->motherPortalID = portalID;
                ((mob_vh_dragonsAI*)pSummoned->AI())->portalLoc = portalLoc;		
            }
        }
    }
    bool IsThereNearElite(float range)
    {
        //Azure captain
        if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_CAPTAIN, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure raider
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_RAIDER, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure sorceror
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_SORCEROR, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        //Azure Stalker
        else if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, NPC_AZURE_STALKER, range))
        {
            if(((mob_vh_dragonsAI*)pTemp->AI())->motherPortalID == portalID)
                return true;
        }
        return false;
    }
    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance)
            return;

        if(m_pInstance->GetData(TYPE_EVENT) != IN_PROGRESS)
            return;

        switch(portalType)
        {
            case 0:
                return;
            case 1:
                if (TimeRiftWave_Timer < diff)
                {
                    error_log("SpawnGroup()");
                    SpawnGroup();
                    TimeRiftWave_Timer = 15000;
                }else TimeRiftWave_Timer -= diff;

                if (!m_creature->IsNonMeleeSpellCasted(false))
                {
                    m_uiNextPortal_Timer = 5000;
                    debug_log("SD2: npc_time_rift: not casting anylonger, i need to die.");
                    m_creature->setDeathState(JUST_DIED);
                }
                break;
            case 2:
                if(!m_uiGroupSpawned)
                {
                    SpawnGroup();
                    m_uiGroupSpawned = true;
                }
                if (Check_Timer < diff)
                {
                    if(!IsThereNearElite(150.0f))
                    {
                        m_uiNextPortal_Timer = 5000;
                        debug_log("SD2: npc_time_rift: No elite, i need to die.");
                        m_creature->setDeathState(JUST_DIED);
                    }
                    Check_Timer = 1000;
                }else Check_Timer -= diff;
                break;
        }
    }
};
/*######
## npc_sinclari
######*/
struct MANGOS_DLL_DECL npc_sinclariAI : public ScriptedAI
{
    npc_sinclariAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    uint8 m_uiRiftPortalCount;
    uint32 m_uiBossCheck_Timer;
    uint32 m_uiPortalCheck_Timer;

    void Reset()
    {
        m_uiRiftPortalCount = 0;
        m_uiNextPortal_Timer = 0;
        m_uiBossCheck_Timer = 0;
        m_uiPortalCheck_Timer = 1000;
    }

    void SetEvent()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        m_uiNextPortal_Timer = 5000;
        m_creature->GetMotionMaster()->MovePoint(0, 1815.571, 800.112, 44.364);
        if (m_pInstance){
            m_pInstance->SetData(TYPE_EVENT, IN_PROGRESS);           
            m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(DATA_SEAL_DOOR));
        }
    }

    void DoSpawnPortal()
    {
        int tmp = urand(1, 6);
        if (Creature* pTemp = m_creature->SummonCreature(NPC_PORTAL, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            //set portal type
            uint8 portalType = rand()%2+1;
            uint32 portalID = rand()%50000;
            ((npc_violet_portalAI*)pTemp->AI())->portalType = portalType; 
            ((npc_violet_portalAI*)pTemp->AI())->portalID = portalID;
            ((npc_violet_portalAI*)pTemp->AI())->portalLoc = tmp;
            
            if(portalType == 1)
            {
                uint32 entry = urand(0, 1) ? NPC_GUARDIAN : NPC_KEEPER;
                if (Creature* pSummoned = pTemp->SummonCreature(entry, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, pTemp->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000))
                {
                    pSummoned->AddThreat(pTemp);
                    pTemp->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL,false);
                    ((mob_vh_dragonsAI*)pSummoned->AI())->motherPortalID = portalID;
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance->GetData(TYPE_EVENT) == NOT_STARTED)
            return;

        if (m_uiNextPortal_Timer)
        {
            if (m_uiNextPortal_Timer <= uiDiff)
            {
                ++m_uiRiftPortalCount;
                if (m_pInstance)
                {
                    m_pInstance->DoUpdateWorldState(WORLD_STATE_VH, m_uiRiftPortalCount);
                    m_pInstance->SetData(TYPE_RIFT, SPECIAL);
                }

                if (m_uiRiftPortalCount != 6 && m_uiRiftPortalCount != 12 && m_uiRiftPortalCount != 18)
                {
                    DoSpawnPortal();
                    if (m_uiRiftPortalCount < 12)
                        m_uiNextPortal_Timer = 120000;
                    else
                        m_uiNextPortal_Timer = 90000;
                }
                else if (m_uiRiftPortalCount == 6 || m_uiRiftPortalCount == 12)
                {
                    if (Creature* pTemp = m_creature->SummonCreature(NPC_PORTAL, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_TIMED_DESPAWN, 1500))
                    {
                        Creature* pSummoned = m_creature->SummonCreature(NPC_AZURE_SABOTEUR, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);

                        pSummoned->AddThreat(pTemp);
                        pTemp->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL, false);
                    }
                    m_pInstance->SetData(TYPE_RIFT, IN_PROGRESS);
                    m_uiBossCheck_Timer = 1000;
                    m_uiNextPortal_Timer = 0;
                }
                else if (m_uiRiftPortalCount == 18)
                {
                    m_creature->SummonCreature(NPC_CYANIGOSA, PortalLoc[0].x, PortalLoc[0].y, PortalLoc[0].z, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 180000);
                    m_pInstance->SetData(TYPE_RIFT, IN_PROGRESS);
                    m_uiNextPortal_Timer = 0;
                }
            }
            else
                m_uiNextPortal_Timer -= uiDiff;

            return;
        }

        if (m_uiBossCheck_Timer)
        {
            if (m_uiBossCheck_Timer <= uiDiff)
            {
                if (!m_pInstance->GetData(DATA_BOSSTIME))
                    m_uiNextPortal_Timer = 30000;

                m_uiBossCheck_Timer = 1000;
            }
            else
                m_uiBossCheck_Timer -= uiDiff;

            return;
        }
    }
};

#define GOSSIP_ITEM_START_EVENT   "Activate the crystals when we get in trouble, right."
#define GOSSIP_ITEM_TELE_IN   "I need to go in!"

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance;
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu( pCreature->GetGUID() );

    if(m_pInstance->GetData(TYPE_EVENT) == NOT_STARTED){
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_START_EVENT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }else{
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TELE_IN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    }
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_sinclari(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            ((npc_sinclariAI*)pCreature->AI())->SetEvent();
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->CastSpell(pPlayer, SPELL_TELEPORT_INSIDE, false);
    }
    return true;
}
/*######
## npc_door_seal_vh
######*/
struct MANGOS_DLL_DECL npc_door_sealAI : public ScriptedAI
{
    npc_door_sealAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();   
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 CheckTimer;
    uint32 SpellCorrupt_Timer;
    uint8 lastPortal;

    void Reset()
    {
        CheckTimer = 0;
        SpellCorrupt_Timer = 0;
        lastPortal = 0;
}
    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (SpellCorrupt_Timer)
            return;

        if (spell->Id == SPELL_CORRUPT)
            SpellCorrupt_Timer = 1000;
    }
    void JustDied(Unit* pKiller)
    {
        m_creature->Respawn();
    }

    void UpdateAI(const uint32 diff){
        if (SpellCorrupt_Timer)
        {
            if (SpellCorrupt_Timer <= diff)
            {
                m_pInstance->SetData(TYPE_DOOR,SPECIAL);

                if (m_creature->HasAura(SPELL_CORRUPT,0))
                    SpellCorrupt_Timer = 1500;
                else
                    SpellCorrupt_Timer = 0;
            }else SpellCorrupt_Timer -= diff;
        }
    }
};

/*######
## npc_azure_saboteur
######*/
struct MANGOS_DLL_DECL npc_azure_saboteurAI : public ScriptedAI
{
    npc_azure_saboteurAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance *m_pInstance;

    bool m_bIsActiving;

    uint32 m_uiDisruption_Timer;
    uint32 m_uiDisruptionCounter;

    uint8 m_uiBossID;
    uint32 m_uiBossType;
    uint64 m_uiBossGUID;
    uint64 m_uiDoorGUID;

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void Reset()
    {
        m_bIsActiving = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveMonsterMoveFlag(MONSTER_MOVE_WALK);
        m_uiDisruptionCounter = 0;
        m_uiDisruption_Timer = 1000;

        if (m_pInstance)
        {
            m_uiBossID = m_pInstance->GetData(TYPE_LASTBOSS);

            switch (m_uiBossID)
            {
                case 0: // Lavanthor
                    m_uiBossType = TYPE_LAVANTHOR;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_LAVANTHOR);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_LAVANTHOR_DOOR);
                    break;
                case 1: // Zuramat
                    m_uiBossType = TYPE_ZURAMAT;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_ZURAMAT);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_ZURAMAT_DOOR);
                    break;
                case 2: // Moragg
                    m_uiBossType = TYPE_MORAGG;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_MORAGG);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_MORAGG_DOOR);
                    break;
                case 3: // Erekem
                    m_uiBossType = TYPE_EREKEM;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_EREKEM);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_EREKEM_DOOR);
                    break;
                case 4: // Ichoron
                    m_uiBossType = TYPE_ICHORON;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_ICHORON);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_ICHORON_DOOR);
                    break;
                case 5: // Xevozz
                    m_uiBossType = TYPE_XEVOZZ;
                    m_uiBossGUID = m_pInstance->GetData64(DATA_XEVOZZ);
                    m_uiDoorGUID = m_pInstance->GetData64(DATA_XEVOZZ_DOOR);
                    break;
            }
            m_creature->GetMotionMaster()->MovePoint(0, BossLoc[m_uiBossID].x,  BossLoc[m_uiBossID].y,  BossLoc[m_uiBossID].z);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE)
                return;

        switch(uiPointId)
        {
            case 0:
                m_bIsActiving = true;
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsActiving)
            if (m_uiDisruption_Timer < uiDiff)
            {
                if (m_uiDisruptionCounter < 3)
                    DoCast(m_creature, SPELL_SHIELD_DISRUPTION);
                else if (m_uiDisruptionCounter == 3)
                {
                    m_pInstance->DoUseDoorOrButton(m_uiDoorGUID);
                    m_pInstance->SetData(m_uiBossType, SPECIAL);
                }
                else
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                ++m_uiDisruptionCounter;
                m_uiDisruption_Timer = 1000;
            }
            else m_uiDisruption_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_azure_saboteur(Creature* pCreature)
{
    return new npc_azure_saboteurAI (pCreature);
}
CreatureAI* GetAI_mob_vh_dragons(Creature* pCreature)
{
    return new mob_vh_dragonsAI(pCreature);
}
CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI (pCreature);
}
CreatureAI* GetAI_npc_violet_portal(Creature* pCreature)
{
    return new npc_violet_portalAI (pCreature);
}
CreatureAI* GetAI_npc_door_seal(Creature* pCreature)
{
    return new npc_door_sealAI(pCreature);
}
void AddSC_violet_hold()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_sinclari";
    newscript->GetAI = &GetAI_npc_sinclari;
    newscript->pGossipHello =  &GossipHello_npc_sinclari;
    newscript->pGossipSelect = &GossipSelect_npc_sinclari;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_violet_portal";
    newscript->GetAI = &GetAI_npc_violet_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_door_seal_vh";
    newscript->GetAI = &GetAI_npc_door_seal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_azure_saboteur";
    newscript->GetAI = &GetAI_npc_azure_saboteur;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name = "mob_vh_dragons";
    newscript->GetAI = &GetAI_mob_vh_dragons;
    newscript->RegisterSelf();
}