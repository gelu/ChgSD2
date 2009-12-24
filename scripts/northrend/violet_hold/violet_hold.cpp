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

static Locations BossLoc[]=
{
    {1857.125, 763.295, 38.654}, // Lavanthor
    {1925.480, 849.981, 47.174}, // Zuramat
    {1892.737, 744.589, 47.666}, // Moragg
    {1876.100, 857.079, 43.333}, // Erekem
    {1908.863, 785.647, 37.435}, // Ichoron
    {1905.364, 840.607, 38.670}, // Xevozz
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

enum
{
    SPELL_SHIELD_DISRUPTION               = 58291,
};

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

struct MANGOS_DLL_DECL npc_sinclariAI : public ScriptedAI
{
    npc_sinclariAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
    	m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
    	Reset();
    }
    ScriptedInstance *m_pInstance;

    uint8 m_uiRiftPortalCount;
    uint32 m_uiNextPortal_Timer;
    uint32 m_uiBossCheck_Timer;

    void Reset()
    {
        m_uiRiftPortalCount = 0;
        m_uiNextPortal_Timer = 0;
        m_uiBossCheck_Timer = 0;
    }

    void SetEvent()
    {
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);
        m_uiNextPortal_Timer = 5000;
        if (m_pInstance)
            m_pInstance->DoUseDoorOrButton(m_pInstance->GetData64(DATA_SEAL_DOOR));
    }

    void DoSpawnPortal()
    {
        int tmp = urand(1, 6);
        if (Creature* pTemp = m_creature->SummonCreature(NPC_PORTAL, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pTemp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            uint32 entry = urand(0, 1) ? NPC_GUARDIAN : NPC_KEEPER;
            if (Creature* pSummoned = pTemp->SummonCreature(entry, PortalLoc[tmp].x, PortalLoc[tmp].y, PortalLoc[tmp].z, pTemp->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000))
            {
                pSummoned->AddThreat(pTemp);
                pTemp->CastSpell(pSummoned, SPELL_PORTAL_CHANNEL,false);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
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
                        m_uiNextPortal_Timer = 140000;
                    else
                        m_uiNextPortal_Timer = 120000;
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

bool GossipHello_npc_sinclari(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu( pCreature->GetGUID() );

    pPlayer->ADD_GOSSIP_ITEM( 0, "I am ready.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());

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
    }
    return true;
}

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

    uint32 TimeRiftWave_Timer;

    void Reset()
    {
        TimeRiftWave_Timer = 30000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_pInstance)
            return;

        if (TimeRiftWave_Timer < diff)
        {
            uint8 uiSpawnCount = (m_pInstance->GetData(TYPE_RIFT) < 12) ? 3 : 4;
            for(uint8 i = 0; i < uiSpawnCount; i++)
            {
                uint32 uiSpawnEntry = 0;
                switch (urand(0, 3))
                {
                    case 0: uiSpawnEntry = NPC_AZURE_CAPTAIN; break;
                    case 1: uiSpawnEntry = NPC_AZURE_RAIDER; break;
                    case 2: uiSpawnEntry = NPC_AZURE_SORCEROR; break;
                    case 3: uiSpawnEntry = NPC_AZURE_STALKER; break;
                }
                if (Creature* pTemp = m_creature->SummonCreature(uiSpawnEntry, m_creature->GetPositionX()-5+rand()%10, m_creature->GetPositionY()-5+rand()%10, m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
                {
                    if (Creature* pTarget = GetClosestCreatureWithEntry(m_creature, NPC_SINCLARI, 150.0f))
                    {
                        pTemp->AddThreat(pTarget);
                        pTemp->AI()->AttackStart(pTarget);
                    }
                }
            }

            TimeRiftWave_Timer = 30000;
        }else TimeRiftWave_Timer -= diff;

        if (m_creature->IsNonMeleeSpellCasted(false))
            return;

        debug_log("SD2: npc_time_rift: not casting anylonger, i need to die.");
        m_creature->setDeathState(JUST_DIED);
    }
};

CreatureAI* GetAI_npc_sinclari(Creature* pCreature)
{
    return new npc_sinclariAI (pCreature);
}

CreatureAI* GetAI_npc_azure_saboteur(Creature* pCreature)
{
    return new npc_azure_saboteurAI (pCreature);
}

CreatureAI* GetAI_npc_violet_portal(Creature* pCreature)
{
    return new npc_violet_portalAI (pCreature);
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
    newscript->Name = "npc_azure_saboteur";
    newscript->GetAI = &GetAI_npc_azure_saboteur;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_violet_portal";
    newscript->GetAI = &GetAI_npc_violet_portal;
    newscript->RegisterSelf();

}
