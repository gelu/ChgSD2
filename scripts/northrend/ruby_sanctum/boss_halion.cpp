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
SDName: boss_halion
SD%Complete: 20%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

//TODO: Add meteor/portal mechanics, twilight cutter (2 orbs), sql spells, sql npcs, TEST

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum 
{
    //SPELLS
    //All
    SPELL_TWILIGHT_PRECISION                    = 78243, // Increases Halion's chance to hit by 5% and decreases all players' chance to dodge by 20%
    SPELL_BERSERK                               = 26663, // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500% for 5 min.  Also grants immunity to Taunt effects.
    SPELL_START_PHASE2                          = 74808, // Phases the caster into the Twilight realm, leaving behind a large rift.
    SPELL_TWILIGHT_ENTER                        = 74807, // Phases the caster into the Twilight realm

    //NEED SCRIPT
    SPELL_TAIL_LASH                             = 74531, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TWILIGHT_DIVISION                     = 75063, // Phases the caster, allowing him to exist and act simultaneously in both the material and Twilight realms.
    SPELL_FIERY_COMBUSTION                      = 74562, // Inflicts 4,000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
    SPELL_SOUL_CONSUMPTION                      = 74792, // Inflicts 4,000 Shadow damage every 2 seconds for 30 seconds to a random raider. Every time Soul Consumption does damage, it applies a stackable Mark of Consumption.
    SPELL_TWILIGHT_CUTTER                       = 77844, // Inflicts 13,875 to 16,125 Shadow damage every second to players touched by the shadow beam
    //CORPOREALITY
    SPELL_CORPOREALITY_EVEN                     = 74826, // Deals & receives normal damage
    SPELL_CORPOREALITY_20I                      = 74827, // Damage dealt increased by 10% & Damage taken increased by 15%
    SPELL_CORPOREALITY_40I                      = 74828, // Damage dealt increased by 30% & Damage taken increased by 50%
    SPELL_CORPOREALITY_60I                      = 74829, // Damage dealt increased by 60% & Damage taken increased by 100%
    SPELL_CORPOREALITY_80I                      = 74830, // Damage dealt increased by 100% & Damage taken increased by 200%
    SPELL_CORPOREALITY_100I                     = 74831, // Damage dealt increased by 200% & Damage taken increased by 400%
    SPELL_CORPOREALITY_20D                      = 74832, // Damage dealt reduced by 10% & Damage taken reduced by 15%
    SPELL_CORPOREALITY_40D                      = 74833, // Damage dealt reduced by 30% & Damage taken reduced by 50%
    SPELL_CORPOREALITY_60D                      = 74834, // Damage dealt reduced by 60% & Damage taken reduced by 100%
    SPELL_CORPOREALITY_80D                      = 74835, // Damage dealt reduced by 100% & Damage taken reduced by 200%
    SPELL_CORPOREALITY_100D                     = 74836, // Damage dealt reduced by 200% & Damage taken reduced by 400% 
    //METEOR STRIKE
    SPELL_METEOR_STRIKE                         = 75877, // Script Start
    SPELL_METEOR                                = 74637, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_IMPACT_ZONE                    = 74641, // IMPACT ZONE FOR METEOR
    SPELL_METEOR_LAND                           = 74648, // LANDING EFFECT
    SPELL_METEOR_FLAME                          = 74718, // FLAME FROM METEOR
    //N10
    SPELL_FLAME_BREATH_0                        = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_0                         = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_0                         = 75484, // Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //H10 & N25
    SPELL_FLAME_BREATH_1                        = 74527, // Inflicts 26,250 to 33,750 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_1                         = 74806, // Inflicts 26,250 to 33,750 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_1                         = 75485, // Inflicts 4,500 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //H25
    SPELL_FLAME_BREATH_2                        = 74404, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH_2                         = 75956, // Inflicts 33,688 to 43,312 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD_2                         = 75486, // Inflicts 6,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //NPC
    NPC_LIVING_EMBER                            = 40683,
    NPC_LIVING_INFERNO                          = 40681,
    //Summons
    NPC_METEOR_STRIKE                           = 40029, //casts "impact zone" then meteor
    NPC_METEORFLAME_0                           = 36673, //meteor flame 0
    NPC_METEORFLAME_1                           = 36674, //meteor flame 1
    NPC_METEORFLAME_2                           = 36675, //meteor flame 2
    NPC_METEORFLAME_3                           = 36676, //meteor flame 3
    NPC_SHADOW_PULSAR_N                         = 40083, //spinning orb N spawn
    NPC_SHADOW_PULSAR_S                         = 40100, //spinning orb S spawn

    //SAYS
    SAY_HALION_SPAWN                = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
    SAY_HALION_AGGRO                = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1               = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2               = -1666103, //17502 Ha Ha Ha!
    SAY_HALION_DEATH                = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
    SAY_HALION_BERSERK              = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1            = -1666106, //17505 The heavens burn!
    SAY_HALION_SPECIAL_2            = -1666107, //17506 Beware the shadow!
    SAY_HALION_PHASE_2              = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
    SAY_HALION_PHASE_3              = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!
};

