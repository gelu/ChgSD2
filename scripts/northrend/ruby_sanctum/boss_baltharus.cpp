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
SD%Complete: 20%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

//TODO: sql spells, sql npcs, TEST

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum BossSpells
{
    SPELL_TWILIGHT_PRECISION         = 78243,
    SPELL_BLADE_TEMPEST              = 75125, //every 22 secs
    SPELL_ENERVATING_BRAND           = 74502, //friendlys in 12yards = 74505
    SPELL_REPELLING_WAVE             = 74509, //every 10-15 secs
    SPELL_SUMMON_CLONE               = 74511, //summons npc 39899 (Clone)
};

/*######
## boss_baltharus
######*/

struct MANGOS_DLL_DECL boss_baltharusAI : public BSWScriptedAI
{
    boss_baltharusAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 clone;

    void Reset()
    {
        if(!pInstance)
            return;

        pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
        resetTimers();
        clone = 0;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        pInstance->SetData(TYPE_BALTHARUS, FAIL);
        pInstance->SetData(DATA_HEALTH_BALTHARUS, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;

        DoScriptText(-1713547,m_creature);

        if (Creature* pClone = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_CLONE)))
            if (!pClone->isAlive())
                pInstance->SetData(TYPE_BALTHARUS, DONE);
        else
            pInstance->SetData(TYPE_BALTHARUS, SPECIAL);
            pInstance->SetData(DATA_HEALTH_BALTHARUS, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
        DoScriptText(-1666300,m_creature);
        pInstance->SetData(DATA_HEALTH_BALTHARUS, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance) return;

        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
          return;

        pInstance->SetData(DATA_HEALTH_BALTHARUS, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance) return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_CLONE) && pInstance->GetData(DATA_HEALTH_CLONE) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_CLONE));

        timedCast(SPELL_TWILIGHT_PRECISION, uiDiff);
        timedCast(SPELL_BLADE_TEMPEST, uiDiff);
        timedCast(SPELL_ENERVATING_BRAND, uiDiff);
        //CLONE
        //10 man = 50%
        //25 man = 66% & 33%
        if ( m_creature->GetHealthPercent() <= 50.0f && clone == 0)
        {
           DoScriptText(-1666303,m_creature);
           timedCast(SPELL_REPELLING_WAVE, uiDiff);
           timedCast(SPELL_SUMMON_CLONE, uiDiff);
           clone = 1;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_baltharus(Creature* pCreature)
{
    return new boss_baltharusAI(pCreature);
}

/*######
## mob_baltharus_clone
######*/

struct MANGOS_DLL_DECL mob_baltharus_cloneAI : public BSWScriptedAI
{
    mob_baltharus_cloneAI(Creature* pCreature) : BSWScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        if(!pInstance)
            return;

        pInstance->SetData(TYPE_BALTHARUS, NOT_STARTED);
        resetTimers();
        m_creature->ForcedDespawn();
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
               DoScriptText(-1666301,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666302,m_creature,pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (!pInstance) return;

        pInstance->SetData(TYPE_BALTHARUS, FAIL);
        pInstance->SetData(DATA_HEALTH_CLONE, m_creature->GetMaxHealth());
        m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance) return;

        if (Creature* pClone = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_BALTHARUS)))
            if (!pClone->isAlive())
                pInstance->SetData(TYPE_BALTHARUS, DONE);
        else
            pInstance->SetData(TYPE_BALTHARUS, SPECIAL);

        pInstance->SetData(DATA_HEALTH_CLONE, 0);
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance) return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_BALTHARUS, IN_PROGRESS);
        pInstance->SetData(DATA_HEALTH_CLONE, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance) return;

        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
            return;

        pInstance->SetData(DATA_HEALTH_CLONE, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_BALTHARUS) && pInstance->GetData(DATA_HEALTH_BALTHARUS) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_BALTHARUS));

        timedCast(SPELL_BLADE_TEMPEST, uiDiff);
        timedCast(SPELL_ENERVATING_BRAND, uiDiff);
    }
};

CreatureAI* GetAI_mob_baltharus_clone(Creature* pCreature)
{
    return new mob_baltharus_cloneAI(pCreature);
}

void AddSC_boss_baltharus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baltharus";
    newscript->GetAI = &GetAI_boss_baltharus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_baltharus_clone";
    newscript->GetAI = &GetAI_mob_baltharus_clone;
    newscript->RegisterSelf();
}
