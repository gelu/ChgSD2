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
SDName: boss_blood_queen_lanathel
SD%Complete: 20%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
        SPELL_BERSERK                           = 47008,
        SPELL_SHROUD_OF_SORROW                  = 72981,
        SPELL_DELRIOUS_SLASH                    = 71623,
        SPELL_BLOOD_MIRROR_1                    = 70821,
        SPELL_BLOOD_MIRROR_2                    = 71510,
        SPELL_VAMPIRIC_BITE                     = 71726,
        SPELL_ESSENCE_OF_BLOOD_QWEEN            = 70867,
        SPELL_FRENZIED_BLOODTHIRST              = 70877,
        SPELL_UNCONTROLLABLE_FRENZY             = 70923,
        SPELL_PACT_OF_DARKFALLEN                = 71340,
        SPELL_PACT_OF_DARKFALLEN_1              = 71341,
        SPELL_SWARMING_SHADOWS                  = 71264,
        SPELL_TWILIGHT_BLOODBOLT                = 71446,
        SPELL_BLOODBOLT_WHIRL                   = 71772,
        SPELL_PRESENCE_OF_DARKFALLEN            = 71952,

        SPELL_FLY_VISUAL                        = 57764,

        NPC_SWARMING_SHADOWS                    = 38163,
        SPELL_SWARMING_SHADOWS_VISUAL           = 71267,
};

static Locations SpawnLoc[]=
{
    {4595.640137f, 2769.195557f, 400.137054f},  // 0 Phased
    {4595.904785f, 2769.315918f, 421.838623f},  // 1 Fly
};


