/* 
SDName: Boss malygos
SDAuthor: Easy
SD%Complete:
SDComment:
SDCategory:
Script Data End */

#include "precompiled.h"
#include "eye_of_eternity.h"

#define SPELL_ARCANE_BREATH_N                        56272
#define SPELL_ARCANE_BREATH_H                        60072
#define SPELL_ARCANE_PULSE                           57432
#define SPELL_ARCANE_STORM_1                         57459
#define SPELL_ARCANE_STORM_2                         61693
#define SPELL_ARCANE_STORM_3                         61694

#define SAY_DEATH                                  -1616007
#define SAY_PHASE1                                 -1616013
#define SAY_PHASE2                                 -1616018
#define SAY_PHASE3                                 -1616024

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI
{
    boss_malygosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

    uint32 phase;
	uint32 SPELL_ARCANE_BREATH_Timer;
    uint32 SPELL_ARCANE_STORM_Timer;
	uint32 SPELL_ARCANE_PULSE_Timer;

    void Reset()
    {
		phase = 0;
		SPELL_ARCANE_BREATH_Timer = 3000;
		SPELL_ARCANE_STORM_Timer = 1000;
		SPELL_ARCANE_PULSE_Timer = 1500;
    }

    void UpdateAI(const uint32 uiDiff)
    {
		if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 10){
            if (phase != 3)
				DoScriptText(SAY_PHASE3, m_creature);
			phase = 3;
        }else if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) <= 50){
			if (phase != 2)
				DoScriptText(SAY_PHASE2, m_creature);
            phase = 2;
		}else{
			if (phase != 1)
				DoScriptText(SAY_PHASE1, m_creature);
			phase = 1;
		}
		
    	if (SPELL_ARCANE_BREATH_Timer <= uiDiff)
        {
			DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_BREATH_N : SPELL_ARCANE_BREATH_H);
            SPELL_ARCANE_BREATH_Timer = 50000;
        } else SPELL_ARCANE_BREATH_Timer -= uiDiff;

		if (SPELL_ARCANE_PULSE_Timer <= uiDiff && phase > 1)
        {
			DoCast(m_creature, SPELL_ARCANE_PULSE);
				SPELL_ARCANE_PULSE_Timer = 20000;
        } else SPELL_ARCANE_PULSE_Timer -= uiDiff;
		
		if (SPELL_ARCANE_STORM_Timer <= uiDiff)
        {
			switch (phase)
			{
				case 1: DoCast(m_creature, SPELL_ARCANE_STORM_1); break;
				case 2: DoCast(m_creature, SPELL_ARCANE_STORM_1); break;
				case 3: DoCast(m_creature, SPELL_ARCANE_STORM_1); break;
			}
            SPELL_ARCANE_STORM_Timer = 3000;
        } else SPELL_ARCANE_STORM_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
		if (m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, DONE);
    }
    void KilledUnit(Unit *victim)
    {
        if (victim == m_creature)
            return;
    }
};

CreatureAI* GetAI_boss_malygos(Creature* pCreature)
{
    return new boss_malygosAI (pCreature);
}

void AddSC_boss_malygos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_malygos";
    newscript->GetAI = &GetAI_boss_malygos;
    newscript->RegisterSelf();
}
