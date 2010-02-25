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
SDName: Boss_Sapphiron
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_BREATH       = -1533082,
    EMOTE_ENRAGE       = -1533083,

    SPELL_ICEBOLT          = 28522,
    SPELL_FROST_BREATH     = 29318,
    SPELL_FROST_BREATH_H   = 28524,
    SPELL_FROST_AURA       = 28531,
    SPELL_LIFE_DRAIN       = 28542,
    SPELL_LIFE_DRAIN_H     = 55665,
    SPELL_BLIZZARD         = 28547,
    SPELL_BESERK           = 26662,
    SPELL_ICEBOLT_VISUAL   = 45776,
    SPELL_CLEAVE           = 19983,
    SPELL_TAIL_LASH        = 55697,
    SPELL_TAIL_LASH_H      = 55696,
    SPELL_DIES             = 29357,

    SAPPHIRON_X            = 3522,
    SAPPHIRON_Y            = -5236,
    SAPPHIRON_Z            = 137
};

struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 Icebolt_Count;
    uint32 Icebolt_Timer;
    uint32 FrostBreath_Timer;
    uint32 FrostAura_Timer;
    uint32 LifeDrain_Timer;
    uint32 Blizzard_Timer;
    uint32 Fly_Timer;
    uint32 Beserk_Timer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 phase;
    bool landoff;
    bool isAtGround;
    uint32 land_Timer;
    std::vector<Unit*> targets;
    uint32 land_time;


    void Reset()
    {
        FrostAura_Timer = 2000;
        FrostBreath_Timer = 2500;
        LifeDrain_Timer = 24000;
        Blizzard_Timer = 20000;
        Fly_Timer = 45000;
        Icebolt_Timer = 4000;
        land_Timer = 2000;
        Beserk_Timer = 900000;
        m_uiCleaveTimer = 7000;
        m_uiTailSweepTimer = 20000;
        phase = 1;
        Icebolt_Count = 0;
        landoff = false;
        isAtGround = true;
        targets.clear();
        land_time = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, NOT_STARTED);
        //m_creature->ApplySpellMod(SPELL_FROST_AURA, SPELLMOD_DURATION, -1);
    }

    void Aggro(Unit* who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_ICEBOLT)
        {
            if (target->isAlive() && target->HasAura(SPELL_ICEBOLT))
            {
                target->CastSpell(target, 62766, true);
                target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
            }

/*            for(std::vector<Unit*>::iterator itr = targets.begin(); itr!= targets.end(); ++itr)
            {
                if (*itr)
                {
                    if (target->isAlive() && (*itr)->GetGUID() == target->GetGUID() && !((*itr)->HasAura(SPELL_ICEBOLT)))
                    {
                        target->CastSpell(target, 62766, true);
                        target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                    }
                }
            }
*/
            return;
        }

        if(spell->Id == SPELL_FROST_BREATH || spell->Id == SPELL_FROST_BREATH_H)
        {
            if (target->GetTypeId() != TYPEID_PLAYER)
                return;

            if (target->HasAura(SPELL_ICEBOLT))
            {
                target->RemoveAurasDueToSpell(62766);
                target->RemoveAurasDueToSpell(SPELL_ICEBOLT);
                target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                return;
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, DONE);

        m_creature->CastSpell(m_creature, SPELL_DIES, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (phase == 1)
        {
            if (FrostAura_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FROST_AURA);
                FrostAura_Timer = 5000;
            }else FrostAura_Timer -= diff;

            if (LifeDrain_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, m_bIsRegularMode ? SPELL_LIFE_DRAIN : SPELL_LIFE_DRAIN_H);

                LifeDrain_Timer = 24000;
            }else LifeDrain_Timer -= diff;

            if (Blizzard_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_BLIZZARD);

                Blizzard_Timer = 20000;
            }else Blizzard_Timer -= diff;

            // Cleave
            if (m_uiCleaveTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = 7000 + rand()%3000;
            }
            else
                m_uiCleaveTimer -= diff;

            // Tail Sweep
            if (m_uiTailSweepTimer < diff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H);
                m_uiTailSweepTimer = 15000 + rand()%5000;
            }
            else
                m_uiTailSweepTimer -= diff;

            if (m_creature->GetHealthPercent() > 10.0f)
            {
                if (Fly_Timer < diff)
                {
                    phase = 2;
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->StopMoving();
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->MonsterMove(SAPPHIRON_X, SAPPHIRON_Y, SAPPHIRON_Z + 20, 1);

                    //DoCast(m_creature,11010);
                    //m_creature->SetHover(true);
                    //DoCast(m_creature,18430);
                    Icebolt_Timer = 4000;
                    Icebolt_Count = 0;
                    landoff = false;
                }else Fly_Timer -= diff;
            }
        }

        if (phase == 2)
        {
            if (Icebolt_Timer < diff && Icebolt_Count < 5)
            {
                if (Icebolt_Count == 1 || Icebolt_Count == 3)
                    if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    {
                        DoCast(target, SPELL_ICEBOLT);
                        targets.push_back(target);
                    }

                ++Icebolt_Count;
                Icebolt_Timer = 4000;
            }else Icebolt_Timer -= diff;

            if (Icebolt_Count == 5 && !landoff)
            {
                if (FrostBreath_Timer < diff)
                {
                    // apply immune
                    ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                    for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                    {
                        Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                        if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER) && pUnit->isAlive())
                        {
                            if (!pUnit->HasAura(SPELL_ICEBOLT))
                            {
                                for(std::vector<Unit*>::iterator itr = targets.begin(); itr!= targets.end(); ++itr)
                                {
                                    if (*itr)
                                    {
                                        if(!(*itr)->isAlive())
                                            return;
                                        if (pUnit->GetDistance2d(*itr) <= 5 && (*itr)->HasAura(SPELL_ICEBOLT))
                                            pUnit->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                    }
                                }
                            }
                        }
                    }

                    DoScriptText(EMOTE_BREATH, m_creature);
                    DoCast(m_creature->getVictim(),m_bIsRegularMode ? SPELL_FROST_BREATH : SPELL_FROST_BREATH_H);
                    land_Timer = 2000;
                    landoff = true;
                    FrostBreath_Timer = 6000;
                }else FrostBreath_Timer -= diff;
            }

            if (landoff)
            {
                if (land_Timer < diff)
                {
                    phase = 1;
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    //m_creature->SetHover(false);
                    //m_creature->GetMotionMaster()->Clear(false);
                    //m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());

                    ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                    for (ThreatList::const_iterator iter = tList.begin();iter != tList.end(); ++iter)
                    {
                        Unit* pUnit = Unit::GetUnit((*m_creature), (*iter)->getUnitGuid());
                        if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                            pUnit->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                    }

                    targets.clear();
                    Fly_Timer = 67000;
                    isAtGround = false;
                    land_time = 3500;
                }else land_Timer -= diff;
            }
        }
        if(phase == 1 && isAtGround == false)
        {
            if(land_time < diff)
            {
                isAtGround = true;
                DoStartMovement(m_creature->getVictim());
            }else land_time -=diff;
        }
        if (Beserk_Timer < diff)
        {
            DoScriptText(EMOTE_ENRAGE, m_creature);
            DoCast(m_creature,SPELL_BESERK);
            Beserk_Timer = 900000;
        }else Beserk_Timer -= diff;


        if (phase!=2 && isAtGround == true)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sapphiron(Creature* pCreature)
{
    return new boss_sapphironAI(pCreature);
}

void AddSC_boss_sapphiron()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_sapphiron";
    NewScript->GetAI = &GetAI_boss_sapphiron;
    NewScript->RegisterSelf();
}
