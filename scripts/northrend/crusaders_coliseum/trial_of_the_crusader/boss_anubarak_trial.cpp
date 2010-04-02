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
SPELL_COLD              = 66013,
SPELL_MARK              = 67574,
SPELL_LEECHING_SWARM    = 66118,
SPELL_LEECHING_HEAL     = 66125,
SPELL_IMPALE            = 65922,
SPELL_POUND             = 66012,
SPELL_SHOUT             = 67730,
SPELL_SUBMERGE_0        = 53421,
SPELL_SUBMERGE_1        = 67322,
SPELL_SUMMON_BEATLES    = 66339,
SPELL_DETERMINATION     = 66092,
SPELL_ACID_MANDIBLE     = 67861,
SPELL_SPIDER_FRENZY     = 66129,
SPELL_EXPOSE_WEAKNESS   = 67847,
SUMMON_SCARAB           = NPC_SCARAB,
SUMMON_BORROWER         = NPC_BURROWER,
SUMMON_FROSTSPHERE      = NPC_FROST_SPHERE,
SPELL_BERSERK           = 26662,
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
    uint8 stage;
    uint32 SubmergeTimer;
    bool intro;
    BossSpellWorker* bsw;

    void Reset() {
        if(!m_pInstance) return;
        stage = 0;
        intro = true;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        bsw = new BossSpellWorker(this);
        bsw->Reset(Difficulty);
        m_creature->SetRespawnDelay(DAY);
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
                bsw->timedCast(SPELL_POUND, uiDiff);
                bsw->timedCast(SPELL_COLD, uiDiff);
                if (bsw->timedQuery(SUMMON_BORROWER, uiDiff)) {
                        bsw->doCast(SUMMON_BORROWER);
                        DoScriptText(-1713556,m_creature);
                        };
                if (bsw->timedQuery(SPELL_SUBMERGE_0, uiDiff)) stage = 1;

                    break;}
            case 1: {
                    bsw->doCast(SPELL_SUBMERGE_0);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    stage = 2;
                    DoScriptText(-1713557,m_creature);
                    break;}
            case 2: {
                    if (bsw->timedQuery(SPELL_IMPALE, uiDiff)) {
                         bsw->doCast(SPELL_IMPALE);
                         bsw->doCast(SPELL_MARK);
                         DoScriptText(-1713558,m_creature);
                         };
                    if (bsw->timedQuery(SPELL_SUMMON_BEATLES, uiDiff)) {
                            bsw->doCast(SPELL_SUMMON_BEATLES);
                            bsw->doCast(SUMMON_SCARAB);
                            DoScriptText(-1713560,m_creature);
                         };
                    if (bsw->timedQuery(SPELL_SUBMERGE_0, uiDiff)) stage = 3;
                    break;}
            case 3: {
                    stage = 0;
                    DoScriptText(-1713559,m_creature);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    bsw->doRemove(SPELL_SUBMERGE_0,m_creature);
                    break;}
            case 4: {
                    bsw->timedCast(SPELL_POUND, uiDiff);
                    bsw->timedCast(SPELL_COLD, uiDiff);
                    if (bsw->timedQuery(SPELL_LEECHING_SWARM, uiDiff)) {
                            bsw->doCast(SPELL_LEECHING_SWARM);
                            bsw->doCast(SPELL_LEECHING_HEAL);
                            DoScriptText(-1713561,m_creature);
                            }
                        break;}

        }
        bsw->timedCast(SUMMON_FROSTSPHERE, uiDiff);

        bsw->timedCast(SPELL_BERSERK, uiDiff);

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
    BossSpellWorker* bsw;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
        bsw = new BossSpellWorker(this);
        bsw->Reset(Difficulty);
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

        bsw->timedCast(SPELL_DETERMINATION, uiDiff);

        bsw->timedCast(SPELL_ACID_MANDIBLE, uiDiff);

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
    bool submerged;
    BossSpellWorker* bsw;
    Unit* currentTarget;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_creature->SetInCombatWithZone();
        m_creature->SetRespawnDelay(DAY);
        submerged = false;
        bsw = new BossSpellWorker(this);
        bsw->Reset(Difficulty);
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

        bsw->timedCast(SPELL_EXPOSE_WEAKNESS, uiDiff);

        if (bsw->timedQuery(SPELL_SPIDER_FRENZY, uiDiff))
            if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_BURROWER, 50.0f))
            {
            currentTarget = pTemp;
            bsw->doCast(SPELL_SPIDER_FRENZY);
            };

        if (m_creature->GetHealthPercent() < 20.0f && bsw->timedQuery(SPELL_SUBMERGE_1, uiDiff) && !submerged)
           {
            bsw->doCast(SPELL_SUBMERGE_1);
            submerged = true;
            DoScriptText(-1713557,m_creature);
            };

        if (m_creature->GetHealthPercent() > 50.0f && submerged)
            {
             bsw->doRemove(SPELL_SUBMERGE_1,m_creature);
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