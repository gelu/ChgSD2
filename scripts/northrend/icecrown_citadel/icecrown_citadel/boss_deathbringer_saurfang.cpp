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
SDName: boss_deathbringer_saurfang
SD%Complete: 10%
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
        NPC_BLOOD_BEASTS                        = 38508,
        //Abilities
        SPELL_BLOOD_LINK                        = 72178,
        SPELL_BLOOD_POWER                       = 72371,
        SPELL_MARK_N                            = 72293,
        SPELL_FRENZY                            = 72737,
        SPELL_BOILING_BLOOD_N                   = 72385,
        SPELL_BLOOD_NOVA_N                      = 72380,
        SPELL_RUNE_OF_BLOOD_N                   = 72408,
        SPELL_CALL_BLOOD_BEASTS                 = 72173,
        SPELL_SCENT_OF_BLOOD                    = 72769,
        SPELL_RESISTANT_SKIN                    = 72723,

        SPELL_MARK_H                            = 72444,
        SPELL_BOILING_BLOOD_H                   = 72442,
        SPELL_BLOOD_NOVA_H                      = 72438,
        SPELL_RUNE_OF_BLOOD_H                   = 72448,

        SPELL_BEAST_1                           = 72176,
        SPELL_BEAST_2                           = 72723,
        SPELL_BEAST_3                           = 21150,

};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public ScriptedAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    bool m_uiIsFrenzy;
    ScriptedInstance *pInstance;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiBloodLink_Timer;
    uint32 m_uiBloodNova_Timer;
    uint32 m_uiBoilingBlood_Timer;
    uint32 m_uiRuneOfBlood_Timer;
    uint32 m_uiSummon_Timer;

    uint8 health;
    uint8 stage;

    void Reset()
    {
    if(pInstance) pInstance->SetData(TYPE_SAURFANG, NOT_STARTED);
    m_uiBerserk_Timer = 480000;
    m_uiIsFrenzy = false;
    stage = 0;

    uint32 m_uiBloodLink_Timer = 10000;
    uint32 m_uiBloodNova_Timer = 20000;
    uint32 m_uiBoilingBlood_Timer = 25000;
    uint32 m_uiRuneOfBlood_Timer = 35000;
    uint32 m_uiSummon_Timer =30000;

    }
    uint64 CallGuard(uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(15, 25), fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        if(pSummon) pSummon->SetInCombatWithZone();
//        DoScriptText(EMOTE_SUMMON, m_creature);
        return pSummon ? pSummon->GetGUID() : 0;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_SAURFANG, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    break;}
            case 1: {
                    if (!m_uiIsFrenzy) {
                    DoCast(m_creature, SPELL_FRENZY);
                    m_uiIsFrenzy = true;
                    stage = 2;
                    }
                    break;}

            case 2: {
                    break;}
        }

                    if (m_uiBloodLink_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_BLOOD_LINK);
                    m_uiBloodLink_Timer=urand(8000,15000);
                    } else m_uiBloodLink_Timer -= diff;

                    if (m_uiBloodNova_Timer < diff) {
                    DoCast(m_creature->getVictim(), Regular ? SPELL_BLOOD_NOVA_N : SPELL_BLOOD_NOVA_H);
                    m_uiBloodNova_Timer=urand(25000,30000);
                    } else m_uiBloodNova_Timer -= diff;

                    if (m_uiBoilingBlood_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_BOILING_BLOOD_N : SPELL_BOILING_BLOOD_H);
                    m_uiBoilingBlood_Timer=urand(20000,40000);
                    } else m_uiBoilingBlood_Timer -= diff;

                    if (m_uiRuneOfBlood_Timer < diff) {
                    if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, Regular ? SPELL_RUNE_OF_BLOOD_N : SPELL_RUNE_OF_BLOOD_H);
                    m_uiRuneOfBlood_Timer=urand(20000,40000);
                    } else m_uiRuneOfBlood_Timer -= diff;

                    if (m_uiSummon_Timer < diff)
                    {
                    CallGuard(NPC_BLOOD_BEASTS, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                    CallGuard(NPC_BLOOD_BEASTS, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                    if(!Regular) CallGuard(NPC_BLOOD_BEASTS, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                    if(!Regular) CallGuard(NPC_BLOOD_BEASTS, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000);
                    m_uiSummon_Timer=60000;
                    } else m_uiSummon_Timer -= diff;

        health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();
        if (health <= 30 && stage == 0) stage = 1;

        if (m_uiBerserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 600000;
        } else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_deathbringer_saurfang(Creature* pCreature)
{
    return new boss_deathbringer_saurfangAI(pCreature);
}

void AddSC_boss_deathbringer_saurfang()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_deathbringer_saurfang";
    newscript->GetAI = &GetAI_boss_deathbringer_saurfang;
    newscript->RegisterSelf();
}
