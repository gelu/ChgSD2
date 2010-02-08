#include "precompiled.h"
#include "def_pit.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        //Abilities
        SPELL_FEAR                              = 68950
};

struct MANGOS_DLL_DECL boss_forgemaster_gafrostAI : public ScriptedAI
{
    boss_forgemaster_gafrostAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_GAFROST, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_GAFROST, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_GAFROST, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_forgemaster_gafrost(Creature* pCreature)
{
    return new boss_forgemaster_gafrostAI(pCreature);
}


void AddSC_boss_forgemaster_gafrost()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_forgemaster_gafrost";
    newscript->GetAI = &GetAI_boss_forgemaster_gafrost;
    newscript->RegisterSelf();

}
