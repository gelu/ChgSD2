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
SDName: boss_lord_marrowgar
SD%Complete: 10%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        //Abilities
        SPELL_SABER_LASH_N                      = 71021,
        SPELL_COLD_FLAME_N                      = 69146,
        SPELL_BONE_STRIKE_N                     = 69057,
        SPELL_BONE_STORM_N                      = 69076,

        SPELL_SABER_LASH_H                      = 70814,
        SPELL_COLD_FLAME_H                      = 70825,
        SPELL_BONE_STRIKE_H                     = 69057,
        SPELL_BONE_STORM_H                      = 69076,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint8 stage;
    uint32 m_uiSaberLash_Timer;
    uint32 m_uiColdFlame_Timer;
    uint32 m_uiBoneStrike_Timer;
    bool m_uiBoneStorm;
    bool m_uiBoneStorm_Timer;
    uint32 m_uiBerserk_Timer;
    uint8 health;

    void Reset()
    {
    stage = 0;
    health = 100;
    m_uiSaberLash_Timer = 10000;
    m_uiColdFlame_Timer = 25000;
    m_uiBoneStrike_Timer = urand(17000,29000);
    m_uiBoneStorm = false;
    m_uiBoneStorm_Timer = 40000;
    m_uiBerserk_Timer = 600000;

        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    if (m_uiBoneStrike_Timer < diff)
                    { if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, Regular ? SPELL_BONE_STRIKE_N :SPELL_BONE_STRIKE_H);
                    m_uiBoneStrike_Timer=urand(17000,29000);
                    } else m_uiBoneStrike_Timer -= diff;
                    break;}

            case 1: {
                    if (!m_uiBoneStorm)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_BONE_STORM_N : SPELL_BONE_STORM_H);
                    m_uiBoneStorm = true;
                    stage = 2;
                    }
                    break;}
            case 2: {
                    if (m_uiBoneStorm_Timer < diff)
                    {
//                    m_creature->RemoveAurasDueToSpell(Regular ? SPELL_BONE_STORM_N : SPELL_BONE_STORM_H);
                    m_creature->SetInCombatWithZone();
                    stage = 3;
                    } else m_uiBoneStorm_Timer -= diff;
                    break;}
            case 3: break;
            }

                    if (m_uiSaberLash_Timer < diff)
                    {DoCastSpellIfCan(m_creature->getVictim(), Regular ? SPELL_SABER_LASH_N :SPELL_SABER_LASH_H);
                    m_uiSaberLash_Timer=urand(8000,12000);
                    } else m_uiSaberLash_Timer -= diff;

                    if (m_uiColdFlame_Timer < diff)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, Regular ? SPELL_COLD_FLAME_N :SPELL_COLD_FLAME_H);
                    m_uiColdFlame_Timer=urand(15000,25000);
                    } else m_uiColdFlame_Timer -= diff;

        health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
        if (health <= 30 && stage == 0) stage = 1;

        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
//            m_uiBerserk_Timer = m_bIsRegularMode ? 300000 : 180000;
        }
        else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrowgar";
    newscript->GetAI = &GetAI_boss_lord_marrowgar;
    newscript->RegisterSelf();
}
