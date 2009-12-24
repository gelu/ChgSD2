#include "precompiled.h"
#include "def_culling_of_stratholme.h"

enum
{
    SPELL_COURSE                 = 52772,
    SPELL_TIME_STOP             = 58848,
    SPELL_TIME_WARP            = 52766,
    SPELL_SPIKE_N                  = 52771,
    SPELL_SPIKE_H                  = 58830,

    SAY_EPOCH_INTRO        = -1594116,
    SAY_EPOCH_AGGRO      = -1594118,   
    SAY_EPOCH_DEATH       = -1594119,  
    SAY_EPOCH_SLAY01      = -1594120, 
    SAY_EPOCH_SLAY02      = -1594121, 
    SAY_EPOCH_SLAY03      = -1594122,
    SAY_EPOCH_WARP01    = -1594123, 
    SAY_EPOCH_WARP02     = -1594124, 
    SAY_EPOCH_WARP03     = -1594125
};

struct MANGOS_DLL_DECL boss_lord_epochAI : public ScriptedAI
{
   boss_lord_epochAI(Creature *c) : ScriptedAI(c)
   {
        m_bIsHeroic = !c->GetMap()->IsRegularDifficulty();
        Reset();
   }

uint32 Step;
uint32 Steptim;
uint32 Intro;
bool m_bIsHeroic;
uint32 Spike_Timer;
uint32 Warp_Timer;
uint32 Stop_Timer;
uint32 Course_Timer;

   void Reset() 
   {
     Intro = 0;
     Step = 1;
     Steptim = 26000;
     Course_Timer = 9300;
     Stop_Timer = 21300;
     Warp_Timer = 25300;
     Spike_Timer = 5300;
   }
   
   void Aggro(Unit* who)
{
   DoScriptText(SAY_EPOCH_AGGRO, m_creature);
}

   void JustDied(Unit *killer)
    {
       DoScriptText(SAY_EPOCH_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_EPOCH_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_EPOCH_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_EPOCH_SLAY03, m_creature); break;
        }
    }

   void UpdateAI(const uint32 diff)
   {
        if(Intro == 0)
          {
             switch(Step)
                {
                 case 1:
                      DoScriptText(SAY_EPOCH_INTRO, m_creature);
                      ++Step;
                      Steptim = 26000;
                      break;
                 case 3:
                      m_creature->setFaction(14);
                      ++Step;
                      Steptim = 1000;
                      break;
                  }
              }else return;

	   if (Steptim <= diff)
	   {
		   ++Step;
		   Steptim = 330000;
	   } Steptim -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

DoMeleeAttackIfReady();

if (Course_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_COURSE);

            Course_Timer = 9300;
        }else Course_Timer -= diff;

if (Spike_Timer < diff)
        {

            DoCast(m_creature->getVictim(),m_bIsHeroic ? SPELL_SPIKE_H : SPELL_SPIKE_N);

            Spike_Timer = 5300;
        }else Spike_Timer -= diff;

if (Stop_Timer < diff)
        {
             if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_TIME_STOP);

            Stop_Timer = 21300;
        }else Stop_Timer -= diff;

if (Warp_Timer < diff)
        {
             if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_TIME_WARP);
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_EPOCH_WARP01, m_creature); break;
            case 1: DoScriptText(SAY_EPOCH_WARP02, m_creature); break;
            case 2: DoScriptText(SAY_EPOCH_WARP03, m_creature); break;
        }

            Warp_Timer = 25300;
        }else Warp_Timer -= diff;

  }
};

CreatureAI* GetAI_boss_lord_epoch(Creature *_Creature)
{
    boss_lord_epochAI* lord_epochAI = new boss_lord_epochAI(_Creature);
    return (CreatureAI*)lord_epochAI;
};

void AddSC_boss_lord_epoch()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_lord_epoch";
    newscript->GetAI = &GetAI_boss_lord_epoch;
    newscript->RegisterSelf();
}
