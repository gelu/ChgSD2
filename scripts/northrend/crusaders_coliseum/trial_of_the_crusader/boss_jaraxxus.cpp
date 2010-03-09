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
SDName: trial_of_the_crusader
SD%Complete: 1
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum BossSpells
{
    SPELL_BUFF,
    SPELL_INFERNAL,
    SPELL_FEL_FIREBALL,
    SPELL_FEL_LIGHTING,
    SPELL_INCINERATE_FLESH,
    SPELL_NETHER,
    SPELL_LEGION_FLAME,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_BUFF,             67108, 67108, 67108, 67108, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_INFERNAL,         66258, 66258, 66258, 66258, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_FEL_FIREBALL,     66532, 66963, 66964, 66965, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_FEL_LIGHTING,     66528, 66528, 67029, 67029, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_INCINERATE_FLESH, 66237, 67049, 67050, 67051, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_NETHER,           66264, 66264, 68405, 68405, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_LEGION_FLAME,     68124, 68124, 68126, 68126, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_BERSERK,          26662, 26662, 26662, 26662, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
};




/*######
## boss_jaraxxus
######*/

struct MANGOS_DLL_DECL boss_jaraxxusAI : public ScriptedAI
{
    boss_jaraxxusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, NOT_STARTED);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, DONE);
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

CreatureAI* GetAI_boss_jaraxxus(Creature* pCreature)
{
    return new boss_jaraxxusAI(pCreature);
}

void AddSC_boss_jaraxxus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_jaraxxus";
    newscript->GetAI = &GetAI_boss_jaraxxus;
    newscript->RegisterSelf();
}
