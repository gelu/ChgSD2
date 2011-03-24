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
SDName: Boss_Four_Horsemen
SD%Complete: 75
SDComment: Lady Blaumeux, Thane Korthazz, Sir Zeliek, Baron Rivendare
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //all horsemen
    SPELL_SHIELDWALL         = 29061,
    SPELL_BERSERK            = 26662,

    //lady blaumeux
    SAY_BLAU_AGGRO          = -1533044,
    SAY_BLAU_TAUNT1         = -1533045,
    SAY_BLAU_TAUNT2         = -1533046,
    SAY_BLAU_TAUNT3         = -1533047,
    SAY_BLAU_SPECIAL        = -1533048,
    SAY_BLAU_SLAY           = -1533049,
    SAY_BLAU_DEATH          = -1533050,

    EMOTE_UNYIELDING_PAIN   = -1533156,                     // NYI

    SPELL_MARK_OF_BLAUMEUX  = 28833,
    SPELL_UNYIELDING_PAIN   = 57381,
    SPELL_VOIDZONE          = 28863,
    H_SPELL_VOIDZONE        = 57463,
    SPELL_SHADOW_BOLT       = 57374,
    H_SPELL_SHADOW_BOLT     = 57464,

    //baron rivendare
    SAY_RIVE_AGGRO1         = -1533065,
    SAY_RIVE_AGGRO2         = -1533066,
    SAY_RIVE_AGGRO3         = -1533067,
    SAY_RIVE_SLAY1          = -1533068,
    SAY_RIVE_SLAY2          = -1533069,
    SAY_RIVE_SPECIAL        = -1533070,
    SAY_RIVE_TAUNT1         = -1533071,
    SAY_RIVE_TAUNT2         = -1533072,
    SAY_RIVE_TAUNT3         = -1533073,
    SAY_RIVE_DEATH          = -1533074,

    SPELL_MARK_OF_RIVENDARE = 28834,
    SPELL_UNHOLY_SHADOW     = 28882,
    H_SPELL_UNHOLY_SHADOW   = 57369,

    //thane korthazz
    SAY_KORT_AGGRO          = -1533051,
    SAY_KORT_TAUNT1         = -1533052,
    SAY_KORT_TAUNT2         = -1533053,
    SAY_KORT_TAUNT3         = -1533054,
    SAY_KORT_SPECIAL        = -1533055,
    SAY_KORT_SLAY           = -1533056,
    SAY_KORT_DEATH          = -1533057,

    SPELL_MARK_OF_KORTHAZZ  = 28832,
    SPELL_METEOR            = 26558,                        // m_creature->getVictim() auto-area spell but with a core problem
    SPELL_METEOR_H          = 57467,

    //sir zeliek
    SAY_ZELI_AGGRO          = -1533058,
    SAY_ZELI_TAUNT1         = -1533059,
    SAY_ZELI_TAUNT2         = -1533060,
    SAY_ZELI_TAUNT3         = -1533061,
    SAY_ZELI_SPECIAL        = -1533062,
    SAY_ZELI_SLAY           = -1533063,
    SAY_ZELI_DEATH          = -1533064,

    EMOTE_CONDEMATION       = -1533157,                     // NYI

    SPELL_MARK_OF_ZELIEK    = 28835,
    SPELL_HOLY_WRATH        = 28883,
    H_SPELL_HOLY_WRATH      = 57466,
    SPELL_HOLY_BOLT         = 57376,
    H_SPELL_HOLY_BOLT       = 57465,
    SPELL_CONDEMNATION      = 57377,

    // horseman spirits
    NPC_SPIRIT_OF_BLAUMEUX    = 16776,
    NPC_SPIRIT_OF_RIVENDARE   = 0,                          //creature entry not known yet
    NPC_SPIRIT_OF_KORTHAZZ    = 16778,
    NPC_SPIRIT_OF_ZELIREK     = 16777,

    //Achievements related to the 4-horsemen are given through spell 59450 which does not exist.
    ACHIEV_MILITARY			= 59450
};

/*walk coords*/
#define WALKX_BLAU                2462.112f
#define WALKY_BLAU                -2956.598f
#define WALKZ_BLAU                241.276f

#define WALKX_RIVE                2579.571f
#define WALKY_RIVE                -2960.945f
#define WALKZ_RIVE                241.32f

#define WALKX_KORT                2529.108f
#define WALKY_KORT                -3015.303f
#define WALKZ_KORT                241.32f
    
#define WALKX_ZELI                2521.039f
#define WALKY_ZELI                -2891.633f
#define WALKZ_ZELI                241.276f

