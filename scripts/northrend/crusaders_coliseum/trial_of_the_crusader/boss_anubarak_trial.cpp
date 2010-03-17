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
SD%Complete: 40%
SDComment: by /dev/rsa
SDCategory:
EndScriptData */

// Anubarak - underground phase not worked, timers need correct
// Burrower - underground phase not implemented
// Leecheng Swarm spell need override
// Frost Sphere - realised by EventAI

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Summons
{
    NPC_FROST_SPHERE     = 34606,
    NPC_BURROWER         = 34607,
    NPC_SCARAB           = 34605,
};

enum BossSpells
{
    SPELL_COLD,
    SPELL_MARK,
    SPELL_LEECHING_SWARM,
    SPELL_IMPALE,
    SPELL_POUND,
    SPELL_SHOUT,
    SPELL_SUBMERGE_0,
    SPELL_SUBMERGE_1,
    SPELL_SUMMON_BEATLES,
    SPELL_DETERMINATION,
    SPELL_ACID_MANDIBLE,
    SPELL_SPIDER_FRENZY,
    SPELL_EXPOSE_WEAKNESS,
    SUMMON_SCARAB,
    SUMMON_BORROWER,
    SUMMON_FROSTSPHERE,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};
static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_COLD,             66013, 67700, 68509, 68510, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_MARK,             67574, 67574, 67574, 67574, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_TARGET, false, true},
{SPELL_LEECHING_SWARM,   67630, 68646, 66118, 68647, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, true},
{SPELL_IMPALE,           65922, 65922, 65922, 65922, 10000, 10000, 10000, 10000, 15000, 15000, 15000, 15000, 65535, CAST_ON_RANDOM, false, true},
{SPELL_POUND,            66012, 66012, 66012, 66012, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHOUT,            67730, 67730, 67730, 67730, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUBMERGE_0,       53421, 53421, 53421, 53421, 45000, 45000, 45000, 45000, 60000, 60000, 60000, 60000, 65535, CAST_ON_SELF, true, false},
{SPELL_SUBMERGE_1,       67322, 67322, 67322, 67322, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, true, true},
{SPELL_SUMMON_BEATLES,   66339, 66339, 66339, 66339, 5000,  5000,  5000,  5000,  10000, 10000, 10000, 10000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_DETERMINATION,    66092, 66092, 66092, 66092, 5000,  5000,  5000,  5000,  20000, 20000, 20000, 20000, 65535, CAST_ON_SELF, false, false},
{SPELL_ACID_MANDIBLE,    67861, 66092, 66092, 66092, 5000,  5000,  5000,  5000,  20000, 20000, 20000, 20000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_SPIDER_FRENZY,    66129, 66129, 66129, 66129, 10000, 10000, 10000, 10000,  20000, 20000, 20000, 20000, 65535, CAST_ON_TARGET, false, false},
{SPELL_EXPOSE_WEAKNESS,  67847, 67847, 67847, 67847, 5000,  5000,  5000,  5000,  20000, 20000, 20000, 20000, 65535, CAST_ON_VICTIM, false, false},
{SUMMON_SCARAB,          NPC_SCARAB, NPC_SCARAB, NPC_SCARAB, NPC_SCARAB, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 65535, SUMMON_NORMAL, true, false},
{SUMMON_BORROWER,        NPC_BURROWER, NPC_BURROWER, NPC_BURROWER, NPC_BURROWER, 70000, 70000, 70000, 70000, 100000, 100000, 100000, 100000, 65535, SUMMON_NORMAL, true, false},
{SUMMON_FROSTSPHERE,     NPC_FROST_SPHERE, NPC_FROST_SPHERE, NPC_FROST_SPHERE, NPC_FROST_SPHERE, 5000, 5000, 5000, 5000, 10000, 10000, 10000, 10000, 65535, SUMMON_NORMAL, true, false},
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
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    uint8 stage;
    uint32 SubmergeTimer;
    Unit* currentTarget;
    bool intro;