struct MANGOS_DLL_DECL boss_blood_queen_lanathelAI : public ScriptedAI
{
    boss_blood_queen_lanathelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    uint8 nextPoint;
    uint8 bloodbolts;
    uint32 UpdateTimer;
    bool movementstarted;
    Unit* MirrorMarked;
    Unit* MirrorTarget;
    Unit* Darkfallen[5];
    uint8 darkfallened;
    uint32 MirrorDamage;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LANATHEL, NOT_STARTED);
        stage = 0;
        UpdateTimer = 1000;
        bloodbolts = 0;
        movementstarted = false;
        MirrorMarked = NULL;
        MirrorTarget = NULL;
        bsw->resetTimers();
        memset(&Darkfallen, 0, sizeof(Darkfallen));
        darkfallened = 0;
        MirrorDamage = 0;
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_LANATHEL, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631330,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631331,m_creature,pVictim);
               break;
        }

        if (pVictim && pVictim->HasAura(SPELL_BLOOD_MIRROR_1))
           bsw->doRemove(SPELL_BLOOD_MIRROR_1,pVictim);

        if (pVictim && pVictim->HasAura(SPELL_BLOOD_MIRROR_2))
           bsw->doRemove(SPELL_BLOOD_MIRROR_2,pVictim);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE || !movementstarted) return;
        if (id == nextPoint) 
        {
            movementstarted = false;
            m_creature->GetMotionMaster()->MovementExpired();
        }
    }

    void StartMovement(uint32 id)
    {
        nextPoint = id;
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
        movementstarted = true;
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return; 
        pInstance->SetData(TYPE_LANATHEL, IN_PROGRESS);

        bsw->doCast(SPELL_SHROUD_OF_SORROW);

        DoScriptText(-1631321,m_creature,who);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LANATHEL, DONE);
        DoScriptText(-1631333,m_creature,killer);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
    }

    void doPactOfDarkfallen(bool command)
    {
       if (command)
       {
          uint8 num = urand(2,5);
          for(uint8 i = 0; i <= num; ++i)
              if (Unit* pTarget = bsw->SelectRandomPlayer(SPELL_PACT_OF_DARKFALLEN, false, 100.0f))
              {
                  Darkfallen[i] = pTarget;
                  bsw->doCast(SPELL_PACT_OF_DARKFALLEN,pTarget);
                  ++darkfallened;
              };
       }
       else if (darkfallened > 0)
       {
          for(uint8 i = 0; i < darkfallened; ++i)
              if (Darkfallen[i]) 
                 if (Darkfallen[i]->isAlive() && Darkfallen[i]->HasAura(SPELL_PACT_OF_DARKFALLEN))
                   for(uint8 j = 0; j < darkfallened; ++j)
                      if (j != i && Darkfallen[j])
                        if(Darkfallen[j]->isAlive() && Darkfallen[j]->HasAura(SPELL_PACT_OF_DARKFALLEN))
                           if (!Darkfallen[j]->IsWithinDistInMap(Darkfallen[i], 5.0f)) return;

          for(uint8 i = 0; i < darkfallened; ++i)
                  if (Darkfallen[i] && Darkfallen[i]->isAlive() && Darkfallen[i]->HasAura(SPELL_PACT_OF_DARKFALLEN))
                       bsw->doRemove(SPELL_PACT_OF_DARKFALLEN, Darkfallen[i]);
          darkfallened = 0;
       };
    }

    void doBloodMirror(bool command)
    {

        if (MirrorMarked)
            if (!MirrorMarked->isAlive() || !MirrorMarked->HasAura(SPELL_BLOOD_MIRROR_1))
               MirrorMarked = NULL;

        if (MirrorTarget)
            if (!MirrorTarget->isAlive() || !MirrorTarget->HasAura(SPELL_BLOOD_MIRROR_2))
               MirrorTarget = NULL;

        if (!MirrorMarked && m_creature->getVictim()) 
           {
               if (MirrorMarked = m_creature->getVictim())
                  bsw->doCast(SPELL_BLOOD_MIRROR_1, MirrorMarked);
           }

        if (!MirrorTarget)
           {
               if (MirrorTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                   bsw->doCast(SPELL_BLOOD_MIRROR_2, MirrorTarget);
           }

    }

    void doMirrorDamage()
    {
        uint32 tempdamage = MirrorDamage;

        if (MirrorTarget)
           if (MirrorTarget->isAlive())
               m_creature->DealDamage(MirrorTarget, tempdamage, NULL, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_SHADOW, NULL, false);
        MirrorDamage -= tempdamage;
    }

    void DamageDeal(Unit* target, uint32 &damage) 
    {
        if (target)
           if (MirrorMarked)
             if (MirrorMarked->isAlive())
                if (target == MirrorMarked)
                   MirrorDamage += damage;
    }

    void UpdateAI(const uint32 diff)
    {
        if (pInstance->GetData(TYPE_EVENT_NPC) == NPC_LANATHEL)
        {
            UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
            if (UpdateTimer <= diff)
            {
            debug_log("EventMGR: creature %u received signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
            switch (pInstance->GetData(TYPE_EVENT))
                {
                case 800:
                          DoScriptText(-1631301, m_creature);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,810);
                          break;
                default:
                          break;
                }
             } else UpdateTimer -= diff;
             pInstance->SetData(TYPE_EVENT_TIMER, UpdateTimer);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 

                    bsw->timedCast(SPELL_SWARMING_SHADOWS, diff);

                    if (bsw->timedQuery(SPELL_TWILIGHT_BLOODBOLT, diff)) bloodbolts = 1;

                    if (bsw->timedQuery(SPELL_DELRIOUS_SLASH, diff))
                        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                            bsw->doCast(SPELL_DELRIOUS_SLASH, pTarget);

                    if (bsw->timedQuery(SPELL_PACT_OF_DARKFALLEN, diff))
                        doPactOfDarkfallen(true);

                    if (bsw->timedQuery(SPELL_VAMPIRIC_BITE,diff))
                        {
                           switch (urand(0,1)) {
                               case 0:
                                      DoScriptText(-1631322,m_creature);
                                      break;
                               case 1:
                                      DoScriptText(-1631323,m_creature);
                                      break;
                               }
                           bsw->doCast(SPELL_VAMPIRIC_BITE);
                        }

                    if (bsw->timedQuery(SPELL_BLOODBOLT_WHIRL,diff))
                        {
                            stage = 1;
                        };

                    doBloodMirror(true);

                    DoMeleeAttackIfReady();

                    break;
            case 1:             // Go in fly phase
                    m_creature->AttackStop();
                    SetCombatMovement(false);
                    StartMovement(1);
                    bsw->doCast(SPELL_FLY_VISUAL);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
                    stage = 2;
                    break;
            case 2:
                    if (movementstarted) return;
                    DoScriptText(-1631327,m_creature);
                    stage = 3;
                    bsw->doCast(SPELL_BLOODBOLT_WHIRL);
                    return;
            case 3:
                    if (bsw->timedQuery(SPELL_TWILIGHT_BLOODBOLT, diff)) bloodbolts = 5;

                    if (bsw->timedQuery(SPELL_PACT_OF_DARKFALLEN, diff))
                        doPactOfDarkfallen(true);

                    bsw->timedCast(SPELL_SWARMING_SHADOWS, diff);

                    if (bsw->timedQuery(SPELL_BLOODBOLT_WHIRL,diff))
                        {
                            stage = 4;
                            DoScriptText(-1631325,m_creature);
                        };
                    break;
            case 4:             // Go in grownd phase
                    m_creature->AttackStop();
                    StartMovement(0);
                    stage = 5;
                    break;
            case 5:
                    if (movementstarted) return;
                    DoScriptText(-1631325,m_creature);
                    bsw->doRemove(SPELL_FLY_VISUAL);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                    m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
                    stage = 0;
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    return;
            default:
                    break;
        }

        doPactOfDarkfallen(false);

        doMirrorDamage();

        if (bloodbolts > 0)
            {
                bsw->doCast(SPELL_TWILIGHT_BLOODBOLT);
                --bloodbolts;
            };

        if (bsw->timedQuery(SPELL_BERSERK, diff)){
                 bsw->doCast(SPELL_BERSERK);
                 DoScriptText(-1631332,m_creature);
                 };

    }
};


CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}

struct MANGOS_DLL_DECL mob_swarming_shadowsAI : public ScriptedAI
{
    mob_swarming_shadowsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_lifetimer;

    void Reset()
    {
        SetCombatMovement(false); 
        m_creature->SetDisplayId(11686);
        m_creature->GetMotionMaster()->MoveRandom();
        m_lifetimer = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LANATHEL) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->HasAura(SPELL_SWARMING_SHADOWS_VISUAL))
              DoCast(m_creature, SPELL_SWARMING_SHADOWS_VISUAL);

        if (m_lifetimer <= uiDiff)
            m_creature->ForcedDespawn();
        else m_lifetimer -= uiDiff;

    }
};

CreatureAI* GetAI_mob_swarming_shadows(Creature* pCreature)
{
     return new mob_swarming_shadowsAI (pCreature);
}


void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarming_shadows";
    newscript->GetAI = &GetAI_mob_swarming_shadows;
    newscript->RegisterSelf();
}
