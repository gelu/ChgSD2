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
SDName: Boss_Faerlina
SD%Complete: 80
SDComment: Need core support: spell 28732
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

#define SPELL_28732_NOT_YET_IMPLEMENTED

enum
{
    SAY_GREET                 =  -1533009,
    SAY_AGGRO                 =  -1533010,
    SAY_ENRAGE1               =  -1533011,
    SAY_ENRAGE2               =  -1533012,
    SAY_ENRAGE3               =  -1533013,
    SAY_SLAY1                 =  -1533014,
    SAY_SLAY2                 =  -1533015,
    SAY_DEATH                 =  -1533016
};
enum
{
    SPELL_POSIONBOLT_VOLLEY   =  28796,
    SPELL_ENRAGE              =  28798,
    SPELL_RAINOFFIRE          =  39024,                   //Not sure if targeted AoEs work if casted directly upon a player

    SPELL_WIDOWS_EMBRACE      =  28732,                    //We will use it, but the real thing will be another
    SPELL_FIREBALL            =  54095
};

struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    uint32 PoisonBoltVolley_Timer;
    uint32 RainOfFire_Timer;
    uint32 Enrage_Timer;
    bool HasTaunted;

    void Reset()
    {
        PoisonBoltVolley_Timer = 8000;
        RainOfFire_Timer = 16000;
        Enrage_Timer = 60000;
        HasTaunted = false;
        
        if(pInstance && m_creature->isAlive())
            pInstance->SetData(TYPE_FAERLINA, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        //Close the room for boss fight
        if(pInstance) pInstance->SetData(TYPE_FAERLINA, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!HasTaunted && m_creature->IsWithinDistInMap(who, 100.0f))
        {
            DoScriptText(SAY_GREET, m_creature);
            HasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(who);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        if(pInstance) pInstance->SetData(TYPE_FAERLINA, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //PoisonBoltVolley_Timer
        if (PoisonBoltVolley_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_POSIONBOLT_VOLLEY);
            PoisonBoltVolley_Timer = 11000;
        }else PoisonBoltVolley_Timer -= diff;

        //RainOfFire_Timer
        if (RainOfFire_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target,SPELL_RAINOFFIRE);
            RainOfFire_Timer = 16000;
        }else RainOfFire_Timer -= diff;

        //Enrage_Timer
        if (Enrage_Timer < diff)
        {
            switch (rand()%3)
            {
                case 0: DoScriptText(SAY_ENRAGE1, m_creature); break;
                case 1: DoScriptText(SAY_ENRAGE2, m_creature); break;
                case 2: DoScriptText(SAY_ENRAGE3, m_creature); break;
            }
            DoCast(m_creature,SPELL_ENRAGE);
            Enrage_Timer = 61000;
        }else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_faerlina(Creature *_Creature)
{
    return new boss_faerlinaAI (_Creature);
}

struct MANGOS_DLL_DECL mob_faerlina_worshipperAI : public ScriptedAI
{
    mob_faerlina_worshipperAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 fireball_timer;

    void Reset()
    {
        fireball_timer = 0;
    }

    void Aggro(Unit *who){}
    
    void JustDied(Unit* Killer)
    {
#ifndef SPELL_28732_NOT_YET_IMPLEMENTED
        DoCast(m_creature,SPELL_WIDOWS_EMBRACE,true);
#else
        Unit* Faerlina = Unit::GetUnit((*m_creature), pInstance->GetData64(GUID_FAERLINA));
        if(Faerlina && m_creature->GetDistance2d(Faerlina) < 15.0f)
            Faerlina->RemoveAurasByCasterSpell(SPELL_ENRAGE,pInstance->GetData64(GUID_FAERLINA));
#endif
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //PoisonBoltVolley_Timer
        if (fireball_timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_FIREBALL);
            fireball_timer = 3000;
        }else fireball_timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_mob_faerlina_worshipper(Creature *_Creature)
{
    return new mob_faerlina_worshipperAI (_Creature);
}

void AddSC_boss_faerlina()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_faerlina";
    newscript->GetAI = &GetAI_boss_faerlina;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_faerlina_worshipper";
    newscript->GetAI = &GetAI_mob_faerlina_worshipper;
    newscript->RegisterSelf();
}
