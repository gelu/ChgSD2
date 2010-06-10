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
SDName: boss_baltharus
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

struct MANGOS_DLL_DECL boss_baltharusAI : public ScriptedAI
{
    boss_baltharusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
        bsw->resetTimers();
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
        if (pInstance) pInstance->SetData(TYPE_BALTHARUS, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_BALTHARUS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_TWILIGHT_PRECISION, diff);

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

void AddSC_boss_baltharus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baltharus";
    newscript->GetAI = &GetAI_boss_baltharus;
    newscript->RegisterSelf();
}
