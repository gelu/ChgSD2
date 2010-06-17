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
SD%Complete: 80%
SDComment: by /dev/rsa. Need correct timers && infection logic
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
    SPELL_MUTATED_INFECTION_AURA  = 69674,
    SPELL_MUTATED_INFECTION  = 70003,
    SPELL_BERSERK            = 47008,

    SPELL_STICKY_OOZE        = 69774,
    SPELL_STICKY_AURA        = 69776,
    SPELL_MERGE_OOZE         = 69889,
    SPELL_RADIATING_OOZE     = 69750,
    SPELL_RADIATING_OOZE_1   = 69760,
    SPELL_UNSTABLE_OOZE      = 69644,
    SPELL_UNSTABLE_OOZE_AURA = 69558,
    SPELL_OOZE_EXPLODE       = 69839,
    SPELL_OOZE_EXPLODE_AURA  = 69840,

    NPC_BIG_OOZE             = 36899,
    NPC_SMALL_OOZE           = 36897,
    NPC_STICKY_OOZE          = 37006,
    NPC_OOZE_SPRAY_STALKER   = 37986,
    NPC_OOZE_STALKER         = 37013,
    NPC_OOZE_EXPLODE_STALKER = 38107,

    MAX_INFECTION_TARGETS     = 5,
};

static Locations SpawnLoc[]=
{
    {4471.821289f, 3162.986084f, 360.38501f},  // 0
    {4471.821289f, 3110.452148f, 360.38501f},  // 1
    {4418.825684f, 3110.452148f, 360.38501f},  // 2
    {4418.825684f, 3162.986084f, 360.38501f},  // 3
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
    Unit* InfectionTarget[MAX_INFECTION_TARGETS];
    bool intro;
    bool pet;
    bool nexttick;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_ROTFACE, NOT_STARTED);
        stage = 0;
        intro = false;
        pet = false;
        nexttick = false;
        memset(&InfectionTarget, 0, sizeof(InfectionTarget));
        bsw->resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if(!pInstance || intro) return;
        if (pWho->GetTypeId() != TYPEID_PLAYER) return;

        pInstance->SetData(TYPE_EVENT, 600);
        debug_log("EventMGR: creature %u send signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
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
        DoScriptText(-1631224,m_creature, killer);
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

        if (nexttick)
              {
                  bsw->doCast(SPELL_OOZE_FLOOD_1);
                  DoScriptText(-1631227,m_creature);
                  nexttick = false;
              };

        if (bsw->timedQuery(SPELL_OOZE_FLOOD_1, diff))
              {
                   uint8 i = urand(0,3);
                   if (Unit* pTemp = bsw->doSummon(NPC_OOZE_STALKER,SpawnLoc[i].x, SpawnLoc[i].y, SpawnLoc[i].z))
                   {
                       bsw->doCast(SPELL_OOZE_FLOOD, pTemp);
                       nexttick = true;
                   }
              };

        if (bsw->timedQuery(SPELL_SLIME_SPRAY, diff))
            if (Unit* pTemp = bsw->doSummon(NPC_OOZE_SPRAY_STALKER))
                bsw->doCast(SPELL_SLIME_SPRAY);

        for(uint8 i = 0; i < MAX_INFECTION_TARGETS; ++i)
           if (InfectionTarget[i] && InfectionTarget[i]->isAlive() && InfectionTarget[i]->IsInMap(m_creature))
              if (!bsw->hasAura(SPELL_MUTATED_INFECTION_AURA,InfectionTarget[i]))
                {
                    float fPosX, fPosY, fPosZ;
                    InfectionTarget[i]->GetPosition(fPosX, fPosY, fPosZ);
                    if (Unit* pTemp = bsw->doSummon(NPC_SMALL_OOZE,fPosX, fPosY, fPosZ))
                        pTemp->AddThreat(InfectionTarget[i], 1000.0f);
                    InfectionTarget[i] = NULL;
                };

        if (bsw->timedQuery(SPELL_MUTATED_INFECTION, diff))
            if (Unit* pTarget = bsw->SelectRandomPlayer(SPELL_MUTATED_INFECTION_AURA, false, 60.0f))
                {
                    for(uint8 i = 0; i < MAX_INFECTION_TARGETS; ++i)
                       if (!InfectionTarget[i])
                           {
                              InfectionTarget[i] = pTarget;
                              bsw->doCast(SPELL_MUTATED_INFECTION, InfectionTarget[i]);
                              DoScriptText(-1631226,m_creature);
                              break;
                           }
                       else continue;
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

struct MANGOS_DLL_DECL  mob_small_oozeAI : public ScriptedAI
{
    mob_small_oozeAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;

    void Reset()
    {
        bsw->resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
        bsw->doCast(SPELL_RADIATING_OOZE);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5);
        m_creature->SetSpeedRate(MOVE_WALK, 0.5);
   }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_STICKY_OOZE, uiDiff);

        if (Creature* pTemp = bsw->SelectNearestCreature(m_creature->GetEntry(),7.0f))
           {
            bsw->doCast(SPELL_MERGE_OOZE, pTemp);
            bsw->doSummon(NPC_BIG_OOZE);
            pTemp->ForcedDespawn();
            m_creature->ForcedDespawn();
            };
    }
};

