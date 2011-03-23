/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: boss_eck
SD%Complete: 100%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{    
    SAY_AGGRO                   =    -1533021,
    SAY_DEATH					=    -1533021,
    EMOTE_SPITE					=    -1533021,
    EMOTE_SPRING                =    -1533021,    
    EMOTE_BIT                   =    -1533021,
    EMOTE_BERSERK               =    -1533021,
    
    SPELL_ECK_RESIDUE_DEBUFF    =    55817, 
    SPELL_ECK_SPITE             =    55814,

    SPELL_ECK_SPRING_WHERE      =    55837,
    SPELL_ECK_SPRING            =    55815,
    
    SPELL_ECK_BIT               =    55813,
    SPELL_ECK_BERSERK           =    55816   
};

/*######
## boss_eck
######*/

struct MANGOS_DLL_DECL boss_eckAI : public ScriptedAI
{
    boss_eckAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bBerserk;

    uint32 m_uiEckSpiteTimer;
    uint32 m_uiEckSpringTimer;
    uint32 m_uiEckBitTimer;
    uint32 m_uiEckBerserkTimer;
        
    void Reset()
    {
        m_uiEckSpiteTimer		= 15000;  
        m_uiEckSpringTimer		= 8000;
        m_uiEckBitTimer			= 5000;
        m_uiEckBerserkTimer		= urand(60000, 90000);
        m_bBerserk = false;

        if (m_creature->HasAura(SPELL_ECK_RESIDUE_DEBUFF))
             m_creature->RemoveAurasDueToSpell(SPELL_ECK_RESIDUE_DEBUFF);
    }
    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, IN_PROGRESS);
    }
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, FAIL);
    }
 
    void SpellHitTarget (Unit* pUnit, const SpellEntry* pSpellEntry)
    {
       if (pSpellEntry->Id == SPELL_ECK_SPITE && pUnit->GetTypeId() == TYPEID_PLAYER
            && !pUnit->HasAura(SPELL_ECK_RESIDUE_DEBUFF))
            pUnit->CastSpell(pUnit, SPELL_ECK_RESIDUE_DEBUFF, true);
    }
    
    void UpdateAI(const uint32 uiDiff)
    {                                
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_uiEckSpiteTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_ECK_SPITE);             
            m_uiEckSpiteTimer = 15000;
        }
        else 
            m_uiEckSpiteTimer -= uiDiff;

        if (m_uiEckSpringTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                DoCastSpellIfCan(target, SPELL_ECK_SPRING);
                m_creature->DeleteThreatList();
                m_creature->SetInCombatWith(target);
                m_creature->AddThreat(target, 1.0f);
                m_creature->Attack(target, true);
            }
            m_uiEckSpringTimer = 8000;
        }
        else 
            m_uiEckSpringTimer -= uiDiff;

        if (m_uiEckBitTimer < uiDiff)
        {
            if (Unit* target = m_creature->getVictim())
                DoCastSpellIfCan(target, SPELL_ECK_BIT);
            m_uiEckBitTimer = 5000;
        }
        else
            m_uiEckBitTimer -= uiDiff;

        if (!m_bBerserk)
        {
            if (m_creature->GetHealthPercent() <= 20.0f)
            {
                DoScriptText(EMOTE_BERSERK, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_ECK_BERSERK);
                m_bBerserk = true;
            }
        }

        if (!m_bBerserk)
        {
            if (m_uiEckBerserkTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ECK_BERSERK);
                DoScriptText(EMOTE_BERSERK, m_creature);
                m_bBerserk = true;
            }
            else 
                m_uiEckBerserkTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eck(Creature* pCreature)
{
    return new boss_eckAI(pCreature);
}

void AddSC_boss_eck()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_eck";
    pNewScript->GetAI = &GetAI_boss_eck;
    pNewScript->RegisterSelf();
}
