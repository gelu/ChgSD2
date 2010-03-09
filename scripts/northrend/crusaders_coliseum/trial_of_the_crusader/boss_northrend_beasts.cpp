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
SDName:
SD%Complete: 1%
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum BossSpells
{
SPELL_IMPALE,
SPELL_STAGGERING_STOMP,
SPELL_ACID_SPIT,
SPELL_PARALYTIC_SPRAY,
SPELL_SWEEP_0,
SPELL_SLIME_POOL,
SPELL_FIRE_SPLIT,
SPELL_MOLTEN_SPEW,
SPELL_SWEEP_1,
SPELL_FEROCIOUS_BUTT,
SPELL_MASSIVE_CRASH,
SPELL_WHIRL,
SPELL_ARCTIC_BREATH,
SPELL_TRAMPLE,
SPELL_BERSERK,
SPELL_ENRAGE,
BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_IMPALE,           67479, 67478, 67477, 66331, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_STAGGERING_STOMP, 67648, 67647, 67649, 66630, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_ACID_SPIT,        67607, 67607, 67607, 67607, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_PARALYTIC_SPRAY,  67616, 67616, 67616, 66616, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SWEEP_0,          66794, 66794, 66794, 66794, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_SLIME_POOL,       67642, 67642, 67642, 67642, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_FIRE_SPLIT,       67632, 67632, 67632, 67632, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_MOLTEN_SPEW,      66821, 66821, 66821, 66821, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SWEEP_1,          67646, 67646, 67646, 67646, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_FEROCIOUS_BUTT,   67655, 67654, 67656, 66770, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_MASSIVE_CRASH,    66683, 67660, 67661, 67662, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_WHIRL,            67345, 67663, 67664, 67665, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_ARCTIC_BREATH,    66689, 66689, 66689, 66689, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_TRAMPLE,          66734, 66734, 66734, 66734, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_BERSERK,          26662, 26662, 26662, 26662, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
{SPELL_ENRAGE,           68335, 68335, 68335, 68335, 20000, 20000, 20000, 20000, 20000, 20000, 20000, 20000, 65535, CAST_ON_SELF, false, false},
};


struct MANGOS_DLL_DECL boss_gormokAI : public ScriptedAI
{
    boss_gormokAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
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

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

struct MANGOS_DLL_DECL boss_acidmawAI : public ScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
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

CreatureAI* GetAI_boss_acidmaw(Creature* pCreature)
{
    return new boss_acidmawAI(pCreature);
}

struct MANGOS_DLL_DECL boss_dreadscaleAI : public ScriptedAI
{
    boss_dreadscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
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

CreatureAI* GetAI_boss_dreadscale(Creature* pCreature)
{
    return new boss_dreadscaleAI(pCreature);
}

struct MANGOS_DLL_DECL boss_icehowlAI : public ScriptedAI
{
    boss_icehowlAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, NOT_STARTED);
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

CreatureAI* GetAI_boss_icehowl(Creature* pCreature)
{
    return new boss_icehowlAI(pCreature);
}

void AddSC_northrend_beasts()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_gormok";
    newscript->GetAI = &GetAI_boss_gormok;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_acidmaw";
    newscript->GetAI = &GetAI_boss_acidmaw;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_dreadscale";
    newscript->GetAI = &GetAI_boss_dreadscale;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_icehowl";
    newscript->GetAI = &GetAI_boss_icehowl;
    newscript->RegisterSelf();

}
