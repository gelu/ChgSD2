
#include "precompiled.h"
#include "def_ulduar.h"

#define SP_BERSERK          47008
#define SP_TERRIFY          64386
#define SP_GUARDIAN_SWARM   64396
#define SP_SONIC_SCREECH    64422
#define H_SP_SONIC_SCREECH  64688
#define SP_SENTINEL_BLAST   64389
#define H_SP_SENTINEL_BLAST 64678

#define CR_FERAL_DEFENDER       34035
#define CR_SWARMING_GUARDIAN    34034


struct MANGOS_DLL_DECL boss_auriayaAI : public ScriptedAI
{
    boss_auriayaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;
    uint32 EnrageTimer;
    uint32 ScreechTimer;
    uint32 TerrifyTimer;
    uint32 SwarmTimer;
    uint32 DefenderTimer;

    void Reset()
    {
        EnrageTimer = 600000; //10 minutes
        DefenderTimer = 60000;
        ScreechTimer = 20000 + rand()%10000;
        TerrifyTimer = 30000 + rand()%15000;
        SwarmTimer = 10000;

        if(pInstance) pInstance->SetData(TYPE_AURIAYA, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_AURIAYA, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_AURIAYA, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DefenderTimer < diff)
        {
            Unit *defender = DoSpawnCreature(CR_FERAL_DEFENDER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(defender && target && target->isAlive())
                defender->AddThreat(target, 1.0f);
            DefenderTimer = 45000;
        }
        else DefenderTimer -= diff;

        if (ScreechTimer < diff)
        {
            DoCast(m_creature, Regular ? SP_SONIC_SCREECH : H_SP_SONIC_SCREECH);
            ScreechTimer = 25000 + rand()%15000;
        }
        else ScreechTimer -= diff;

        if (SwarmTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
            {
                DoCast(target, SP_GUARDIAN_SWARM);
                /*Creature *add = NULL;
                for(int i=0; i<16; i++)
                {
                    add = DoSpawnCreature(CR_SWARMING_GUARDIAN, 
                        target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0,
                        TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    if(add) add->AddThreat(target, 1.0f);
                }*/
            }

            SwarmTimer = 20000 + rand()%20000;
        }
        else SwarmTimer -= diff;

        if (TerrifyTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_TERRIFY);
            TerrifyTimer = 30000 + rand()%15000;
        }
        else TerrifyTimer -= diff;

        if (EnrageTimer < diff)
        {
            DoCast(m_creature, SP_BERSERK);
            EnrageTimer = 20000 + rand()%20000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_auriaya(Creature* pCreature)
{
    return new boss_auriayaAI(pCreature);
}

void AddSC_boss_auriaya()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_auriaya";
    newscript->GetAI = &GetAI_boss_auriaya;
    newscript->RegisterSelf();
}
