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
// Gormok - Firebomb not implemented, timers need correct
// Snakes - Underground phase not worked, timers need correct
// Icehowl - Trample&Crash event not implemented, timers need correct

/* ScriptData
SDName: northrend_beasts
SD%Complete: 60% 
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Equipment
{
    EQUIP_MAIN           = 50760,
    EQUIP_OFFHAND        = 48040,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_SNOBOLD_VASSAL   = 34800,
    NPC_SLIME_POOL       = 35176,
    NPC_FIRE_BOMB        = 34854,
};

enum BossSpells
{
SPELL_IMPALE,
SPELL_STAGGERING_STOMP,
SPELL_RISING_ANGER,
SPELL_ACID_SPIT,
SPELL_PARALYTIC_SPRAY,
SPELL_ACID_SPEW,
SPELL_PARALYTIC_BITE,
SPELL_SWEEP_0,
SPELL_SLIME_POOL,
SPELL_FIRE_SPIT,
SPELL_MOLTEN_SPEW,
SPELL_BURNING_BITE,
SPELL_BURNING_SPRAY,
SPELL_SWEEP_1,
SPELL_FEROCIOUS_BUTT,
SPELL_MASSIVE_CRASH,
SPELL_WHIRL,
SPELL_ARCTIC_BREATH,
SPELL_TRAMPLE,
SUMMON_SNOBOLD,
SPELL_SNOBOLLED,
SPELL_BATTER,
SPELL_FIRE_BOMB,
SPELL_HEAD_CRACK,
SPELL_SUBMERGE_0,
SPELL_BERSERK,
SPELL_ENRAGE,
BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_IMPALE,           66331, 67477, 67478, 67479, 20000, 20000, 20000, 20000, 40000, 40000, 40000, 40000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_STAGGERING_STOMP, 67648, 67648, 67648, 66648, 15000, 15000, 15000, 15000, 40000, 40000, 40000, 40000, 65535, CAST_ON_SELF, false, false},
{SPELL_RISING_ANGER,     66636, 66636, 66636, 66636, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, APPLY_AURA_SELF, false, true},
{SPELL_ACID_SPIT,        66880, 67606, 67607, 67608, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_PARALYTIC_SPRAY,  66901, 67615, 67616, 66617, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_ACID_SPEW,        66819, 66819, 66819, 66819, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_PARALYTIC_BITE,   66824, 67612, 67613, 67614, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SWEEP_0,          66794, 67644, 67645, 67646, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, true},
{SPELL_SLIME_POOL,       67642, 67642, 67642, 67642, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, true},
{SPELL_FIRE_SPIT,        66796, 67632, 67633, 67634, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_MOLTEN_SPEW,      66821, 66821, 66821, 66821, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_BURNING_BITE,     66879, 67624, 67625, 67626, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_BURNING_SPRAY,    66902, 67627, 67628, 67629, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SWEEP_1,          67646, 67646, 67646, 67646, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_FEROCIOUS_BUTT,   66770, 67654, 67655, 67656, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_MASSIVE_CRASH,    66683, 67660, 67661, 67662, 15000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_WHIRL,            67345, 67663, 67664, 67665, 15000, 15000, 15000, 15000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_ARCTIC_BREATH,    66689, 67650, 67651, 67652, 25000, 25000, 25000, 25000, 40000, 40000, 40000, 40000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_TRAMPLE,          66734, 66734, 66734, 66734, 30000, 30000, 30000, 30000, 60000, 60000, 60000, 60000, 65535, CAST_ON_RANDOM, false, false},
{SUMMON_SNOBOLD,         NPC_SNOBOLD_VASSAL, NPC_SNOBOLD_VASSAL, NPC_SNOBOLD_VASSAL, NPC_SNOBOLD_VASSAL, 40000, 40000, 40000, 40000, 60000, 60000, 60000, 60000, 65535, SUMMON_NORMAL, false, false},
{SPELL_SNOBOLLED,        66406, 66406, 66406, 66406, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, APPLY_AURA_TARGET, false, true},
{SPELL_BATTER,           66408, 66408, 66408, 66408, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_FIRE_BOMB,        66313, 66313, 66313, 66313, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_HEAD_CRACK,       66407, 66407, 66407, 66407, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SUBMERGE_0,       53421, 53421, 53421, 53421, 40000, 40000, 40000, 40000, 60000, 60000, 60000, 60000, 65535, CAST_ON_SELF, true, false},
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
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    uint8 SnoboldsCount;
    Unit* currentTarget;

#include "sc_boss_spell_worker.cpp"

    void Reset() {
        if(!m_pInstance) return;
        SnoboldsCount = 4;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, GORMOK_IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_IMPALE, uiDiff))
                        CastBossSpell(SPELL_IMPALE);

        if (QuerySpellPeriod(SPELL_STAGGERING_STOMP, uiDiff))
                        CastBossSpell(SPELL_STAGGERING_STOMP);

        if (QuerySpellPeriod(SUMMON_SNOBOLD, uiDiff) && SnoboldsCount > 0 ) {
                        CastBossSpell(SUMMON_SNOBOLD);
                        CastBossSpell(SPELL_RISING_ANGER);
                        DoScriptText(-1713601,m_creature);
                        --SnoboldsCount;
                        };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gormok(Creature* pCreature)
{
    return new boss_gormokAI(pCreature);
}

struct MANGOS_DLL_DECL mob_snobold_vassalAI : public ScriptedAI
{
    mob_snobold_vassalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;
    Unit* defaultTarget;

#include "sc_boss_spell_worker.cpp"

    void Reset()
    {
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
        defaultTarget = who;
        CastBossSpell(SPELL_SNOBOLLED);
    }

    void JustDied(Unit* pKiller)
    {
    if (defaultTarget && defaultTarget->isAlive())
                  defaultTarget->RemoveAurasDueToSpell(m_BossSpell[SPELL_SNOBOLLED].m_uiSpellEntry[Difficulty]);
    if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_GORMOK)))
        if (pTemp->isAlive() && pTemp->GetAura(m_BossSpell[SPELL_RISING_ANGER].m_uiSpellEntry[Difficulty], EFFECT_INDEX_0)->modStackAmount(-1))
                  pTemp->RemoveAurasDueToSpell(m_BossSpell[SPELL_RISING_ANGER].m_uiSpellEntry[Difficulty]);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != GORMOK_IN_PROGRESS) {
                if (defaultTarget && defaultTarget->isAlive())
                      defaultTarget->RemoveAurasDueToSpell(m_BossSpell[SPELL_SNOBOLLED].m_uiSpellEntry[Difficulty]);
            m_creature->ForcedDespawn();
            }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_BATTER, uiDiff))
                    CastBossSpell(SPELL_BATTER);

        if (QuerySpellPeriod(SPELL_FIRE_BOMB, uiDiff))
                    CastBossSpell(SPELL_FIRE_BOMB);

        if (QuerySpellPeriod(SPELL_HEAD_CRACK, uiDiff))
                    CastBossSpell(SPELL_HEAD_CRACK);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_snobold_vassal(Creature* pCreature)
{
    return new mob_snobold_vassalAI(pCreature);
}

struct MANGOS_DLL_DECL boss_acidmawAI : public ScriptedAI
{
    boss_acidmawAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;
    uint8 stage;
    bool enraged;

#include "sc_boss_spell_worker.cpp"

    void Reset()
    {
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        stage = 1;
        enraged = false;
        m_creature->SetInCombatWithZone();

    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SPECIAL);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if ((!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
        && (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != ACIDMAW_SUBMERGED))
            return;

    switch (stage) 
        {
        case 0: {
                if (QuerySpellPeriod(SPELL_ACID_SPEW, uiDiff))
                    CastBossSpell(SPELL_ACID_SPEW);

                if (QuerySpellPeriod(SPELL_PARALYTIC_BITE, uiDiff))
                    CastBossSpell(SPELL_PARALYTIC_BITE);

                if (QuerySpellPeriod(SPELL_SLIME_POOL, uiDiff))
                    CastBossSpell(SPELL_SLIME_POOL);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == ACIDMAW_SUBMERGED)
                     stage = 1;

                    break;}
        case 1: {
                    CastBossSpell(SPELL_SUBMERGE_0);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);
                    break;}
        case 2: {
                if (QuerySpellPeriod(SPELL_ACID_SPIT, uiDiff))
                    CastBossSpell(SPELL_ACID_SPIT);

                if (QuerySpellPeriod(SPELL_PARALYTIC_SPRAY, uiDiff))
                    CastBossSpell(SPELL_PARALYTIC_SPRAY);

                if (QuerySpellPeriod(SPELL_SWEEP_0, uiDiff))
                    CastBossSpell(SPELL_SWEEP_0);

                if (QuerySpellPeriod(SPELL_SUBMERGE_0, uiDiff) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == ACIDMAW_SUBMERGED) 
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DREADSCALE_SUBMERGED);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != ACIDMAW_SUBMERGED) 
                        stage = 3;

                    break;}
        case 3: {
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                    stage = 0;
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DREADSCALE_SUBMERGED);
                    break;}
        }

        if (Creature* pSister = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_DREADSCALE)))
                 if (!pSister->isAlive() && !enraged)
                        {
                        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
                        m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                        CastBossSpell(SPELL_ENRAGE);
                        enraged = true;
                        DoScriptText(-1713504,m_creature);
                        };


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
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;
    uint8 stage;
    bool enraged;

#include "sc_boss_spell_worker.cpp"

    void Reset()
    {
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        stage = 0;
        enraged = false;
        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, SPECIAL);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
    }

    void Aggro(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if ((!m_creature->SelectHostileTarget() || !m_creature->getVictim()) 
        && (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != DREADSCALE_SUBMERGED))
            return;

        switch (stage) 
        {
        case 0: {
                if (QuerySpellPeriod(SPELL_BURNING_BITE, uiDiff))
                    CastBossSpell(SPELL_BURNING_BITE);

                if (QuerySpellPeriod(SPELL_MOLTEN_SPEW, uiDiff))
                    CastBossSpell(SPELL_MOLTEN_SPEW);

                if (QuerySpellPeriod(SPELL_SLIME_POOL, uiDiff))
                    CastBossSpell(SPELL_SLIME_POOL);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DREADSCALE_SUBMERGED)
                     stage = 1;

                    break;}
        case 1: {
                    CastBossSpell(SPELL_SUBMERGE_0);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, DREADSCALE_SUBMERGED);
                    break;}
        case 2: {
                if (QuerySpellPeriod(SPELL_FIRE_SPIT, uiDiff))
                    CastBossSpell(SPELL_FIRE_SPIT);

                if (QuerySpellPeriod(SPELL_BURNING_SPRAY, uiDiff))
                    CastBossSpell(SPELL_BURNING_SPRAY);

                if (QuerySpellPeriod(SPELL_SWEEP_0, uiDiff))
                    CastBossSpell(SPELL_SWEEP_0);

                if (QuerySpellPeriod(SPELL_SUBMERGE_0, uiDiff) && m_pInstance->GetData(TYPE_NORTHREND_BEASTS) == DREADSCALE_SUBMERGED) 
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);

                if (m_pInstance->GetData(TYPE_NORTHREND_BEASTS) != DREADSCALE_SUBMERGED)
                    stage = 3;

                    break;}
        case 3: {
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                    stage = 0;
                    m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ACIDMAW_SUBMERGED);
                    break;}
        }

        if (Creature* pSister = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_ACIDMAW)))
                 if (!pSister->isAlive() && !enraged)
                        {
                        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, IN_PROGRESS);
                        m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                        CastBossSpell(SPELL_ENRAGE);
                        enraged = true;
                        DoScriptText(-1713504,m_creature);
                        };

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
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;

    void Reset() {
        if(!m_pInstance) return;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
    }

#include "sc_boss_spell_worker.cpp"

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_NORTHREND_BEASTS, FAIL);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_NORTHREND_BEASTS, ICEHOWL_IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_FEROCIOUS_BUTT, uiDiff))
                        CastBossSpell(SPELL_FEROCIOUS_BUTT);

        if (QuerySpellPeriod(SPELL_ARCTIC_BREATH, uiDiff))
                        CastBossSpell(SPELL_ARCTIC_BREATH);

        if (QuerySpellPeriod(SPELL_WHIRL, uiDiff))
                        CastBossSpell(SPELL_WHIRL);

        if (QuerySpellPeriod(SPELL_MASSIVE_CRASH, uiDiff))
                        {
                        CastBossSpell(SPELL_MASSIVE_CRASH);
//                        m_uiSpell_Timer[SPELL_TRAMPLE] = 2000;
                        DoScriptText(-1713506,m_creature,currentTarget);
                        }

        if (QuerySpellPeriod(SPELL_TRAMPLE, uiDiff))
                        if (CastBossSpell(SPELL_TRAMPLE) != CAST_OK)
                                DoScriptText(-1713507,m_creature);

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

    newscript = new Script;
    newscript->Name = "mob_snobold_vassal";
    newscript->GetAI = &GetAI_mob_snobold_vassal;
    newscript->RegisterSelf();

}
