/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"
#include <math.h> 

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010
};

enum
{
    SPELL_MOJO_WAVE				= 55626,
    SPELL_MOJO_WAVE_H			= 58993,
    SPELL_MOJO_PUDDLE			= 55627,
    SPELL_MOJO_PUDDLE_H			= 58994,

    SPELL_MIGHTY_BLOW			= 54719,
    SPELL_EMERGE				= 54850,

    MOJO_ENTRY					= 29830,
    ELEMENTAL_ENTRY				= 29573,
    COLOSSUS_ENTRY				= 29307,

    MODELID_FAKE				= 26592,
    MODELID_RIGHT				= 26589
};

float distance = 10.0f;

#define START_POS_X					1672.959961f
#define START_POS_Y					743.487976f
#define START_POS_Z					143.337997f

/*######
## boss_colossus
######*/

struct MANGOS_DLL_DECL boss_colossusAI : public ScriptedAI
{
    boss_colossusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bHasCreatedList;
    bool m_bIsElementarPhase;
    bool m_bIsSpawnt;
    bool m_bIsDead;
    bool m_bElementsHasMoved;
    bool m_bEventStarted;
    bool m_bModelChanged;

    std::list<uint64> m_lLivingMojoGUIDList;

    ObjectGuid m_elementalGuid;
    ObjectGuid m_firstTargetPlayerGuid;

    uint32 m_uielementalLifepoints;
    uint32 m_uimightyBlowTimer;
    uint32 m_uiemergeTimer;
    uint32 m_uispawnElementalTimer;
    uint32 m_uichangePhaseTimer;
    uint32 m_uiElementalDeathTimer;
    uint32 m_uiFirstModelChangeTimer;

    void Reset()
    {
        m_bHasCreatedList = true;
        m_bIsElementarPhase = false;
        m_bIsSpawnt = false;
        m_bIsDead = false;
        m_bElementsHasMoved = false;
        m_bEventStarted = false;
        m_bModelChanged = false;

        m_uiemergeTimer = 12000;
        m_uispawnElementalTimer = 25000;
        m_uiElementalDeathTimer = 4000;
        m_uiFirstModelChangeTimer = 1000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, NOT_STARTED);

        RespawnElementalsIfDeadOrEvade();

        m_lLivingMojoGUIDList.clear();