CreatureAI* GetAI_mob_small_ooze(Creature* pCreature)
{
    return new mob_small_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_big_oozeAI : public ScriptedAI
{
    mob_big_oozeAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    bool exploded;

    void Reset()
    {
        bsw->resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
        bsw->doCast(SPELL_UNSTABLE_OOZE);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5);
        m_creature->SetSpeedRate(MOVE_WALK, 0.5);
        exploded = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS)
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        bsw->timedCast(SPELL_STICKY_OOZE, uiDiff);

        if (Creature* pSmall = bsw->SelectNearestCreature(NPC_SMALL_OOZE,5.0f))
            {
                pSmall->ForcedDespawn();
                bsw->doCast(SPELL_UNSTABLE_OOZE);
            };

        if (Creature* pBig = bsw->SelectNearestCreature(NPC_BIG_OOZE, 8.0f))
            {
                pBig->ForcedDespawn();
                bsw->doCast(SPELL_UNSTABLE_OOZE);
            }

        if ( bsw->auraCount(SPELL_UNSTABLE_OOZE_AURA) > 4 && !exploded)
            {
                bsw->doCast(SPELL_OOZE_EXPLODE);
                exploded = true;
            }

    }
};

CreatureAI* GetAI_mob_big_ooze(Creature* pCreature)
{
    return new mob_big_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_sticky_oozeAI : public ScriptedAI
{
    mob_sticky_oozeAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->CastSpell(m_creature, SPELL_STICKY_AURA, true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetInCombatWithZone();
        SetCombatMovement(false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

    }
};

CreatureAI* GetAI_mob_sticky_ooze(Creature* pCreature)
{
    return new mob_sticky_oozeAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_ooze_stalkerAI : public ScriptedAI
{
    mob_ooze_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetInCombatWithZone();
        SetCombatMovement(false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

    }
};

CreatureAI* GetAI_mob_ooze_stalker(Creature* pCreature)
{
    return new mob_ooze_stalkerAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_ooze_explode_stalkerAI : public ScriptedAI
{
    mob_ooze_explode_stalkerAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    Creature* creator;
    BossSpellWorker* bsw;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
        bsw->doCast(SPELL_OOZE_EXPLODE_AURA);
        creator = bsw->SelectNearestCreature(NPC_BIG_OOZE, 20.0f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_ROTFACE) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (creator && creator->isAlive())
              creator->ForcedDespawn();

        if (bsw->timedQuery(SPELL_OOZE_EXPLODE_AURA, uiDiff))
              m_creature->ForcedDespawn();

    }
};

CreatureAI* GetAI_mob_ooze_explode_stalker(Creature* pCreature)
{
    return new mob_ooze_explode_stalkerAI(pCreature);
}

void AddSC_boss_rotface()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_rotface";
    newscript->GetAI = &GetAI_boss_rotface;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_small_ooze";
    newscript->GetAI = &GetAI_mob_small_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_big_ooze";
    newscript->GetAI = &GetAI_mob_big_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sticky_ooze";
    newscript->GetAI = &GetAI_mob_sticky_ooze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ooze_stalker";
    newscript->GetAI = &GetAI_mob_ooze_stalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ooze_explode_stalker";
    newscript->GetAI = &GetAI_mob_ooze_explode_stalker;
    newscript->RegisterSelf();
}
