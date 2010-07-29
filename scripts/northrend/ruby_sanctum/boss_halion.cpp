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
    SPELL_TWILIGHT_ENTER                        = 74807, // Phases the caster into the Twilight realm - phase 32
    SPELL_TWILIGHT_ENTER2                       = 74812, //

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
    SPELL_METEOR                                = 74637, // Script Start (summon NPC_METEOR_STRIKE)
    SPELL_METEOR_IMPACT                         = 74641, // IMPACT ZONE FOR METEOR
    SPELL_METEOR_STRIKE                         = 74648, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_FLAME                          = 74718, // FLAME FROM METEOR
    //N10
    SPELL_FLAME_BREATH                          = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH                           = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD                           = 75484, // Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //NPC
    NPC_HALION_CONTROL                          = 40146,
    NPC_COMBUSTION                              = 40001,
    NPC_CONSUMPTION                             = 40135,
    //Summons
    NPC_METEOR_STRIKE                           = 40029, //casts "impact zone" then meteor
    NPC_METEOR_STRIKE_1                         = 40041,
    NPC_METEOR_STRIKE_2                         = 40042,
    NPC_METEOR_STRIKE_3                         = 40043,
    NPC_METEOR_STRIKE_4                         = 40044,
    NPC_METEOR_STRIKE_5                         = 40055,

    FR_RADIUS                                   = 40,

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
    EMOTE_WARNING                   = -1666110, //orbs charge warning
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
    uint8 stage;
    bool intro;

    void Reset()
    {
        if(!pInstance)
            return;

        stage = 0;
        if (m_creature->isAlive()) pInstance->SetData(TYPE_HALION, NOT_STARTED);
        resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);

        if (intro || !pInstance) return;
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

        if (Creature* pclone = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_HALION_TWILIGHT)))
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
        if (!pInstance || !m_creature->isAlive())
            return;

        if (pDoneBy->GetGUID() == m_creature->GetGUID())
            return;

        pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
        pInstance->SetData(DATA_P_1, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void doTwilightBuff()
    {
    switch (pInstance->GetData(DATA_P_BUFF))
    {
     case 0:
      doCast(SPELL_CORPOREALITY_EVEN);
     break;
     case 1:
      doCast(SPELL_CORPOREALITY_20I);
     break;
     case 2:
      doCast(SPELL_CORPOREALITY_40I);
     break;
     case 3:
      doCast(SPELL_CORPOREALITY_60I);
     break;
     case 4:
      doCast(SPELL_CORPOREALITY_80I);
     break;
     case 5:
      doCast(SPELL_CORPOREALITY_100I);
     break;
     case 6:
      doCast(SPELL_CORPOREALITY_20D);
     break;
     case 7:
      doCast(SPELL_CORPOREALITY_40D);
     break;
     case 8:
      doCast(SPELL_CORPOREALITY_60D);
     break;
     case 9:
      doCast(SPELL_CORPOREALITY_80D);
     break;
     case 10:
      doCast(SPELL_CORPOREALITY_100D);
     break;
    }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_T) && pInstance->GetData(DATA_HEALTH_HALION_T) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_T));

        switch (stage)
        {
            case 0: //PHASE 1 PHYSICAL REALM
                timedCast(SPELL_FLAME_BREATH, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                timedCast(SPELL_METEOR, uiDiff);
                if (m_creature->GetHealthPercent() < 75.0f) stage = 1;
                break;

            case 1: // Switch to phase 2
                DoScriptText(-1666108,m_creature);
                stage = 2;
                break;

            case 2: //DEPHASE HALION FORCE TO TWILIGHT REALM
                //setflag unattackable, unselectable, remove combat
                if (m_creature->GetHealthPercent() < 50.0f) stage = 3;
                break;

            case 3: // Switch to phase 3
                DoScriptText(-1666109,m_creature);
                stage = 4;
//                doCast(SPELL_TWILIGHT_DIVISION);
                break;

            case 4: //PHASE 3 BOTH REALMS
                timedCast(SPELL_FLAME_BREATH, uiDiff);
                timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
                timedCast(SPELL_METEOR, uiDiff);
                // Needs Script
                doTwilightBuff();
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
    uint8 stage;

    void Reset() 
    {
        if(!pInstance)
            return;
        stage = 0;
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
        doCast(SPELL_TWILIGHT_PRECISION);

        //turn on the orb cutting
//        pInstance->SetData(DATA_ORB, 1);
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
        pInstance->SetData(DATA_T_1, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > pInstance->GetData(DATA_HEALTH_HALION_P) && pInstance->GetData(DATA_HEALTH_HALION_P) != 0)
            m_creature->SetHealth(pInstance->GetData(DATA_HEALTH_HALION_P));

        if (m_creature->GetHealthPercent() < 50.0f && stage == 2)
        {
            DoScriptText(-1666109,m_creature);
            stage = 3;
        }


        switch (stage)
        {
            case 0: //SPAWNED
                break;
            case 1: //FAIL
                //HOW DID U GET HERE
                break;
            case 2: //PHASE 2 TWILIGHT REALM
                doCast(SPELL_TWILIGHT_DIVISION);
                doCast(SPELL_DUSK_SHROUD);
                timedCast(SPELL_DARK_BREATH, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                break;
            case 3: //PHASE 3 BOTH REALMS
                doCast(SPELL_TWILIGHT_DIVISION);
                doCast(SPELL_DUSK_SHROUD);
                timedCast(SPELL_DARK_BREATH, uiDiff);
                timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
                //needs test
                break;

            default:
                break;
        }

        timedCast(SPELL_BERSERK, uiDiff);

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
        Reset();
    }

    void Reset()
    {
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        setStage(0);
        m_creature->SetInCombatWithZone();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void UpdateAI(const uint32 diff)
    {
        switch (getStage())
        {
            case 0:
                 if (timedCast(SPELL_METEOR_IMPACT, diff) == CAST_OK) setStage(1);
                 break;

            case 1:
                 if (timedCast(SPELL_METEOR_STRIKE, diff) == CAST_OK) setStage(2);
                 break;

            case 2:
                 // Place summon flames there
                 {
                     float direction = 2.0f*M_PI_F*((float)urand(0,15)/16.0f);
                     float x, y, radius;
                     radius = 0.0f;
                     for(uint8 i = 0; i < getSpellData(SPELL_METEOR_STRIKE); ++i)
                     {
                         radius = radius + 5.0f;
                         m_creature->GetNearPoint2D(x, y, radius, direction);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                     }
                 };
                 {
                     float direction = 2.0f*M_PI_F*((float)urand(0,15)/16.0f);
                     float x, y, radius;
                     radius = 0.0f;
                     for(uint8 i = 0; i < getSpellData(SPELL_METEOR_STRIKE); ++i)
                     {
                         radius = radius + 5.0f;
                         m_creature->GetNearPoint2D(x, y, radius, direction);
                         doSummon(NPC_METEOR_STRIKE_1, x, y, m_creature->GetPositionZ(), TEMPSUMMON_TIMED_DESPAWN, 20000);
                     }

                 };
                 setStage(3);
                 break;

            case 3:
                 if (timedQuery(SPELL_METEOR_IMPACT, diff)) m_creature->ForcedDespawn();
                 break;

            default:
                 break;
        }

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
        Reset();
    }

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(11686);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false); 
        m_creature->SetInCombatWithZone();
//        doCast(SPELL_METEOR_FLAME);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!hasAura(SPELL_METEOR_FLAME))
              doCast(SPELL_METEOR_FLAME);

    }

};

CreatureAI* GetAI_mob_halion_flame(Creature* pCreature)
{
    return new mob_halion_flameAI(pCreature);
}

struct MANGOS_DLL_DECL mob_halion_controlAI : public ScriptedAI
{
    mob_halion_controlAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint32 timer;
    uint32 timer2;
    uint32 timer3;

    void Reset() 
    {
       timer = 1000;
       timer2 = 5000;
       timer3 = 3000;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        //corporeality get damage every second
        if (timer < diff)
        {
        //CORPOREALITY
            pInstance->SetData(DATA_P_0, (pInstance->GetData(DATA_P_0) + (pInstance->GetData(DATA_P_1))));
            pInstance->SetData(DATA_T_0, (pInstance->GetData(DATA_T_0) + (pInstance->GetData(DATA_T_1))));
            timer = 1000;
        }else timer -= diff;

  //average it out
  if (timer2 < diff)
        {
   //CORPOREALITY
   pInstance->SetData(DATA_P_2, (pInstance->GetData(DATA_P_0) / 5));
   pInstance->SetData(DATA_T_2, (pInstance->GetData(DATA_T_0) / 5));
   //clean up
   pInstance->SetData(DATA_P_0, 0);
   pInstance->SetData(DATA_T_0, 0);
   pInstance->SetData(DATA_P_1, 0);
   pInstance->SetData(DATA_T_1, 0);
   //the buff Physical side
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 1500) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 2999))
   {
    //BUFF LVL 1
    pInstance->SetData(DATA_P_BUFF, 1);
    pInstance->SetData(DATA_T_BUFF, 6);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 3000) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 4499))
   {
    //BUFF LVL 2
    pInstance->SetData(DATA_P_BUFF, 2);
    pInstance->SetData(DATA_T_BUFF, 7);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 4500) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 5999))
   {
    //BUFF LVL 3
    pInstance->SetData(DATA_P_BUFF, 3);
    pInstance->SetData(DATA_T_BUFF, 8);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 6000) && pInstance->GetData(DATA_P_2) < (pInstance->GetData(DATA_T_2) + 7499))
   {
    //BUFF LVL 4PT_BUFF, 1);
    pInstance->SetData(DATA_P_BUFF, 4);
    pInstance->SetData(DATA_T_BUFF, 9);
   }
   if(pInstance->GetData(DATA_P_2) > (pInstance->GetData(DATA_T_2) + 7500))
   {
    //BUFF LVL 5
    pInstance->SetData(DATA_P_BUFF, 5);
    pInstance->SetData(DATA_T_BUFF, 10);
   }
   else
    //NORMAL BUFF
    pInstance->SetData(DATA_P_BUFF, 0);

   //the buff twilight side
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 1500) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 2999))
   {
    //BUFF LVL 1
    pInstance->SetData(DATA_P_BUFF, 6);
    pInstance->SetData(DATA_T_BUFF, 1);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 3000) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 4499))
   {
    //BUFF LVL 2
    pInstance->SetData(DATA_P_BUFF, 7);
    pInstance->SetData(DATA_T_BUFF, 2);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 4500) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 5999))
   {
    //BUFF LVL 3
    pInstance->SetData(DATA_P_BUFF, 8);
    pInstance->SetData(DATA_T_BUFF, 3);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 6000) && pInstance->GetData(DATA_T_2) < (pInstance->GetData(DATA_P_2) + 7499))
   {
    //BUFF LVL 4
    pInstance->SetData(DATA_P_BUFF, 9);
    pInstance->SetData(DATA_T_BUFF, 4);
   }
   if(pInstance->GetData(DATA_T_2) > (pInstance->GetData(DATA_P_2) + 7500))
   {
    //BUFF LVL 5
    pInstance->SetData(DATA_P_BUFF, 10);
    pInstance->SetData(DATA_T_BUFF, 5);
   }
   else
    //NORMAL BUFF
    pInstance->SetData(DATA_T_BUFF, 0);




   timer2 = 5000;
        }else timer2 -= diff;

    }
};

