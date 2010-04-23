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
        SPELL_MARK                              = 72293,
        SPELL_FRENZY                            = 72737,
        SPELL_BOILING_BLOOD                     = 72385,
        SPELL_BLOOD_NOVA                        = 72380,
        SPELL_RUNE_OF_BLOOD                     = 72408,
        SPELL_CALL_BLOOD_BEASTS                 = 72173,
        SPELL_SCENT_OF_BLOOD                    = 72769,
        SPELL_RESISTANT_SKIN                    = 72723,

        SPELL_BEAST_1                           = 72176,
        SPELL_BEAST_2                           = 72723,
        SPELL_BEAST_3                           = 21150,

};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public ScriptedAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    bool m_uiIsFrenzy;
    uint8 stage;
    uint8 Difficulty;

    void Reset()
    {
        if(!pInstance) return;
        Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
        pInstance->SetData(TYPE_SAURFANG, NOT_STARTED);
        m_uiIsFrenzy = false;
        stage = 0;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);
        DoScriptText(-1631100,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631103,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631104,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_SAURFANG, DONE);
        DoScriptText(-1631106,m_creature);
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
                    bsw->doCast(SPELL_FRENZY);
                    m_uiIsFrenzy = true;
                    stage = 2;
                    DoScriptText(-1631101,m_creature);
                    }
                    break;}

            case 2: {
                    break;}
        }

                    bsw->timedCast(SPELL_BLOOD_LINK, diff);

                    bsw->timedCast(SPELL_BLOOD_NOVA, diff);

                    bsw->timedCast(SPELL_BOILING_BLOOD, diff);

                    bsw->timedCast(SPELL_RUNE_OF_BLOOD, diff);

                    if (bsw->timedQuery(SPELL_CALL_BLOOD_BEASTS, diff))
                    {
                          if (Unit* pTemp = bsw->doSummon(NPC_BLOOD_BEASTS))
                              if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                                   pTemp->AddThreat(pTarget, 100.0f);
                                   pTemp->GetMotionMaster()->MoveChase(pTarget);
                                };
                          if (Unit* pTemp = bsw->doSummon(NPC_BLOOD_BEASTS))
                              if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                                   pTemp->AddThreat(pTarget, 100.0f);
                                   pTemp->GetMotionMaster()->MoveChase(pTarget);
                                };
                        DoScriptText(-1631102,m_creature);
                        if (Difficulty == RAID_DIFFICULTY_25MAN_NORMAL
                            || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                            {
                            if (Unit* pTemp = bsw->doSummon(NPC_BLOOD_BEASTS))
                               if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                                   pTemp->AddThreat(pTarget, 100.0f);
                                   pTemp->GetMotionMaster()->MoveChase(pTarget);
                                };
                            if (Unit* pTemp = bsw->doSummon(NPC_BLOOD_BEASTS))
                               if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) {
                                   pTemp->AddThreat(pTarget, 100.0f);
                                   pTemp->GetMotionMaster()->MoveChase(pTarget);
                                };
                            }
                    }

            if (m_creature->GetHealthPercent() <= 30.0f && stage == 0) stage = 1;

            bsw->timedCast(SPELL_BERSERK, diff);

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
