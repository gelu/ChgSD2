
#include "precompiled.h"
#include "def_ulduar.h"

#define CR_GUARDIAN_OF_YOGG_SARON   33136
#define CR_IMMORTAL_GUARDIAN        33988

struct MANGOS_DLL_DECL boss_yogg_saronAI : public ScriptedAI
{
    boss_yogg_saronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_YOGG_SARON, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_YOGG_SARON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_YOGG_SARON, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

CreatureAI* GetAI_boss_yogg_saron(Creature* pCreature)
{
    return new boss_yogg_saronAI(pCreature);
}

void AddSC_boss_yogg_saron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_yogg_saron";
    newscript->GetAI = &GetAI_boss_yogg_saron;
    newscript->RegisterSelf();
}
