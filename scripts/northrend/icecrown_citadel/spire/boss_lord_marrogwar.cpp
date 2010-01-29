#include "precompiled.h"
#include "def_spire.h"

struct MANGOS_DLL_DECL boss_lord_marrogwarAI : public ScriptedAI
{
    boss_lord_marrogwarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_MARROGWAR, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_MARROGWAR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROGWAR, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lord_marrogwar(Creature* pCreature)
{
    return new boss_lord_marrogwarAI(pCreature);
}

void AddSC_boss_lord_marrogwar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrogwar";
    newscript->GetAI = &GetAI_boss_lord_marrogwar;
    newscript->RegisterSelf();
}
