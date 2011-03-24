/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
    EMOTE_BREATH                = -1533082,
    EMOTE_GENERIC_ENRAGED       = -1000003,
    EMOTE_FLY                   = -1533022,                 // NYI
    EMOTE_GROUND                = -1533083,                 // NYI

    SPELL_ICEBOLT               = 28522,
    //SPELL_FROST_BREATH          = 29318,
    SPELL_FROST_AURA            = 28531,
    SPELL_FROST_AURA_H          = 55799,
    SPELL_LIFE_DRAIN            = 28542,
    SPELL_LIFE_DRAIN_H          = 55665,
    SPELL_BLIZZARD              = 28547,
    SPELL_BLIZZARD_H            = 55699,
    SPELL_BERSERK               = 26662,
    SPELL_CLEAVE                = 19983,
    SPELL_TAIL_SWEEP            = 55697,
    SPELL_TAIL_SWEEP_H          = 55696,
    SPELL_ICE_BLOCK_VISUAL      = 62766, //hack
    SPELL_FROST_BREATH          = 28524,
    SPELL_FROST_BREATH_BALL     = 30101,
    SPELL_WING_BUFFET           = 29328,
    SPELL_DIES                  = 29357
};

#define CENTER_X            3522.39f
#define CENTER_Y            -5236.78f
#define CENTER_Z            137.71f

struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiIcebolt_Count;
    uint32 m_uiIcebolt_Timer;
    uint32 m_uiFrostBreath_Timer;
    uint32 m_uiFrostAura_Timer;
    uint32 m_uiLifeDrain_Timer;
    uint32 m_uiBlizzard_Timer;
    uint32 m_uiPhase_Timer;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uim_bLandingDelay_Timer;
    uint32 m_uiWingBuffet_Timer;
    uint32 m_uiFrostBreath_Phase;
    uint32 m_uiBirthTimer;
    uint8 m_uiBirthState;
    bool m_bLandPhase;
    bool m_bLandingDelay;
    std::list<Unit*> m_lIceBlockTargets;


    void Reset()
    {
        m_uiLifeDrain_Timer = 10000;
        m_uiBlizzard_Timer = 16000;
        m_uiPhase_Timer = 45000;
        m_uiBerserk_Timer = 900000;
        m_uiCleaveTimer =  urand(10000, 15000);
        m_uiTailSweepTimer =  urand(10000, 15000);
        m_uiWingBuffet_Timer = 2000;
        m_uim_bLandingDelay_Timer = 0;
        m_bLandingDelay = false;
        m_bLandPhase = true;
        m_uiFrostBreath_Phase = 1;
        m_lIceBlockTargets.clear();
        m_uiBirthTimer = 19000;
        m_uiBirthState = 0;
        m_creature->SetVisibility(VISIBILITY_OFF);
        m_creature->setFaction(35);
        if(m_pInstance)
            if(GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_SAPPHIRON_BIRTH)))
                pGo->Respawn();
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (m_uiBirthState == 0 && m_pInstance)
        {
            if (GameObject* pGo = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_SAPPHIRON_BIRTH)))
                pGo->Respawn();
             m_uiBirthState = 1;
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, FAIL);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoCast(m_creature, m_bIsRegularMode ? SPELL_FROST_AURA : SPELL_FROST_AURA_H, true);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, DONE);

        m_creature->CastSpell(m_creature, SPELL_DIES, true);
 
        Map *map = m_creature->GetMap();
        if (map && map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                {
                    itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                    itr->getSource()->RemoveAurasDueToSpell(SPELL_ICE_BLOCK_VISUAL);
                }
        }
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type == POINT_MOTION_TYPE && id == 1)
        {
            m_creature->MonsterTextEmote("Sapphiron lifts off into the air!", NULL, true);
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
            m_creature->SetHover(true);
            m_uiIcebolt_Timer = 6000;
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_uiIcebolt_Count = 0;
        }
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        switch (spell->Id)
        {
            case SPELL_ICEBOLT:
            {
                if (target && target->isAlive() && target->HasAura(SPELL_ICEBOLT))
                {
                    target->CastSpell(target, SPELL_ICE_BLOCK_VISUAL, true);
                    m_lIceBlockTargets.push_back(target);
                }
                if (m_uiIcebolt_Count == (m_bIsRegularMode ? 2 : 3))
                {
                    DoScriptText(EMOTE_BREATH, m_creature);
                    DoCast(m_creature, SPELL_FROST_BREATH_BALL);
                    m_creature->SetHover(true);
                    m_uiFrostBreath_Timer = 6900;
                }
                break;
            }
            case SPELL_FROST_BREATH:
            {
                if (target && target->GetTypeId() == TYPEID_PLAYER && target->HasAura(SPELL_ICEBOLT))
                {
                    target->RemoveAurasDueToSpell(SPELL_ICE_BLOCK_VISUAL);
                    target->RemoveAurasDueToSpell(SPELL_ICEBOLT);
                }
                break;
            }
        }
    }

    bool IsInBetween(WorldObject* obj1, WorldObject* obj2, WorldObject* obj3, float size)
    {
        if (obj1->GetPositionX() > std::max(obj2->GetPositionX(), obj3->GetPositionX())
            || obj1->GetPositionX() < std::min(obj2->GetPositionX(), obj3->GetPositionX())
            || obj1->GetPositionY() > std::max(obj2->GetPositionY(), obj3->GetPositionY())
            || obj1->GetPositionY() < std::min(obj2->GetPositionY(), obj3->GetPositionY()))
            return false;

        if (!size)
            size = obj1->GetObjectBoundingRadius() / 2;

        float angle = obj2->GetAngle(obj1) - obj2->GetAngle(obj3);
        return abs(sin(angle)) * obj1->GetDistance2d(obj2->GetPositionX(), obj2->GetPositionY()) < size;
    }

    void UpdateAI(const uint32 diff)
    {
        if(m_uiBirthState == 1)
            if(m_uiBirthTimer < diff)
            {
                m_creature->SetVisibility(VISIBILITY_ON);
                m_creature->setFaction(21);
                m_uiBirthState = 2;
            }
            else
                m_uiBirthTimer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserk_Timer < diff)
        {
            DoScriptText(EMOTE_GENERIC_ENRAGED, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 300000;
        }
        else
            m_uiBerserk_Timer -= diff;

        if (m_bLandingDelay)
        {
            if (m_uim_bLandingDelay_Timer < diff)
            {
                m_bLandPhase = true;
                m_creature->MonsterTextEmote("Sapphiron resumes his attacks!", NULL, true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase_Timer = 45000;
                m_bLandingDelay = false;
            }
            else
                m_uim_bLandingDelay_Timer -= diff;
        
            return;
        }

        if (m_bLandPhase)
        {
            if (m_uiLifeDrain_Timer < diff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_LIFE_DRAIN : SPELL_LIFE_DRAIN_H);
                m_uiLifeDrain_Timer = 24000;
            }
            else
                m_uiLifeDrain_Timer -= diff;

            if (m_uiCleaveTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = urand(10000, 15000);
            }
            else
                m_uiCleaveTimer -= diff;

            if (m_uiTailSweepTimer < diff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_TAIL_SWEEP : SPELL_TAIL_SWEEP_H);
                m_uiTailSweepTimer = urand(10000, 15000);
            }
            else
                m_uiTailSweepTimer -= diff;

            if (m_uiBlizzard_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, m_bIsRegularMode ? SPELL_BLIZZARD : SPELL_BLIZZARD_H);

                m_uiBlizzard_Timer = 3500;
            }
            else
                m_uiBlizzard_Timer -= diff;

            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 10)
                if (m_uiPhase_Timer < diff)
                {
                    m_bLandPhase = false;
                    m_uiIcebolt_Timer = 60000;
                    m_uiFrostBreath_Timer = 60000;
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MovePoint(1, CENTER_X, CENTER_Y, CENTER_Z);
                }
                else
                    m_uiPhase_Timer -= diff;
        }
        else
        {
            if (m_uiIcebolt_Count < uint8(m_bIsRegularMode ? 2 : 3) && m_uiIcebolt_Timer < diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_ICEBOLT);

                m_creature->SetHover(true);
                ++m_uiIcebolt_Count;
                m_uiIcebolt_Timer = 4000;
            }
            else
                m_uiIcebolt_Timer -= diff;

            /*if (m_uiWingBuffet_Timer < diff)
            {
                DoCast(m_creature, SPELL_WING_BUFFET);
                m_creature->SetHover(true);
                m_uiWingBuffet_Timer = 2000;
            }
            else
                m_uiWingBuffet_Timer -= diff;*/

            if (m_uiFrostBreath_Timer < diff)
            {
                switch (m_uiFrostBreath_Phase)
                {
                    case 1:
                    {
                        if (!m_lIceBlockTargets.empty())
                        {
                            Map *map = m_creature->GetMap();
                            if (map && map->IsDungeon())
                            {
                                Map::PlayerList const &PlayerList = map->GetPlayers();
                                if (!PlayerList.isEmpty())
                                    for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                                    {
                                        if (itr->getSource()->isGameMaster())
                                            continue;

                                        for (std::list<Unit*>::const_iterator i = m_lIceBlockTargets.begin(); i != m_lIceBlockTargets.end(); ++i)
                                        {
                                            if (IsInBetween(*i, m_creature, itr->getSource(), 3.0f) && 
                                                m_creature->GetDistance2d(itr->getSource()->GetPositionX(), itr->getSource()->GetPositionY()) - m_creature->GetDistance2d((*i)->GetPositionX(), (*i)->GetPositionY()) < 5.0f)
                                                itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                                (*i)->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                        }
                                    }
                            }
                        }
                        m_uiFrostBreath_Phase = 2;
                        m_uiFrostBreath_Timer = 100;
                        break;
                    }
                    case 2:
                    {
                        DoCast(m_creature, SPELL_FROST_BREATH);
                        m_uiFrostBreath_Phase = 3;
                        m_uiFrostBreath_Timer = 100;
                        break;
                    }
                    case 3:
                    {
                        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                        m_bLandingDelay = true;
                        m_uim_bLandingDelay_Timer = 3000;
                        m_creature->SetHover(false);
                        m_lIceBlockTargets.clear();
                        Map *map = m_creature->GetMap();
                        if (map && map->IsDungeon())
                        {
                            Map::PlayerList const &PlayerList = map->GetPlayers();
                            if (!PlayerList.isEmpty())
                                for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
                                    itr->getSource()->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                        }
                        m_uiFrostBreath_Phase = 1;
                        break;
                    }
                }
            }
            else
                m_uiFrostBreath_Timer -= diff;
        }

        if (m_bLandPhase)
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