static float m_xflame[12][8]=
{
    //RND0 RND1 RND2 RND3
    //x y x y x y x y
    {2.0f,0.0f,2.0f,1.0f,1.5f,1.5f,1.0f,2.0f}, //NPC0 SPAWN1
    {4.5f,0.0f,4.0f,2.0f,3.0f,3.0f,2.0f,4.0f}, //NPC0 SPAWN2
    {7.5f,0.0f,6.5f,3.5f,4.5f,4.5f,3.5f,6.5f}, //NPC0 SPAWN3
    {0.0f,2.0f,-2.0f,-1.0f,-1.5f,1.5f,-1.0f,2.0f}, //NPC1 SPAWN1
    {0.0f,4.5f,-4.0f,-2.0f,-3.0f,3.0f,-2.0f,4.0f}, //NPC1 SPAWN2
    {0.0f,7.5f,-6.5f,-3.5f,-4.5f,4.5f,-3.5f,6.5f}, //NPC1 SPAWN3
    {-2.0f, 0.0f,1.0f,2.0f,-1.5f,-1.5f,-1.0f,-2.0f}, //NPC2 SPAWN1
    {-4.5f, 0.0f,2.0f,4.0f,-3.0f,-3.0f,-2.0f,-4.0f}, //NPC2 SPAWN2
    {-7.5f,0.0f,3.5f,6.5f,-4.5f,-4.5f,-3.5f,-6.5f}, //NPC2 SPAWN3
    {0.0f,-2.0f,-1.0f,-2.0f,1.5f,-1.5f,1.0f,-2.0f}, //NPC3 SPAWN1
    {0.0f,-4.5f,-2.0f,-4.0f,3.0f,-3.0f,2.5f,-4.0f}, //NPC3 SPAWN2
    {0.0f,-7.5f,-3.5f,-6.5f,4.5f,-4.5f,3.5f,-6.5f}, //NPC3 SPAWN3
};

static float m_pulsar[16][2]=
{
    {0.0f, 0.0f}, //S
    {0.0f, 0.0f}, //SSW
    {0.0f, 0.0f}, //SW
    {0.0f, 0.0f}, //SWW
    {0.0f, 0.0f}, //W
    {0.0f, 0.0f}, //NWW
    {0.0f, 0.0f}, //NW
    {0.0f, 0.0f}, //NNW
    {0.0f, 0.0f}, //N
    {0.0f, 0.0f}, //NNE
    {0.0f, 0.0f}, //NE
    {0.0f, 0.0f}, //NEE
    {0.0f, 0.0f}, //E
    {0.0f, 0.0f}, //SEE
    {0.0f, 0.0f}, //SE
    {0.0f, 0.0f}, //SSE
};

/*######
## boss_halion_real (Physical version)
######*/
struct MANGOS_DLL_DECL boss_halion_realAI : public BSWScriptedAI
{
    boss_halion_realAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint8 p_phase;
    bool intro;

    void Reset()
    {
        if(!pInstance)
            return;

        p_phase = 0;
        pInstance->SetData(TYPE_HALION, NOT_STARTED);
        resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (intro) return;
        else
        {
        DoScriptText(-1666100,m_creature);
        intro = true;
        }
    }

