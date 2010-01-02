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
SDName: Boss_Noth
SD%Complete: 40
SDComment: Missing Balcony stage
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1                          = -1533075,
    SAY_AGGRO2                          = -1533076,
    SAY_AGGRO3                          = -1533077,
    SAY_SUMMON                          = -1533078,
    SAY_SLAY1                           = -1533079,
    SAY_SLAY2                           = -1533080,
    SAY_DEATH                           = -1533081,

    SPELL_BLINK                         = 29211,            //29208, 29209 and 29210 too
    SPELL_CRIPPLE                       = 29212,
    SPELL_CRIPPLE_H                     = 54814,
    SPELL_CURSE_PLAGUEBRINGER           = 29213,
    SPELL_CURSE_PLAGUEBRINGER_H         = 54835,

    SPELL_SUMMON_CHAMPION_AND_CONSTRUCT = 29240,
    SPELL_SUMMON_GUARDIAN_AND_CONSTRUCT = 29269,

    NPC_PLAGUED_WARRIOR                 = 16984,
    NPC_PLAGUED_CHAMPION                = 16983
};

// Teleport position of Noth on his balcony
#define TELE_X 2631.370
#define TELE_Y -3529.680
#define TELE_Z 274.040
#define TELE_O 6.277

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        AddsCount = Regular ? 2 : 3; //normal phase
        BalconyAddsCount = Regular ? 2 : 4; //balcony phase
        Reset();
    }

    ScriptedInstance* pInstance;
    bool Regular;

    uint32 BlinkTimer;
    uint32 CurseTimer;
    uint32 SummonTimer;
    uint32 BalconySummonTimer;

    uint32 AddsCount;
    uint32 BalconyAddsCount;

    bool BalconyPhase;
    uint32 PhaseTimer;

    void Reset()
    {
        BlinkTimer = 25000;
        CurseTimer = 4000;
        SummonTimer = 12000;
        BalconySummonTimer = 5000;

        BalconyPhase = false;
        PhaseTimer = 90000;
        
        if(pInstance) pInstance->SetData(TYPE_NOTH, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
        
        if(pInstance) pInstance->SetData(TYPE_NOTH, IN_PROGRESS);
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
            summoned->AddThreat(target);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(pInstance) pInstance->SetData(TYPE_NOTH, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(PhaseTimer < diff)
        {
            if(!BalconyPhase)
            {
                BalconyPhase = true;
                PhaseTimer = 70000;
                //teleport to balcony and make inactive
                SetCombatMovement(false);                
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveIdle();
                m_creature->NearTeleportTo(TELE_X, TELE_Y, TELE_Z, TELE_O);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
            else
            {
                BalconyPhase = false;
                PhaseTimer = 110000;
                //teleport back and make active
                m_creature->NearTeleportTo(2684.804, -3502.517, 261.313, 0);
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());   
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
        else PhaseTimer -= diff;

        if(BalconyPhase)
        {
            if(BalconySummonTimer < diff)
            {
                DoScriptText(SAY_SUMMON, m_creature);
                for(uint8 i = 0; i < BalconyAddsCount; ++i)
                    m_creature->SummonCreature(NPC_PLAGUED_CHAMPION, 2704.65, -3460.58, 262.86, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                BalconySummonTimer = 35000;
            }
            else BalconySummonTimer -= diff;
        }
        else
        {
            if (BlinkTimer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CRIPPLE);
                if(!Regular)
                {
                    DoCast(m_creature, SPELL_BLINK);
                    DoResetThreat();
                }
                BlinkTimer = 25000;
            }else BlinkTimer -= diff;

            if (CurseTimer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_CURSE_PLAGUEBRINGER);
                CurseTimer = 28000;
            }
            else CurseTimer -= diff;

            //Summon_Timer
            if (SummonTimer < diff)
            {
                DoScriptText(SAY_SUMMON, m_creature);
                for(uint8 i = 0; i < AddsCount; ++i)
                    m_creature->SummonCreature(NPC_PLAGUED_WARRIOR, 2684.804, -3502.517, 261.313, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 15000);
                SummonTimer = 30000;
            }   
            else SummonTimer -= diff;

            DoMeleeAttackIfReady();
        }       
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_noth";
    newscript->GetAI = &GetAI_boss_noth;
    newscript->RegisterSelf();
}
