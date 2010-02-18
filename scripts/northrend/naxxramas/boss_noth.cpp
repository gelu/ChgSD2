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
    NPC_PLAGUED_CHAMPIONS               = 16983,
    NPC_PLAGUED_GUARDIANS               = 16981

};

uint32 m_auiSpellSummonPlaguedWarrior[]=
{
    29247, 29248, 29249
};

uint32 m_auiSpellSummonPlaguedChampion[]=
{
    29217, 29224, 29225, 29227, 29238, 29255, 29257, 29258, 29262, 29267
};

uint32 m_auiSpellSummonPlaguedGuardian[]=
{
    29226, 29239, 29256, 29268
};

// Teleport position of Noth on his balcony
#define TELE_X 2631.370
#define TELE_Y -3529.680
#define TELE_Z 274.040
#define TELE_O 6.277

// IMPORTANT: BALCONY TELEPORT NOT ADDED YET! WILL BE ADDED SOON!
// Dev note 26.12.2008: When is soon? :)
// Dev note 12.10.2009: http://www.wowwiki.com/Soon

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool isTeleported;

    uint8 SecondPhaseCounter;

    uint32 Blink_Timer;
    uint32 Curse_Timer;
    uint32 Summon_Timer;
    uint32 SecondPhase_Timer;
    uint32 Teleport_Timer;

    float LastX, LastY, LastZ;

    void Reset()
    {
        isTeleported = false;
        SecondPhaseCounter = 0;
        Blink_Timer = 25000;
        Curse_Timer = 4000;
        Summon_Timer = 30000;
        SecondPhase_Timer = 17000;
        Teleport_Timer = 120000;

        LastX = 0;
        LastY = 0;
        LastZ = 0;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
            AttackStart(who);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, IN_PROGRESS);
    }

    void AttackStart(Unit* who)
    {
        if (isTeleported)
            return;

        if (!who || who == m_creature)
            return;

        if (m_creature->Attack(who, true))
        {
            m_creature->SetInCombatWithZone();
            DoStartMovement(who);
        }
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
        {
            summoned->AddThreat(target,0.0f);
            summoned->AI()->AttackStart(target);
        }
    }

    void KilledUnit(Unit* victim)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (isTeleported)
        {
            if (Teleport_Timer < diff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->MonsterMove(LastX, LastY, LastZ,0);
                DoStartMovement(m_creature->getVictim());
                LastX = 0;
                LastY = 0;
                LastZ = 0;
                isTeleported = false;
                Teleport_Timer = 120000;
            }else Teleport_Timer -= diff;

            if (SecondPhase_Timer < diff)
            {
                switch (SecondPhaseCounter)
                {
                    case 0:
                        for(uint8 i = 0; i < (m_bIsRegularMode ? 2 : 4); i++)
                            m_creature->SummonCreature(NPC_PLAGUED_CHAMPIONS,2684.804,-3502.517,261.313,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                        break;
                    case 1:
                    case 2:
                        for(uint8 i = 0; i < (m_bIsRegularMode ? 2 : 4) - (m_bIsRegularMode ? 2 : 1); i++)
                            m_creature->SummonCreature(NPC_PLAGUED_CHAMPIONS,2684.804,-3502.517,261.313,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                        for(uint8 i = 0; i < (m_bIsRegularMode ? 1 : 2); i++)
                            m_creature->SummonCreature(NPC_PLAGUED_GUARDIANS,2684.804,-3502.517,261.313,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                        break;
                }
                SecondPhaseCounter ++;
                SecondPhase_Timer = 22000;
            } else SecondPhase_Timer -= diff;
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Blink_Timer
        if (Blink_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CRIPPLE : SPELL_CRIPPLE_H);
            //DoCast(m_creature, SPELL_BLINK);
            m_creature->GetMap()->CreatureRelocation(m_creature, 2670.804 + rand()%30, -3517.517 + rand()%30, 261.313, m_creature->GetOrientation());
            DoResetThreat();
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                AttackStart(pTarget);
            Blink_Timer = 25000;
        }else Blink_Timer -= diff;

        //Curse_Timer
        if (Curse_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode? SPELL_CURSE_PLAGUEBRINGER : SPELL_CURSE_PLAGUEBRINGER_H);
            Curse_Timer = 28000;
        }else Curse_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {
            DoScriptText(SAY_SUMMON, m_creature);

            for(uint8 i = 0; i < (m_bIsRegularMode ? 2 : 3); ++i)
                m_creature->SummonCreature(NPC_PLAGUED_WARRIOR, 2672.804 + rand()%15,-3509.517 + rand()%15, 261.313, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 80000);

            Summon_Timer = 30000;
        } else Summon_Timer -= diff;

        if (Teleport_Timer < diff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            LastX = m_creature->GetPositionX();
            LastY = m_creature->GetPositionY();
            LastZ = m_creature->GetPositionZ();
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->MonsterMove(TELE_X, TELE_Y, TELE_Z,0);
            isTeleported = true;
            SecondPhaseCounter = 0;
            SecondPhase_Timer = 0;
            Teleport_Timer = 70000;
            return;
        }else Teleport_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_noth";
    NewScript->GetAI = &GetAI_boss_noth;
    NewScript->RegisterSelf();
}
