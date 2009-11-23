


#include "precompiled.h"
#include "def_ulduar.h"

#define SP_DEVOURING_FLAME1  63236 //animation?
#define SP_DEVOURING_FLAME   63014
#define H_SP_DEVOURING_FLAME 63816
#define SP_FIREBALL          62796 //only air phase
#define H_SP_FIREBALL        63815
#define SP_FLAME_BUFFET      64016
#define H_SP_FLAME_BUFFET    64023
#define SP_WING_BUFFET       62666
#define SP_BERSERK           47008
#define SP_FLAME_BREATH      63317
#define H_SP_FLAME_BREATH    64021
#define SP_FUSE_ARMOR        64771




struct MANGOS_DLL_DECL boss_razorscaleAI : public ScriptedAI
{
    boss_razorscaleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    //Timers
    uint32 BerserkTimer;
    uint32 DevouringFlameTimer;
    uint32 FlameBuffetTimer;
    uint32 WingBuffetTimer;
    uint32 FlameBreathTimer;
    uint32 FuseArmorTimer;
    uint32 FireballTimer;


    void Reset()
    {
        BerserkTimer = 420000; //7 minutes
        DevouringFlameTimer = 10000+rand()%10000; //every 10 sec
        FlameBuffetTimer = 20000+rand()%15000; //every 10-15 seconds
        WingBuffetTimer = 30000 + rand()%60000; //completely random
        FlameBreathTimer = 15000+rand()%5000; //every 15 sec
        FuseArmorTimer = 10000+rand()%5000; //every 10 seconds
        FireballTimer = 5000 + rand()%5000;

        if(pInstance) pInstance->SetData(TYPE_RAZORSCALE, NOT_STARTED);
    }

    void Aggro(Unit* who) 
    {
        if(pInstance) pInstance->SetData(TYPE_RAZORSCALE, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_RAZORSCALE, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (FuseArmorTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_FUSE_ARMOR);
            FuseArmorTimer = 10000;
        }
        else FuseArmorTimer -= diff;

        if (WingBuffetTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_WING_BUFFET);
            WingBuffetTimer = 30000 + rand()%60000;
        }
        else WingBuffetTimer -= diff;

        if (DevouringFlameTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, Regular ? SP_DEVOURING_FLAME : H_SP_DEVOURING_FLAME);
            DevouringFlameTimer = 9000+rand()%3000;
        }
        else DevouringFlameTimer -= diff;

        if (FlameBreathTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_FLAME_BREATH : H_SP_FLAME_BREATH);
            FlameBreathTimer = 15000 + rand()%5000;
        }
        else FlameBreathTimer -= diff;

        if (FlameBuffetTimer < diff)
        {
            DoCast(m_creature, Regular ? SP_FLAME_BUFFET : H_SP_FLAME_BUFFET, true);
            FlameBuffetTimer = 20000 + rand()%15000;
        }
        else FlameBuffetTimer -= diff;

        if (FireballTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, Regular ? SP_FIREBALL : H_SP_FIREBALL);
            FireballTimer = 4000 + rand()%4000;
        }
        else FireballTimer -= diff;

        if (BerserkTimer < diff)
        {
            DoCast(m_creature, SP_BERSERK);
            BerserkTimer = 30000;
        }
        else BerserkTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_razorscaleAI(Creature* pCreature)
{
    return new boss_razorscaleAI(pCreature);
}

void AddSC_boss_razorscale()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_razorscale";
    newscript->GetAI = &GetAI_boss_razorscaleAI;
    newscript->RegisterSelf();
}
