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
    SPELL_SHIELDWALL        = 29061,
    SPELL_BESERK            = 26662,

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
    SPELL_UNYILDING_PAIN    = 57381,
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
    NPC_SPIRIT_OF_RIVENDARE    = 0,                          //creature entry not known yet
    NPC_SPIRIT_OF_KORTHAZZ    = 16778,
    NPC_SPIRIT_OF_ZELIREK    = 16777,
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

#define HIGH_THREAT               50.0f

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

    uint32 Mark_Timer;
    uint32 VoidZone_Timer;
    uint32 Cast_Timer;
    bool Move_Check;
    bool Chase;

    bool ShieldWall1;
    bool ShieldWall2;

    void Reset()
    {
        Mark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        VoidZone_Timer = 12000;                             // right
        Cast_Timer = 9000;
        Move_Check = true;
        Chase = true;

        ShieldWall1 = true;
        ShieldWall2 = true;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_BLAU_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLAUMEUX, IN_PROGRESS);

        m_creature->AddThreat(who, HIGH_THREAT);
        m_creature->CallForHelp(50.0f);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_BLAU_SLAY, m_creature);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;
 
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            if(Chase)
            {
                m_creature->GetMotionMaster()->MoveChase(pWho);
                Chase = false;
            }
        }
    }

    Unit *PickNearestPlayer()
    {
        Unit *nearp = NULL;
        float neardist = 0.0f;
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &lPlayers = pMap->GetPlayers();
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (!pPlayer)
                continue;
            float pudist = pPlayer->GetDistance((const Creature *)m_creature);
            if (!nearp || (neardist > pudist))
            {
                nearp = pPlayer;
                neardist = pudist;
            }
        }
        return nearp;
    }


    void Cast(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (!pWho)
            return;

        AttackStart(pWho);
        if(pWho->IsWithinDist(m_creature, 40))
            DoCast(pWho, m_bIsRegularMode ? SPELL_SHADOW_BOLT : H_SPELL_SHADOW_BOLT);
        else
            DoCast(pWho, SPELL_UNYILDING_PAIN);
        Cast_Timer = 2050;
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_BLAU_DEATH, m_creature);
        
        if (m_pInstance)
            m_pInstance->SetData(TYPE_BLAUMEUX, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        //run on aggro
        if (m_creature->getVictim() && Move_Check)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovePoint(0, WALKX_BLAU, WALKY_BLAU, WALKZ_BLAU);
            Move_Check = false;
        }
        
        // Cast
        if (Cast_Timer < uiDiff)
        {
            Unit *nearu = PickNearestPlayer();
            Cast(nearu);
        }else Cast_Timer -= uiDiff;

        // Mark of Blaumeux
        if (Mark_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_BLAUMEUX);
            Mark_Timer = 12000;
        }else Mark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (ShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if (ShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Void Zone
        if (VoidZone_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_VOIDZONE);
            VoidZone_Timer = 12000;
        }else VoidZone_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_lady_blaumeux(Creature* pCreature)
{
    return new boss_lady_blaumeuxAI(pCreature);
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

    uint32 Mark_Timer;
    uint32 UnholyShadow_Timer;
    bool Move_Check;
    bool Attack_Check;
    bool ShieldWall1;
    bool ShieldWall2;

    void Reset()
    {
        Mark_Timer = 20000;
        UnholyShadow_Timer = 15000;
        Move_Check = true;
        Attack_Check = true;
        ShieldWall1 = true;
        ShieldWall2 = true;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_RIVE_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_RIVE_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FOUR_HORSEMEN, IN_PROGRESS);
            m_pInstance->SetData(TYPE_RIVENDARE, IN_PROGRESS);

        m_creature->AddThreat(who, HIGH_THREAT);
        m_creature->CallForHelp(50.0f);
    }

    void KilledUnit(Unit* Victim)
    {
        switch(rand()%2)
        {
            case 0: DoScriptText(SAY_RIVE_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_RIVE_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RIVENDARE, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //run on aggro
        if (m_creature->getVictim() && Move_Check)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovePoint(0, WALKX_RIVE, WALKY_RIVE, WALKZ_RIVE);
            Move_Check = false;
        }
        
        //when reach position, set possible to attack
        if (m_creature->GetDistance2d(WALKX_RIVE, WALKY_RIVE) <= 2 && Attack_Check)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            Attack_Check = false;
        }

        // Mark of Rivendare
        if (Mark_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_RIVENDARE);
            Mark_Timer = 15000;
        }else Mark_Timer -= diff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (ShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            if (ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if (ShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            if (ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        if (UnholyShadow_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_UNHOLY_SHADOW : H_SPELL_UNHOLY_SHADOW);
            UnholyShadow_Timer = 15000;
        }else UnholyShadow_Timer -= diff;

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

    uint32 Mark_Timer;
    uint32 Meteor_Timer;
    bool Move_Check;
    bool Attack_Check;

    bool ShieldWall1;
    bool ShieldWall2;

    void Reset()
    {
        Mark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        Meteor_Timer = 30000;                               // wrong
        
        Move_Check = true;
        Attack_Check = true;
        ShieldWall1 = true;
        ShieldWall2 = true;
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_KORT_SLAY, m_creature);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_KORT_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KORTHAZZ, IN_PROGRESS);

        m_creature->AddThreat(who, HIGH_THREAT);
        m_creature->CallForHelp(50.0f);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_KORT_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KORTHAZZ, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //run on aggro
        if (m_creature->getVictim() && Move_Check)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovePoint(0, WALKX_KORT, WALKY_KORT, WALKZ_KORT);
            Move_Check = false;
        }
        
        //when reach position, set possible to attack
        if (m_creature->GetDistance2d(WALKX_KORT, WALKY_KORT) <= 2 && Attack_Check)
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            Attack_Check = false;
        }

        // Mark of Korthazz
        if (Mark_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_KORTHAZZ);
            Mark_Timer = 12000;
        }else Mark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (ShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if (ShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Meteor
        if (Meteor_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_METEOR);
            Meteor_Timer = 20000;                           // wrong
        }else Meteor_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thane_korthazz(Creature* pCreature)
{
    return new boss_thane_korthazzAI(pCreature);
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

    uint32 Cast_Timer;
    uint32 Mark_Timer;
    uint32 HolyWrath_Timer;
    bool Move_Check;
    bool Chase;
    
    bool ShieldWall1;
    bool ShieldWall2;

    void Reset()
    {
        Mark_Timer = 20000;                                 // First Horsemen Mark is applied at 20 sec.
        HolyWrath_Timer = 12000;                            // right
        Cast_Timer = 9000;
        Move_Check = true;
        Chase = true;
        
        ShieldWall1 = true;
        ShieldWall2 = true;
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_ZELI_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZELIEK, IN_PROGRESS);

        m_creature->AddThreat(who, HIGH_THREAT);
        m_creature->CallForHelp(50.0f);
    }

    Unit *PickNearestPlayer()
    {
        Unit *nearp = NULL;
        float neardist = 0.0f;
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &lPlayers = pMap->GetPlayers();
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            Player* pPlayer = itr->getSource();
            if (!pPlayer)
                continue;
            float pudist = pPlayer->GetDistance((const Creature *)m_creature);
            if (!nearp || (neardist > pudist))
            {
                nearp = pPlayer;
                neardist = pudist;
            }
        }
        return nearp;
    }

    void Cast(Unit* pWho)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        
        AttackStart(pWho);
        if(pWho->IsWithinDist(m_creature, 40))
            DoCast(pWho, m_bIsRegularMode ? SPELL_HOLY_BOLT : H_SPELL_HOLY_BOLT);
        else
            DoCast(pWho, SPELL_CONDEMNATION);
        Cast_Timer = 2050;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;
 
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            if(Chase)
            {
                m_creature->GetMotionMaster()->MoveChase(pWho);
                Chase = false;
            }
        }
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_ZELI_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_ZELI_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ZELIEK, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //run on aggro
        if (m_creature->getVictim() && Move_Check)
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MovePoint(0, WALKX_ZELI, WALKY_ZELI, WALKZ_ZELI);
            Move_Check = false;
        }

        // Cast
        if (Cast_Timer < uiDiff)
        {
            Unit *nearu = PickNearestPlayer();
            Cast(nearu);
        }else Cast_Timer -= uiDiff;
        
        // Mark of Zeliek
        if (Mark_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_MARK_OF_ZELIEK);
            Mark_Timer = 12000;
        }else Mark_Timer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (ShieldWall1 && m_creature->GetHealthPercent() < 50.0f)
        {
            if (ShieldWall1)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall1 = false;
            }
        }
        if (ShieldWall2 && m_creature->GetHealthPercent() < 20.0f)
        {
            if (ShieldWall2)
            {
                DoCast(m_creature,SPELL_SHIELDWALL);
                ShieldWall2 = false;
            }
        }

        // Holy Wrath
        if (HolyWrath_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_HOLY_WRATH);
            HolyWrath_Timer = 12000;
        }else HolyWrath_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_sir_zeliek(Creature* pCreature)
{
    return new boss_sir_zeliekAI(pCreature);
}

void AddSC_boss_four_horsemen()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_lady_blaumeux";
    NewScript->GetAI = &GetAI_boss_lady_blaumeux;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_rivendare_naxx";
    NewScript->GetAI = &GetAI_boss_rivendare_naxx;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_thane_korthazz";
    NewScript->GetAI = &GetAI_boss_thane_korthazz;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_sir_zeliek";
    NewScript->GetAI = &GetAI_boss_sir_zeliek;
    NewScript->RegisterSelf();
}