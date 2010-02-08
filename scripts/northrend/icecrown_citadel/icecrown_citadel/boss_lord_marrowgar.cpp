#include "precompiled.h"
#include "def_spire.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        //Abilities
        SPELL_SABER_LASH_N                      = 71021,
        SPELL_COLD_FLAME_N                      = 69146,
        SPELL_BONE_STRIKE_N                     = 69057,
        SPELL_BONE_STORM_N                      = 69076,

        SPELL_SABER_LASH_H                      = 70814,
        SPELL_COLD_FLAME_H                      = 70825,
        SPELL_BONE_STRIKE_H                     = 69057,
        SPELL_BONE_STORM_H                      = 69076,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint8 stage;
    uint32 m_uiSaberLash_Timer;
    uint32 m_uiColdFlame_Timer;
    uint32 m_uiBoneStrike_Timer;
    bool m_uiBoneStorm_Timer;
    uint32 m_uiBerserk_Timer;
    uint8 health;

    void Reset()
    {
    stage = 0;
    health = 100;
    m_uiSaberLash_Timer = 10000;
    m_uiColdFlame_Timer = 25000;
    m_uiBoneStrike_Timer = urand(17000,29000);
    m_uiBoneStorm_Timer = false;
    m_uiBerserk_Timer = 600000;

        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    if (m_uiBoneStrike_Timer < diff)
                    { if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_BONE_STRIKE_N :SPELL_BONE_STRIKE_H);
                    m_uiBoneStrike_Timer=urand(17000,29000);
                    } else m_uiSaberLash_Timer -= diff;
                    break;}

            case 1: {
                    if (!m_uiBoneStorm_Timer)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_BONE_STORM_N :SPELL_BONE_STORM_H);
                    m_uiBoneStorm_Timer = true;
                    }
                    break;}
                 }

                    if (m_uiSaberLash_Timer < diff)
                    {DoCast(m_creature->getVictim(), Regular ? SPELL_SABER_LASH_N :SPELL_SABER_LASH_H);
                    m_uiSaberLash_Timer=urand(10000,15000);
                    } else m_uiSaberLash_Timer -= diff;

                    if (m_uiColdFlame_Timer < diff)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_COLD_FLAME_N :SPELL_COLD_FLAME_H);
                    m_uiColdFlame_Timer=urand(25000,35000);
                    } else m_uiColdFlame_Timer -= diff;

        health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
        if (health <= 50) stage = 1;

        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
//            m_uiBerserk_Timer = m_bIsRegularMode ? 300000 : 180000;
        }
        else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrowgar";
    newscript->GetAI = &GetAI_boss_lord_marrowgar;
    newscript->RegisterSelf();
}
