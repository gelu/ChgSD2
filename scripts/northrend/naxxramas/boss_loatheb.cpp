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
SDName: Boss_Loatheb
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_NECROTIC_AURA_FADE    = -1533130,
    
    SPELL_DEATHBLOOM        = 29865,
    H_SPELL_DEATHBLOOM      = 55053,
    SPELL_INEVITABLE_DOOM   = 29204,
    H_SPELL_INEVITABLE_DOOM = 55052,
    SPELL_NECROTIC_AURA     = 55593,

    SPELL_FUNGAL_CREEP      = 29232
};

#define ADD_1X 2957.040
#define ADD_1Y -3997.590
#define ADD_1Z 274.280

#define ADD_2X 2909.130
#define ADD_2Y -4042.970
#define ADD_2Z 274.280

#define ADD_3X 2861.102
#define ADD_3Y -3997.901
#define ADD_3Z 274.280

struct MANGOS_DLL_DECL boss_loathebAI : public ScriptedAI
{
    boss_loathebAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 DeathbloomTimer;
    uint32 InevitableDoomTimer;
    uint32 IDoomTimeShortage;
    uint32 IDoomCount;
    uint32 IDoom7minsTimer;
    uint32 SummonTimer;
    uint32 NecroticAuraTimer;
    uint32 NecroticAuraFadeWarning;

    void Reset()
    {
        DeathbloomTimer = 2500;
        InevitableDoomTimer = 120000;
        IDoomTimeShortage = 15000;
        IDoomCount = 1;
        IDoom7minsTimer = 300000;
        SummonTimer = 8000;
        NecroticAuraTimer = 1000;
        NecroticAuraFadeWarning = 15000;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO,0))
            summoned->AI()->AttackStart(target);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Necrotic Aura
        if ( NecroticAuraTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_NECROTIC_AURA);
            NecroticAuraTimer = 20000;
        }else NecroticAuraTimer -= uiDiff;

        // Necrotic Aura fade warning
        if ( NecroticAuraFadeWarning < uiDiff)
        {
            DoScriptText(SAY_NECROTIC_AURA_FADE, m_creature);
            NecroticAuraFadeWarning = 20000;
        }else NecroticAuraFadeWarning -= uiDiff;

        // Deathbloom
        if ( DeathbloomTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_DEATHBLOOM: H_SPELL_DEATHBLOOM);
            DeathbloomTimer = 30000;
        }else DeathbloomTimer -= uiDiff;

        // Inevitable Doom
        if ( InevitableDoomTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_INEVITABLE_DOOM : H_SPELL_INEVITABLE_DOOM);
            InevitableDoomTimer = 120000 - ( IDoomCount* IDoomTimeShortage);
            IDoomCount ++;
        }else InevitableDoomTimer -= uiDiff;

        // Inevitable Doom 7mins
        if ( IDoom7minsTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_INEVITABLE_DOOM);
            IDoom7minsTimer = 15000;
        }else IDoom7minsTimer -= uiDiff;

        // Summon
        if ( SummonTimer < uiDiff)
        {
            Unit* pSummonedSpores = NULL;

            pSummonedSpores = m_creature->SummonCreature(16286,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            pSummonedSpores = m_creature->SummonCreature(16286,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            pSummonedSpores = m_creature->SummonCreature(16286,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            if (pSummonedSpores)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    pSummonedSpores->AddThreat(pTarget);
            }

             SummonTimer = 28000;
        }
        else
             SummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_loatheb_sporesAI : public ScriptedAI
{
    npc_loatheb_sporesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 DieDelay_Timer;

    void Reset()
    {
        DieDelay_Timer = 0;
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && !DieDelay_Timer)
        {
            m_creature->CastSpell(m_creature, SPELL_FUNGAL_CREEP, true);
            DieDelay_Timer = 500;
        }
        if (DieDelay_Timer)
        {
            damage = 0;
            return;
        }
    }

    void JustDied(Unit* Killer) {}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DieDelay_Timer)
            if (DieDelay_Timer < diff)
            {
                m_creature->ForcedDespawn();
                DieDelay_Timer = 0;
            }else DieDelay_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_loatheb(Creature* pCreature)
{
    return new boss_loathebAI(pCreature);
}
CreatureAI* GetAI_npc_loatheb_spores(Creature* pCreature)
{
    return new npc_loatheb_sporesAI(pCreature);
}

void AddSC_boss_loatheb()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_loatheb";
    NewScript->GetAI = &GetAI_boss_loatheb;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_loatheb_spores";
    NewScript->GetAI = &GetAI_npc_loatheb_spores;
    NewScript->RegisterSelf();
}
