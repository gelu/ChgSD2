
#include "precompiled.h"
#include "def_ulduar.h"

#define SP_HEAVENS              64487
#define SP_BIG_BANG             64443
#define H_SP_BIG_BANG           64584
#define SP_QUANTUM_STRIKE       64395
#define H_SP_QUANTUM_STRIKE     64592
#define SP_ENRAGE               47008

struct MANGOS_DLL_DECL boss_algalonAI : public ScriptedAI
{
    boss_algalonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint32 QuantumStrikeTimer;
    uint32 EnrageTimer;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_ALGALON, NOT_STARTED);
        QuantumStrikeTimer = 4000 + rand()%10000;
        EnrageTimer = 360000; //6 minutes
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_ALGALON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_ALGALON, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(QuantumStrikeTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_QUANTUM_STRIKE : H_SP_QUANTUM_STRIKE);
            QuantumStrikeTimer = 4000 + rand()%10000;
        }
        else QuantumStrikeTimer -= diff;

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SP_ENRAGE, true);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_algalon(Creature* pCreature)
{
    return new boss_algalonAI(pCreature);
}

void AddSC_boss_algalon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_algalon";
    newscript->GetAI = &GetAI_boss_algalon;
    newscript->RegisterSelf();
}