CreatureAI* GetAI_mob_halion_control(Creature* pCreature)
{
    return new mob_halion_controlAI(pCreature);
};

struct MANGOS_DLL_DECL mob_orb_rotation_focusAI : public ScriptedAI
{
    mob_orb_rotation_focusAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;
    uint32 m_timer;
    float m_direction, m_nextdirection;
    bool m_warning;

    void Reset()
    {
//        m_creature->SetDisplayId(11686);
        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);
        m_creature->SetPhaseMask(32, true);
        SetCombatMovement(false); 
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_direction = 0.0f;
        m_nextdirection = 0.0f;
        m_timer = 30000;
        m_warning = false;

        float x,y;
        m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
        m_creature->SummonCreature(NPC_SHADOW_PULSAR_N, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_direction + M_PI_F);
        m_creature->SummonCreature(NPC_SHADOW_PULSAR_S, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
//        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS) 
//              m_creature->ForcedDespawn();

        if (pInstance->GetData(DATA_ORB_S) == DONE && pInstance->GetData(DATA_ORB_N) == DONE)
        {
            m_direction = m_nextdirection;
            m_nextdirection = (m_direction - M_PI_F/64.0f);
            if (m_nextdirection < 0.0f ) m_nextdirection = m_nextdirection + 2.0f*M_PI_F;
            pInstance->SetData(DATA_ORB_DIRECTION, (uint32)(m_nextdirection*1000));
            pInstance->SetData(DATA_ORB_N, SPECIAL);
            pInstance->SetData(DATA_ORB_S, SPECIAL);
            debug_log("EventMGR: creature %u send direction %u ",m_creature->GetEntry(),pInstance->GetData(DATA_ORB_DIRECTION));
        }

        if (m_timer - 6000 <= uiDiff && !m_warning)
        {
            DoScriptText(-1666110,m_creature);
            m_warning = true;
        }

        if (m_timer <= uiDiff)
        {
            float x,y;
            m_creature->GetNearPoint2D(x, y, FR_RADIUS, m_nextdirection);
            m_creature->SummonCreature(NPC_ORB_CARRIER, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
            m_timer = 30000;
            m_warning = false;
        }   else m_timer -= uiDiff;


//      DoCastSpellIfCan(orb_target, SPELL_TWILIGHT_CUTTER);
    }
};

CreatureAI* GetAI_mob_orb_rotation_focus(Creature* pCreature)
{
    return new mob_orb_rotation_focusAI(pCreature);
};

struct MANGOS_DLL_DECL mob_halion_orbAI : public ScriptedAI
{
    mob_halion_orbAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;
    float m_direction,m_delta;
    uint32 m_flag;
    uint32 m_flag1;
    bool MovementStarted;
    Unit* focus;
    uint32 nextPoint;

