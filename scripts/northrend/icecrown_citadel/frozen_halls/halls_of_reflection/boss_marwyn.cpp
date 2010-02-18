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
SDName: boss_marwyn
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
        SPELL_OBLITERATE_N                      = 72360,
        SPELL_SHARED_SUFFERING_N                = 72368,
        SPELL_WELL_OF_CORRUPTION                = 72362,
        SPELL_CORRUPTED_FLESH_N                 = 72363,

        SPELL_OBLITERATE_H                      = 72434,
        SPELL_SHARED_SUFFERING_H                = 72369,
        SPELL_CORRUPTED_FLESH_H                 = 72436,

};

struct MANGOS_DLL_DECL boss_marwynAI : public ScriptedAI
{
    boss_marwynAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiSharedSuffering_Timer;
    uint32 m_uiWell_Timer;
    uint32 m_uiTouch_Timer;
    uint32 m_uiFlesh_Timer;
    uint32 m_uiObliterate_Timer;

    uint8 health;
    uint8 stage;

    void Reset()
    {
    if(pInstance) pInstance->SetData(TYPE_FALRYN, NOT_STARTED);
    m_uiBerserk_Timer = 180000;
    m_uiSharedSuffering_Timer = 4000;
    m_uiWell_Timer = 5000;
    m_uiTouch_Timer = 8000;
    m_uiFlesh_Timer = 10000;
    m_uiObliterate_Timer = 1000;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARWYN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

                    if (m_uiSharedSuffering_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_SHARED_SUFFERING_N : SPELL_SHARED_SUFFERING_H);
                    m_uiSharedSuffering_Timer= 20000;
                    } else m_uiSharedSuffering_Timer -= diff;

                    if (m_uiWell_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_WELL_OF_CORRUPTION);
                    m_uiWell_Timer= 30000;
                    } else m_uiWell_Timer -= diff;

/*                    if (m_uiTouch_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_WELL_OF_CORRUPTION_N : SPELL_WELL_OF_CORRUPTION_H);
                    m_uiTouch_Timer= 30000;
                    } else m_uiTouch_Timer -= diff;
*/
                    if (m_uiFlesh_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_CORRUPTED_FLESH_N : SPELL_CORRUPTED_FLESH_H);
                    m_uiWell_Timer= 10000;
                    } else m_uiWell_Timer -= diff;

                    if (m_uiObliterate_Timer < diff)
                    {DoCastSpellIfCan(m_creature->getVictim(), Regular ? SPELL_OBLITERATE_N : SPELL_OBLITERATE_H);
                    m_uiObliterate_Timer=urand(8000,12000);
                    } else m_uiObliterate_Timer -= diff;



        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 180000;
        } else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_marwyn(Creature* pCreature)
{
    return new boss_marwynAI(pCreature);
}

void AddSC_boss_marwyn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_marwyn";
    newscript->GetAI = &GetAI_boss_marwyn;
    newscript->RegisterSelf();
}
