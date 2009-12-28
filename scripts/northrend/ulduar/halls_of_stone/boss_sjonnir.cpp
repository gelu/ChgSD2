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
SDName: Boss_Sjonnir
SD%Complete: 50%
SDComment: Need adding summons and correct casting time
SDCategory: Halls of Stone
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_AGGRO                   = -1599000,
    SAY_SLAY_1                  = -1599001,
    SAY_SLAY_2                  = -1599002,
    SAY_SLAY_3                  = -1599003,
    SAY_DEATH                   = -1599004,
    EMOTE_GENERIC_FRENZY        = -1000002,

    SPELL_BERSERK                 = 28747,
    SPELL_RING_LIGHTNING_H        = 59848,
    SPELL_STATIC_OVERLOAD_H       = 59846,

    SPELL_RING_LIGHTNING_N        = 50840,
    SPELL_STATIC_OVERLOAD_N       = 50834,
    SPELL_CHAIN_LIGHTING_N        = 50830,
    SPELL_CHAIN_LIGHTING_H        = 59844,

    BERSERK_TIME_H               = 180000,
    BERSERK_TIME_N               = 300000,

    SPELL_FRENZY                    = 28747,

    SPELL_CHAIN_LIGHTNING           = 50830,
//    SPELL_CHAIN_LIGHTNING_H         = 59844,

    SPELL_STATIC_CHARGE             = 50834,
    SPELL_STATIC_CHARGE_H           = 59846,

    SPELL_LIGHTNING_SHIELD          = 50831,
    SPELL_LIGHTNING_SHIELD_H        = 59845,

    SPELL_LIGHTNING_RING            = 50840,
    SPELL_LIGHTNING_RING_H          = 59848,

    SPELL_SUMMON_IRON_TROGG         = 50792,                // periodic dummy aura, tick each 10sec or each 7sec in heroic
    SPELL_SUMMON_IRON_TROGG_H       = 59859,                // left/right 50790,50791

    SPELL_SUMMON_MALFORMED_OOZE     = 50801,                // periodic dummy aura, tick each 5sec or each 3sec in heroic
    SPELL_SUMMON_MALFORMED_OOZE_H   = 59858,                // left/right 50802,50803

    SPELL_SUMMON_IRON_SLUDGE        = 50747,                // instakill TARGET_SCRIPT
    SPELL_IRON_SLUDGE_SPAWN_VISUAL  = 50777,

    NPC_IRON_TROGG                  = 27979,
    NPC_MALFORMED_OOZE              = 27981,
    NPC_IRON_SLUDGE                 = 28165
};

/*######
## boss_sjonnir
######*/

struct MANGOS_DLL_DECL boss_sjonnirAI : public ScriptedAI
{
    boss_sjonnirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStaticOverload_Timer;
    uint32 m_uiRingLightning_Timer;
    uint32 m_uiChainLightning_Timer;
    uint32 m_uiBerserk_Timer;

    void Reset()
    {
        m_uiStaticOverload_Timer = urand(5000, 6000);
        m_uiRingLightning_Timer = urand(30000, 45000);
        m_uiChainLightning_Timer = urand(15000, 17000);
        m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
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
//////////
        if (m_uiStaticOverload_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_STATIC_OVERLOAD_N : SPELL_STATIC_OVERLOAD_H );
//	    DoScriptText(SAY_SLAY_1, m_creature); 
            m_uiStaticOverload_Timer = urand(20000, 30000);
        }
        else
            m_uiStaticOverload_Timer -= uiDiff;

if (m_uiRingLightning_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_RING_LIGHTNING_N : SPELL_RING_LIGHTNING_H);
	    DoScriptText(SAY_SLAY_2, m_creature); 
            m_uiRingLightning_Timer = urand(40000, 45000);
        }
        else
            m_uiRingLightning_Timer -= uiDiff;

if (m_uiChainLightning_Timer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CHAIN_LIGHTING_N :SPELL_CHAIN_LIGHTING_H);
	    DoScriptText(SAY_SLAY_3, m_creature); 
            m_uiChainLightning_Timer = urand(11000, 15000);
        }
        else
            m_uiChainLightning_Timer -= uiDiff;

if (m_uiBerserk_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = m_bIsRegularMode ? BERSERK_TIME_N : BERSERK_TIME_H ;
        }
        else
            m_uiBerserk_Timer -= uiDiff;
/////////////////
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sjonnir(Creature* pCreature)
{
    return new boss_sjonnirAI(pCreature);
}

void AddSC_boss_sjonnir()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sjonnir";
    newscript->GetAI = &GetAI_boss_sjonnir;
    newscript->RegisterSelf();
}