    void Reset()
    {
        if (!pInstance) return;
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false);
        m_creature->SetPhaseMask(32, true);
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        if (m_creature->GetEntry() == NPC_SHADOW_PULSAR_N)
        {
            m_flag = DATA_ORB_N;
            m_delta = 0.0f;
        } else if (m_creature->GetEntry() == NPC_SHADOW_PULSAR_S)
        {
            m_flag = DATA_ORB_S;
            m_delta = M_PI_F;
        };
        m_direction = 0.0f;
        nextPoint = 0;
        MovementStarted = false;
        pInstance->SetData(m_flag, DONE);
        debug_log("EventMGR: creature %u assume m_flag %u ",m_creature->GetEntry(),m_flag);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == nextPoint) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                pInstance->SetData(m_flag, DONE);
                }
    }

    void StartMovement(uint32 id)
    {
        if (!pInstance) return;
        nextPoint = id;
        float x,y;
        pInstance->SetData(m_flag, IN_PROGRESS);
        MovementStarted = true;
        m_direction = ((float)pInstance->GetData(DATA_ORB_DIRECTION)/1000 + m_delta);
        if (m_direction > 2.0f*M_PI_F) m_direction = m_direction - 2.0f*M_PI_F;
        if (focus = Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ORB_ROTATION_FOCUS)))
            focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
            else m_creature->ForcedDespawn();
        debug_log("EventMGR: creature %u go to move point %u ",m_creature->GetEntry(),id);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(id, x, y,  m_creature->GetPositionZ());
    }

    void UpdateAI(const uint32 uiDiff)
    {
//        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS) 
//              m_creature->ForcedDespawn();

        if (!MovementStarted && pInstance->GetData(m_flag) == SPECIAL)
        {
            debug_log("EventMGR: creature %u get direction %u ",m_creature->GetEntry(),pInstance->GetData(DATA_ORB_DIRECTION));
            StartMovement(1);
        }

    }
};

