/**
 *
 * @File : sd2p_npc_guild_guard.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 13/06/2009
 *
 * @Version : 2.1
 *
 * @Synopsis : Gardiens des QG de guilde :
 *                 - Attaque les joueurs n'appartenant pas a la guilde correspondante.
 *                 - Teleporte les cadavres a des destinations random.
 *
 **/



#include "precompiled.h"
#include "sd2p_sc_npc_guild_guard.h"

using namespace SD2P_NAMESPACE;
using namespace SD2P_NAMESPACE::SD2P_GUILD_GUARD_NAMESPACE;


enum
{
    SPELL_DEATH             = 5,
    SPELL_DEATHALL          = 35354,

    MSG_AGGRO               = -2000000,
    MSG_DEATH               = -2000001,
    MSG_DEATHALL_PREVENT    = -2000002,
    MSG_DEATHALL            = -2000003,
    MSG_KILL_PLAYER         = -2000004,
};


struct MANGOS_DLL_DECL npc_guild_guard_base_ai : public ScriptedAI
{
    npc_guild_guard_base_ai(Creature * pCreature)
        : ScriptedAI(pCreature)
    {
        Reset();
    }


    void Reset(void)
    {
        m_creature->setFaction(35);
    }

    void Aggro(Unit *)
    {
        m_creature->setFaction(14);
    }

    void MoveInLineOfSight(Unit * pWho)
    {
        if (!m_creature->IsHostileTo(pWho))
        {
            if (pWho->GetTypeId() != TYPEID_PLAYER)
                return;

            if (Guard const * pGuard = GetGuard(m_creature->GetEntry()))
            {
                for (Guard::GuildsList_t::const_iterator It = pGuard->m_lGuilds.begin(); It != pGuard->m_lGuilds.end(); ++It)
                    if (*It == ((Player*)pWho)->GetGuildId())
                        return;
            }
        }

        if( !m_creature->getVictim() && pWho->isTargetableForAttack() && pWho->isInAccessablePlaceFor(m_creature))
        {
            if (m_creature->GetDistanceZ(pWho) > CREATURE_Z_ATTACK_RANGE)
                return;

            float attackRadius = m_creature->GetAttackDistance(pWho);
            if(m_creature->IsWithinDistInMap(pWho, attackRadius) && m_creature->IsWithinLOSInMap(pWho) )
            {
                pWho->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                AttackStart(pWho);
            }
        }
    }

    void KilledUnit(Unit * pWho)
    {
        if (VGuardDest.empty() || pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (Player * pPlayer = ((Player*)pWho)->GetSession()->GetPlayer())
        {
            uint32 DestId = VGuardDest[rand() % VGuardDest.size()];
            if (Destination const * pDest = GetDestination(DestId))
                pPlayer->TeleportTo(pDest->m_Map, pDest->m_X, pDest->m_Y, pDest->m_Z, pDest->m_Orient);
            else
                outstring_log("SD2P >> [Gardien Guilde] Destination introuvable (DestID: %u).", DestId);
        }
    }
};

struct MANGOS_DLL_DECL npc_guild_guard_ai : public npc_guild_guard_base_ai
{
    npc_guild_guard_ai(Creature * pCreature)
        : npc_guild_guard_base_ai(pCreature)
    {
        Reset();
    }

    uint32 Death_Timer;
    uint32 DeathAll_Timer;
    bool   IsPhaseTwo;


    void Reset(void)
    {
        npc_guild_guard_base_ai::Reset();
    
        Death_Timer = 6000;
        DeathAll_Timer = 3000;
        IsPhaseTwo = false;
    }

    void Aggro(Unit * pWho)
    {
        npc_guild_guard_base_ai::Aggro(pWho);
    
        DoScriptText(MSG_AGGRO, m_creature);
    }

    void UpdateAI(const uint32 Diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!IsPhaseTwo)
        {
            if (Death_Timer < Diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATH);
                Death_Timer = 5000;
            } else Death_Timer -= Diff;
        }
        else
        {
            if (DeathAll_Timer < Diff)
            {
                DoScriptText(MSG_DEATHALL, m_creature);
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATHALL);
                DeathAll_Timer = 5000;
            } else DeathAll_Timer -= Diff;
        }

        if (m_creature->GetHealthPercent() < 30 && !IsPhaseTwo)
        {
            DoScriptText(MSG_DEATHALL_PREVENT, m_creature);
            IsPhaseTwo = true;
        }

        DoMeleeAttackIfReady();
    }

    void KilledUnit(Unit * pWho)
    {
        npc_guild_guard_base_ai::KilledUnit(pWho);

        if (IsPhaseTwo) return;
        DoScriptText(MSG_KILL_PLAYER, m_creature, pWho);
    }

    void JustDied(Unit*)
    {
        DoScriptText(MSG_DEATH, m_creature);
    }
};

CreatureAI* GetAI_npc_guild_guard(Creature * pCreature)
{
    return new npc_guild_guard_ai(pCreature);
}


void AddSC_npc_guild_guard(void)
{
    Script * newscript;

    newscript = new Script;
    newscript->Name="sd2p_npc_guild_guard";
    newscript->GetAI = &GetAI_npc_guild_guard;
    newscript->RegisterSelf();
}
