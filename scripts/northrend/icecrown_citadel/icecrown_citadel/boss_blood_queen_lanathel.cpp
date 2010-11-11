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
SD%Complete: 70%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */
// Need correct work of spells and timers
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

        NPC_SWARMING_SHADOWS                    = 38163,
        SPELL_SWARMING_SHADOWS_VISUAL           = 71267,
        QUEST_24756                             = 72934,
};

static Locations SpawnLoc[]=
{
    {4595.640137f, 2769.195557f, 400.137054f},  // 0 Phased
    {4595.904785f, 2769.315918f, 421.838623f},  // 1 Fly
};


struct MANGOS_DLL_DECL boss_blood_queen_lanathelAI : public BSWScriptedAI
{
    boss_blood_queen_lanathelAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
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
        if (m_creature->isAlive()) pInstance->SetData(TYPE_LANATHEL, NOT_STARTED);
        stage = 0;
        UpdateTimer = 1000;
        bloodbolts = 0;
        movementstarted = false;
        MirrorMarked = NULL;
        MirrorTarget = NULL;
        resetTimers();
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
           doRemove(SPELL_BLOOD_MIRROR_1,pVictim);

        if (pVictim && pVictim->HasAura(SPELL_BLOOD_MIRROR_2))
           doRemove(SPELL_BLOOD_MIRROR_2,pVictim);
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

        doCast(SPELL_SHROUD_OF_SORROW);

        DoScriptText(-1631321,m_creature,who);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        doBloodMirror(false);
        pInstance->SetData(TYPE_LANATHEL, DONE);
        DoScriptText(-1631333,m_creature,killer);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        for (uint8 i = 0; i < 5; ++i)
        {
             if (Unit* pPlayer = doSelectRandomPlayer(SPELL_ESSENCE_OF_BLOOD_QWEEN, true, 100.0f))
             {
                 doCast(QUEST_24756, pPlayer);
                 doRemove(SPELL_ESSENCE_OF_BLOOD_QWEEN, pPlayer);
             }
        }
    }

    void doPactOfDarkfallen(bool command)
    {
       if (command)
       {
          uint8 num = urand(3,5);
          for(uint8 i = 0; i <= num; ++i)
              if (Unit* pTarget = doSelectRandomPlayer(SPELL_PACT_OF_DARKFALLEN, false, 60.0f))
              {
                  if (doCast(SPELL_PACT_OF_DARKFALLEN,pTarget) == CAST_OK)
                  {
                      Darkfallen[i] = pTarget;
                      ++darkfallened;
                  };
              };
       }
       else if (darkfallened > 0)
       {
          for(uint8 i = 0; i < darkfallened; ++i)
              if (Darkfallen[i])
              {
                 if (hasAura(SPELL_PACT_OF_DARKFALLEN,Darkfallen[i]))
                    {
                    for(uint8 j = 0; j < darkfallened; ++j)
                       if (j != i && Darkfallen[j])
                       {
                          if(Darkfallen[j])
                          {
                             if (hasAura(SPELL_PACT_OF_DARKFALLEN,Darkfallen[j]))
                                {
                                    if (!Darkfallen[j]->IsWithinDistInMap(Darkfallen[i], 5.0f)) return;
                                } else Darkfallen[j] = NULL;
                          }
                       }
                    } else Darkfallen[i] = NULL;
              }
          for(uint8 i = 0; i < darkfallened; ++i)
                 if (hasAura(SPELL_PACT_OF_DARKFALLEN,Darkfallen[i]))
                       doRemove(SPELL_PACT_OF_DARKFALLEN, Darkfallen[i]);
          darkfallened = 0;
       };
    }

    void doBloodMirror(bool command)
    {
        if (command)
        {
        if (MirrorMarked)
            if (!hasAura(SPELL_BLOOD_MIRROR_1,MirrorMarked))
               MirrorMarked = NULL;

        if (MirrorTarget)
            if (!hasAura(SPELL_BLOOD_MIRROR_2,MirrorTarget))
               MirrorTarget = NULL;

        if (!MirrorMarked && m_creature->getVictim())
           {
               MirrorMarked = m_creature->getVictim();
               if (MirrorMarked)
                  doCast(SPELL_BLOOD_MIRROR_1, MirrorMarked);
           }

        if (!MirrorTarget)
           {
              MirrorTarget = doSelectRandomPlayer(SPELL_BLOOD_MIRROR_1, false, 40.0f);
              if (MirrorTarget)
                   doCast(SPELL_BLOOD_MIRROR_2, MirrorTarget);
           }
        } else
        {
        if (MirrorMarked)
            if (hasAura(SPELL_BLOOD_MIRROR_1,MirrorMarked))
            {
               doRemove(SPELL_BLOOD_MIRROR_1, MirrorMarked);
               MirrorMarked = NULL;
            }

        if (MirrorTarget)
            if (hasAura(SPELL_BLOOD_MIRROR_2,MirrorTarget))
            {
               doRemove(SPELL_BLOOD_MIRROR_2, MirrorTarget);
               MirrorTarget = NULL;
            }
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

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 

                    timedCast(SPELL_SWARMING_SHADOWS, diff);

                    if (timedQuery(SPELL_TWILIGHT_BLOODBOLT, diff)) bloodbolts = 1;

                    if (timedQuery(SPELL_DELRIOUS_SLASH, diff))
                        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                            doCast(SPELL_DELRIOUS_SLASH, pTarget);

                    if (timedQuery(SPELL_PACT_OF_DARKFALLEN, diff))
                        doPactOfDarkfallen(true);

                    timedCast(SPELL_SWARMING_SHADOWS, diff);

                    if (timedQuery(SPELL_VAMPIRIC_BITE,diff))
                        {
                           switch (urand(0,1)) {
                               case 0:
                                      DoScriptText(-1631322,m_creature);
                                      break;
                               case 1:
                                      DoScriptText(-1631323,m_creature);
                                      break;
                               }
                           doCast(SPELL_VAMPIRIC_BITE);
                        }

                    if (timedQuery(SPELL_BLOODBOLT_WHIRL,diff) && m_creature->GetHealthPercent() > 10.0f)
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
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                    m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
                    doBloodMirror(false);
                    stage = 2;
                    break;
            case 2:
                    if (movementstarted) return;
                    DoScriptText(-1631327,m_creature);
                    doCast(SPELL_BLOODBOLT_WHIRL);
                    stage = 3;
                    return;
            case 3:
                    if (m_creature->IsNonMeleeSpellCasted(false))
                        return;
                    if (timedQuery(SPELL_TWILIGHT_BLOODBOLT,diff) || m_creature->GetHealthPercent() < 10.0f)
                    {
                        stage = 4;
//                        DoScriptText(-1631325,m_creature);
                        bloodbolts = 3;
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
            doCast(SPELL_TWILIGHT_BLOODBOLT);
            --bloodbolts;
        };

        if (timedQuery(SPELL_BERSERK, diff))
        {
             doCast(SPELL_BERSERK);
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
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_lifetimer;

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false); 
        m_creature->SetInCombatWithZone();
        m_lifetimer = 10000;
        DoCast(m_creature, SPELL_SWARMING_SHADOWS_VISUAL);
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