CreatureAI* GetAI_mob_halion_orb(Creature* pCreature)
{
    return new mob_halion_orbAI(pCreature);
}

struct MANGOS_DLL_DECL mob_orb_carrierAI : public BSWScriptedAI
{
    mob_orb_carrierAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* pInstance;
    bool MovementStarted;

    void Reset()
    {
        m_creature->SetDisplayId(10045);
        m_creature->SetRespawnDelay(7*DAY);
        SetCombatMovement(false); 
        m_creature->SetPhaseMask(32, true);
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        MovementStarted = false;
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        m_creature->SetSpeedRate(MOVE_RUN, 9.0f);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (!pInstance) return;

        if (type != POINT_MOTION_TYPE || !MovementStarted) return;

        if (id == 1) {
                m_creature->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                m_creature->ForcedDespawn();
                }
    }

    void UpdateAI(const uint32 uiDiff)
    {
//        if (!pInstance || pInstance->GetData(TYPE_HALION) != IN_PROGRESS)
//              m_creature->ForcedDespawn();

        if (Unit* pTarget = doSelectRandomPlayerAtRange(1.0f))
            doCast(SPELL_TWILIGHT_CUTTER, pTarget);

        if (!MovementStarted)
        {
            float x,y;
            float m_direction = ((float)pInstance->GetData(DATA_ORB_DIRECTION)/1000.0f + M_PI_F - M_PI_F/32.0f);
            if (m_direction > 2.0f*M_PI_F) m_direction = m_direction - 2.0f*M_PI_F;
            if (Unit* focus = Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_ORB_ROTATION_FOCUS)))
                focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
                else m_creature->ForcedDespawn();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(1, x, y,  m_creature->GetPositionZ());
            MovementStarted = true;
        }

    }

};

