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
SDName: boss_sindragosa
SD%Complete: 0%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
// Rimefang
    SPELL_FROST_AURA         = 71387,
    SPELL_FROST_BREATH       = 71386,
    SPELL_ICY_BLAST          = 71376,
// Spinestalker
    SPELL_BELLOWING_ROAR     = 36922,
    SPELL_CLEAVE             = 40505,
    SPELL_TAIL_SWEEP         = 71369,
};

static Locations SpawnLoc[]=
{
    {4408.052734f, 2484.825439f, 203.374207f},  // 0 Sindragosa spawn
};

struct MANGOS_DLL_DECL mob_rimefangAI : public ScriptedAI
{
    mob_rimefangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    Creature* pBrother;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);
        pBrother = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_SPINESTALKER));
        if (pBrother && !pBrother->isAlive()) pBrother->Respawn();
        if (pBrother) pBrother->SetInCombatWithZone();
        bsw->doCast(SPELL_FROST_AURA);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        if (pBrother && !pBrother->isAlive())
                 m_creature->SummonCreature(NPC_SINDRAGOSA, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 3.17f, TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_FROST_BREATH, diff);

        bsw->timedCast(SPELL_ICY_BLAST, diff);

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_mob_rimefang(Creature* pCreature)
{
    return new mob_rimefangAI(pCreature);
}

struct MANGOS_DLL_DECL mob_spinestalkerAI : public ScriptedAI
{
    mob_spinestalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    Creature* pBrother;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, IN_PROGRESS);
        pBrother = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_RIMEFANG));
        if (pBrother && !pBrother->isAlive()) pBrother->Respawn();
        if (pBrother) pBrother->SetInCombatWithZone();
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        if (pBrother && !pBrother->isAlive())
                 m_creature->SummonCreature(NPC_SINDRAGOSA, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 3.17f, TEMPSUMMON_MANUAL_DESPAWN, DESPAWN_TIME);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_BELLOWING_ROAR, diff);

        bsw->timedCast(SPELL_CLEAVE, diff);

        bsw->timedCast(SPELL_TAIL_SWEEP, diff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_spinestalker(Creature* pCreature)
{
    return new mob_spinestalkerAI(pCreature);
}

void AddSC_boss_sindragosa()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_rimefang";
    newscript->GetAI = &GetAI_mob_rimefang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spinestalker";
    newscript->GetAI = &GetAI_mob_spinestalker;
    newscript->RegisterSelf();

}
