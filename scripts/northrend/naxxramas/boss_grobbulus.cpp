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


/*Poison Cloud 26590
Slime Spray 28157
Fallout slime 28218
Mutating Injection 28169
Enrages 26527*/

#include "precompiled.h"

#define SP_ENRAGE       26662
#define SP_SLIME_SPRAY  28157
#define SP_INJECTION    28169

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 EnrageTimer;
    uint32 MutatingInjectionTimer;
    uint32 PoisonCloudTimer;
    uint32 SlimeSprayTimer;

    void Reset()
    {
        EnrageTimer = 720000;
        SlimeSprayTimer = 15000 + rand()%10000;
        MutatingInjectionTimer = 10000;
    };

    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(SlimeSprayTimer < diff)
        {
            DoCast(m_creature, SP_SLIME_SPRAY);
            SlimeSprayTimer = 15000 + rand()%10000;
        }
        else SlimeSprayTimer -= diff;

        if(MutatingInjectionTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target) DoCast(target, SP_INJECTION);
            MutatingInjectionTimer = 10000;
        }
        else MutatingInjectionTimer -= diff;

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SP_ENRAGE);
            EnrageTimer = 120000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

void AddSC_boss_grobbulus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grobbulus";
    newscript->GetAI = &GetAI_boss_grobbulus;
    newscript->RegisterSelf();
};
