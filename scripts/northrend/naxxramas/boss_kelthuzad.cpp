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
SDName: Boss_KelThuzud
SD%Complete: 0
SDComment: VERIFY SCRIPT
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //when shappiron dies. dialog between kel and lich king (in this order)
    SAY_SAPP_DIALOG1            = -1533084,
    SAY_SAPP_DIALOG2_LICH       = -1533085,
    SAY_SAPP_DIALOG3            = -1533086,
    SAY_SAPP_DIALOG4_LICH       = -1533087,
    SAY_SAPP_DIALOG5            = -1533088,

    //when cat dies
    SAY_CAT_DIED                = -1533089,

    //when each of the 4 wing bosses dies
    SAY_TAUNT1                  = -1533090,
    SAY_TAUNT2                  = -1533091,
    SAY_TAUNT3                  = -1533092,
    SAY_TAUNT4                  = -1533093,

    SAY_SUMMON_MINIONS          = -1533105,                //start of phase 1

    SAY_AGGRO1                  = -1533094,                //start of phase 2
    SAY_AGGRO2                  = -1533095,
    SAY_AGGRO3                  = -1533096,

    SAY_SLAY1                   = -1533097,
    SAY_SLAY2                   = -1533098,

    SAY_DEATH                   = -1533099,

    SAY_CHAIN1                  = -1533100,
    SAY_CHAIN2                  = -1533101,
    SAY_FROST_BLAST             = -1533102,

    SAY_REQUEST_AID             = -1533103,                //start of phase 3
    SAY_ANSWER_REQUEST          = -1533104,                //lich king answer

    SAY_SPECIAL1_MANA_DET       = -1533106,
    SAY_SPECIAL3_MANA_DET       = -1533107,
    SAY_SPECIAL2_DISPELL        = -1533108,

    //spells to be casted
    SPELL_FROST_BOLT            = 28478,
    H_SPELL_FROST_BOLT          = 55802,
    SPELL_FROST_BOLT_VOLLEY       = 28479,
    H_SPELL_FROST_BOLT_VOLLEY     = 55807,

    SPELL_CHAINS_OF_KELTHUZAD   = 28410,                   //casted spell should be 28408. Also as of 303, heroic only
    SPELL_MANA_DETONATION       = 27819,
    SPELL_SHADOW_FISURE         = 27810,
    SPELL_FROST_BLAST           = 27808,

    NPC_SOLDIERS_FROZEN_WASTES  = 16427,
    NPC_UNSTOPPABLE_ABOMINATIONS= 16428,
    NPC_SOUL_WEAVERS            = 16429,

    NPC_GUARDIAN                = 16441,
};

//Positional defines
const float AddPos[6][4] =
{
    {3769.2f, -5071.6f, 143.7f, 3.6f},
    {3729.3f, -5044.24f, 143.96f, 4.5f},
    {3683.87f, -5057.28f, 143.18f, 5.2f},
    {3749.35f, -5158.12f, 143.8f, 2.2f},
    {3703.73f, -5169.12f, 143.93f, 1.3f},
    {3665.12f, -5138.68f, 143.18f, 0.6f},
};

const float MovePos[6][4] =
{
    {3754.4f, -5080.73f, 142.03f, 3.7f},
    {3724.39f, -5061.33f, 142.03f, 4.6f},
    {3687.16f, -5076.83f, 142.02f, 5.2f},
    {3687.57f, -5126.83f, 142.01f, 0.6f},
    {3707.99f, -5151.45f, 142.03f, 1.4f},
    {3739.5f, -5141.88f, 142.01f, 2.1f},
};

