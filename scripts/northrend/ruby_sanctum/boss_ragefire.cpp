/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_ragefire
SD%Complete: 50%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

//TODO: work on air phase movement/cast/co-ords, sql spells, sql npcs, TEST

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum BossSpells
{
    SPELL_TWILIGHT_PRECISION         = 78243,
    SPELL_ENRAGE                     = 78722, //soft enrage
    SPELL_FLAME_BREATH               = 74404,
    SPELL_BEACON                     = 74453, //mark for conflag
    SPELL_CONFLAG                    = 74452,
};

struct MANGOS_DLL_DECL boss_ragefireAI : public BSWScriptedAI
{
    boss_ragefireAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 phase;

    void Reset()
    {
        if(!pInstance)
            return;

        pInstance->SetData(TYPE_RAGEFIRE, NOT_STARTED);
        resetTimers();
        phase = 0;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666401,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666402,m_creature,pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (pInstance)
            pInstance->SetData(TYPE_RAGEFIRE, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(pInstance)
            pInstance->SetData(TYPE_RAGEFIRE, IN_PROGRESS);

        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_RAGEFIRE, IN_PROGRESS);
        DoScriptText(-1666400,m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData(TYPE_RAGEFIRE, DONE);

        DoScriptText(-1666403,m_creature);
        pInstance->SetData(TYPE_BALTHARUS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_TWILIGHT_PRECISION, diff);

        if ( m_creature->GetHealthPercent() <= 80.0f && phase == 0)
        {
            phase = 1;
        }
        if ( m_creature->GetHealthPercent() <= 60.0f && phase == 1)
        {
            phase = 2;
        }
        if ( m_creature->GetHealthPercent() <= 40.0f && phase == 2)
        {
           phase = 3;
        }
        if ( m_creature->GetHealthPercent() <= 20.0f && phase == 3)
        {
           phase = 4;
        }

        switch (phase)
        {
            case 0: //GROUND
                 doCast(SPELL_FLAME_BREATH);
                 doCast(SPELL_ENRAGE);
                 break;
            case 1: //AIR
                    //NEED SCRIPT AIR MOVEMENT
                DoScriptText(-1666404,m_creature);
                doCast(SPELL_BEACON);
                doCast(SPELL_CONFLAG);
                break;
            case 2: //GROUND
                doCast(SPELL_FLAME_BREATH);
                doCast(SPELL_ENRAGE);
                break;
            case 3: //AIR
                    //NEED SCRIPT AIR MOVEMENT
                DoScriptText(-1666404,m_creature);
                doCast(SPELL_BEACON);
                doCast(SPELL_CONFLAG);
                break;
            case 4: //GROUND
                doCast(SPELL_FLAME_BREATH);
                doCast(SPELL_ENRAGE);
                break;
            default:
                break;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ragefire(Creature* pCreature)
{
    return new boss_ragefireAI(pCreature);
}

void AddSC_boss_ragefire()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ragefire";
    newscript->GetAI = &GetAI_boss_ragefire;
    newscript->RegisterSelf();
}
