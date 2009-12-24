#include "precompiled.h"
#include "def_culling_of_stratholme.h"

enum
{
    SPELL_CHAIN_N                 = 52696,
    SPELL_CHAIN_H                 = 58823,
    SPELL_EXPLODED_N        = 52666,
    SPELL_EXPLODED_H        = 58821,
    SPELL_FRENZY                   = 58841,

    SAY_MEATHOOK_SPAWN      = -1594110,
    SAY_MEATHOOK_AGGRO      = -1594111,   
    SAY_MEATHOOK_DEATH       = -1594112,  
    SAY_MEATHOOK_SLAY01      = -1594113, 
    SAY_MEATHOOK_SLAY02      = -1594114, 
    SAY_MEATHOOK_SLAY03      = -1594115
};

struct MANGOS_DLL_DECL boss_meathookAI : public ScriptedAI
{
   boss_meathookAI(Creature *c) : ScriptedAI(c)
   {
        m_bIsHeroic = !c->GetMap()->IsRegularDifficulty();
        Reset();
   }

bool m_bIsHeroic;
uint32 phaseArthas;
uint32 Chain_Timer;
uint32 Exploded_Timer;
uint32 Frenzy_Timer;

   void Reset() 
   {
     phaseArthas = 0;
     Chain_Timer = 6300;
     Exploded_Timer = 9300;
     Frenzy_Timer = 23300;
   }
   
   void Aggro(Unit* who)
{
   DoScriptText(SAY_MEATHOOK_AGGRO, m_creature);
}

   void JustDied(Unit *killer)
    {
       DoScriptText(SAY_MEATHOOK_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_MEATHOOK_SLAY01, m_creature); break;
            case 1: DoScriptText(SAY_MEATHOOK_SLAY02, m_creature); break;
            case 2: DoScriptText(SAY_MEATHOOK_SLAY03, m_creature); break;
        }
    }

   void UpdateAI(const uint32 diff)
   {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

DoMeleeAttackIfReady();

if (Chain_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, m_bIsHeroic ? SPELL_CHAIN_H : SPELL_CHAIN_N);

            Chain_Timer = 6300;
        }else Chain_Timer -= diff;

if (Exploded_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, m_bIsHeroic ? SPELL_EXPLODED_H : SPELL_EXPLODED_N);

            Exploded_Timer = 9300;
        }else Exploded_Timer -= diff;

if (Frenzy_Timer < diff)
        {
                DoCast(m_creature,SPELL_FRENZY);

            Frenzy_Timer = 23300;
        }else Frenzy_Timer -= diff;

  }
};

CreatureAI* GetAI_boss_meathook(Creature *_Creature)
{
    boss_meathookAI* meathookAI = new boss_meathookAI(_Creature);
    return (CreatureAI*)meathookAI;
};

void AddSC_boss_meathook()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_meathook";
    newscript->GetAI = &GetAI_boss_meathook;
    newscript->RegisterSelf();
}
