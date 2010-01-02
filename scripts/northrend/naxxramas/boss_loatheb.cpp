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
SDName: Boss_Loatheb
SD%Complete: 90
SDComment: Doors don't work, timing not off, but very close
SDCategory: Naxxramas
EndScriptData */

/*SQL
update creature_template set ScriptName="mob_loatheb_spores" where entry=16286
update creature_template set ScriptName="boss_loatheb" where entry=16011
*/

#include "precompiled.h"
#include "naxxramas.h"

//Boss Loatheb spells
#define SPELL_NECROTIC_AURA         55593
#define SPELL_DEATHBLOOM            29865
#define SPELL_INEVITABLE_DOOM       29204

//Mob Loatheb Spore and his spell
#define MOB_LOATHEB_SPORE            16286
#define SPELL_FUNGAL_CREEP          29232

//Loatheb Spore spawn places
#define ADD_1X 2957.040
#define ADD_1Y -3997.590
#define ADD_1Z 274.280

#define ADD_2X 2909.130
#define ADD_2Y -4042.970
#define ADD_2Z 274.280

#define ADD_3X 2861.102
#define ADD_3Y -3997.901
#define ADD_3Z 274.280

struct MANGOS_DLL_DECL boss_loathebAI : public ScriptedAI
{
    boss_loathebAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 NecroticAura_Timer;
    uint32 Deathbloom_Timer;
    uint32 InevitableDoom_Timer;
    uint32 InevitableDoom_Cooldown;
    uint32 Summon_Timer;

    void Reset()
    {
        NecroticAura_Timer = 20000;
        Deathbloom_Timer = 30000;
        InevitableDoom_Timer = 120000;
        InevitableDoom_Cooldown = 40000;
        //This is cooldown for Doom spell. 40000 means 30sec
        //cooldown + 10sec spelltime, so next doom will be cast 30 sec
        //after first ends. cooldown decreases by 5 sec after each doom
        Summon_Timer = 8000;
        
        if(pInstance) pInstance->SetData(TYPE_LOATHEB, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        if(pInstance) pInstance->SetData(TYPE_LOATHEB, IN_PROGRESS);
    }
    
    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_LOATHEB, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //NecroticAura_Timer
        if (NecroticAura_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_NECROTIC_AURA);
            NecroticAura_Timer = 20000;
        }else NecroticAura_Timer -= diff;

        //Deathbloom_Timer
        if (Deathbloom_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_DEATHBLOOM);
            Deathbloom_Timer = 30000;
        }else Deathbloom_Timer -= diff;

        //InevitableDoom_Timer
        if (InevitableDoom_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_INEVITABLE_DOOM);
            InevitableDoom_Timer = InevitableDoom_Cooldown;
            if (InevitableDoom_Cooldown > 15000)
                InevitableDoom_Cooldown -= 5000;
        }else InevitableDoom_Timer -= diff;

        //Summon_Timer
        if (Summon_Timer < diff)
        {
            Unit* SummonedSpores = NULL;
            switch (rand()%3)
            {
                case 0:
                    SummonedSpores = m_creature->SummonCreature(16286,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    break;
                case 1:
                    SummonedSpores = m_creature->SummonCreature(16286,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    break;
                case 2:
                    SummonedSpores = m_creature->SummonCreature(16286,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    break;
            };
            if (SummonedSpores)
                SummonedSpores->AddThreat(m_creature->getVictim(), 1.0f);
            Summon_Timer = 24000;
        } else Summon_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_loatheb(Creature *_Creature)
{
    return new boss_loathebAI (_Creature);
}

struct MANGOS_DLL_DECL mob_loatheb_sporesAI : public ScriptedAI
{
    mob_loatheb_sporesAI(Creature *c) : ScriptedAI(c){Reset();}

    bool InCombat;
    void Reset(){}

    void Aggro(Unit *who){}
    void JustDied(Unit* Killer)
    {
        DoCast(m_creature,SPELL_FUNGAL_CREEP,true);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_loatheb_spores(Creature *_Creature)
{
    return new mob_loatheb_sporesAI (_Creature);
}

void AddSC_boss_loatheb()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_loatheb";
    newscript->GetAI = &GetAI_boss_loatheb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_loatheb_spores";
    newscript->GetAI = &GetAI_mob_loatheb_spores;
    newscript->RegisterSelf();
}
