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
SD%Complete: 0%
SDComment: by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum BossSpells
{
    SPELL_TWILIGHT_PRECISION         = 78243,
};

struct MANGOS_DLL_DECL boss_ragefireAI : public BSWScriptedAI
{
    boss_ragefireAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_RAGEFIRE, NOT_STARTED);
        resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE) return;
    }

    void KilledUnit(Unit* pVictim)
    {
/*    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };*/
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_RAGEFIRE, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_RAGEFIRE, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_RAGEFIRE, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_TWILIGHT_PRECISION, diff);

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
