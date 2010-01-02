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
SDName: Boss_Sapphiron
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"

#define EMOTE_BREATH            -1533082
#define EMOTE_ENRAGE            -1533083

#define SPELL_ICEBOLT           28522
#define SPELL_FROST_BREATH      29318 //should be 28524
#define SPELL_FROST_AURA        28531
#define H_SPELL_FROST_AURA      55799
#define SPELL_LIFE_DRAIN        28542
#define H_SPELL_LIFE_DRAIN      55665
#define SPELL_BLIZZARD          28547
#define H_SPELL_BLIZZARD        55699
#define SPELL_TAIL_SWEEP        55697
#define H_SPELL_TAIL_SWEEP      55696
#define SPELL_CLEAVE            19983
#define SPELL_BERSERK           26662

struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;

    uint32 Icebolt_Count;
    uint32 Icebolt_Timer;
    uint32 FrostBreath_Timer;
    uint32 FrostAura_Timer;
    uint32 LifeDrain_Timer;
    uint32 Blizzard_Timer;
    uint32 TailSweep_Timer;
    uint32 Cleave_Timer;
    uint32 Fly_Timer;
    uint32 Berserk_Timer;
    uint32 phase;
    bool landoff;
    uint32 land_Timer;

    void Reset()
    {
        FrostAura_Timer = 2000;
        FrostBreath_Timer = 6000;
        LifeDrain_Timer = 24000;
        Blizzard_Timer = 20000;
        Fly_Timer = 45000;
        TailSweep_Timer = 10000 + rand()%10000;
        Cleave_Timer = 10000 + rand()%10000;
        Icebolt_Timer = 4000;
        land_Timer = 2000;
        Berserk_Timer = 900000;
        phase = 1;
        Icebolt_Count = 0;
        landoff = false;

        //m_creature->ApplySpellMod(SPELL_FROST_AURA, SPELLMOD_DURATION, -1);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (phase == 1)
        {
            if (FrostAura_Timer < diff)
            {
                DoCast(m_creature->getVictim(), Regular ? SPELL_FROST_AURA : H_SPELL_FROST_AURA);
                FrostAura_Timer = 5000;
            }
            else FrostAura_Timer -= diff;

            if (LifeDrain_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, Regular ? SPELL_LIFE_DRAIN : H_SPELL_LIFE_DRAIN);

                LifeDrain_Timer = 24000;
            }
            else LifeDrain_Timer -= diff;

            if (Blizzard_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, Regular ? SPELL_BLIZZARD : H_SPELL_BLIZZARD);

                Blizzard_Timer = 20000;
            }
            else Blizzard_Timer -= diff;

            if(TailSweep_Timer < diff)
            {
                DoCast(m_creature->getVictim(), Regular ? SPELL_TAIL_SWEEP : H_SPELL_TAIL_SWEEP);
                TailSweep_Timer = 10000 + rand()%10000;
            }
            else TailSweep_Timer -= diff;

            if(Cleave_Timer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = 10000 + rand()%10000;
            }
            else Cleave_Timer -= diff;

            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 10)
            {
                if (Fly_Timer < diff)
                {
                    phase = 2;
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();
                    //DoCast(m_creature,11010);
                    m_creature->SetHover(true);
                    //DoCast(m_creature,18430);
                    Icebolt_Timer = 4000;
                    Icebolt_Count = 0;
                    landoff = false;
                }
                else Fly_Timer -= diff;
            }
        }

        if (phase == 2)
        {
            if (Icebolt_Timer < diff && Icebolt_Count < 5)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, SPELL_ICEBOLT);

                ++Icebolt_Count;
                Icebolt_Timer = 4000;
            }
            else Icebolt_Timer -= diff;

            if (Icebolt_Count == 5 && !landoff)
            {
                if (FrostBreath_Timer < diff)
                {
                    DoScriptText(EMOTE_BREATH, m_creature);
                    //DoCast(m_creature->getVictim(),SPELL_FROST_BREATH);
                    land_Timer = 2000;
                    landoff = true;
                    FrostBreath_Timer = 6000;
                }
                else FrostBreath_Timer -= diff;
            }

            if (landoff)
            {
                if (land_Timer < diff)
                {
                    phase = 1;
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    m_creature->SetHover(false);
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    Fly_Timer = 67000;
                }
                else land_Timer -= diff;
            }
        }

        if (Berserk_Timer < diff)
        {
            DoScriptText(EMOTE_ENRAGE, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            Berserk_Timer = 300000;
        }
        else Berserk_Timer -= diff;

        if (phase!=2)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sapphiron(Creature* pCreature)
{
    return new boss_sapphironAI(pCreature);
}

void AddSC_boss_sapphiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_sapphiron";
    newscript->GetAI = &GetAI_boss_sapphiron;
    newscript->RegisterSelf();
}
