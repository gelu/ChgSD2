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
SDName: boss_lady_deathwhisper
SD%Complete: 20%
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
        NPC_VENGEFUL_SHADE                      = 38222,
        NPC_FANATIC                             = 37890,
        NPC_REANIMATED_FANATIC                  = 38009,
        NPC_ADHERENT                            = 37949,
        NPC_REANIMATED_ADHERENT                 = 38010,
        //Abilities
        SPELL_MANA_BARRIER                      = 70842,
        SPELL_SHADOW_BOLT                       = 71254,
        SPELL_DEATH_AND_DECAY                   = 71001,
        SPELL_DARK_EMPOWERMENT                  = 70901,
        SPELL_FROSTBOLT                         = 71420,
        SPELL_INSIGNIFICANCE                    = 71204,

        SPELL_DOMINATE_MIND                     = 71289,

        SPELL_VENGEFUL_BLAST                    = 71494,
        SPELL_VENGEFUL_BLAST_0                  = 71544,
};

static Locations SpawnLoc[]=
{
    {-623.055481f, 2211.326660f, 51.764259f},  // 0 Lady's stay point
    {-620.197449f, 2272.062256f, 50.848679f},  // 1 Right Door 1
    {-598.636353f, 2272.062256f, 50.848679f},  // 2 Right Door 2
    {-578.495728f, 2272.062256f, 50.848679f},  // 3 Right Door 3
    {-578.495728f, 2149.211182f, 50.848679f},  // 4 Left Door 1
    {-598.636353f, 2149.211182f, 50.848679f},  // 5 Left Door 2
    {-620.197449f, 2149.211182f, 50.848679f},  // 6 Left Door 3
    {-517.652466f, 2216.611328f, 62.823681f},  // 7 Upper marsh 1
    {-517.652466f, 2211.611328f, 62.823681f},  // 8 Upper marsh 2
    {-517.652466f, 2206.611328f, 62.823681f},  // 9 Upper marsh 3
};

struct MANGOS_DLL_DECL boss_lady_deathwhisperAI : public ScriptedAI
{
    boss_lady_deathwhisperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    BossSpellWorker* bsw;
    ScriptedInstance *pInstance;
    uint8 stage;
    uint8 Difficulty;
    bool MovementStarted;
    bool intro;

