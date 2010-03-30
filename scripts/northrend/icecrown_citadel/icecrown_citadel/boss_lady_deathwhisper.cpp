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
SDName: boss_lady_deathwhisper
SD%Complete: 20%
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
        NPC_VENGEFUL_SHADE                      = 38222,
        NPC_FANATIC                             = 37890,
        NPC_REANIMATED_FANATIC                  = 38009,
        NPC_ADHERENT                            = 37949,
        NPC_REANIMATED_ADHERENT                 = 38010,
        //Abilities
        SPELL_MANA_BARRIER                      = 70842,
        SPELL_SHADOW_BOLT_N                     = 71254,
        SPELL_DEATH_AND_DECAY_N                 = 71001,
        SPELL_DARK_EMPOWERMENT_N                = 70901,
        SPELL_FROSTBOLT_N                       = 71420,
        SPELL_INSIGNIFICANCE_N                  = 71204,

        SPELL_SHADOW_BOLT_H                     = 72008,
        SPELL_FROSTBOLT_H                       = 72007,
        SPELL_DOMINATE_MIND_H                   = 71289,

        SPELL_VENGEFUL_BLAST_N                  = 72011,
        SPELL_VENGEFUL_BLAST_H                  = 72012,
};

struct MANGOS_DLL_DECL boss_lady_deathwhisperAI : public ScriptedAI
{
    boss_lady_deathwhisperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    uint8 stage;
    uint32 m_uiManaBarrier_Timer;
    uint32 m_uiShadowBolt_Timer;
    uint32 m_uiDeathAndDecay_Timer;
    uint32 m_uiDarkEmpowerment_Timer;
    uint32 m_uiFrostBolt_Timer;
    uint32 m_uiInsignificance_Timer;
    uint32 m_uiDominateMind_Timer;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiSummon_Timer;
    uint32 m_uiSummon2_Timer;
    bool isManaBarrier;
    uint8 health;
    uint64 m_uiMana;
    
    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, NOT_STARTED);
    m_uiManaBarrier_Timer = 30000;
    m_uiSummon_Timer = 60000;
    m_uiSummon2_Timer = 20000;
    m_uiShadowBolt_Timer = urand(8000,10000);
    m_uiDeathAndDecay_Timer = urand(40000,60000);
    m_uiDarkEmpowerment_Timer = 45000;
    m_uiFrostBolt_Timer = urand(20000,25000);
    m_uiInsignificance_Timer = 40000;
    m_uiDominateMind_Timer = 30000;
    m_uiBerserk_Timer = 600000;

    stage = 0;
    isManaBarrier = false;
    }
    
    uint64 CallGuard(uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(75, 100), fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        if(pSummon) pSummon->SetInCombatWithZone();
//        DoScriptText(EMOTE_SUMMON, m_creature);
        return pSummon ? pSummon->GetGUID() : 0;
    }


    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, IN_PROGRESS);
        DoCastSpellIfCan(m_creature, SPELL_MANA_BARRIER );
        isManaBarrier = true;
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_DEATHWHISPER, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;
         if (isManaBarrier) {
            m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-uiDamage);
            uiDamage = 0;
            if(m_creature->GetHealth() <= m_creature->GetMaxHealth()) {
                        m_uiMana = m_creature->GetPower(POWER_MANA)-(m_creature->GetMaxHealth()- m_creature->GetHealth());
                        if (m_uiMana <= 0) m_uiMana =0;
                                 m_creature->SetPower(POWER_MANA,m_uiMana);
                                 m_creature->SetHealth(m_creature->GetMaxHealth());
                                 };
            if(m_creature->GetPower(POWER_MANA) <= m_creature->GetMaxPower(POWER_MANA)/10 ) {
                                  m_creature->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
                                  isManaBarrier = false;
                                  };
            } else return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    if (m_uiShadowBolt_Timer < diff)
                    { if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, Regular ? SPELL_SHADOW_BOLT_N : SPELL_SHADOW_BOLT_H );
                    m_uiShadowBolt_Timer=urand(8000,10000);
                    } else m_uiShadowBolt_Timer -= diff;

                    if (m_uiSummon_Timer < diff)
                    { 
                    CallGuard(NPC_FANATIC, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    CallGuard(NPC_ADHERENT, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    if(!Regular){
                                 CallGuard(NPC_FANATIC, TEMPSUMMON_TIMED_DESPAWN, 60000);
                                 CallGuard(NPC_ADHERENT, TEMPSUMMON_TIMED_DESPAWN, 60000);
                                 };
                    m_uiSummon_Timer=40000;
                    } else m_uiSummon_Timer -= diff;

                    if (m_uiDarkEmpowerment_Timer < diff)
                    { 
                    if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_FANATIC, 30.0f))
                    DoCast(pGuard, SPELL_DARK_EMPOWERMENT_N);
                    else if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_ADHERENT, 30.0f))
                    DoCast(pGuard, SPELL_DARK_EMPOWERMENT_N);
                    m_uiDarkEmpowerment_Timer=urand(20000,40000);
                    } else m_uiDarkEmpowerment_Timer -= diff;

                    break;}

            case 1: {
                    if (m_uiFrostBolt_Timer < diff)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, Regular ? SPELL_FROSTBOLT_N : SPELL_FROSTBOLT_H);
                    m_uiFrostBolt_Timer=urand(8000,15000);
                    } else m_uiFrostBolt_Timer -= diff;

                    if (m_uiInsignificance_Timer < diff) {
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_INSIGNIFICANCE_N);
                    m_uiInsignificance_Timer=urand(20000,40000);
                    } else m_uiInsignificance_Timer -= diff;

                    if (m_uiSummon2_Timer < diff)
                    { 
                    CallGuard(NPC_VENGEFUL_SHADE, TEMPSUMMON_TIMED_DESPAWN, 30000);
                    if(!Regular) CallGuard(NPC_VENGEFUL_SHADE, TEMPSUMMON_TIMED_DESPAWN, 30000);
                    m_uiSummon2_Timer=10000;
                    } else m_uiSummon2_Timer -= diff;

                    if (m_uiDominateMind_Timer < diff)
                    {if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, SPELL_DOMINATE_MIND_H);
                    m_uiDominateMind_Timer=urand(15000,25000);
                    } else m_uiDominateMind_Timer -= diff;

                    break;}
        }

                    if (m_uiDeathAndDecay_Timer < diff)
                    {DoCastSpellIfCan(m_creature->getVictim(), SPELL_DEATH_AND_DECAY_N);
                    m_uiDeathAndDecay_Timer=urand(30000,45000);
                    } else m_uiDeathAndDecay_Timer -= diff;


         health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
         if (!isManaBarrier && stage == 0) stage = 1;

        if (m_uiBerserk_Timer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 600000;
        } else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_vengeful_shadeAI : public ScriptedAI
{
    mob_vengeful_shadeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    bool Regular;


    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
        if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.8);
                } else
        if (Unit* pTarget = Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LADY_DEATHWHISPER))) {
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.8);
                }
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                    if (m_creature->IsWithinDist(m_creature->getVictim(), 2.0f, false))
                    {
                        DoCast(m_creature->getVictim(), Regular ? SPELL_VENGEFUL_BLAST_N : SPELL_VENGEFUL_BLAST_H);
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    }
            }
            m_uiRangeCheck_Timer = 1000;
            if (m_creature->getVictim()) {
                                  m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                                  m_creature->SetSpeedRate(MOVE_RUN, 0.8);
                                  }
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_vengeful_shade(Creature* pCreature)
{
    return new mob_vengeful_shadeAI(pCreature);
}


CreatureAI* GetAI_boss_lady_deathwhisper(Creature* pCreature)
{
    return new boss_lady_deathwhisperAI(pCreature);
}

void AddSC_boss_lady_deathwhisper()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lady_deathwhisper";
    newscript->GetAI = &GetAI_boss_lady_deathwhisper;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vengeful_shade";
    newscript->GetAI = &GetAI_mob_vengeful_shade;
    newscript->RegisterSelf();
}
