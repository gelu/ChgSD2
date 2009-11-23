
#include "precompiled.h"
#include "def_ulduar.h"

#define SP_ENRAGE           26662

#define SP_FROZEN_BLOWS     62478
#define H_SP_FROZEN_BLOWS   63512
#define SP_FREEZE           62469

#define SAY_AGGRO   -1603012
#define SAY_DEATH   -1603013
#define SAY_SLAY01  -1603014
#define SAY_SLAY02  -1603015
#define SAY_FLASH_FREEZE    -1603016
#define SAY_FROZEN_BLOWS    -1603017
#define SOUND_FROZEN_BLOWS  15556

struct MANGOS_DLL_DECL boss_hodirAI : public ScriptedAI
{
    boss_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    
    uint64 SpeedKillTimer;
    uint64 EnrageTimer;
    uint64 FlashFreezeTimer;
    uint64 FrozenBlowsTimer;
    uint64 FreezeTimer;

    void Reset()
    {
        SpeedKillTimer = 180000;
        EnrageTimer = 480000;
        FlashFreezeTimer = 50000;
        FrozenBlowsTimer = 60000;
        FreezeTimer = 40000;

        if(pInstance) pInstance->SetData(TYPE_HODIR, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_HODIR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) 
        {
            if(SpeedKillTimer > 0)
                pInstance->SetData(TYPE_HODIR_HARD, DONE);
            else
                pInstance->SetData(TYPE_HODIR, DONE);
        }
        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *who)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY01, m_creature);
        else
            DoScriptText(SAY_SLAY02, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        SpeedKillTimer -= diff;

        if(FlashFreezeTimer < diff)
        {
            DoScriptText(SAY_FLASH_FREEZE, m_creature);
            FlashFreezeTimer = 60000;
        }
        else FlashFreezeTimer -= diff;

        if(FrozenBlowsTimer < diff)
        {
            DoPlaySoundToSet(m_creature, SOUND_FROZEN_BLOWS);
            DoCast(m_creature, Regular ? SP_FROZEN_BLOWS : H_SP_FROZEN_BLOWS);
            FrozenBlowsTimer = 60000;
        }
        else FrozenBlowsTimer -= diff;

        if(FreezeTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target) DoCast(target, SP_FREEZE);
            FreezeTimer = 60000;
        }
        else FreezeTimer -= diff;

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SP_ENRAGE);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();
}
