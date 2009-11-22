
#include "precompiled.h"
#include "def_ulduar.h"

#define SP_TANTRUM              62776
#define SP_SEARING_LIGHT        63018
#define H_SP_SEARING_LIGHT      65121
#define SEARING_LIGHT_EFFECT    63023
#define H_SEARING_LIGHT_EFFECT  65120
#define SP_ENRAGE               47008
#define SP_GRAVITY              63024
#define H_SP_GRAVITY            64234


#define CR_PUMMELER         33344
#define CR_SCRAPBOT         33343
#define CR_BOOMBOT          33346

#define SAY_AGGRO       -1603000
#define SAY_DEATH       -1603008
#define SAY_TANTRUM     -1603001
#define SAY_SLAY_01     -1603002
#define SAY_SLAY_02     -1603003
#define SAY_BERSERK     -1603007
#define SAY_ADDS        -1603006
#define SAY_HEART_OPEN  -1603004
#define SAY_HEART_CLOSE -1603005

/*
792.706  64.033 413.632 - 4.823
879.750  64.815 409.804 - 3.816
896.488 -93.018 411.731 - 1.858
791.016 -83.516 409.804 - 0.782
*/

float AddX[4];
float AddY[4];
float AddZ[4];

struct MANGOS_DLL_DECL boss_xt002 : public ScriptedAI
{
    boss_xt002(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        //PummelerCount = Regular ? 1 : 2;
        ScrapbotCount = Regular ? 4 : 6;
        BoombotCount = Regular ? 2 : 4;
        AddX[0] = 792.706; AddY[0] = 64.033; AddZ[0] = 413.632;
        AddX[1] = 879.750; AddY[1] = 64.815; AddZ[1] = 409.804;
        AddX[2] = 896.488; AddY[2] = -93.018; AddZ[2] = 411.731;
        AddX[3] = 791.016; AddY[3] = -83.516; AddZ[3] = 409.804;
        Reset();
    }

    ScriptedInstance *pInstance;
    bool Regular;

    uint32 AddsPhaseTimer;
    bool addsPhase;
    uint32 addsPhaseNumber;
    uint32 NextWaveTimer;

    //uint32 PummelerCount;
    uint32 ScrapbotCount;
    uint32 BoombotCount;

    uint32 TantrumTimer;
    uint32 LightTimer;
    uint32 EnrageTimer;
    uint32 GravityTimer;

    void Reset()
    {
        addsPhase = false;
        AddsPhaseTimer = 30000;
        addsPhaseNumber = 3;

        TantrumTimer = 45000;
        LightTimer = 10000;
        EnrageTimer = 600000;
        GravityTimer = 15000 + rand()%5000;

        if(pInstance) pInstance->SetData(TYPE_XT002, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_XT002, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if(addsPhase) damage += damage;
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_XT002, DONE);

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit *who)
    {
        if(irand(0,1))
            DoScriptText(SAY_SLAY_01, m_creature);
        else
            DoScriptText(SAY_SLAY_02, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(addsPhase)
        {
            if(AddsPhaseTimer < diff)
            {
                addsPhase = false;
                //make boss active and attackable
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                //m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                DoScriptText(SAY_HEART_CLOSE, m_creature);
            }
            else AddsPhaseTimer -= diff;

            if(NextWaveTimer < diff)
            {
                NextWaveTimer = 15000;
                int rnd = irand(0,3);
                /*if( m_creature->GetDistance2d(AddX[rnd], AddY[rnd]) > 220)
                {
                    EnterEvadeMode();
                    return;
                }*/
                int i;
                Creature *add;
                Unit *target;
                if(!Regular || (AddsPhaseTimer > 15000))
                {
                    add = m_creature->SummonCreature(CR_PUMMELER, AddX[rnd], AddY[rnd], AddZ[rnd], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    add->SetActiveObjectState(true);
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if(add && target && target->isAlive())
                        add->AddThreat(target, 1.0f);
                }
                for(i=0; i < ScrapbotCount; i++)
                {
                    add = m_creature->SummonCreature(CR_SCRAPBOT, AddX[rnd], AddY[rnd], AddZ[rnd], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    add->SetActiveObjectState(true);
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if(add && target && target->isAlive())
                        add->AddThreat(target, 1.0f);
                }
                for(i=0; i < BoombotCount; i++)
                {
                    add = m_creature->SummonCreature(CR_BOOMBOT, AddX[rnd], AddY[rnd], AddZ[rnd], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    add->SetActiveObjectState(true);
                    target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                    if(add && target && target->isAlive())
                        add->AddThreat(target, 1.0f);
                }
            }
            else NextWaveTimer -= diff;
        }
        else
        {
            if (addsPhaseNumber && ((m_creature->GetHealth()*100) / m_creature->GetMaxHealth() <= addsPhaseNumber*25))
            {
                addsPhaseNumber--;
                addsPhase = true;
                AddsPhaseTimer = 30000;
                NextWaveTimer = 5000;
                //make boss unattackable and inactive
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->Clear();
                //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                DoScriptText(SAY_HEART_OPEN, m_creature);
            }

            //regular spells
            if(TantrumTimer < diff)
            {
                DoCast(m_creature->getVictim(), SP_TANTRUM);
                TantrumTimer = 45000;
                DoScriptText(SAY_TANTRUM, m_creature);
            }
            else TantrumTimer -= diff;

            if(LightTimer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target && target->isAlive())
                    DoCast(target, Regular ? SP_SEARING_LIGHT : H_SP_SEARING_LIGHT);
                LightTimer = 10000;
            }
            else LightTimer -= diff;

            if(GravityTimer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target && target->isAlive())
                    DoCast(target, Regular ? SP_GRAVITY : H_SP_GRAVITY);
                GravityTimer = 15000 + rand()%5000;
            }
            else GravityTimer -= diff;

            if(EnrageTimer < diff)
            {
                DoCast(m_creature, SP_ENRAGE);
                EnrageTimer = 60000;

                DoScriptText(SAY_BERSERK, m_creature);
            }
            else EnrageTimer -= diff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_xt002(Creature* pCreature)
{
    return new boss_xt002(pCreature);
}

void AddSC_boss_xt002()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_xt002";
    newscript->GetAI = &GetAI_boss_xt002;
    newscript->RegisterSelf();
}
