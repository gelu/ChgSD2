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
SDName: boss_festergut
SD%Complete: 20%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
    SPELL_GASEOUS_BLIGHT_0   = 70138,
    SPELL_GASEOUS_BLIGHT_1   = 69161,
    SPELL_GASEOUS_BLIGHT_2   = 70468,
    SPELL_INHALE_BLIGHT      = 69165,
    SPELL_INHALED_BLIGHT     = 71912,
    SPELL_PUNGENT_BLIGHT     = 69195,
    SPELL_PUNGENT_BLIGHT_1   = 69157,
    SPELL_PUNGENT_BLIGHT_2   = 69126,
    SPELL_GAS_SPORE          = 69278,
    SPELL_INOCULATE          = 72103,
    SPELL_GASTRIC_BLOAT      = 72219,
    SPELL_GASTRIC_EXPLOSION  = 72227,
    SPELL_VILE_GAS           = 72272,
    SPELL_BERSERK            = 47008,
};

struct MANGOS_DLL_DECL boss_festergutAI : public ScriptedAI
{
    boss_festergutAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
        bsw->resetTimers();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool intro;
    bool pet;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_FESTERGUT, NOT_STARTED);
        stage = 0;
        intro = false;
        pet = false;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if(!pInstance || intro) return;
        if (pWho->GetTypeId() != TYPEID_PLAYER) return;

        pInstance->SetData(TYPE_EVENT, 500);
        debug_log("EventMGR: creature %u send signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
        intro = true;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631204,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631205,m_creature,pVictim);
               break;
        }
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_FESTERGUT, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_FESTERGUT, IN_PROGRESS);
        DoScriptText(-1631203,m_creature,who);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_FESTERGUT, DONE);
        pInstance->SetData(TYPE_EVENT, 550);
        bsw->doRemove(SPELL_PUNGENT_BLIGHT);
        bsw->doRemove(SPELL_PUNGENT_BLIGHT_1);
        bsw->doRemove(SPELL_PUNGENT_BLIGHT_2);
        DoScriptText(-1631206,m_creature);
    }

    void UpdateAI(const uint32 diff)
    {

        if (!pet) {
                  if (Creature* pGuard = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_STINKY)))
                                if (!pGuard->isAlive())  {
                                                         pet = true;
                                                         DoScriptText(-1631209,m_creature);
                                                         }
                            }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                    if (bsw->timedQuery(SPELL_GASEOUS_BLIGHT_0, diff))
                        {
                        bsw->doCast(SPELL_GASEOUS_BLIGHT_0);
                        bsw->doCast(SPELL_INHALE_BLIGHT);
                        stage = 1;
                        }
                    break;
            case 1: 
                    if (bsw->timedQuery(SPELL_GASEOUS_BLIGHT_0, diff))
                        {
                        bsw->doCast(SPELL_GASEOUS_BLIGHT_0);
                        bsw->doCast(SPELL_INHALE_BLIGHT);
                        stage = 2;
                        }
                    break;
            case 2: 
                    if (bsw->timedQuery(SPELL_GASEOUS_BLIGHT_0, diff))
                        {
                        bsw->doCast(SPELL_GASEOUS_BLIGHT_0);
                        bsw->doCast(SPELL_INHALE_BLIGHT);
                        stage = 3;
                        }
                    break;
            case 3: 
                    if (bsw->timedQuery(SPELL_PUNGENT_BLIGHT, diff))
                        {
                        DoScriptText(-1631208,m_creature);
                        bsw->doCast(SPELL_PUNGENT_BLIGHT);
                        stage = 0;
                        }
                    break;
        }
        bsw->timedCast(SPELL_GAS_SPORE, diff);

        bsw->timedCast(SPELL_GASTRIC_BLOAT, diff);

        if (bsw->timedQuery(SPELL_VILE_GAS, diff)) {
                        bsw->doCast(SPELL_VILE_GAS);
                        DoScriptText(-1631213,m_creature);
                        };

        if (bsw->timedQuery(SPELL_BERSERK, diff)){
                 bsw->doCast(SPELL_BERSERK);
                 DoScriptText(-1631207,m_creature);
                 };


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_festergut(Creature* pCreature)
{
    return new boss_festergutAI(pCreature);
}


void AddSC_boss_festergut()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_festergut";
    newscript->GetAI = &GetAI_boss_festergut;
    newscript->RegisterSelf();
}