struct MANGOS_DLL_DECL boss_lady_blaumeuxAI : public ScriptedAI
{
    boss_lady_blaumeuxAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMark_Timer;
    uint32 m_uiVoidZone_Timer;
    bool m_bShieldWall1;
    bool m_bShieldWall2;
    uint32 m_uiMark_Count;
    uint32 m_uiEnrage_Timer;
    uint32 m_uiShadowBolt_Timer;

    void Reset()
    {
        m_uiMark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        m_uiVoidZone_Timer = 12000;                             // right
        m_bShieldWall1 = true;
        m_bShieldWall2 = true;
        m_uiMark_Count = 0;
        m_uiEnrage_Timer = 0;
        m_uiShadowBolt_Timer = 0;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, FAIL);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_BLAU_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, IN_PROGRESS);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_BLAU, WALKY_BLAU, WALKZ_BLAU);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_BLAU_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_BLAU_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pZeliek && pThane && pRivendare && pZeliek->isDead() && pThane->isDead() && pRivendare->isDead())
            {
                m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);
                 

                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEV_MILITARY);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        if (m_uiMark_Count >= 100)
            if (m_uiEnrage_Timer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCast(m_creature, SPELL_BERSERK, true);
                m_uiEnrage_Timer = 300000;
            }
            else
                m_uiEnrage_Timer -= uiDiff;

        // Mark of Blaumeux
        if (m_uiMark_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MARK_OF_BLAUMEUX,CAST_TRIGGERED);
            m_uiMark_Timer = 15000;
            m_uiMark_Count++;
        }
        else 
            m_uiMark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (m_bShieldWall1)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall1 = false;
            }
        }
        else if (m_bShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (m_bShieldWall2)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall2 = false;
            }
        }

        // Void Zone
        if (m_uiVoidZone_Timer < uiDiff)
        {
            // get random player in 45yard range (maybe this can be done easier?)
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();

            if(!tList.empty())
            {
                std::list<uint64> lClosePlayerList;
                lClosePlayerList.clear();

                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                    if(Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                        if (m_creature->GetDistance2d(pUnit) < 45.f)
                            lClosePlayerList.push_back(pUnit->GetGUID());

                if(!lClosePlayerList.empty())
                {
                    std::list<uint64>::iterator i = lClosePlayerList.begin();
                    advance(i, (rand() % lClosePlayerList.size()));
                    if(Player* pTarget = m_creature->GetMap()->GetPlayer(*i))
                        DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_VOIDZONE : H_SPELL_VOIDZONE, CAST_TRIGGERED);
                }
            }
            m_uiVoidZone_Timer = 12000;
        }
        else 
            m_uiVoidZone_Timer -= uiDiff;

        if (m_uiShadowBolt_Timer < uiDiff)
        {
            // cast on closest player
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            if (!tList.empty())
            {
                Unit* pTarget = m_creature->GetMap()->GetUnit((*tList.begin())->getUnitGuid());
                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                    if (Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                        if (pTarget && m_creature->GetDistance2d(pUnit) < m_creature->GetDistance2d(pTarget))
                            pTarget = pUnit;
                if (pTarget && m_creature->GetDistance2d(pTarget)< 45.0f)
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT, true);
                else
                    DoCast(m_creature, SPELL_UNYIELDING_PAIN, true);
            }
            m_uiShadowBolt_Timer = 2000;
        }
        else
            m_uiShadowBolt_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_blaumeux(Creature* pCreature)
{
    return new boss_lady_blaumeuxAI(pCreature);
}

struct MANGOS_DLL_DECL boss_sir_zeliekAI : public ScriptedAI
{
    boss_sir_zeliekAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMark_Timer;
    uint32 m_uiMark_Count;
    uint32 m_uiHolyWrath_Timer;
    uint32 m_uiHolyBolt_Timer;
    uint32 m_uiEnrage_Timer;
    bool m_bShieldWall1;
    bool m_bShieldWall2;

