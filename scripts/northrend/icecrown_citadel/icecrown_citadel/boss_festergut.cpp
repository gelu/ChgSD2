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
SD%Complete: 80%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
// Need correct timers
#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
    SPELL_GASEOUS_BLIGHT     = 69162,
    SPELL_INHALE_BLIGHT      = 69165,
    SPELL_INHALED_BLIGHT     = 69166,
    SPELL_PUNGENT_BLIGHT     = 69195,
    SPELL_PUNGENT_BLIGHT_1   = 69157,
    SPELL_PUNGENT_BLIGHT_2   = 69126,
    SPELL_GAS_SPORE          = 69278,
    SPELL_SPORE_AURA_0       = 69279,
    SPELL_SPORE_AURA_1       = 69290,
    SPELL_INOCULATE          = 72103,
    SPELL_GASTRIC_BLOAT      = 72219,
//    SPELL_GASTRIC_BLOAT      = 72214,
    SPELL_GASTRIC_EXPLOSION  = 72227,
    SPELL_VILE_GAS           = 72272,
    SPELL_VILE_GAS_AURA      = 69244,
    SPELL_VILE_GAS_AURA_0    = 69248,
    SPELL_BERSERK            = 47008,
    SPELL_SUMMON_VILE_STALKER       = 72287,

    NPC_VILE_GAS_STALKER     = 38548,
    MAX_SPORE_TARGETS        = 6,
};

struct MANGOS_DLL_DECL boss_festergutAI : public BSWScriptedAI
{
    boss_festergutAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    bool intro;
    bool pet;
    bool sporeCasted;
    Unit* spored[MAX_SPORE_TARGETS];

    void Reset()
    {
        if(!pInstance) return;
        resetTimers();
        pInstance->SetData(TYPE_FESTERGUT, NOT_STARTED);
        stage = 0;
        intro = false;
        pet = false;
        sporeCasted = false;
        memset(&spored, 0, sizeof(spored));
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);
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
        doRemoveFromAll(SPELL_PUNGENT_BLIGHT);
        doRemoveFromAll(SPELL_PUNGENT_BLIGHT_1);
        doRemoveFromAll(SPELL_PUNGENT_BLIGHT_2);
        DoScriptText(-1631206,m_creature);
    }

    void doTriggerUnoculated()
    {
         for(uint8 i = 0; i < MAX_SPORE_TARGETS; ++i)
             if (spored[i])
                if (!hasAura(SPELL_SPORE_AURA_1,spored[i]))
                {
                   doCast(SPELL_INOCULATE,spored[i]);
                   spored[i] = NULL;
                }
    }

    void doSearchSpored()
    {
         Unit* searched = doSelectRandomPlayer(SPELL_SPORE_AURA_1, true, 80.0f);

         if (!searched) return;

         bool exist = false;

         for(uint8 i = 0; i < MAX_SPORE_TARGETS; ++i)
             if (spored[i])
                if (spored[i] = searched)
                {
                    exist = true;
                    break;
                }

         if (exist) return;

         for(uint8 i = 0; i < MAX_SPORE_TARGETS; ++i)
             if (!spored[i])
             {
                 spored[i] = searched;
                 break;
             }
    }

    void UpdateAI(const uint32 diff)
    {

        if(!pInstance) return;

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
                    if (timedQuery(SPELL_GASEOUS_BLIGHT, diff))
                        {
                            doCast(SPELL_GASEOUS_BLIGHT);
                            stage = 1;
                        }
                    break;
            case 1:
                    switch (urand(0,2)) {
                            case 0:  DoScriptText(-1631210,m_creature); break;
                            case 1:  DoScriptText(-1631211,m_creature); break;
                            case 2:  DoScriptText(-1631212,m_creature); break;
                            }
                        doCast(SPELL_INHALE_BLIGHT);
                        stage = 2;
                    break;
            case 2:
                    if (timedQuery(SPELL_GASEOUS_BLIGHT, diff))
                        {
                            doCast(SPELL_GASEOUS_BLIGHT);
                            stage = 3;
                        }
                    break;
            case 3:
                    switch (urand(0,2)) {
                            case 0:  DoScriptText(-1631210,m_creature); break;
                            case 1:  DoScriptText(-1631211,m_creature); break;
                            case 2:  DoScriptText(-1631212,m_creature); break;
                            }
                        doCast(SPELL_INHALE_BLIGHT);
                        stage = 4;
                    break;
            case 4:
                    if (timedQuery(SPELL_GASEOUS_BLIGHT, diff))
                        {
                            doCast(SPELL_GASEOUS_BLIGHT);
                            stage = 5;
                        }
                    break;
            case 5:
                    switch (urand(0,2)) {
                            case 0:  DoScriptText(-1631210,m_creature); break;
                            case 1:  DoScriptText(-1631211,m_creature); break;
                            case 2:  DoScriptText(-1631212,m_creature); break;
                            }
                        doCast(SPELL_INHALE_BLIGHT);
                        stage = 6;
                    break;
            case 6:
                    if (timedQuery(SPELL_PUNGENT_BLIGHT, diff))
                        {
                            DoScriptText(-1631208,m_creature);
                            doCast(SPELL_PUNGENT_BLIGHT);
                            stage = 7;
                        }
                    break;
            case 7:
                    switch (urand(0,2)) {
                            case 0:  DoScriptText(-1631210,m_creature); break;
                            case 1:  DoScriptText(-1631211,m_creature); break;
                            case 2:  DoScriptText(-1631212,m_creature); break;
                            }
                        m_creature->RemoveAurasDueToSpell(SPELL_INHALED_BLIGHT);
                        stage = 0;
                    break;
        }

        doTriggerUnoculated();

        timedCast(SPELL_GAS_SPORE, diff);

        doSearchSpored();

        timedCast(SPELL_GASTRIC_BLOAT, diff);

        if (auraCount(SPELL_GASTRIC_BLOAT,m_creature->getVictim(),EFFECT_INDEX_1) > 9)
        {
            doCast(SPELL_GASTRIC_EXPLOSION,m_creature->getVictim());
            m_creature->getVictim()->RemoveAurasDueToSpell(SPELL_GASTRIC_BLOAT);
        }

        if (timedQuery(SPELL_VILE_GAS, diff)) {
//                        if (Unit* pTemp = doSummon(NPC_VILE_GAS_STALKER))
                            doCast(SPELL_VILE_GAS);
                        DoScriptText(-1631213,m_creature);
                        };

        if (timedQuery(SPELL_BERSERK, diff)){
                 doCast(SPELL_BERSERK);
                 DoScriptText(-1631207,m_creature);
                 };

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_festergut(Creature* pCreature)
{
    return new boss_festergutAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_vile_gas_stalkerAI : public ScriptedAI
{
    mob_vile_gas_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 m_lifetimer;

    void Reset()
    {
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetInCombatWithZone();
        SetCombatMovement(false);
        m_creature->SetDisplayId(11686);
        m_lifetimer = 12000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_FESTERGUT) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_lifetimer <= uiDiff)
            m_creature->ForcedDespawn();
        else m_lifetimer -= uiDiff;

    }
};

CreatureAI* GetAI_mob_vile_gas_stalker(Creature* pCreature)
{
    return new mob_vile_gas_stalkerAI(pCreature);
}

void AddSC_boss_festergut()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_festergut";
    newscript->GetAI = &GetAI_boss_festergut;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vile_gas_stalker";
    newscript->GetAI = &GetAI_mob_vile_gas_stalker;
    newscript->RegisterSelf();
}