CreatureAI* GetAI_mob_orb_carrier(Creature* pCreature)
{
    return new mob_orb_carrierAI(pCreature);
};

bool GOGossipHello_go_halion_portal_twilight(Player *player, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if(!pInstance) return false;
    player->CastSpell(player,SPELL_TWILIGHT_ENTER,false);
    return true;
}

bool GOGossipHello_go_halion_portal_real(Player *player, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();
    if(!pInstance) return false;
    player->RemoveAurasDueToSpell(SPELL_TWILIGHT_ENTER);
    return true;
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
    newscript->Name = "mob_halion_meteor";
    newscript->GetAI = &GetAI_mob_halion_meteor;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_flame";
    newscript->GetAI = &GetAI_mob_halion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_orb";
    newscript->GetAI = &GetAI_mob_halion_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_halion_control";
    newscript->GetAI = &GetAI_mob_halion_control;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_orb_rotation_focus";
    newscript->GetAI = &GetAI_mob_orb_rotation_focus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_orb_carrier";
    newscript->GetAI = &GetAI_mob_orb_carrier;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_halion_portal_twilight";
    newscript->pGOGossipHello  = &GOGossipHello_go_halion_portal_twilight;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_halion_portal_real";
    newscript->pGOGossipHello  = &GOGossipHello_go_halion_portal_real;
    newscript->RegisterSelf();
}
