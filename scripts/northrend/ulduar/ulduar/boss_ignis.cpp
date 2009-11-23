

#include "precompiled.h"
#include "def_ulduar.h"


#define SP_FLAME_JETS       62680
#define H_SP_FLAME_JETS     63472
#define SP_SCORCH           62546
#define H_SP_SCORCH         63474
#define SP_SLAG_POT         62717
#define H_SP_SLAG_POT       63477
#define SP_STRENGTH_OF_THE_CREATOR 64473

#define CR_CONSTRUCT        34085

struct MANGOS_DLL_DECL boss_ignisAI : public ScriptedAI
{
    boss_ignisAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    //Timers
    uint32 ScorchTimer;
    uint32 SlagPotTimer;
    uint32 FlameJetsTimer;
    uint32 SummonTimer;

    void Reset()
    {
        ScorchTimer = 25000;
        SlagPotTimer = 15000;
        FlameJetsTimer = 20000;
        SummonTimer = 40000 + rand()%20000;

        if(pInstance) pInstance->SetData(TYPE_IGNIS, NOT_STARTED);
    }

    void Aggro(Unit* who) 
    {
        if(pInstance) pInstance->SetData(TYPE_IGNIS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_IGNIS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (ScorchTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_SCORCH : H_SP_SCORCH);
            ScorchTimer = 25000;
        }
        else ScorchTimer -= diff;

        if (FlameJetsTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_FLAME_JETS : H_SP_FLAME_JETS);
            FlameJetsTimer = 20000+rand()%10000;
        }
        else FlameJetsTimer -= diff;

        /*if (SlagPotTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, Regular ? SP_SLAG_POT : H_SP_SLAG_POT);
            SlagPotTimer = 15000;
        }
        else SlagPotTimer -= diff;*/

        if (SummonTimer < diff)
        {
            Creature *construct;
            construct = DoSpawnCreature(CR_CONSTRUCT, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(construct && target && target->isAlive())
            {
                construct->AddThreat(target, 1.0f);
            }
            SummonTimer = 60000+rand()%30000;
        }
        else SummonTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ignisAI(Creature* pCreature)
{
    return new boss_ignisAI(pCreature);
}

void AddSC_boss_ignis()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ignis";
    newscript->GetAI = &GetAI_boss_ignisAI;
    newscript->RegisterSelf();
}
