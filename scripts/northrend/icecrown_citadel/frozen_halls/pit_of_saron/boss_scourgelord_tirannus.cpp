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

struct MANGOS_DLL_DECL boss_scourgelord_tirannusAI : public ScriptedAI
{
    boss_scourgelord_tirannusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_TIRANNUS, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_TIRANNUS, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_TIRANNUS, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_scourgelord_tirannus(Creature* pCreature)
{
    return new boss_scourgelord_tirannusAI(pCreature);
}


void AddSC_boss_scourgelord_tirannus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_scourgelord_tirannus";
    newscript->GetAI = &GetAI_boss_scourgelord_tirannus;
    newscript->RegisterSelf();

}
