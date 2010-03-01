/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: boss_lord_marrowgar
SD%Complete: 30%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"
enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        MOB_BONE_SPIKE                          = 38711,
        MOB_COLDFLAME                           = 36672,
        //Abilities
        SPELL_SABER_LASH_N                      = 71021,
        SPELL_COLD_FLAME_N                      = 69146,
        SPELL_BONE_STRIKE                       = 69057,
        SPELL_BONE_STORM                        = 69076,
        SPELL_BONE_STRIKE_IMPALE                = 69065,
        SPELL_BONE_STORM_STRIKE_N               = 69075,

        SPELL_SABER_LASH_H                      = 71021,
        SPELL_COLD_FLAME_H                      = 70824,
        SPELL_BONE_STORM_STRIKE_H               = 70835, //h25 - 70836
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
    bool m_uiBoneStorm;
    bool m_uiBoneStorm_Timer;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiBoneStormStrike_Timer;
    uint8 health;

    void Reset()
    {
    stage = 0;
    health = 100;
    m_uiSaberLash_Timer = 10000;
    m_uiColdFlame_Timer = 5000;
    m_uiBoneStormStrike_Timer = 2000;
    m_uiBoneStrike_Timer = urand(17000,29000);
    m_uiBoneStorm = false;
    m_uiBoneStorm_Timer = 40000;
    m_uiBerserk_Timer = 600000;

        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
    }

    uint64 CallGuard(uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(0, 10), fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        return pSummon ? pSummon->GetGUID() : 0;
    }

    uint64 CallSpike(Unit* pPlayer, uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        pPlayer->GetPosition(fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        if (pSummon) pSummon->AddThreat(pPlayer);
        return pSummon ? pSummon->GetGUID() : 0;
    }
/*
    void JustSummoned(Creature* _summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
            _summoned->AddThreat(target);
    }
*/
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
                    { if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                          if (DoCastSpellIfCan(pTarget, SPELL_BONE_STRIKE))
                                CallSpike(pTarget, MOB_BONE_SPIKE, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                    m_uiBoneStrike_Timer=urand(17000,29000);
                    } else m_uiBoneStrike_Timer -= diff;
                    break;}

            case 1: {
                    if (!m_uiBoneStorm)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_BONE_STORM );
                    m_uiBoneStorm = true;
                    stage = 2;
                    }
                    break;}
            case 2: {
                    if (m_uiBoneStorm_Timer < diff)
                    {
                    m_creature->RemoveAurasDueToSpell(SPELL_BONE_STORM);
                    m_creature->SetInCombatWithZone();
                    stage = 3;
                    } else  m_uiBoneStorm_Timer -= diff;

                    if (m_uiBoneStormStrike_Timer < diff) {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                              m_creature->CastSpell(pPlayer, Regular ? SPELL_BONE_STORM_STRIKE_N : SPELL_BONE_STORM_STRIKE_H, true);
                     }
                     m_uiBoneStormStrike_Timer = 2000;
                     }
                    break;}
            case 3: break;
            }

                    if (m_uiSaberLash_Timer < diff)
                    {DoCastSpellIfCan(m_creature->getVictim(), Regular ? SPELL_SABER_LASH_N :SPELL_SABER_LASH_H);
                    m_uiSaberLash_Timer=urand(8000,12000);
                    } else m_uiSaberLash_Timer -= diff;

                    if (m_uiColdFlame_Timer < diff) {
                    CallGuard(MOB_COLDFLAME, TEMPSUMMON_TIMED_DESPAWN, 30000);
                    m_uiColdFlame_Timer=urand(10000,30000);
                    } else m_uiColdFlame_Timer -= diff;

        health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
        if (health <= 30 && stage == 0) stage = 1;

        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
        }
        else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();

    }
};

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    bool Regular;
    uint32 _diff;
        float fPosX, fPosY, fPosZ;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(150, 200), fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(0, fPosX, fPosY, fPosZ);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
    _diff = uiDiff;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
        m_creature->CastSpell(m_creature, Regular ? SPELL_COLD_FLAME_N : SPELL_COLD_FLAME_H, false);
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                            if (Creature* pMarrowgar = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_LORD_MARROWGAR))))
                                pMarrowgar->CastSpell(pPlayer, Regular ? SPELL_COLD_FLAME_N : SPELL_COLD_FLAME_H, true);
                     }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    bool Regular;
    uint32 _diff;
    uint64 m_uiVictimGUID;


    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetSpeedRate(MOVE_RUN, 5);
        _diff = 1000;
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        m_creature->GetMotionMaster()->MoveChase(pWho);
        DoCast(pWho, SPELL_BONE_STRIKE_IMPALE);
        m_uiVictimGUID = pWho->GetGUID();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
        if (Killer)
            Killer->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

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

    newscript = new Script;
    newscript->Name = "mob_coldflame";
    newscript->GetAI = &GetAI_mob_coldflame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bone_spike";
    newscript->GetAI = &GetAI_mob_bone_spike;
    newscript->RegisterSelf();

}
