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
SDName: boss_falryn
SD%Complete: 40%
SDComment: by /dev/rsa
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "def_halls.h"

enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        //Abilities
        SPELL_HOPELESSNESS                      = 72395,
        SPELL_IMPENDING_DESPAIR                 = 72426,
        SPELL_DEFILING_HORROR_N                 = 72435,
        SPELL_QUIVERING_STRIKE_N                = 72422,

        SPELL_DEFILING_HORROR_H                 = 72452,
        SPELL_QUIVERING_STRIKE_H                = 72453,

};

struct MANGOS_DLL_DECL boss_falrynAI : public ScriptedAI
{
    boss_falrynAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiDespair_Timer;
    uint32 m_uiHorror_Timer;
    uint32 m_uiStrike_Timer;

    uint8 health;
    uint8 stage;

    void Reset()
    {
    if(pInstance) pInstance->SetData(TYPE_FALRYN, NOT_STARTED);
    m_uiBerserk_Timer = 180000;
    stage = 0;
    m_uiDespair_Timer = Regular ? 40000 : 30000;
    m_uiHorror_Timer = urand(25000,35000);
    m_uiStrike_Timer = urand(10000,15000);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_FALRYN, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_FALRYN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    break;}
            case 1: {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_HOPELESSNESS);
                    stage = 2;
                    break;}
            case 2: {
                    break;}
            case 3: {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_HOPELESSNESS);
                    stage = 4;
                    break;}
            case 4: {
                    break;}
            case 5: {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_HOPELESSNESS);
                    stage = 6;
                    break;}
            case 6: {
                    break;}
        }
                    if (m_uiDespair_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_IMPENDING_DESPAIR);
                    m_uiDespair_Timer= Regular ? 40000 : 30000;
                    } else m_uiDespair_Timer -= diff;

                    if (m_uiStrike_Timer < diff)
                    {DoCastSpellIfCan(m_creature->getVictim(), Regular ? SPELL_QUIVERING_STRIKE_N : SPELL_QUIVERING_STRIKE_H);
                    m_uiStrike_Timer=urand(10000,15000);
                    } else m_uiStrike_Timer -= diff;

                    if (m_uiHorror_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_DEFILING_HORROR_N : SPELL_DEFILING_HORROR_H);
                    m_uiHorror_Timer=urand(25000,35000);
                    } else m_uiHorror_Timer -= diff;

        health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
        if (health <= 66 && stage == 0) stage = 1;
        if (health <= 33 && stage == 2) stage = 3;
        if (health <= 10 && stage == 4) stage = 5;

        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 180000;
        } else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_falryn(Creature* pCreature)
{
    return new boss_falrynAI(pCreature);
}

void AddSC_boss_falryn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_falryn";
    newscript->GetAI = &GetAI_boss_falryn;
    newscript->RegisterSelf();
}
