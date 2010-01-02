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

#include "precompiled.h"
#include "naxxramas.h"

//GO: the door
#define GO_GOTHIK_GATE      181170

//creatures id
//Gothik - 16060
#define CR_UN_TRAINEE       16124
#define CR_UN_DEATHKNIGHT   16125
#define CR_UN_RIDER         16126
#define CR_SP_TRAINEE       16127
#define CR_SP_DEATHKNIGHT   16148
#define CR_SP_RIDER         16150

//SPELLS
//Gothik the Harvester
#define SP_HARVEST_SOUL     28679
#define SP_SHADOWBOLT       29317
#define H_SP_SHADOWBOLT     56405
//Unrelenting Trainee
#define SP_DEATH_PLAGUE     55604
#define H_SP_DEATH_PLAGUE   55645
//Unrelenting Death Knight
#define SP_SHADOW_MARK      27825
//Unrelenting Rider
#define SP_SHADOW_VOLLEY    27831
#define H_SP_SHADOW_VOLLEY  55638
//Spectral Trainee
#define SP_EXPLOSION        27989
#define H_SP_EXPLOSION      56407
//Spectral Death Knight
#define SP_WHIRLWIND        56408
//Spectral Rider
#define SP_UNHOLY_AURA      55606
#define H_SP_UNHOLY_AURA    55608
#define SP_UNHOLY_FRENZY    55648
#define H_SP_UNHOLY_FRENZY  27995
#define SP_DRAIN_LIFE       27994
#define H_SP_DRAIN_LIFE     55646


//Gothik Teleport Coordinates
#define LIVE_X      2691.991699
#define LIVE_Y      -3422.510742
#define LIVE_Z      267.689362
#define UNDEAD_X    2693.844238
#define UNDEAD_Y    -3298.302490
#define UNDEAD_Z    267.683167
//Mobs summon coords
//Live:
//Undead:


/*
Фаза 1 (3:30 минут)
суммон мобов, готик стоит на балконе неактивный
Фаза 2 (60 сек)
готик на балконе неактивный, мобы перестают появляться
Фаза 3 (15 сек)
готик на стороне 1, потом тп
Фаза 4 (15 сек)
готик на стороне 2, потом тп
Фаза 5 (от 30% до конца)
дверь открывается, готик перестает телепортироваться
*/


struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        
        trainees = Regular ? 2 : 3;

        LiveX[0]=2669.430176; LiveY[0]=-3430.828613; LiveZ[0]=268.563049;
        LiveX[1]=2692.187988; LiveY[1]=-3431.384277; LiveZ[1]=268.563538;
        LiveX[2]=2714.282959; LiveY[2]=-3431.556152; LiveZ[2]=268.563538;
        
        Reset();
    }

    //summon coordinates
    float LiveX[3];
    float LiveY[3];
    float LiveZ[3];
    
    bool Regular;
    uint32 phase;
    uint32 Phase_Timer;
    uint32 Shadowbolt_Timer;

    //summon timers
    uint32 Trainee_Timer;
    uint32 DeathKnight_Timer;
    uint32 Rider_Timer;
    
    int trainees;

    ScriptedInstance* pInstance;

    void Reset()
    {
        phase = 1;
        Phase_Timer = 210000;
        Shadowbolt_Timer = 1200;

        Trainee_Timer = 12000;
        DeathKnight_Timer = 20000;
        Rider_Timer = 60000;

        SetCombatMovement(false);
        
        if(pInstance) pInstance->SetData(TYPE_GOTHIK, NOT_STARTED);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(phase==1 || phase==2) damage=0;
    }

    void Aggro(Unit *who)
    {
        if(pInstance) pInstance->SetData(TYPE_GOTHIK, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_GOTHIK, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        int i;

        //PHASE 1
        if(phase==1)
        {
            if(Phase_Timer < diff)
            {
                phase = 2;
                Phase_Timer = 60000; //phase 2 lasts 60 seconds
            }
            else Phase_Timer -= diff;

            //summon trainees
            if(Trainee_Timer < diff)
            {
                for(i=0; i<trainees; i++)
                {
                    Unit *mob = m_creature->SummonCreature(CR_UN_TRAINEE, LiveX[i], LiveY[i], LiveZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if(mob && target) mob->AddThreat(target, 1.0f);
                }
                Trainee_Timer = 15000;
            }
            else Trainee_Timer -= diff;

            //summon death knights
            if(DeathKnight_Timer < diff)
            {
                i = irand(0,2);
                Unit *mob = m_creature->SummonCreature(CR_UN_DEATHKNIGHT, LiveX[i], LiveY[i], LiveZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(mob && target) mob->AddThreat(target, 1.0f);
                DeathKnight_Timer = 30000;
            }
            else DeathKnight_Timer -= diff;

            //summon rider
            if(Rider_Timer < diff)
            {
                i = irand(0,2);
                Unit *mob = m_creature->SummonCreature(CR_UN_RIDER, LiveX[i], LiveY[i], LiveZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(mob && target) mob->AddThreat(target, 1.0f);
                Rider_Timer = 45000;
            }
            else Rider_Timer -= diff;
        }

        //PHASE 2
        if(phase==2)
        {
            if(Phase_Timer < diff)
            {
                //teleport gothik down
                m_creature->NearTeleportTo(UNDEAD_X, UNDEAD_Y, UNDEAD_Z, 0);
                Phase_Timer = 15000;
                phase = 3;

                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            }
            else Phase_Timer -= diff;
        }

        //PHASE 3,4
        if(phase==3 || phase==4)
        {
            if(Phase_Timer < diff)
            {
                //cast Harvest Soul (-10% stats to the raid)
                Unit* target = NULL;
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                {
                    if (Unit* target = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid()))
                        if(target->isAlive())
                            DoCast(target, SP_HARVEST_SOUL, true);
                }
                //teleport gothik to the other side
                if(phase==3)
                    m_creature->NearTeleportTo(LIVE_X, LIVE_Y, LIVE_Z, 0);
                else
                    m_creature->NearTeleportTo(UNDEAD_X, UNDEAD_Y, UNDEAD_Z, 0);

                Phase_Timer = 15000;
                phase = (phase==3) ? 4 : 3;
            }
            else Phase_Timer -= diff;

            //cast shadowbolts
            if(Shadowbolt_Timer < diff)
            {
                DoCast(m_creature->getVictim(), Regular ? SP_SHADOWBOLT : H_SP_SHADOWBOLT);
                Shadowbolt_Timer = 1200;
            }
            else Shadowbolt_Timer -= diff;

            //if 30% left, stop teleporting
            if ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() <= 30)
            {
                phase=5;
                if(pInstance) pInstance->SetData(TYPE_GOTHIK, SPECIAL);
            }
        }

        //PHASE 5
        if(phase==5)
        {
            if(Shadowbolt_Timer < diff)
            {
                DoCast(m_creature->getVictim(), Regular ? SP_SHADOWBOLT : H_SP_SHADOWBOLT);
                Shadowbolt_Timer = 1200;
            }
            else Shadowbolt_Timer -= diff;
        }
    }
};


/*******************************************************************/
//         UNRELENTING / SPECTRAL  TRAINEE  AI                     //
/*******************************************************************/

struct MANGOS_DLL_DECL mob_gothik_trainee_addAI : public ScriptedAI
{
    mob_gothik_trainee_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    float UndeadX[5];
    float UndeadY[5];
    float UndeadZ[5];
    uint32 PlagueTimer;
    uint32 ExplosionTimer;

    void Reset()
    {
        UndeadX[0]=2733.049561; UndeadY[0]=-3349.463135; UndeadZ[0]=268.113251;
        UndeadX[1]=2725.951416; UndeadY[1]=-3310.180664; UndeadZ[1]=269.058899;
        UndeadX[2]=2700.589111; UndeadY[2]=-3322.897949; UndeadZ[2]=269.107666;
        UndeadX[3]=2683.933350; UndeadY[3]=-3304.313232; UndeadZ[3]=269.135010;
        UndeadX[4]=2665.220459; UndeadY[4]=-3339.860840; UndeadZ[4]=268.846222;
        PlagueTimer = 3000;
        ExplosionTimer = 3000+rand()%8000;
    }

    void Aggro(Unit *who) {}

    void JustDied(Unit *killer)
    {
        //if unrelenting trainee then spawn spectral trainee
        if(m_creature->GetEntry()==CR_UN_TRAINEE) //maybe need check Regular entry too
        {
            int i = irand(0,5);
            Unit *mob = m_creature->SummonCreature(CR_SP_TRAINEE, UndeadX[i], UndeadY[i], UndeadZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if(mob) mob->AddThreat(killer, 1.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_creature->GetEntry()==CR_UN_TRAINEE)
        {
            if(PlagueTimer < diff)
            {
                DoCast(m_creature->getVictim(), Regular ? SP_DEATH_PLAGUE : H_SP_DEATH_PLAGUE);
                PlagueTimer = 3500;
            }
            PlagueTimer -= diff;
        }
        else
        {
            if(ExplosionTimer < diff)
            {
                DoCast(m_creature, Regular ? SP_EXPLOSION : H_SP_EXPLOSION);
                ExplosionTimer = 5000+rand()%10000;
            }
            ExplosionTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

/*******************************************************************/
//           UNRELENTING / SPECTRAL  DEATH KNIGHT  AI              //
/*******************************************************************/

struct MANGOS_DLL_DECL mob_gothik_dk_addAI : public ScriptedAI
{
    mob_gothik_dk_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();      
    }

    float UndeadX[5];
    float UndeadY[5];
    float UndeadZ[5];
    uint32 SpellTimer;

    void Reset()
    {
        UndeadX[0]=2733.049561; UndeadY[0]=-3349.463135; UndeadZ[0]=268.113251;
        UndeadX[1]=2725.951416; UndeadY[1]=-3310.180664; UndeadZ[1]=269.058899;
        UndeadX[2]=2700.589111; UndeadY[2]=-3322.897949; UndeadZ[2]=269.107666;
        UndeadX[3]=2683.933350; UndeadY[3]=-3304.313232; UndeadZ[3]=269.135010;
        UndeadX[4]=2665.220459; UndeadY[4]=-3339.860840; UndeadZ[4]=268.846222;
        SpellTimer = 8000+rand()%8000; //Shadow Mark and Whirlwind
    }

    void Aggro(Unit *who) {}

    void JustDied(Unit *killer)
    {
        //if unrelenting DK then spawn spectral DK
        if(m_creature->GetEntry()==CR_UN_DEATHKNIGHT)
        {
            int i = irand(0,5);
            Unit *mob = m_creature->SummonCreature(CR_SP_DEATHKNIGHT, UndeadX[i], UndeadY[i], UndeadZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if(mob) mob->AddThreat(killer, 1.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(SpellTimer < diff)
        {
            if(m_creature->GetEntry()==CR_UN_DEATHKNIGHT)
                DoCast(m_creature->getVictim(), SP_SHADOW_MARK);
            else //Spectral DK
                DoCast(m_creature->getVictim(), SP_WHIRLWIND);
            SpellTimer = 8000+rand()%8000;
        }
        else SpellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};



/*******************************************************************/
//             UNRELENTING / SPECTRAL  RIDER  AI                   //
/*******************************************************************/

struct MANGOS_DLL_DECL mob_gothik_rider_addAI : public ScriptedAI
{
    mob_gothik_rider_addAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    uint32 VolleyTimer;
    uint32 DrainTimer;
    //uint32 FrenzyTimer;
    float UndeadX[5];
    float UndeadY[5];
    float UndeadZ[5];

    void Reset()
    {
        UndeadX[0]=2733.049561; UndeadY[0]=-3349.463135; UndeadZ[0]=268.113251;
        UndeadX[1]=2725.951416; UndeadY[1]=-3310.180664; UndeadZ[1]=269.058899;
        UndeadX[2]=2700.589111; UndeadY[2]=-3322.897949; UndeadZ[2]=269.107666;
        UndeadX[3]=2683.933350; UndeadY[3]=-3304.313232; UndeadZ[3]=269.135010;
        UndeadX[4]=2665.220459; UndeadY[4]=-3339.860840; UndeadZ[4]=268.846222;
        VolleyTimer = 5000+rand()%10000;
        DrainTimer = 8000+rand()%10000;
        //FrenzyTimer = 2000;
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SP_UNHOLY_AURA);
    }

    void JustDied(Unit *killer)
    {
        //if unrelenting rider then spawn spectral rider
        if(m_creature->GetEntry()==CR_UN_RIDER) //maybe need check Regular entry too
        {
            int i = irand(0,5);
            Unit *mob = m_creature->SummonCreature(CR_SP_RIDER, UndeadX[i], UndeadY[i], UndeadZ[i], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            if(mob) mob->AddThreat(killer, 1.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_creature->GetEntry()==CR_UN_RIDER)
        {
            if(VolleyTimer < diff)
            {
                //hit only targets with shadow mark
                /*Unit* target = NULL;
                std::list<HostileReference*>::iterator i = m_creature->getThreatManager().getThreatList().begin();
                for (i = m_creature->getThreatManager().getThreatList().begin(); i!=m_creature->getThreatManager().getThreatList().end(); ++i)
                {
                    target = Unit::GetUnit((*m_creature),(*i)->getUnitGuid());
                    if(target && target->isAlive() && target->HasAura(SP_SHADOW_MARK))
                        DoCast(target, Regular ? SP_SHADOW_VOLLEY : H_SP_SHADOW_VOLLEY);
                }*/
                DoCast(m_creature->getVictim(), Regular ? SP_SHADOW_VOLLEY : H_SP_SHADOW_VOLLEY);
                VolleyTimer = 10000+rand()%10000;
            }
            else VolleyTimer -= diff;
        }
        else
        {
            if(DrainTimer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
                if(target) DoCast(target, Regular ? SP_DRAIN_LIFE : H_SP_DRAIN_LIFE);
                DrainTimer = 10000+rand()%10000;
            }
            else DrainTimer -= diff;

            /*if(FrenzyTimer < diff)
            { 
                FrenzyTimer = 30000;
            }
            else FrenzyTimer -= diff;*/
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_mob_gothik_trainee_addAI(Creature* pCreature)
{
    return new mob_gothik_trainee_addAI(pCreature);
}

CreatureAI* GetAI_mob_gothik_dk_addAI(Creature* pCreature)
{
    return new mob_gothik_dk_addAI(pCreature);
}

CreatureAI* GetAI_mob_gothik_rider_addAI(Creature* pCreature)
{
    return new mob_gothik_rider_addAI(pCreature);
}

CreatureAI* GetAI_boss_gothik(Creature* pCreature)
{
    return new boss_gothikAI(pCreature);
}

void AddSC_boss_gothik()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gothik";
    newscript->GetAI = &GetAI_boss_gothik;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_gothik_trainee";
    newscript->GetAI = &GetAI_mob_gothik_trainee_addAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_gothik_dk";
    newscript->GetAI = &GetAI_mob_gothik_dk_addAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_gothik_rider";
    newscript->GetAI = &GetAI_mob_gothik_rider_addAI;
    newscript->RegisterSelf();
}
