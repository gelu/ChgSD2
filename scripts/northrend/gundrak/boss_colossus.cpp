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

/* ScriptData
SDName: Boss_Colossus
SD%Complete: 20%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    EMOTE_SURGE                 = -1604008,
    EMOTE_SEEP                  = -1604009,
    EMOTE_GLOW                  = -1604010,

    // collosus' abilities
    SPELL_EMERGE				= 54850,
    SPELL_MIGHTY_BLOW			= 54719,
    SPELL_SUMMON_ELEMENTAL		= 54851,

    // elemental's abilities
    SPELL_MERGE					= 54878,
    SPELL_SURGE					= 54801,
    SPELL_SURGE_EFFECT			= 54827,
    SPELL_MOJO_PUDDLE			= 55627,
    SPELL_MOJO_PUDDLE_H			= 58994,
    SPELL_MOJO_WAVE				= 55626,
    SPELL_MOJO_WAVE_H			= 58993,
    SPELL_FREEZE				= 16245,

    NPC_LIVING_MOJO				= 29830
};

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
    bool m_bMergeMojoEvent;
    bool m_bMergeMojoEventStarted;
    
    uint32 m_uiMojoEventTimer;
    uint32 m_uiSummonEventTimer;

    bool m_bFirstEmerge;
    bool m_bSecondEmerge;
    bool m_bSummonElemental;
    bool m_bElementalDead;

    uint32 m_uiMightyBlowTimer;

    void Reset()
    {
        m_bMergeMojoEvent = false;
        m_bMergeMojoEventStarted = false;

        m_uiMojoEventTimer = 3000;
        m_uiSummonEventTimer = 2500;

        m_bFirstEmerge = false;
        m_bSecondEmerge = false;
        m_bSummonElemental = false;
        m_bElementalDead = false;

        m_uiMightyBlowTimer = 10000;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance)
            if(Creature* pElemental = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ELEMENTAL)))
                pElemental->ForcedDespawn();
    }

    void Aggro()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
    }

    void AttackStart(Unit *pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::AttackStart(pWho);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, NOT_STARTED);

        std::list<Creature*> pLivingMojos;
        GetCreatureListWithEntryInGrid(pLivingMojos, m_creature, NPC_LIVING_MOJO, 15.0f);

        if(!pLivingMojos.empty())
        {
            for (std::list<Creature*>::iterator iter = pLivingMojos.begin(); iter != pLivingMojos.end(); ++iter)
                (*iter)->Respawn();
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_COLOSSUS, DONE);
    }

    void JustSummoned(Creature *pSummoned)
    {
        // Second summon - when colossus nearly dead
        if (pSummoned->GetEntry() == NPC_ELEMENTAL)
            if (m_creature->GetHealthPercent() < 10.0f)
                pSummoned->SetHealthPercent(50.0f);
    }

    void DamageTaken(Unit* /*pDoneBy*/, uint32& uiDamage)
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            if (!m_bElementalDead)
            {
                uiDamage = 0;
                m_creature->SetHealth(1);
            }

            if (!m_bSecondEmerge)
            {
                m_bSecondEmerge = true;
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bSummonElemental = true;
                DoCast(m_creature, SPELL_EMERGE);
            }
        }
    }

    void Activate()
    {
        m_creature->RemoveAurasDueToSpell(SPELL_FREEZE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (Unit* pVictim = m_creature->getVictim())
            m_creature->GetMotionMaster()->MoveChase(pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_COLOSSUS) == IN_PROGRESS && !m_bMergeMojoEvent)
            {
                m_bMergeMojoEvent = true;

                std::list<Creature*> pLivingMojos;
                GetCreatureListWithEntryInGrid(pLivingMojos, m_creature, NPC_LIVING_MOJO, 15.0f);

                if (!pLivingMojos.empty())
                {
                    for (std::list<Creature*>::iterator iter = pLivingMojos.begin(); iter != pLivingMojos.end(); ++iter)
                    {
                        (*iter)->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                        (*iter)->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                    }
                }
            }
        }

        if (m_bMergeMojoEvent && !m_bMergeMojoEventStarted)
        {
            if (m_uiMojoEventTimer <= uiDiff)
            {
                m_bMergeMojoEventStarted = true;
                Activate();
            }
            else
                m_uiMojoEventTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMightyBlowTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_BLOW);
            m_uiMightyBlowTimer = 10000;
        }
        else
            m_uiMightyBlowTimer -= uiDiff;

        if (m_creature->GetHealthPercent() < 50.0f && !m_bFirstEmerge)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
            m_bFirstEmerge = true;
            m_bSummonElemental = true;
            m_uiSummonEventTimer = 2500;

            DoCast(m_creature, SPELL_EMERGE);
        }

        if (m_bSummonElemental)
        {
            if (m_uiSummonEventTimer <= uiDiff)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->InterruptNonMeleeSpells(true);
                m_creature->CastSpell(m_creature, SPELL_SUMMON_ELEMENTAL, true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
                SetCombatMovement(false);
                m_creature->CastSpell(m_creature, SPELL_FREEZE, true);
                m_bSummonElemental = false;
            }
            else
                m_uiSummonEventTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_elementalAI : public ScriptedAI
{
    boss_elementalAI (Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSurgeTimer;
    uint32 m_uiMojoPuddleTimer;
    uint32 m_uiMojoWaveTimer;

    bool m_bMerge;

    void Reset()
    {
        m_uiSurgeTimer = 7000;
        m_uiMojoPuddleTimer = 3000;
        m_uiMojoWaveTimer = 10000;
        m_bMerge = false;
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if (m_pInstance)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                ((boss_colossusAI*)pColossus->AI())->m_bElementalDead = true;
                pColossus->DealDamage(pColossus, pColossus->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_MERGE && pTarget->GetEntry() == NPC_COLOSSUS)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                ((boss_colossusAI*)pColossus->AI())->Activate();
                m_creature->ForcedDespawn();
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bMerge && m_creature->GetHealthPercent() < 50.0f && m_pInstance)
        {
            if (Creature* pColossus = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                if (pColossus->GetHealthPercent() > 10.0f)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->InterruptNonMeleeSpells(true);
                    DoCast(pColossus, SPELL_MERGE);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->SetUInt32Value(UNIT_FIELD_TARGET, 0);
                    SetCombatMovement(false);
                    m_bMerge = true;
                }
            }
        }

        if (m_uiSurgeTimer <= uiDiff)
        {
            DoScriptText(EMOTE_SURGE, m_creature);

            if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pVictim, SPELL_SURGE);

            m_uiSurgeTimer = 15000;
        }
        else
            m_uiSurgeTimer -= uiDiff;

        if (m_uiMojoPuddleTimer <= uiDiff)
        {
            if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pVictim, m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);

            m_uiMojoPuddleTimer = 3000;
        }
        else
            m_uiMojoPuddleTimer -= uiDiff;

        if (m_uiMojoWaveTimer <= uiDiff)
        {
            if (Unit *pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pVictim, m_bIsRegularMode ? SPELL_MOJO_WAVE : SPELL_MOJO_WAVE_H);

            m_uiMojoWaveTimer = urand(7000, 1000);
        }
        else
            m_uiMojoWaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_living_mojoAI : public ScriptedAI
{
    npc_living_mojoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMojoWaveTimer;
    uint32 m_uiMojoPuddleTimer;

    void Reset()
    {
        m_uiMojoWaveTimer = 2000;
        m_uiMojoPuddleTimer = 7000;
        SetCombatMovement(true);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pColossus = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_COLOSSUS)))
            {
                if (m_creature->GetDistance2d(pColossus) < 15.0f)
                {
                    SetCombatMovement(false);
                    m_pInstance->SetData(TYPE_COLOSSUS, IN_PROGRESS);
                }
            }
        }
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == 1)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_COLOSSUS) == IN_PROGRESS)
                return;
        }

        if (m_uiMojoWaveTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_WAVE : SPELL_MOJO_WAVE_H);
            m_uiMojoWaveTimer = urand(12000, 15000);
        }
        else
            m_uiMojoWaveTimer -= uiDiff;

        if (m_uiMojoPuddleTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MOJO_PUDDLE : SPELL_MOJO_PUDDLE_H);
            m_uiMojoPuddleTimer = urand(10000, 20000);
        }
        else
            m_uiMojoPuddleTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_colossus(Creature* pCreature)
{
    return new boss_colossusAI(pCreature);
}

CreatureAI* GetAI_boss_elemental(Creature* pCreature)
{
    return new boss_elementalAI(pCreature);
}

CreatureAI* GetAI_npc_living_mojo(Creature* pCreature)
{
    return new npc_living_mojoAI(pCreature);
}

void AddSC_boss_colossus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_colossus";
    newscript->GetAI = &GetAI_boss_colossus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elemental";
    newscript->GetAI = &GetAI_boss_elemental;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_living_mojo";
    newscript->GetAI = &GetAI_npc_living_mojo;
    newscript->RegisterSelf();
}
