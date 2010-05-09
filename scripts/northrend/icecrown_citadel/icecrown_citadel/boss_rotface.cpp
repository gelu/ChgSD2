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
SDName: boss_rotface
SD%Complete: 0%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
    SPELL_OOZE_FLOOD         = 69789,
    SPELL_OOZE_FLOOD_0       = 69788,
    SPELL_OOZE_FLOOD_1       = 69783,
    SPELL_SLIME_SPRAY        = 69508,
    SPELL_MUTATED_INFECTION  = 69674,
    SPELL_BERSERK            = 47008,
    SPELL_STICKY_OOZE        = 69774,
    SPELL_RADIATING_OOZE     = 69750,
    SPELL_RADIATING_OOZE_1   = 69760,
    SPELL_UNSTABLE_OOZE      = 69558,
    SPELL_OOZE_EXPLODE       = 69839,
};

struct MANGOS_DLL_DECL boss_rotfaceAI : public ScriptedAI
{
    boss_rotfaceAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool intro;
    bool pet;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_ROTFACE, NOT_STARTED);
        stage = 0;
        intro = false;
        pet = false;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if(!pInstance || intro) return
        pInstance->SetData(TYPE_EVENT, 600);
        intro = true;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631222,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631223,m_creature,pVictim);
               break;
        }
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_ROTFACE, IN_PROGRESS);
        DoScriptText(-1631221,m_creature,who);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_ROTFACE, DONE);
        DoScriptText(-1631224,m_creature);
    }

    void UpdateAI(const uint32 diff)
    {

    if (!pet) {
              if (Creature* pGuard = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_PRECIOUS)))
                                if (!pGuard->isAlive())  {
                                                         pet = true;
                                                         DoScriptText(-1631228,m_creature);
                                                         };
                            };

    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bsw->timedQuery(SPELL_OOZE_FLOOD_1, diff)){
                 bsw->doCast(SPELL_OOZE_FLOOD_1);
                 DoScriptText(-1631225,m_creature);
                 };

        bsw->timedCast(SPELL_SLIME_SPRAY, diff);

        if (bsw->timedQuery(SPELL_MUTATED_INFECTION, diff)){
                 bsw->doCast(SPELL_MUTATED_INFECTION);
                 DoScriptText(-1631226,m_creature);
                 };

        if (bsw->timedQuery(SPELL_BERSERK, diff)){
                 bsw->doCast(SPELL_BERSERK);
                 DoScriptText(-1631225,m_creature);
                 };

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_rotface(Creature* pCreature)
{
    return new boss_rotfaceAI(pCreature);
}

void AddSC_boss_rotface()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_rotface";
    newscript->GetAI = &GetAI_boss_rotface;
    newscript->RegisterSelf();
}
