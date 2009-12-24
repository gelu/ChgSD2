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
SDName: Boss_Krystallus
SD%Complete: 40%
SDComment: Need adding summons and correct casting time(s), add boss texts
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                   = -1712001,
    SAY_SLAY_1                  = -1712002,
    SAY_SLAY_2                  = -1712003,
    SAY_SLAY_3                  = -1712004,
    SAY_SLAY_4                  = -1712005,
    SAY_KILL                    = -1712006,
    SAY_DEATH                   = -1712007,
    SAY_BERSERK                 = -1712008,


    SPELL_BOMB_N         = 50843,
    SPELL_SPIKE_N        = 59750,
    SPELL_ICE_N          = 61546,
    SPELL_TOPOT_N        = 50868,

    SPELL_BOMB_H         = 59742,
    SPELL_SPIKE_H        = 59750,
    SPELL_ICE_H          = 61546,
    SPELL_TOPOT_H        = 59744,

    SPELL_BERSERK                = 28747,
    BERSERK_TIME_H               = 180000,
    BERSERK_TIME_N               = 300000

};

/*######
## boss_krystallus
######*/

struct MANGOS_DLL_DECL boss_krystallusAI : public ScriptedAI
{
    boss_krystallusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBomb_Timer;
    uint32 m_uiSpike_Timer;
    uint32 m_uiIce_Timer;
    uint32 m_uiTopot_Timer;
    uint32 m_uiBerserk_Timer;

    void Reset()
    {
        m_uiBomb_Timer = urand(10000, 20000);
        m_uiSpike_Timer = urand(20000, 30000);
        m_uiIce_Timer = urand(25000, 35000);
        m_uiTopot_Timer = urand(35000, 45000);
        m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
//////////
        if (m_uiBomb_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_BOMB_N : SPELL_BOMB_H );
            m_uiBomb_Timer = urand(10000, 20000);
            DoScriptText(SAY_SLAY_1, m_creature);
        }
        else
            m_uiBomb_Timer -= uiDiff;

if (m_uiSpike_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SPIKE_N : SPELL_SPIKE_H);
            m_uiSpike_Timer = urand(20000, 30000);
            DoScriptText(SAY_SLAY_3, m_creature);
        }
        else
            m_uiSpike_Timer -= uiDiff;

if (m_uiIce_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_ICE_N : SPELL_ICE_H);
            DoScriptText(SAY_SLAY_2, m_creature); 
            m_uiIce_Timer = urand(25000, 35000);
        }
        else
            m_uiIce_Timer -= uiDiff;

if (m_uiTopot_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            DoCast(pTarget, m_bIsRegularMode ? SPELL_TOPOT_N : SPELL_TOPOT_H);
            m_uiTopot_Timer = urand(20000, 45000);
            DoScriptText(SAY_SLAY_4, m_creature);
        }
        else
            m_uiTopot_Timer -= uiDiff;

if (m_uiBerserk_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;
            DoScriptText(SAY_BERSERK, m_creature);
        }
        else
            m_uiBerserk_Timer -= uiDiff;
/////////////////
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krystallus(Creature* pCreature)
{
    return new boss_krystallusAI(pCreature);
}

void AddSC_boss_krystallus()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_krystallus";
    newscript->GetAI = &GetAI_boss_krystallus;
    newscript->RegisterSelf();
}
