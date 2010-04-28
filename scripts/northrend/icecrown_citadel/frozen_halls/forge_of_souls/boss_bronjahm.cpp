#include "precompiled.h"
#include "def_forge.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_SOUL_FRAGMENT                       = 36535,
        //Abilities
        SPELL_MAGIC_BANE                        = 68793,
        SPELL_CORRUPT_SOUL                      = 68839,
        SPELL_CONSUME_SOUL                      = 68858,
        SPELL_TELEPORT                          = 68988,
        SPELL_SOULSTORM_N                       = 68872,
        SPELL_SOULSTORM_2                       = 68921,
        SPELL_FEAR                              = 68950,
        SPELL_SHADOW_BOLT                       = 70043,
};

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_soul_fragmentAI : public ScriptedAI
{
    mob_soul_fragmentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                if (Creature* pBronjahm = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BRONJAHM))))
                {
                    float fDistance = m_creature->GetDistance2d(pBronjahm);
                    if (fDistance <= 2)
                    {
//                        ((boss_bronjahmAI*)pBronjahm->AI())->SoulFragmentHit();
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
                }
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

    void JustDied(Unit* pKiller)
    {
//        DoCast(m_creature, SPELL_SPLASH);
    }
};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

CreatureAI* GetAI_mob_soul_fragment(Creature* pCreature)
{
    return new mob_soul_fragmentAI (pCreature);
}


void AddSC_boss_bronjahm()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_bronjahm";
    newscript->GetAI = &GetAI_boss_bronjahm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soul_fragment";
    newscript->GetAI = &GetAI_mob_soul_fragment;
    newscript->RegisterSelf();

}