const float Middle[3] = {3716.384f, -5106.453f, 142.0f};
const float Home[2] = {3748, -5113};

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lSummonsGUIDList;
    std::list<uint64>::iterator m_uiSendSummon;

    uint32 GuardiansOfIcecrown_Timer;
    uint32 GuardiansOfIcecrown_Count;
    uint32 GuardiansOfIcecrown_Max;
    uint32 FrostBolt_Timer;
    uint32 FrostBoltVolley_Timer;
    uint32 ChainsOfKelthuzad_Timer;
    uint32 ManaDetonation_Timer;
    uint32 ShadowFisure_Timer;
    uint32 FrostBlast_Timer;
    uint32 ChainsOfKelthuzad_Targets;
    uint32 Phase1_Timer;
    uint32 Phase1Encounter_Timer;
    uint32 DropChains_Timer;
    bool SendSummon;
    bool Phase1;
    bool Phase2;
    bool PhaseGuardian;
    bool DropChains_Check;

    void Reset()
    {
        FrostBolt_Timer = urand(20000, 25000);              //Frostbolt casted every 20-25 sec
        FrostBoltVolley_Timer = 15000;                      //Frostbolt Volley casted every 15 sec
        ChainsOfKelthuzad_Timer = urand(40000, 50000);      //Posses casted every 40-50 sec
        ManaDetonation_Timer = 20000;                       //Mana Detionation casted every 20 sec
        ShadowFisure_Timer = 25000;                         //Shadow Fissure spawned every 25 sec
        FrostBlast_Timer = (rand()%30+30)*1000;             //Random time between 30-60 seconds
        GuardiansOfIcecrown_Timer = 5000;                   //5 seconds for summoning each Guardian of Icecrown in phase 3
        GuardiansOfIcecrown_Max = m_bIsRegularMode ? 2 : 4;
        GuardiansOfIcecrown_Count = 0;
        Phase1 = false;
        Phase2 = false;
        PhaseGuardian = false;
        DropChains_Check = false;

        DespawnSummons();

        Phase1_Timer = 228000;                              //Phase 1 lasts 3 minutes and 48 seconds
        Phase1Encounter_Timer = 3000;
        SendSummon = false;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, NOT_STARTED);
    }

    void KilledUnit()
    {
        if (rand()%2)
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);

        DespawnSummons();
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (!who)
            return;

        if (Phase1 || Phase2)
            return;

        if (who->isTargetableForAttack() && who->GetTypeId() == TYPEID_PLAYER && m_creature->GetDistance2d(who) <= 50)
        {
            m_creature->AddThreat(who, 0.0f);
            m_creature->SetInCombatWith(who);

            for(uint8 i = 0; i <= 80; ++i)
            {
                if (i == 5 || i == 15 || i == 25 || i == 35 || i == 45 || i == 55 || i == 65 || i == 75)
                    DoSpawnAdds(NPC_SOUL_WEAVERS);
                else if (i == 10 || i == 20 || i == 30 || i == 40 || i == 50 || i == 60 || i == 70 || i == 80)
                    DoSpawnAdds(NPC_UNSTOPPABLE_ABOMINATIONS);
                else
                    DoSpawnAdds(NPC_SOLDIERS_FROZEN_WASTES);
            }
            if (!m_lSummonsGUIDList.empty())
            {
                m_uiSendSummon = m_lSummonsGUIDList.begin();
                SendSummon = true;
            }
            Phase1 = true;
        }
    }

    void Aggro(Unit* who)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (Phase1)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            m_creature->SetInCombatWithZone();
            DoStartMovement(pWho);
        }
    }

    void DoSpawnAdds(uint32 uiEntry)
    {
        int8 Pos = rand()%6;
        if (Creature* pTemp = m_creature->SummonCreature(uiEntry, AddPos[Pos][0]-5 + rand()%10, AddPos[Pos][1]-5 + rand()%10, AddPos[Pos][2], 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000))
            m_lSummonsGUIDList.push_back(pTemp->GetGUID());
    }

    void DespawnSummons()
    {
        if (m_lSummonsGUIDList.empty())
            return;

        for(std::list<uint64>::iterator itr = m_lSummonsGUIDList.begin(); itr != m_lSummonsGUIDList.end(); ++itr)
        {
            if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                if (pTemp->isAlive())
                    pTemp->ForcedDespawn();
        }

        m_lSummonsGUIDList.clear();

        std::list<Creature*> m_pGuardian;
        GetCreatureListWithEntryInGrid(m_pGuardian, m_creature, NPC_GUARDIAN, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pGuardian.empty())
            for(std::list<Creature*>::iterator itr = m_pGuardian.begin(); itr != m_pGuardian.end(); ++itr)
            {
                (*itr)->ForcedDespawn();
            }
    }

    void Possess()
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
        {
            pTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
            pTarget->setFaction(pTarget->getFaction());
        }
    }

    void UpdateAI(const uint32 diff)
    {
        // First Phase
        if (Phase1)
        {
            if (SendSummon)
            {
                if (Phase1Encounter_Timer < diff)
                {
                    if (m_lSummonsGUIDList.empty())
                        return;

                    if (m_uiSendSummon != m_lSummonsGUIDList.end())
                    {
                        if (Creature* pTemp = m_creature->GetMap()->GetCreature(*m_uiSendSummon))
                            if (pTemp->isAlive() && !pTemp->getVictim())
                                pTemp->GetMotionMaster()->MovePoint(0, Middle[0], Middle[1], Middle[2]);
                        ++m_uiSendSummon;
                        Phase1Encounter_Timer = 3000;
                    }
                    else
                        SendSummon = false;

                }else Phase1Encounter_Timer -= diff;
            }
            
            if (Phase1_Timer < diff)
            {
                if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                AttackStart(m_creature->getVictim());
                Phase1 = false;
                Phase2 = true;
            }else Phase1_Timer -= diff;
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Spell casting for second and third Phase
        if(Phase2)
        {
            //start phase 3 when we are 40% health
            if (!PhaseGuardian && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40)
            {
                PhaseGuardian = true;
                DoScriptText(SAY_REQUEST_AID, m_creature);
                //here Lich King should respond to KelThuzad but I don't know which creature to make talk
                //so for now just make Kelthuzad says it.
                DoScriptText(SAY_ANSWER_REQUEST, m_creature);
            }

            //Check for Frost Bolt
            if (FrostBolt_Timer < diff)
            {
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT : H_SPELL_FROST_BOLT, true);
                FrostBolt_Timer = urand(20000, 25000);
            }else FrostBolt_Timer -= diff;

            //Check for Frost Bolt Volley
            if (FrostBoltVolley_Timer < diff)
            {
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_FROST_BOLT_VOLLEY : H_SPELL_FROST_BOLT_VOLLEY, true);
                FrostBoltVolley_Timer = 15000;
            }else FrostBoltVolley_Timer -= diff;

            //Check for Chains Of Kelthuzad
            if (ChainsOfKelthuzad_Timer < diff && !m_bIsRegularMode)
            {
                //DoCast(m_creature->getVictim(),SPELL_CHAINS_OF_KELTHUZAD);
                Possess();
                Possess();
                Possess();

                if (rand()%2)
                    DoScriptText(SAY_CHAIN1, m_creature);
                else
                    DoScriptText(SAY_CHAIN2, m_creature);

                ChainsOfKelthuzad_Timer = (rand()%30+30)*1000;
                DropChains_Timer = 20000;
                DropChains_Check = true;
            }else ChainsOfKelthuzad_Timer -= diff;

            //Restore faction
            if (DropChains_Timer < diff && DropChains_Check)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                if (!lPlayers.isEmpty())
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        if (Player* pPlayer = itr->getSource())
                        {
                            pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED);
                            pPlayer->setFactionForRace(pPlayer->getRace());
                        }
                    }
                DropChains_Check = false;
            }else DropChains_Timer -= diff;

            //Check for Mana Detonation
            if (ManaDetonation_Timer < diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    if (pTarget->getPowerType() == POWER_MANA)
                    {
                        int32 curPower = pTarget->GetPower(POWER_MANA);
                        if (curPower < (m_bIsRegularMode ? 4000 : 5500))
                            return;

                        m_creature->CastSpell(pTarget,SPELL_MANA_DETONATION, true);
                        int32 manareduction =  m_bIsRegularMode ? urand(2500,4000) : urand(3500,5500);
                        int32 mana = curPower - manareduction;
                        pTarget->SetPower(POWER_MANA, mana);

                        Map *map = m_creature->GetMap();
                        if (map->IsDungeon())
                        {
                            Map::PlayerList const &PlayerList = map->GetPlayers();

                            if (!PlayerList.isEmpty())

                                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                {
                                    if (i->getSource()->isAlive() && pTarget->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 15)
                                        i->getSource()->DealDamage(i->getSource(), manareduction, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, true);
                                }
                        } 
                    }

                if (rand()%2)
                    DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);

                ManaDetonation_Timer = 15000;
            }else ManaDetonation_Timer -= diff;

            //Check for Shadow Fissure
            if (ShadowFisure_Timer < diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    DoCast(pTarget,SPELL_SHADOW_FISURE);

                if (rand()%2)
                    DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

                ShadowFisure_Timer = 25000;
            }else ShadowFisure_Timer -= diff;

            //Check for Frost Blast
            if (FrostBlast_Timer < diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    m_creature->CastSpell(pTarget,SPELL_FROST_BLAST,true);

                if (rand()%2)
                    DoScriptText(SAY_FROST_BLAST, m_creature);

                FrostBlast_Timer = urand(40000,50000);
            }else FrostBlast_Timer -= diff;
        }
        
        //Guardian Summoning
        if (PhaseGuardian && (GuardiansOfIcecrown_Count < GuardiansOfIcecrown_Max))
        {
            if (GuardiansOfIcecrown_Timer < diff)
            {
                int8 Pos = rand()%6;
                if( Creature* pGuardian = m_creature->SummonCreature(NPC_GUARDIAN, AddPos[Pos][0], AddPos[Pos][1], AddPos[Pos][2], AddPos[Pos][3], TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000))
                {
                    pGuardian->Attack(m_creature->getVictim(),true);
                    pGuardian->GetMotionMaster()->MoveChase(m_creature->getVictim());
                }
            
                ++GuardiansOfIcecrown_Count;
                //5 seconds until summoning next guardian
                GuardiansOfIcecrown_Timer = 5000;
            }else GuardiansOfIcecrown_Timer -= diff;
        }

        if (m_creature->GetDistance2d(Home[0], Home[1]) > 80)
            EnterEvadeMode();

        DoMeleeAttackIfReady();
    }
};


