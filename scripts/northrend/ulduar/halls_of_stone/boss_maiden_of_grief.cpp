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
SDName: Boss_Maiden_of_Grief
SD%Complete: 20%
SDComment:
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                   = -1599005,
    SAY_SLAY_1                  = -1599006,
    SAY_SLAY_2                  = -1599007,
    SAY_SLAY_3                  = -1599008,
    SAY_SLAY_4                  = -1599009,
    SAY_STUN                    = -1599010,
    SAY_DEATH                   = -1599011,

    SPELL_BERSERK               = 28747,
    SPELL_SHOCK_N               = 50760, // not worked
    SPELL_STOLP_N               = 50761,
    SPELL_STORM_N               = 50752, // not worked
    SPELL_MANABURN_N            = 59723, // not worked

    SPELL_SHOCK_H               = 50726, // not worked
//    SPELL_STOLP_H               = 50727, // not worked
    SPELL_STOLP_H               = 50761, 
    SPELL_STORM_H               = 50772, // not worked
    SPELL_MANABURN_H            = 59723, // not worked

    BERSERK_TIME_H               = 180000,
    BERSERK_TIME_N               = 300000,

    SPELL_STORM_OF_GRIEF        = 50752,
    SPELL_STORM_OF_GRIEF_H      = 59772,

    SPELL_SHOCK_OF_SORROW       = 50760,
    SPELL_SHOCK_OF_SORROW_H     = 59726,

    SPELL_PILLAR_OF_WOE         = 50761,
    SPELL_PILLAR_OF_WOE_H       = 59727,

    SPELL_PARTING_SORROW        = 59723
};

/*######
## boss_maiden_of_grief
######*/

struct MANGOS_DLL_DECL boss_maiden_of_griefAI : public ScriptedAI
{
    boss_maiden_of_griefAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32  m_uiStorm_Timer;
    uint32 m_uiShock_Timer;
    uint32 m_uiStolp_Timer;
    uint32 m_uiManaburn_Timer;
    uint32 m_uiBerserk_Timer;

    void Reset()
    {
        m_uiStorm_Timer = urand(25000, 40000);
        m_uiShock_Timer = urand(10000, 15000);
        m_uiStolp_Timer = urand(15000, 25000);
        m_uiManaburn_Timer = urand(30000, 60000);
        m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;

    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
            case 3: DoScriptText(SAY_SLAY_4, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
////
        if (m_uiStorm_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_STORM_N : SPELL_STORM_H );
//	    DoScriptText(SAY_SLAY_1, m_creature);
            m_uiStorm_Timer = urand(25000, 40000);
        }
        else
            m_uiStorm_Timer -= uiDiff;

        if (m_uiShock_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHOCK_N : SPELL_SHOCK_H );
//	    DoScriptText(SAY_SLAY_2, m_creature);
            m_uiShock_Timer = urand(10000, 15000);
        }
        else
            m_uiShock_Timer -= uiDiff;

        if (m_uiStolp_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_STOLP_N : SPELL_STOLP_H );
	    DoScriptText(SAY_SLAY_3, m_creature);
            m_uiStolp_Timer = urand(15000, 25000);
        }
        else
            m_uiStolp_Timer -= uiDiff;

        if (m_uiManaburn_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_MANABURN_N : SPELL_MANABURN_H );
	    DoScriptText(SAY_STUN, m_creature);
            m_uiStorm_Timer = urand(30000, 60000);
        }
        else
            m_uiStorm_Timer -= uiDiff;

if (m_uiBerserk_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;
            DoScriptText(SAY_SLAY_4, m_creature);
        }
        else
            m_uiBerserk_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_maiden_of_grief(Creature* pCreature)
{
    return new boss_maiden_of_griefAI(pCreature);
}

void AddSC_boss_maiden_of_grief()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_maiden_of_grief";
    newscript->GetAI = &GetAI_boss_maiden_of_grief;
    newscript->RegisterSelf();
}