    void JustReachedHome()
    {
        if (!pInstance)
            return;

        pInstance->SetData(TYPE_HALION, FAIL);
        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;

        DoScriptText(-1666104,m_creature);
        if (Creature* pclone = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_HALION_T)))
            if (!pclone->isAlive())
                pInstance->SetData(TYPE_HALION, DONE);
        else
            pInstance->SetData(TYPE_HALION, SPECIAL);
        pInstance->SetData(DATA_HEALTH_HALION_P, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(TYPE_HALION, IN_PROGRESS);     
        DoScriptText(-1666101,m_creature);
        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance)
            return;
        if (!m_creature || !m_creature->isAlive())
            return;
        if (pDoneBy->GetGUID() == m_creature->GetGUID())
            return;

        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance)
            return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_T) && pInstance->GetData(DATA_HEALTH_HALION_T) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_T));

        if (m_creature->GetHealthPercent() < 100.0f && p_phase == 0)
        {
            pInstance->SetData(TYPE_HALION_LOCK, DONE);
            p_phase = 1;
        }

        if (m_creature->GetHealthPercent() < 75.0f && p_phase == 1)
        {
            DoScriptText(-1666108,m_creature);
            p_phase = 2;
        }

        if (m_creature->GetHealthPercent() < 50.0f && p_phase == 2)
        {
            DoScriptText(-1666109,m_creature);
            p_phase = 3;
        }

        switch (p_phase)
        {
            case 0: //SPAWNED
                break;
            case 1: //PHASE 1 PHYSICAL REALM
                timedCast(SPELL_FLAME_BREATH_0, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                /* Meteor Needs Test */
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
                {
                    if (doCast(SPELL_METEOR, pTarget) == CAST_OK)
                    {
                        DoScriptText(-1666106,m_creature,pTarget);
                        float fPosX, fPosY, fPosZ;
                        m_creature->GetPosition(fPosX, fPosY, fPosZ);
                        if (Unit* pMeteor = doSummon(NPC_METEOR_STRIKE, fPosX, fPosY, fPosZ))
                            pMeteor->AddThreat(pTarget, 100.0f);
                    };
                }
                break;
            case 2: //DEPHASE HALION FORCE TO TWILIGHT REALM
                //setflag unattackable, unselectable, remove combat
                break;
            case 3: //PHASE 3 BOTH REALMS
                timedCast(SPELL_FLAME_BREATH_0, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                doCast(SPELL_TWILIGHT_DIVISION);
                /* Needs Script
                timedCast(SPELL_METEOR_STRIKE, uiDiff); 
                timedCast(SPELL_CORPREALITY_EVEN, uiDiff); 
                */
                break;
            default:
                break;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_real(Creature* pCreature)
{
    return new boss_halion_realAI(pCreature);
}

/*######
## boss_halion_twilight (Twilight version)
######*/

struct MANGOS_DLL_DECL boss_halion_twilightAI : public BSWScriptedAI
{
    boss_halion_twilightAI(Creature* pCreature) : BSWScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint8 t_phase;

    void Reset() 
    {
        if(!pInstance)
            return;

        t_phase = 0;
        pInstance->SetData(TYPE_HALION, NOT_STARTED);
        resetTimers();
    }

    void JustReachedHome()
    {
        if (!pInstance)
            return;

        pInstance->SetData(TYPE_HALION, FAIL);
        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!pInstance)
            return;

        DoScriptText(-1666104,m_creature);
        pInstance->SetData(DATA_HEALTH_HALION_T, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoScriptText(-1631006,m_creature,pVictim);
                break;
            case 1:
                DoScriptText(-1631007,m_creature,pVictim);
                break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!pInstance)
            return;

        m_creature->SetInCombatWithZone();
        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());

        if (t_phase == 0)
            t_phase = 2;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!pInstance)
            return;
        if (!m_creature || !m_creature->isAlive())
            return;
        if(pDoneBy->GetGUID() == m_creature->GetGUID())
            return;

        pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_P) && pInstance->GetData(DATA_HEALTH_HALION_P) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_P));

        if (m_creature->GetHealthPercent() < 50.0f && t_phase == 2)
        {
            DoScriptText(-1666109,m_creature);
            t_phase = 3;
        }

        switch (t_phase)
        {
            case 0: //SPAWNED
                break;
            case 1: //FAIL
                //HOW DID U GET HERE
                break;
            case 2: //PHASE 2 TWILIGHT REALM
                doCast(SPELL_TWILIGHT_DIVISION);
                doCast(SPELL_DUSK_SHROUD_0);
                timedCast(SPELL_DARK_BREATH_0, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                break;
            case 3: //PHASE 3 BOTH REALMS
                doCast(SPELL_TWILIGHT_DIVISION);
                doCast(SPELL_DUSK_SHROUD_0);
                timedCast(SPELL_DARK_BREATH_0, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                break;
            default:
                break;
        }
        // timedCast(SPELL_BERSERK, uiDiff);
        // should be SPELL_TWILIGHT_PRECISION according to RSA's script, Notagain should have a look at this
        timedCast(SPELL_TWILIGHT_PRECISION, uiDiff);
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_twilight(Creature* pCreature)
{
    return new boss_halion_twilightAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_meteorAI : public BSWScriptedAI
{
    mob_halion_meteorAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    uint32 m_uiTimer;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_uiTimer = 5000;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!pInstance)
            return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

//        doCast(SPELL_METEOR);
        timedCast(SPELL_METEOR_IMPACT_ZONE, diff);
        timedCast(SPELL_METEOR_LAND, diff);

        if (m_uiTimer < diff)
            {
            switch (urand(0,3)) 
            {
            case 0:
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[0][0], m_xflame[0][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[1][0], m_xflame[1][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[2][0], m_xflame[2][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[3][0], m_xflame[3][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[4][0], m_xflame[4][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[5][0], m_xflame[5][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[6][0], m_xflame[6][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[7][0], m_xflame[7][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[8][0], m_xflame[8][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[9][0], m_xflame[9][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[10][0], m_xflame[10][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[11][0], m_xflame[11][1], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  break;
             case 1:
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[0][2], m_xflame[0][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[1][2], m_xflame[1][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[2][2], m_xflame[2][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[3][2], m_xflame[3][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[4][2], m_xflame[4][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[5][2], m_xflame[5][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[6][2], m_xflame[6][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[7][2], m_xflame[7][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[8][2], m_xflame[8][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[9][2], m_xflame[9][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[10][2], m_xflame[10][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[11][2], m_xflame[11][3], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  break;
             case 2:
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[0][4], m_xflame[0][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[1][4], m_xflame[1][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[2][4], m_xflame[2][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[3][4], m_xflame[3][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[4][4], m_xflame[4][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[5][4], m_xflame[5][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[6][4], m_xflame[6][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[7][4], m_xflame[7][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[8][4], m_xflame[8][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[9][4], m_xflame[9][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[10][4], m_xflame[10][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[11][4], m_xflame[11][5], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  break;
             case 3:
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[0][6], m_xflame[0][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[1][6], m_xflame[1][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_0, m_xflame[2][6], m_xflame[2][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[3][6], m_xflame[3][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[4][6], m_xflame[4][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_1, m_xflame[5][6], m_xflame[5][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[6][6], m_xflame[6][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[7][6], m_xflame[7][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_2, m_xflame[8][6], m_xflame[8][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[9][6], m_xflame[9][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[10][6], m_xflame[10][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  m_creature->SummonCreature(NPC_METEORFLAME_3, m_xflame[11][6], m_xflame[11][7], 0.0f, 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 8000);
                  break;
             };
            m_uiTimer = 998000;
          } else  m_uiTimer -= diff;
     }
};

CreatureAI* GetAI_mob_halion_meteor(Creature* pCreature)
{
    return new mob_halion_meteorAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_flameAI : public BSWScriptedAI
{
    mob_halion_flameAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        doCast(SPELL_METEOR_FLAME);
    }
};
CreatureAI* GetAI_mob_halion_flame(Creature* pCreature)
{
    return new mob_halion_flameAI(pCreature);
}

void AddSC_boss_halion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_halion_real";
    newscript->GetAI = &GetAI_boss_halion_real;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_halion_twilight";
    newscript->GetAI = &GetAI_boss_halion_twilight;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_flame";
    newscript->GetAI = &GetAI_mob_halion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_meteor";
    newscript->GetAI = &GetAI_mob_halion_meteor;
    newscript->RegisterSelf();
}