/*######
## Mob Shadow Issure
######*/

struct MANGOS_DLL_DECL mob_shadow_issureAI : public ScriptedAI
{
    mob_shadow_issureAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowIssure_Timer;

    void AttackStart(){}
    void Reset()
    {
        m_uiShadowIssure_Timer = 4000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiShadowIssure_Timer)
            if (m_uiShadowIssure_Timer < uiDiff)
            {
                Map *map = m_creature->GetMap();
                if (map->IsDungeon())
                {
                    Map::PlayerList const &PlayerList = map->GetPlayers();

                    if (PlayerList.isEmpty())
                        return;

                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        if (i->getSource()->isAlive() && m_creature->GetDistance2d(i->getSource()->GetPositionX(), i->getSource()->GetPositionY()) < 2)
                            i->getSource()->DealDamage(i->getSource(), i->getSource()->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
                m_creature->ForcedDespawn();
            }
            else m_uiShadowIssure_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_kelthuzadAI(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

CreatureAI* GetAI_mob_shadow_issureAI(Creature* pCreature)
{
    return new mob_shadow_issureAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kelthuzad";
    newscript->GetAI = &GetAI_boss_kelthuzadAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadow_issure";
    newscript->GetAI = &GetAI_mob_shadow_issureAI;
    newscript->RegisterSelf();
}
