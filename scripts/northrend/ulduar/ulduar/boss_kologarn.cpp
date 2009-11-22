
#include "precompiled.h"
#include "def_ulduar.h"

#define SP_OVERHEAD_SMASH_2H    63356 //64710
#define SP_OVERHEAD_SMASH_1H    63573 //64715
#define SP_STONE_SHOUT          64004
#define SP_PETRIFYING_BREATH    62030
#define H_SP_PETRIFYING_BREATH  63980
#define SP_FOCUSED_EYEBEAM      63346
#define H_SP_FOCUSED_EYEBEAM    63976

#define SP_BERSERK              64238

#define SP_SHOCKWAVE            63783
#define H_SP_SHOCKWAVE          63982

#define SP_STONE_GRIP           62056
#define H_SP_STONE_GRIP         63985

#define CR_RUBBLE               33768

#define LEFT    0x01 //01
#define RIGHT   0x02 //10


struct MANGOS_DLL_DECL boss_kologarnAI : public Scripted_NoMovementAI
{
    boss_kologarnAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    uint32 BreathTimer;
    uint32 SmashTimer;
    uint32 EnrageTimer;
    uint8 arms;
    uint32 LeftArmRespawn;
    uint32 RightArmRespawn;

    void Reset()
    {
        BreathTimer = 5000;
        SmashTimer = 10000+rand()%5000;
        EnrageTimer = 600000;
        arms = LEFT & RIGHT;
        if(pInstance)
        {
            pInstance->SetData(TYPE_KOLOGARN, NOT_STARTED);
            Creature *arm = NULL;
            arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_LEFT_ARM));
            if(arm) arm->Respawn();
            arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_RIGHT_ARM));
            if(arm) arm->Respawn();
        }
        LeftArmRespawn = 30000;
        RightArmRespawn = 30000;
    }

    void ArmDied(int arm)
    {
        arms = arms & !arm;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance)
        {
            pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);
            /*Creature *arm = NULL;
            arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_LEFT_ARM));
            if(arm) arm->Respawn();
            arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_RIGHT_ARM));
            if(arm) arm->Respawn();*/
        }
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_KOLOGARN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(BreathTimer < diff)
        {
            if(!m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
                DoCast(m_creature->getVictim(), Regular ? SP_PETRIFYING_BREATH : H_SP_PETRIFYING_BREATH);
            BreathTimer = 5000;
        }
        else BreathTimer -= diff;

        if(pInstance)
        {
            if((arms & LEFT) != LEFT)
            {
                if(LeftArmRespawn < diff)
                {
                    Creature *arm = NULL;
                    arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_LEFT_ARM));
                    if(arm) arm->Respawn();
                }
                else LeftArmRespawn -= diff;
            }

            if((arms & RIGHT) != RIGHT)
            {
                if(RightArmRespawn < diff)
                {
                    Creature *arm = NULL;
                    arm = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN_RIGHT_ARM));
                    if(arm) arm->Respawn();
                }
                else RightArmRespawn -= diff;
            }
        }

        if(SmashTimer < diff)
        {
            if(arms== (LEFT & RIGHT)) DoCast(m_creature->getVictim(), SP_OVERHEAD_SMASH_2H);
            if(arms==LEFT || arms==RIGHT) DoCast(m_creature->getVictim(), SP_OVERHEAD_SMASH_1H);
            //if(arms==0) DoCast(m_creature->getVictim(), SP_STONE_SHOUT);
            SmashTimer = 10000 + rand()%5000;
        }
        else SmashTimer -= diff;

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SP_BERSERK);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_kologarn_left_armAI : public Scripted_NoMovementAI
{
    boss_kologarn_left_armAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    uint32 ShockwaveTimer;

    void Reset()
    {
        ShockwaveTimer = 20000 + rand()%10000;
    }

    void Aggro(Unit *who) {}

    void JustDied(Unit *killer)
    {
        Creature *kologarn = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN));
        if(kologarn && kologarn->isAlive())
        {
            kologarn->DealDamage(kologarn, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            ((boss_kologarnAI *)kologarn->AI())->ArmDied(LEFT);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(ShockwaveTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_SHOCKWAVE);
            ShockwaveTimer = 20000 + rand()%10000;
        }
        else ShockwaveTimer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_kologarn_right_armAI : public Scripted_NoMovementAI
{
    boss_kologarn_right_armAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    uint32 GripThreshold;
    uint32 GripTimer;
    Unit *GrippedPlayer;
    bool Gripped;

    void Reset()
    {
        GrippedPlayer = NULL;
        Gripped = false;
        GripTimer = 10000 + rand()%10000;
        GripThreshold = Regular ? 100000 : 480000;
    }

    void Aggro(Unit *who) {}

    void JustDied(Unit *killer)
    {
        Creature *kologarn = (Creature *) Unit::GetUnit((*m_creature), pInstance->GetData64(NPC_KOLOGARN));
        if(kologarn && kologarn->isAlive())
        {
            kologarn->DealDamage(kologarn, m_creature->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            ((boss_kologarnAI *)kologarn->AI())->ArmDied(RIGHT);
        }

        //summon rubbles
        Creature *rubble;
        Unit *target;
        for(int i=0; i<5; i++)
        {
            rubble = DoSpawnCreature(CR_RUBBLE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(rubble && target && target->isAlive())
                rubble->AddThreat(target, 1.0f);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &dmg)
    {
        if(Gripped) GripThreshold -= dmg;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(Gripped)
        {
            if(!GrippedPlayer || GrippedPlayer->isDead() || GripThreshold < 0) 
            {
                //release gripped player
                //GrippedPlayer->RemoveAurasDueToSpell(SP_STONE_GRIP);
                GrippedPlayer = NULL;
                GripTimer = 10000 + rand()%10000;
                Gripped = false;
                GripThreshold = Regular ? 100000 : 400000;
            }
        }
        else
        {
            if(GripTimer < diff)
            {
                GrippedPlayer = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(GrippedPlayer && GrippedPlayer->isAlive())
                {
                    //grip player
                    //DoCast(GrippedPlayer, SP_STONE_GRIP);
                    Gripped = true;
                }
                else GripTimer = 5000;
            }
            else GripTimer -= diff;
        }
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

CreatureAI* GetAI_boss_kologarn_left_arm(Creature* pCreature)
{
    return new boss_kologarn_left_armAI(pCreature);
}

CreatureAI* GetAI_boss_kologarn_right_arm(Creature* pCreature)
{
    return new boss_kologarn_right_armAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_kologarn";
    newscript->GetAI = &GetAI_boss_kologarn;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_kologarn_left_arm";
    newscript->GetAI = &GetAI_boss_kologarn_left_arm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_kologarn_right_arm";
    newscript->GetAI = &GetAI_boss_kologarn_right_arm;
    newscript->RegisterSelf();
}