    void Reset()
    {
        m_uiMark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        m_uiMark_Count = 0;
        m_uiHolyWrath_Timer = 12000;                            // right
        m_uiHolyBolt_Timer = 0;
        m_uiEnrage_Timer = 0;
        m_bShieldWall1 = true;
        m_bShieldWall2 = true;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_ZELI_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_ZELI, WALKY_ZELI, WALKZ_ZELI);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_ZELI_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_ZELI_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pThane && pRivendare && pBlaumeux && pThane->isDead() && pRivendare->isDead() && pBlaumeux->isDead())
            {
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);

                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEV_MILITARY);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        if (m_uiMark_Count >= 100)
            if (m_uiEnrage_Timer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                DoCast(m_creature, SPELL_BERSERK);
                m_uiEnrage_Timer = 300000;
            }
            else
                m_uiEnrage_Timer -= uiDiff;

        // Mark of Zeliek
        if (m_uiMark_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(false);
            DoCastSpellIfCan(m_creature,SPELL_MARK_OF_ZELIEK,CAST_TRIGGERED);
            m_uiMark_Timer = 15000;
            m_uiMark_Count++;
        }
        else 
            m_uiMark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (m_bShieldWall1)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall1 = false;
            }
        }
        else if (m_bShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (m_bShieldWall2)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall2 = false;
            }
        }

        // Holy Wrath
        if (m_uiHolyWrath_Timer < uiDiff)
        {
            // get random player in 45yard range (maybe this can be done easier?)
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            
            if(!tList.empty())
            {
                std::list<uint64> lClosePlayerList;
                lClosePlayerList.clear();

                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                    if(Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                        if (m_creature->GetDistance2d(pUnit) < 45.f)
                            lClosePlayerList.push_back(pUnit->GetGUID());

                if(!lClosePlayerList.empty())
                {
                    std::list<uint64>::iterator i = lClosePlayerList.begin();
                    advance(i, (rand() % lClosePlayerList.size()));
                    if(Player* pTarget = m_creature->GetMap()->GetPlayer(*i))
                        DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_HOLY_WRATH : H_SPELL_HOLY_WRATH, CAST_TRIGGERED);
                }
            }
            m_uiHolyWrath_Timer = 12000;
        }
        else 
            m_uiHolyWrath_Timer -= uiDiff;

        if (m_uiHolyBolt_Timer < uiDiff)
        {
            // cast on closest player
            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            if (!tList.empty())
            {
                Unit* pTarget = m_creature->GetMap()->GetUnit((*tList.begin())->getUnitGuid());
                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                    if (Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                        if (pTarget && m_creature->GetDistance2d(pUnit) < m_creature->GetDistance2d(pTarget))
                            pTarget = pUnit;
                if (pTarget && m_creature->GetDistance2d(pTarget)< 45.0f)
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_HOLY_BOLT : H_SPELL_HOLY_BOLT, true);
                else
                    DoCast(m_creature, SPELL_CONDEMNATION, true);
            }
            m_uiHolyBolt_Timer = 2000;
        }
        else
            m_uiHolyBolt_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sir_zeliek(Creature* pCreature)
{
    return new boss_sir_zeliekAI(pCreature);
}

struct MANGOS_DLL_DECL boss_rivendare_naxxAI : public ScriptedAI
{
    boss_rivendare_naxxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsMovingToCorner;

    uint32 m_uiMark_Timer;
    uint32 m_uiMark_Count;
    uint32 m_uiUnholyShadow_Timer;
    uint32 m_uiEnrage_Timer;

    void Reset()
    {
        m_uiMark_Timer = 20000;
        m_uiMark_Count = 0;
        m_uiUnholyShadow_Timer = 15000;
        m_bIsMovingToCorner = true;
        m_uiEnrage_Timer = 0;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pThane && pThane->isDead())
                pThane->Respawn();
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_RIVE_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_RIVE_AGGRO3, m_creature); break;
        }

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_RIVE, WALKY_RIVE, WALKZ_RIVE);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(urand(0, 1) ? SAY_RIVE_SLAY1 : SAY_RIVE_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_RIVE_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pThane = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_THANE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pZeliek && pThane && pBlaumeux && pZeliek->isDead() && pThane->isDead() && pBlaumeux->isDead())
            {
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);

                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEV_MILITARY);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsMovingToCorner)
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
            {
                m_bIsMovingToCorner = false;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            return;
        }

        if (m_uiMark_Count >= 100)
            if (m_uiEnrage_Timer < uiDiff)
            {
                DoCast(m_creature, SPELL_BERSERK);
                m_uiEnrage_Timer = 300000;
            }
            else
                m_uiEnrage_Timer -= uiDiff;

        if (m_uiMark_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MARK_OF_RIVENDARE,CAST_TRIGGERED);
            m_uiMark_Timer = 12000;
            m_uiMark_Count++;
        }
        else
            m_uiMark_Timer -= uiDiff;

        if (m_uiUnholyShadow_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_UNHOLY_SHADOW : H_SPELL_UNHOLY_SHADOW);
            m_uiUnholyShadow_Timer = 15000;
        }
        else
            m_uiUnholyShadow_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rivendare_naxx(Creature* pCreature)
{
    return new boss_rivendare_naxxAI(pCreature);
}

struct MANGOS_DLL_DECL boss_thane_korthazzAI : public ScriptedAI
{
    boss_thane_korthazzAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsMovingToCorner;

