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
SD%Complete: 1%
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Summons
{
    NPC_FROST_SPHERE     = 34606,
    NPC_GUARDIAN         = 34607,
    NPC_DATTER           = 34605,
};

enum BossSpells
{
    SPELL_COLD,
    SPELL_LEECHING_SWARM,
    SPELL_IMPALE,
    SPELL_POUND,
    SPELL_SUBMERGE,
    SPELL_SUMMON_BEATLES,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};
static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_COLD,             66013, 67700, 68509, 68510, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_LEECHING_SWARM,   67630, 68646, 66118, 68647, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_IMPALE,           65922, 65922, 65922, 65922, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_POUND,            66013, 67700, 68509, 68510, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUBMERGE,         53421, 53421, 53421, 53421, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUMMON_BEATLES,   66339, 66339, 66339, 66339, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_BERSERK,          26662, 26662, 26662, 26662, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
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