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
SDName: boss_anubarak_trial
SD%Complete: 0
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum
{
    TYPE_BOSS            = TYPE_ANUBARAK,
    NPC_FROST_SPHERE     = 34606,

};
enum BossSpells
{
    SPELL_0,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};
static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_0, 70138, 70140, 70140, 70137, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_BERSERK,          47008, 47008, 47008, 47008, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 65535, CAST_ON_SELF, false, false},
};

struct MANGOS_DLL_DECL boss_anubarak_trialAI : public ScriptedAI
{
    boss_anubarak_trialAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubarak_trial(Creature* pCreature)
{
    return new boss_anubarak_trialAI(pCreature);
}

void AddSC_boss_anubarak_trial()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak_trial";
    newscript->GetAI = &GetAI_boss_anubarak_trial;
    newscript->RegisterSelf();
}