    void Reset()
    {
        if(!pInstance) return;
        Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
        pInstance->SetData(TYPE_DEATHWHISPER, NOT_STARTED);
        stage = 0;
        MovementStarted = false;
        intro = false;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (stage) return;
        else intro = true;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631029,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631030,m_creature,pVictim);
               break;
        };
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
        if(MovementStarted && id != 1)
        {
             m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
        }
        else    {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                SetCombatMovement(false);
                }
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, IN_PROGRESS);
        bsw->doCast(SPELL_MANA_BARRIER );
        MovementStarted = true;
        SetCombatMovement(false);
        DoScriptText(-1631023,m_creature);
        m_creature->GetMotionMaster()->MovePoint(1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, DONE);
        DoScriptText(-1631032,m_creature,killer);
    }

    void CallGuard(uint8 place)
    {
    if (Unit* pTemp = bsw->doSummon(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+1].x, SpawnLoc[3*place+1].y, SpawnLoc[3*place+1].z))
    if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                pTemp->AddThreat(pTarget, 100.0f);
                pTemp->GetMotionMaster()->MoveChase(pTarget);
                };
    if (Unit* pTemp = bsw->doSummon(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+2].x, SpawnLoc[3*place+2].y, SpawnLoc[3*place+2].z))
    if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                pTemp->AddThreat(pTarget, 100.0f);
                pTemp->GetMotionMaster()->MoveChase(pTarget);
                };
    if (Unit* pTemp = bsw->doSummon(urand(0,1) ? NPC_FANATIC : NPC_ADHERENT, SpawnLoc[3*place+3].x, SpawnLoc[3*place+3].y, SpawnLoc[3*place+3].z))
    if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                pTemp->AddThreat(pTarget, 100.0f);
                pTemp->GetMotionMaster()->MoveChase(pTarget);
                };
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;

        if (bsw->hasAura(SPELL_MANA_BARRIER, m_creature)) {
            if (m_creature->GetPower(POWER_MANA) > uiDamage) {
                     m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-uiDamage);
                     uiDamage = 0;
                     }
                else {
                     m_creature->SetPower(POWER_MANA,0);
                     bsw->doRemove(SPELL_MANA_BARRIER);
                     };
            } else return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (intro && bsw->timedQuery(SPELL_SHADOW_BOLT,diff)) 
            switch (stage) {
                                   case 0:
                                          DoScriptText(-1631020,m_creature);
                                          stage = 1;
                                          break;
                                   case 1:
                                          DoScriptText(-1631021,m_creature);
                                          stage = 2;
                                          break;
                                   case 2:
                                          DoScriptText(-1631022,m_creature);
                                          stage = 3;
                                          break;
                                   default:
                                          break;
                                   }

        if (bsw->hasAura(SPELL_MANA_BARRIER, m_creature)) {
             if(m_creature->GetHealth() <= m_creature->GetMaxHealth()) {
                  if (m_creature->GetPower(POWER_MANA) > (m_creature->GetMaxHealth() - m_creature->GetHealth()))
                        {
                         m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-(m_creature->GetMaxHealth() - m_creature->GetHealth()));
                         m_creature->SetHealth(m_creature->GetMaxHealth());
                        }
                        else m_creature->SetPower(POWER_MANA,0);
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (MovementStarted) return;

        switch(stage)
        {
            case 3: {
                    if (IsCombatMovement())
                        SetCombatMovement(false);

                    bsw->timedCast(SPELL_SHADOW_BOLT,diff);

                    if (bsw->timedQuery(NPC_FANATIC, diff))
                        {
                        DoScriptText(-1631028,m_creature);
                        switch (Difficulty) {
                             case RAID_DIFFICULTY_10MAN_NORMAL:
                                       CallGuard(urand(0,1));
                                       break;
                             case RAID_DIFFICULTY_10MAN_HEROIC:
                                       CallGuard(urand(0,1));
                                       if (urand(0,1)) CallGuard(2);
                                       break;
                             case RAID_DIFFICULTY_25MAN_NORMAL:
                                       CallGuard(0);
                                       CallGuard(1);
                                       if (urand(0,1)) CallGuard(2);
                                       break;
                             case RAID_DIFFICULTY_25MAN_HEROIC:
                                       CallGuard(0);
                                       CallGuard(1);
                                       CallGuard(2);
                                       break;
                             default:
                                       break;

                                            }
                        }

                    if (bsw->timedQuery(SPELL_DARK_EMPOWERMENT ,diff))
                    { 
                    switch (urand(0,1)) {
                            case 0:
                                  if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_FANATIC, 100.0f))
                                  bsw->doCast(SPELL_DARK_EMPOWERMENT, pGuard);
                                  DoScriptText(-1631026,m_creature);
                                  break;
                            case 1:
                                  if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_ADHERENT, 100.0f))
                                  bsw->doCast(SPELL_DARK_EMPOWERMENT, pGuard);
                                  DoScriptText(-1631027,m_creature);
                                  break;
                                  }
                    }

                    break;}

            case 4: {
                    bsw->timedCast(SPELL_FROSTBOLT, diff);

                    bsw->timedCast(SPELL_INSIGNIFICANCE, diff);

                    bsw->timedCast(NPC_VENGEFUL_SHADE, diff);

                    bsw->timedCast(SPELL_DOMINATE_MIND, diff);

                    if (bsw->timedQuery(NPC_FANATIC, diff))
                        {
                        switch (Difficulty) {
                             case RAID_DIFFICULTY_10MAN_HEROIC:
                                       CallGuard(urand(0,1));
                                       if (urand(0,1)) CallGuard(2);
                                       break;
                             case RAID_DIFFICULTY_25MAN_HEROIC:
                                       CallGuard(0);
                                       CallGuard(1);
                                       CallGuard(2);
                                       break;
                             default:
                                       break;

                                            }
                        }

                    break;}
        }

                    bsw->timedCast(SPELL_DEATH_AND_DECAY, diff);


         if (!bsw->hasAura(SPELL_MANA_BARRIER, m_creature) && stage == 3) 
               {
                stage = 4;
                DoScriptText(-1631024,m_creature);
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
               }

         if (bsw->timedQuery(SPELL_BERSERK, diff))
                {
                bsw->doCast(SPELL_BERSERK);
                DoScriptText(-1631031,m_creature);
                };


         DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_vengeful_shadeAI : public ScriptedAI
{
    mob_vengeful_shadeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    BossSpellWorker* bsw;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                m_creature->AddThreat(pTarget, 1000.0f);
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.5);
                }
        bsw->doCast(SPELL_VENGEFUL_BLAST);
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_DEATHWHISPER) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (bsw->timedQuery(SPELL_VENGEFUL_BLAST_0, uiDiff))
        {
            if (m_creature->IsWithinDist(m_creature->getVictim(), 3.0f, false))
                    {
//                        bsw->doCast(SPELL_VENGEFUL_BLAST_0);
                        m_creature->ForcedDespawn();
                    }
                    else
                    {
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                        m_creature->SetSpeedRate(MOVE_RUN, 0.5);
                    }
        }
    }

};

CreatureAI* GetAI_mob_vengeful_shade(Creature* pCreature)
{
    return new mob_vengeful_shadeAI(pCreature);
}


CreatureAI* GetAI_boss_lady_deathwhisper(Creature* pCreature)
{
    return new boss_lady_deathwhisperAI(pCreature);
}

void AddSC_boss_lady_deathwhisper()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lady_deathwhisper";
    newscript->GetAI = &GetAI_boss_lady_deathwhisper;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vengeful_shade";
    newscript->GetAI = &GetAI_mob_vengeful_shade;
    newscript->RegisterSelf();
}