        m_creature->RemoveAllAuras();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        m_creature->RemoveAllAuras();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, FAIL);
    }

    void JustDied(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);

        if(Creature* pElemental = m_creature->GetMap()->GetCreature(m_elementalGuid))
            pElemental->DealDamage(pElemental, pElemental->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    void RespawnElementalsIfDeadOrEvade()
    {
        if (!m_lLivingMojoGUIDList.empty() && m_pInstance)
        {
            for(std::list<uint64>::iterator itr = m_lLivingMojoGUIDList.begin(); itr != m_lLivingMojoGUIDList.end(); ++itr)
            {
                if (Creature* pMojo = m_pInstance->instance->GetCreature(*itr))
                {
                    if (!pMojo->isAlive())
                        pMojo->Respawn();
                    else
                        pMojo->AI()->EnterEvadeMode();

                    if (pMojo->GetVisibility() == VISIBILITY_OFF)
                        pMojo->SetVisibility(VISIBILITY_ON);
                }
            }
        }
    }

    void MoveInLineOfSight(Unit* pUnit)
    {
        if (pUnit->GetTypeId() == TYPEID_PLAYER && !m_bElementsHasMoved)
        {
            if (m_creature->GetDistance(pUnit) < 25.0f)
            {
                m_firstTargetPlayerGuid = ((Player*)pUnit)->GetObjectGuid();
                PrepareElementals();
                pUnit->SetInCombatWith(m_creature);
                m_creature->GetMotionMaster()->Clear(false);
                m_creature->GetMotionMaster()->MoveIdle();
                m_bEventStarted = true;
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (!m_lLivingMojoGUIDList.empty() && m_pInstance)
                {
                    for(std::list<uint64>::iterator itr = m_lLivingMojoGUIDList.begin(); itr != m_lLivingMojoGUIDList.end(); ++itr)
                    {
                        if (Creature* pElemental = m_pInstance->instance->GetCreature(*itr))
                        {
                            if (pElemental->isAlive())
                                pElemental->GetMotionMaster()->MovePoint(0, START_POS_X, START_POS_Y, START_POS_Z);
                        }
                    }
                }

                m_bElementsHasMoved = true;
            }
        }
    }

    void KillElementals()
    {
        if (!m_lLivingMojoGUIDList.empty() && m_pInstance)
        {
            for(std::list<uint64>::iterator itr = m_lLivingMojoGUIDList.begin(); itr != m_lLivingMojoGUIDList.end(); ++itr)
            {
                if (Creature* pElemental = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pElemental->isAlive())
                    {
                        m_creature->DealDamage(pElemental, pElemental->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); 
                        pElemental->SetVisibility(VISIBILITY_OFF);
                        if(Player* pFirstTarget = m_creature->GetMap()->GetPlayer(m_firstTargetPlayerGuid))
                        {
                            pFirstTarget->SetInCombatWith(m_creature);
                            m_creature->AddThreat(pFirstTarget);
                            m_creature->GetMotionMaster()->MoveChase(pFirstTarget);
                        }
                    }
                }
            }
        }
    }

    void PrepareElementals()
    {
        std::list<Creature*> lLivingMojoList;
        GetCreatureListWithEntryInGrid(lLivingMojoList,m_creature, MOJO_ENTRY, 50.0f);

        if (!lLivingMojoList.empty())
        {
            m_lLivingMojoGUIDList.clear();

            for(std::list<Creature*>::iterator itr = lLivingMojoList.begin(); itr != lLivingMojoList.end(); ++itr)
                m_lLivingMojoGUIDList.push_back((*itr)->GetGUID());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bEventStarted)
        {
            if (!m_bIsDead)
            {
                if (m_uiElementalDeathTimer < uiDiff)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    KillElementals();
                    m_bIsDead = true;
                    m_bEventStarted = false;
                }
                else 
                    m_uiElementalDeathTimer -= uiDiff;

                return;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsElementarPhase)
        {
            if (m_uichangePhaseTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->GetMotionMaster()->MoveChase(pTarget);
                    m_creature->SetInCombatWith(pTarget);
                }

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                
                m_uichangePhaseTimer = 25000;
                m_bIsElementarPhase = false;
            }else m_uichangePhaseTimer -= uiDiff;

            return;
        }

        if (m_uimightyBlowTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature->getVictim(), SPELL_MIGHTY_BLOW, false);
            m_uimightyBlowTimer = urand(14000, 18000);
        }
        else 
            m_uimightyBlowTimer -= uiDiff;

        if (m_uiemergeTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_EMERGE, true);
            m_uimightyBlowTimer += 3000;
            m_uiemergeTimer = 9999999;
            m_uispawnElementalTimer = 2700;
        }
        else
            m_uiemergeTimer -= uiDiff;

        if (m_uispawnElementalTimer < uiDiff)
        {
            m_bIsElementarPhase = true;
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();
            float x;
            float y;
            float winkel = m_creature->GetOrientation() / 0.01744444444444444f;
            if (winkel < 90)
            {
                float sinWinkel = sin(winkel);
                float h = sinWinkel * distance;
                float p = sqrt(distance * distance - h * h);
                x = p;
                y = h;
            }
            else if (winkel < 180)
            {
                float sinWinkel = sin(winkel);
                float h = sinWinkel * distance;
                float p = sqrt(distance * distance - h * h);
                x = p*(-1);
                y = h;
            }
            else if (winkel < 270)
            {
                float sinWinkel = sin(winkel);
                float h = sinWinkel * distance;
                float p = sqrt(distance * distance - h * h);
                x = p*(-1);
                y = h*(-1);
            }
            else
            {
                float sinWinkel = sin(winkel);
                float h = sinWinkel * distance;
                float p = sqrt(distance * distance - h * h);
                x = p;
                y = h*(-1);
            }

            if (!m_bIsSpawnt)
            {
                if (Creature* pElemental = m_creature->SummonCreature(ELEMENTAL_ENTRY, m_creature->GetPositionX() + x, m_creature->GetPositionY() + y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 2000))
                {
                    m_elementalGuid = pElemental->GetGUID();
                    pElemental->SetInCombatWith(m_creature->getVictim());
                    m_bIsSpawnt = true;
                }
            }
            else
            {
                if(Creature* pElemental = m_creature->GetMap()->GetCreature(m_elementalGuid))
                {
                    pElemental->NearTeleportTo(m_creature->GetPositionX() + x, m_creature->GetPositionY() + y, m_creature->GetPositionZ(), 0);
                    pElemental->SetInCombatWith(m_creature->getVictim());
                    pElemental->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    pElemental->SetVisibility(VISIBILITY_ON);
                    pElemental->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_uispawnElementalTimer = 25000;
            m_uichangePhaseTimer = 25000;
        }else m_uispawnElementalTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_colossus_elementalAI : public ScriptedAI
{
    mob_colossus_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMergeTimer;

    void Reset()
    {
        m_uiMergeTimer = 25000;
    }

    void Aggro(Unit* pWho)
    {
    }

    void JustDied(Unit* pKiller)
    {
        std::list<Creature*> coloss;
        GetCreatureListWithEntryInGrid(coloss, m_creature, COLOSSUS_ENTRY, 5000.0f);
        if (!coloss.empty())
        {
            for(std::list<Creature*>::iterator itr = coloss.begin(); itr != coloss.end(); ++itr)
            {
                if ((*itr)->isAlive())
                    (*itr)->DealDamage((*itr), (*itr)->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); 
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_creature->GetVisibility() == VISIBILITY_ON)
        {
            if (m_uiMergeTimer < uiDiff)
            {
                m_creature->SetVisibility(VISIBILITY_OFF);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();

                m_uiMergeTimer = 25000;
            }
            else 
                m_uiMergeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_mob_colossus_elemental(Creature* pCreature)
{
    return new mob_colossus_elementalAI(pCreature);
}

void AddSC_boss_colossus()
{
    Script *pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_colossus";
    pNewscript->GetAI = &GetAI_boss_colossus;
    pNewscript->RegisterSelf();

    pNewscript = new Script;
    pNewscript->Name = "mob_colossus_elemental";
    pNewscript->GetAI = &GetAI_mob_colossus_elemental;
    pNewscript->RegisterSelf();
}
