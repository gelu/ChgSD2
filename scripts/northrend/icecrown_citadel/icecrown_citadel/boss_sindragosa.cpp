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
// Sindragosa
    SPELL_FROST_AURA_1       = 70084,
    SPELL_CLEAVE_1           = 19983,
    SPELL_TAIL_SMASH         = 71077,
    SPELL_FROST_BREATH_1     = 69649,
    SPELL_PERMEATING_CHILL   = 70107,
    SPELL_UNCHAINED_MAGIC    = 69762,
    SPELL_INSTABILITY        = 69766,
    SPELL_ICY_GRIP           = 70117,
    SPELL_BLISTERING_COLD    = 70123,

    SPELL_FROST_BEACON       = 70126,
    SPELL_ICY_TOMB           = 70157,
    SPELL_ASPHYXATION        = 71665,
    SPELL_FROST_BOMB         = 71053,

    SPELL_MYSTIC_BUFFET      = 70128,

    NPC_ICE_TOMB             = 36980,

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

struct MANGOS_DLL_DECL boss_sindragosaAI : public ScriptedAI
{
    boss_sindragosaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    Unit* pTarget1;
    Unit* pTarget2;

    void Reset()
    {
        if(!pInstance) return;
        bsw->resetTimers();
        stage = 0;
        m_creature->SetRespawnDelay(7*DAY);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631421,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-16311422,m_creature,pVictim);
               break;
        }
    }

    void JustReachedHome()
    {
        if (!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, FAIL);
        if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_RIMEFANG)))
            pTemp->Respawn();
        if (Creature* pTemp = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_SPINESTALKER)))
            pTemp->Respawn();
        DoScriptText(-1631422,m_creature);
        m_creature->ForcedDespawn();
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return;
           DoScriptText(-16311420,m_creature,who);
        bsw->doCast(SPELL_FROST_AURA_1);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SINDRAGOSA, DONE);
        DoScriptText(-16311423,m_creature,killer);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                    bsw->timedCast(SPELL_CLEAVE_1, diff);
                    bsw->timedCast(SPELL_TAIL_SMASH, diff);
                    bsw->timedCast(SPELL_FROST_BREATH, diff);
                    bsw->timedCast(SPELL_PERMEATING_CHILL, diff);

                    bsw->timedCast(SPELL_UNCHAINED_MAGIC, diff);
                    if (bsw->timedQuery(SPELL_ICY_GRIP, diff))
                         {
                         bsw->doCast(SPELL_ICY_GRIP);
                         bsw->doCast(SPELL_BLISTERING_COLD);
                         DoScriptText(-16311426,m_creature);
                         }

                    if (bsw->timedQuery(SPELL_FROST_BEACON, diff) && m_creature->GetHealthPercent() < 85.0f) stage = 1;
            break;
            case 1: 
                    DoScriptText(-1631425,m_creature);
                    bsw->doCast(SPELL_FROST_BEACON);
                    stage = 2;
            break;
            case 2: 
                    bsw->timedCast(SPELL_ICY_TOMB, diff);
                    bsw->timedCast(SPELL_FROST_BOMB, diff);
                    if (bsw->timedQuery(SPELL_FROST_BEACON, diff)) stage = 0;
            break;
            case 3: 
                    bsw->timedCast(SPELL_CLEAVE_1, diff);
                    bsw->timedCast(SPELL_TAIL_SMASH, diff);
                    bsw->timedCast(SPELL_FROST_BREATH, diff);
                    bsw->timedCast(SPELL_PERMEATING_CHILL, diff);
                    bsw->timedCast(SPELL_UNCHAINED_MAGIC, diff);
                    bsw->timedCast(SPELL_MYSTIC_BUFFET, diff);
                    if (bsw->timedQuery(SPELL_ICY_GRIP, diff))
                         {
                         bsw->doCast(SPELL_ICY_GRIP);
                         bsw->doCast(SPELL_BLISTERING_COLD);
                         DoScriptText(-16311426,m_creature);
                         }
            break;
            default: break;
        }

        if (m_creature->GetHealthPercent() < 35.0f) 
               {
                stage = 3;
                DoScriptText(-1631429,m_creature);
               }


        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_sindragosa(Creature* pCreature)
{
    return new boss_sindragosaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_ice_tombAI : public ScriptedAI
{
    mob_ice_tombAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    Unit* pVictim;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
        pVictim = NULL;
    }

    void Aggro(Unit* pWho)
    {
        if (!pVictim && pWho)  {
                        pVictim = pWho;
                        m_creature->SetInCombatWith(pVictim);
                        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_ICY_TOMB);
    }

    void KilledUnit(Unit* _Victim)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_ICY_TOMB);
    }

    void JustDied(Unit* Killer)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_ICY_TOMB);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_SINDRAGOSA) != IN_PROGRESS)
        {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_ICY_TOMB);
            m_creature->ForcedDespawn();
        }

    }

};

CreatureAI* GetAI_mob_ice_tomb(Creature* pCreature)
{
    return new mob_ice_tombAI(pCreature);
}

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
    newscript->Name = "boss_sindragosa";
    newscript->GetAI = &GetAI_boss_sindragosa;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_rimefang";
    newscript->GetAI = &GetAI_mob_rimefang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spinestalker";
    newscript->GetAI = &GetAI_mob_spinestalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ice_tomb";
    newscript->GetAI = &GetAI_mob_ice_tomb;
    newscript->RegisterSelf();

}