#include "sc_boss_spell_worker.cpp"

    void Reset() {
        if(!m_pInstance) return;
        stage = 0;
        intro = true;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
    }


    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(-1713563,m_creature);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!intro) return;
        DoScriptText(-1713554,m_creature);
        intro = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713564,m_creature);
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void Aggro(Unit* pWho)
    {
//        CastBossSpell(SPELL_MARK);
        if (!intro) DoScriptText(-1713555,m_creature);
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                if (QuerySpellPeriod(SPELL_POUND, uiDiff))
                        CastBossSpell(SPELL_POUND);
                if (QuerySpellPeriod(SPELL_COLD, uiDiff))
                        CastBossSpell(SPELL_COLD);
                if (QuerySpellPeriod(SUMMON_BORROWER, uiDiff)) {
                        CastBossSpell(SUMMON_BORROWER);
                        DoScriptText(-1713556,m_creature);
                        };
                if (QuerySpellPeriod(SPELL_SUBMERGE_0, uiDiff)) stage = 1;
                    break;}
            case 1: {
                    CastBossSpell(SPELL_SUBMERGE_0);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    break;}
            case 2: {
                    if (QuerySpellPeriod(SPELL_IMPALE, uiDiff)) {
                         CastBossSpell(SPELL_IMPALE);
                         CastBossSpell(SPELL_MARK);
                         DoScriptText(-1713558,m_creature,currentTarget);
                         };
                    if (QuerySpellPeriod(SPELL_SUMMON_BEATLES, uiDiff)) {
                            CastBossSpell(SPELL_SUMMON_BEATLES);
                            CastBossSpell(SUMMON_SCARAB);
                            DoScriptText(-1713560,m_creature);
                         };
                    if (QuerySpellPeriod(SPELL_SUBMERGE_0, uiDiff)) stage = 3;
                    break;}
            case 3: {
                    stage = 0;
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                    break;}
            case 4: {
                    if (QuerySpellPeriod(SPELL_POUND, uiDiff))
                            CastBossSpell(SPELL_POUND);
                    if (QuerySpellPeriod(SPELL_COLD, uiDiff))
                            CastBossSpell(SPELL_COLD);
                    if (QuerySpellPeriod(SPELL_LEECHING_SWARM, uiDiff)) {
                            CastBossSpell(SPELL_LEECHING_SWARM);
                            DoScriptText(-1713561,m_creature);
                            }
                        break;}

        }
        if (QuerySpellPeriod(SUMMON_FROSTSPHERE, uiDiff))
                    CastBossSpell(SUMMON_FROSTSPHERE);

        if (QuerySpellPeriod(SPELL_BERSERK, uiDiff))
                    CastBossSpell(SPELL_BERSERK);

        if (m_creature->GetHealthPercent() < 30.0f && stage == 0) stage = 4;


        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubarak_trial(Creature* pCreature)
{
    return new boss_anubarak_trialAI(pCreature);
}

struct MANGOS_DLL_DECL mob_swarm_scarabAI : public ScriptedAI
{
    mob_swarm_scarabAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;

#include "sc_boss_spell_worker.cpp"

    void Reset()
    {
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_DETERMINATION, uiDiff))
                    CastBossSpell(SPELL_DETERMINATION);

        if (QuerySpellPeriod(SPELL_ACID_MANDIBLE, uiDiff))
                    CastBossSpell(SPELL_ACID_MANDIBLE);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_swarm_scarab(Creature* pCreature)
{
    return new mob_swarm_scarabAI(pCreature);
}

struct MANGOS_DLL_DECL mob_nerubian_borrowerAI : public ScriptedAI
{
    mob_nerubian_borrowerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    Unit* currentTarget;
    bool submerged;

#include "sc_boss_spell_worker.cpp"

    void Reset()
    {
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
        submerged = false;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_ANUBARAK) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_EXPOSE_WEAKNESS, uiDiff))
                    CastBossSpell(SPELL_EXPOSE_WEAKNESS);

        if (QuerySpellPeriod(SPELL_SPIDER_FRENZY, uiDiff))
            if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BURROWER, 50.0f))
            {
            currentTarget = pTemp;
            CastBossSpell(SPELL_SPIDER_FRENZY);
            };

        if (m_creature->GetHealthPercent() < 20.0f && QuerySpellPeriod(SPELL_SUBMERGE_1, uiDiff) && !submerged)
           {
            CastBossSpell(SPELL_SUBMERGE_1);
            submerged = true;
            DoScriptText(-1713557,m_creature);
            };

        if (m_creature->GetHealthPercent() > 50.0f && submerged)
            {
             m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_1].m_uiSpellEntry[Difficulty]);
             submerged = false;
             DoScriptText(-1713559,m_creature);
             };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_nerubian_borrower(Creature* pCreature)
{
    return new mob_nerubian_borrowerAI(pCreature);
}

void AddSC_boss_anubarak_trial()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_anubarak_trial";
    newscript->GetAI = &GetAI_boss_anubarak_trial;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarm_scarab";
    newscript->GetAI = &GetAI_mob_swarm_scarab;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nerubian_borrower";
    newscript->GetAI = &GetAI_mob_nerubian_borrower;
    newscript->RegisterSelf();

}