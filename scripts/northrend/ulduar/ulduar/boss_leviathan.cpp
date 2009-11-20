

#include "precompiled.h"
#include "def_ulduar.h"

#define SP_BATTERING_RAM    62376
#define SP_FLAME_VENTS      62396
#define SP_GATHERING_SPEED  62375

#define SP_ROCKET           62400

#define SAY_AGGRO   -1603009
#define SAY_DEATH   -1603010
#define SAY_SLAY    -1603011




struct MANGOS_DLL_DECL boss_flame_leviathan : public ScriptedAI
{
    boss_flame_leviathan(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;

    uint32 BatteringRamTimer;
    uint32 FlameVentsTimer;
    uint32 RocketTimer;

    void Reset()
    {
        BatteringRamTimer = 15000 + rand()%20000;
        FlameVentsTimer = 20000 + rand()%10000;
        RocketTimer = 1000;

        if(pInstance) pInstance->SetData(TYPE_FLAME_LEVIATHAN, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_FLAME_LEVIATHAN, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_FLAME_LEVIATHAN, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *who)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void DamageTaken(Unit *pDoneBy, uint32 &dmg)
    {
        //компенсируем отсутствие машинок и большое хп босса
        dmg *= 4;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(FlameVentsTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_FLAME_VENTS);
            FlameVentsTimer = 30000 + rand()%20000;
        }
        else FlameVentsTimer -= diff;

        if(BatteringRamTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_BATTERING_RAM);
            BatteringRamTimer = 25000 + rand()%15000;
        }
        else BatteringRamTimer -= diff;

        if(RocketTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            int32 dmg = Regular ? (3000 + rand()%2000) : (2000 + rand()%1200);
            if(target && target->isAlive())
                m_creature->CastCustomSpell(target, SP_ROCKET, &dmg, 0, 0, false);
            RocketTimer = 3000 + rand()%2000;
        }
        else RocketTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathan(pCreature);
}

void AddSC_boss_leviathan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flame_leviathan";
    newscript->GetAI = &GetAI_boss_flame_leviathan;
    newscript->RegisterSelf();
}
