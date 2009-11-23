

#include "precompiled.h"
#include "def_ulduar.h"

#define SP_SUPERCHARGE          61920

//Steelbreaker spells
#define SP_HIGH_VOLTAGE         61890
#define H_SP_HIGH_VOLTAGE       63498
#define SP_FUSION_PUNCH         61903
#define H_SP_FUSION_PUNCH       63493

//Stormcaller Brundir spells
#define SP_CHAIN_LIGHTNING      61879
#define H_SP_CHAIN_LIGHTNING    63479
#define SP_OVERLOAD             61869
#define H_SP_OVERLOAD           63481

//Runemaster Molgeim spells
#define SP_RUNE_OF_DEATH        62269
#define H_SP_RUNE_OF_DEATH      63490
#define CR_LIGHTNING_ELEMENTAL  32958

#define CR_STEELBREAKER     32867
#define CR_BRUNDIR          32857
#define CR_MOLGEIM          32927


struct MANGOS_DLL_DECL iron_councilAI : public ScriptedAI
{
    iron_councilAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
    ScriptedInstance *pInstance;
    bool Regular;
    int CouncilAlive;
    
    void Reset()
    {
        if(pInstance)
        {
            RespawnFriends();
            CouncilAlive = 3;
            pInstance->SetData(TYPE_IRON_COUNCIL, NOT_STARTED);
        }
    }
    
    void EnterCombat(Unit *who)
    {
        if(!who) return;
        if(!pInstance) return;
        
        uint32 entry = m_creature->GetEntry();
        Creature *council;
        
        if(entry!=CR_STEELBREAKER)
        {
            council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_STEELBREAKER));
            if(council && !council->getVictim()) council->AddThreat(who, 0.0f);
        };
        if(entry!=CR_BRUNDIR)
        {
            council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_BRUNDIR));
            if(council && !council->getVictim()) council->AddThreat(who, 0.0f);
        };
        if(entry!=CR_MOLGEIM)
        {
            council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MOLGEIM));
            if(council && !council->getVictim()) council->AddThreat(who, 0.0f);
        };
        
        pInstance->SetData(TYPE_IRON_COUNCIL, IN_PROGRESS);
        
        Aggro(who);
    }
    
    void JustDied(Unit *killer)
    {
        if(!pInstance) return;

        --CouncilAlive;
        if(CouncilAlive <= 0) pInstance->SetData(TYPE_IRON_COUNCIL, DONE);
    }
    
    void RespawnFriends()
    {
        if(!pInstance) return;
        
        Creature *council;
        council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_STEELBREAKER));
        if(council && council->isDead()) council->Respawn();
        council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_BRUNDIR));
        if(council && council->isDead()) council->Respawn();
        council = pInstance->instance->GetCreature(pInstance->GetData64(NPC_MOLGEIM));
        if(council && council->isDead()) council->Respawn();
    }
};


/**************************************
            Steelbreakeer
**************************************/
struct MANGOS_DLL_DECL boss_steelbreakerAI : public iron_councilAI
{
    boss_steelbreakerAI(Creature* pCreature) : iron_councilAI(pCreature)
    {
        Reset();
    }

    uint32 PunchTimer;

    void Reset()
    {
        PunchTimer = 30000;
        //iron_councilAI::Reset();
    }

    void Aggro(Unit *who) 
    {
        DoCast(m_creature, Regular ? SP_HIGH_VOLTAGE : H_SP_HIGH_VOLTAGE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(PunchTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_FUSION_PUNCH : H_SP_FUSION_PUNCH);
            PunchTimer = 30000;
        }
        else PunchTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

/************************************
        Stormcaller Brundir
************************************/
struct MANGOS_DLL_DECL boss_brundirAI : public iron_councilAI
{
    boss_brundirAI(Creature* pCreature) : iron_councilAI(pCreature)
    {
        Reset();
    }

    uint32 LightningTimer;
    uint32 OverloadTimer;

    void Reset()
    {
        LightningTimer = 5000 + rand()%5000;
        OverloadTimer = 60000;
        //iron_councilAI::Reset();
    }

    void Aggro(Unit *who) 
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(OverloadTimer < diff)
        {
            DoCast(m_creature->getVictim(), Regular ? SP_OVERLOAD : H_SP_OVERLOAD);
            OverloadTimer = 60000;
        }
        else OverloadTimer -= diff;

        if(LightningTimer < diff)
        {
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(target && target->isAlive())
                DoCast(target, Regular ? SP_CHAIN_LIGHTNING : H_SP_CHAIN_LIGHTNING);
            LightningTimer = 5000 + rand()%5000;
        }
        else LightningTimer -= diff;

        DoMeleeAttackIfReady();
    }
};


/************************************
        Runemaster Molgeim
************************************/
struct MANGOS_DLL_DECL boss_molgeimAI : public iron_councilAI
{
    boss_molgeimAI(Creature* pCreature) : iron_councilAI(pCreature)
    {
        Reset();
    }

    uint32 ElementalTimer;
    uint32 DeathRuneTimer;

    void Reset()
    {
        ElementalTimer = 15000 + rand()%10000;
        DeathRuneTimer = 15000;
        //iron_councilAI::Reset();
    }

    void Aggro(Unit *who) 
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*if(ElementalTimer < diff)
        {
            Creature *elem = DoSpawnCreature(CR_LIGHTNING_ELEMENTAL, 0,0,0,0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
            if(elem && target && target->isAlive())
                elem->AddThreat(target, 1.0f);
            ElementalTimer = 15000 + rand()%10000;
        }
        else ElementalTimer -= diff;*/

        if(CouncilAlive < 3)
        {
            if(DeathRuneTimer < diff)
            {
                Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                if(target) DoCast(target, Regular ? SP_RUNE_OF_DEATH : H_SP_RUNE_OF_DEATH);
            }
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_steelbreaker(Creature* pCreature)
{
    return new boss_steelbreakerAI(pCreature);
}

CreatureAI* GetAI_boss_brundir(Creature* pCreature)
{
    return new boss_brundirAI(pCreature);
}

CreatureAI* GetAI_boss_molgeim(Creature* pCreature)
{
    return new boss_molgeimAI(pCreature);
}

void AddSC_boss_iron_council()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_steelbreaker";
    newscript->GetAI = &GetAI_boss_steelbreaker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_brundir";
    newscript->GetAI = &GetAI_boss_brundir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_molgeim";
    newscript->GetAI = &GetAI_boss_molgeim;
    newscript->RegisterSelf();
}
