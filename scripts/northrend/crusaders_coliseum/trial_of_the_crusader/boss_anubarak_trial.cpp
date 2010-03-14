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
{SPELL_MARK,             67574, 67574, 67574, 67574, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_LEECHING_SWARM,   67630, 68646, 66118, 68647, 10000, 10000, 10000, 10000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_IMPALE,           65922, 65922, 65922, 65922, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_POUND,            66012, 66012, 66012, 66012, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHOUT,            67730, 67730, 67730, 67730, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUBMERGE_0,       53421, 53421, 53421, 53421, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUBMERGE_1,       67322, 67322, 67322, 67322, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SUMMON_BEATLES,   66339, 66339, 66339, 66339, 5000, 5000, 5000, 5000, 10000, 10000, 10000, 10000, 65535, CAST_ON_RANDOM, true, false},
{SUMMON_SCARAB,          NPC_SCARAB, NPC_SCARAB, NPC_SCARAB, NPC_SCARAB, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 65535, SUMMON_TEMP, true, false},
{SUMMON_BORROWER,        NPC_BURROWER, NPC_BURROWER, NPC_BURROWER, NPC_BURROWER, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 90000, 65535, SUMMON_TEMP, true, false},
{SUMMON_FROSTSPHERE,     NPC_FROST_SPHERE, NPC_FROST_SPHERE, NPC_FROST_SPHERE, NPC_FROST_SPHERE, 5000, 5000, 5000, 5000, 15000, 15000, 15000, 15000, 65535, SUMMON_INSTANT, true, false},
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

    void Reset() {
        if(!m_pInstance) return;
        stage = 0;
        intro = true;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
        memset(&m_uiSpell_Timer, 0, sizeof(m_uiSpell_Timer));
        m_uiSpell_Timer[SPELL_BERSERK] = m_BossSpell[SPELL_BERSERK].m_uiSpellTimerMin[Difficulty];
    }

    bool QuerySpellPeriod(uint32 m_uiSpellIdx, uint32 diff)
    {
    if(!m_pInstance) return false;
    bool result;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        if (m_uiSpellIdx != pSpell->id) return false;

        if (m_uiSpell_Timer[m_uiSpellIdx] == 0 ) m_uiSpell_Timer[m_uiSpellIdx]=urand(0,pSpell->m_uiSpellTimerMax[Difficulty]);

        if (m_uiSpell_Timer[m_uiSpellIdx] < diff) {
            m_uiSpell_Timer[m_uiSpellIdx]=urand(pSpell->m_uiSpellTimerMin[Difficulty],pSpell->m_uiSpellTimerMax[Difficulty]);
            result = true;
            } else {
            m_uiSpell_Timer[m_uiSpellIdx] -= diff;
            result = false;
            };
        return result;
    }

    CanCastResult CastBossSpell(uint32 m_uiSpellIdx)
    {
    if(!m_pInstance) return CAST_FAIL_OTHER;
    Unit* pTarget = NULL;
    Creature* pSummon = NULL;
    bool isSummon = false;
    float fPosX, fPosY, fPosZ;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        // Find spell index - temporary direct insert from spelltable
        if (m_uiSpellIdx != pSpell->id) return CAST_FAIL_OTHER;

        switch (pSpell->m_CastTarget) {
            case CAST_ON_SELF:
                   pTarget = m_creature;
                   break;
            case CAST_ON_SUMMONS:
                   pTarget = m_creature->getVictim(); //CHANGE IT!!!
                   break;
            case CAST_ON_VICTIM:
                   pTarget = m_creature->getVictim();
                   break;
            case CAST_ON_RANDOM:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
                   break;
            case CAST_ON_BOTTOMAGGRO:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
                   break;
            case SUMMON_INSTANT:
                   isSummon = true;
                   m_creature->GetPosition(fPosX, fPosY, fPosZ);
                   m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(50, 120), fPosX, fPosY, fPosZ);
                   pSummon = m_creature->SummonCreature(pSpell->m_uiSpellEntry[Difficulty], fPosX, fPosY, fPosZ, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
                   break;
            case SUMMON_TEMP:
                   isSummon = true;
                   m_creature->GetPosition(fPosX, fPosY, fPosZ);
                   m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(50, 120), fPosX, fPosY, fPosZ);
                   pSummon = m_creature->SummonCreature(pSpell->m_uiSpellEntry[Difficulty], fPosX, fPosY, fPosZ, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                   if(pSummon) pSummon->SetInCombatWithZone();
                   break;

            };
            currentTarget = pTarget;
            if (pTarget && !isSummon) return DoCastSpellIfCan(pTarget,pSpell->m_uiSpellEntry[Difficulty]);
            else return pSummon ?  CAST_OK :  CAST_FAIL_OTHER;
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
        DoScriptText(-1713555,m_creature);
        m_creature->SetInCombatWithZone();
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
                if (QuerySpellPeriod(SUMMON_BORROWER, uiDiff))
                        CastBossSpell(SUMMON_BORROWER);
                if (m_creature->GetHealthPercent() < 50.0f) stage = 1;
                    break;}
            case 1: {
                    CastBossSpell(SPELL_SUBMERGE_0);
                    stage = 2;
                    SubmergeTimer = 90000;
                    DoScriptText(-1713557,m_creature);
                    break;}
            case 2: {
                    if (SubmergeTimer < uiDiff) {
                    stage = 3;
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveAurasDueToSpell(m_BossSpell[SPELL_SUBMERGE_0].m_uiSpellEntry[Difficulty]);
                    } else SubmergeTimer -= uiDiff;
                    if (QuerySpellPeriod(SPELL_IMPALE, uiDiff))
                         CastBossSpell(SPELL_IMPALE);
                    if (QuerySpellPeriod(SPELL_SUMMON_BEATLES, uiDiff)) {
                            CastBossSpell(SPELL_SUMMON_BEATLES);
                            CastBossSpell(SUMMON_SCARAB);
                            DoScriptText(-1713560,m_creature);
                         }
                    break;}
            case 3: {
                    if (QuerySpellPeriod(SPELL_POUND, uiDiff))
                            CastBossSpell(SPELL_POUND);
                    if (QuerySpellPeriod(SPELL_COLD, uiDiff))
                            CastBossSpell(SPELL_COLD);
                    if (QuerySpellPeriod(SUMMON_BORROWER, uiDiff))
                            CastBossSpell(SUMMON_BORROWER);
                    if (m_creature->GetHealthPercent() < 30.0f) stage = 4;
                    break;}
            case 4: {
                    if (QuerySpellPeriod(SPELL_POUND, uiDiff))
                            CastBossSpell(SPELL_POUND);
                    if (QuerySpellPeriod(SPELL_COLD, uiDiff))
                            CastBossSpell(SPELL_COLD);
                    if (QuerySpellPeriod(SPELL_LEECHING_SWARM, uiDiff)) {
                            CastBossSpell(SPELL_LEECHING_SWARM);
                            CastBossSpell(SUMMON_SCARAB);
                            CastBossSpell(SUMMON_SCARAB);
                            DoScriptText(-1713561,m_creature);
                            }
                        break;}

        }
        if (QuerySpellPeriod(SUMMON_FROSTSPHERE, uiDiff))
                    CastBossSpell(SUMMON_FROSTSPHERE);

        if (QuerySpellPeriod(SPELL_BERSERK, uiDiff))
                    CastBossSpell(SPELL_BERSERK);

        if (m_creature->GetHealthPercent() < 30.0f && stage == 3) stage = 1;


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