    uint32 m_uiMark_Timer;
    uint32 m_uiMark_Count;
    uint32 m_uiMeteor_Timer;
    bool m_bShieldWall1;
    bool m_bShieldWall2;
    uint32 m_uiEnrage_Timer;

    void Reset()
    {
        m_uiMark_Timer = 20000;                             // First Horsemen Mark is applied at 20 sec.
        m_uiMark_Count = 0;
        m_uiMeteor_Timer = 30000;                               // wrong
        m_uiEnrage_Timer = 0;
        m_bShieldWall1 = true;
        m_bShieldWall2 = true;
        m_bIsMovingToCorner = true;
        SetCombatMovement(false);

        if (m_pInstance)
        {
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            if (pBlaumeux && pBlaumeux->isDead())
                pBlaumeux->Respawn();
            if (pRivendare && pRivendare->isDead())
                pRivendare->Respawn();
            if (pZeliek && pZeliek->isDead())
                pZeliek->Respawn();
        }
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_KORT_AGGRO, m_creature);

        m_creature->SetInCombatWithZone();
        m_creature->CallForHelp(50.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->GetMotionMaster()->MovePoint(0, WALKX_KORT, WALKY_KORT, WALKZ_KORT);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_KORT_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_KORT_DEATH, m_creature);

        if (m_pInstance)
        {
            Creature* pZeliek = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_ZELIEK));
            Creature* pRivendare = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_RIVENDARE));
            Creature* pBlaumeux = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_BLAUMEUX));
            if (pZeliek && pRivendare && pBlaumeux && pZeliek->isDead() && pRivendare->isDead() && pBlaumeux->isDead())
            {
                 m_pInstance->SetData(TYPE_FOUR_HORSEMEN, DONE);

                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEV_MILITARY);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsMovingToCorner)
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE)
            {
                m_bIsMovingToCorner = false;
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            return;
        }

        if (m_uiMark_Count >= 100)
            if (m_uiEnrage_Timer < uiDiff)
            {
                DoCast(m_creature, SPELL_BERSERK);
                m_uiEnrage_Timer = 300000;
            }
            else
                m_uiEnrage_Timer -= uiDiff;

        // Mark of Korthazz
        if (m_uiMark_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,SPELL_MARK_OF_KORTHAZZ,CAST_TRIGGERED);
            m_uiMark_Timer = 12000;
            m_uiMark_Count++;
        }
        else 
            m_uiMark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (m_bShieldWall1)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall1 = false;
            }
        }
        else if (m_bShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (m_bShieldWall2)
            {
                DoCastSpellIfCan(m_creature,SPELL_SHIELDWALL);
                m_bShieldWall2 = false;
            }
        }

        // Meteor
        if (m_uiMeteor_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_METEOR : SPELL_METEOR_H);
            m_uiMeteor_Timer = 15000;
        }
        else 
            m_uiMeteor_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thane_korthazz(Creature* pCreature)
{
    return new boss_thane_korthazzAI(pCreature);
}

struct MANGOS_DLL_DECL mob_void_zone_naxxAI : public ScriptedAI
{
    mob_void_zone_naxxAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_despawn_timer;
    uint32 m_uiConsumptionTimer;

    void Reset()
    {
        m_despawn_timer = 75000;
        m_uiConsumptionTimer = 1000;
        SetCombatMovement(false);
        m_creature->GetMotionMaster()->MoveIdle();
    }

    void UpdateAI(const uint32 diff) 
    {
        if (m_uiConsumptionTimer <= diff)
        {
            int32 dmg = m_bIsRegularMode ? 3000 : 4500;
            m_creature->CastCustomSpell(m_creature, 30498, &dmg, NULL, NULL, true);
            m_uiConsumptionTimer = 1000;
        }
        else
            m_uiConsumptionTimer -= diff;

        if (m_despawn_timer <= diff)
        {
            m_creature->ForcedDespawn();
        }
        else
            m_despawn_timer -= diff;
    }
};

CreatureAI* GetAI_mob_void_zone_naxx(Creature* pCreature)
{
    return new mob_void_zone_naxxAI(pCreature);
}

void AddSC_boss_four_horsemen()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_lady_blaumeux";
    pNewScript->GetAI = &GetAI_boss_lady_blaumeux;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_rivendare_naxx";
    pNewScript->GetAI = &GetAI_boss_rivendare_naxx;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_thane_korthazz";
    pNewScript->GetAI = &GetAI_boss_thane_korthazz;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_sir_zeliek";
    pNewScript->GetAI = &GetAI_boss_sir_zeliek;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_void_zone_naxx";
    pNewScript->GetAI = &GetAI_mob_void_zone_naxx;
    pNewScript->RegisterSelf();
}
