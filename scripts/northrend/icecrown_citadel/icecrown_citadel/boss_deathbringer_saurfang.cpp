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
        SPELL_BLOOD_LINK_BEAST                  = 72176,

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
    uint8 stage;
    uint8 Difficulty;
    uint8 beasts;
    Unit* MarkTarget;

    void Reset()
    {
        if(!pInstance) return;
        Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
        pInstance->SetData(TYPE_SAURFANG, NOT_STARTED);
        stage = 0;
        beasts = 0;
        bsw->resetTimers();
        MarkTarget = NULL;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);
        DoScriptText(-1631100,m_creature);
        bsw->doCast(SPELL_BLOOD_LINK);
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_SAURFANG, FAIL);
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

        if (pVictim && pVictim->HasAura(SPELL_MARK))
        {
            m_creature->ModifyHealth(m_creature->GetMaxHealth() * 0.05f);
            pVictim->RemoveAurasDueToSpell(SPELL_MARK);
        }
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned) return;

        if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0) ) {
            summoned->AddThreat(pTarget, 100.0f);
            summoned->GetMotionMaster()->MoveChase(pTarget);
            }
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_SAURFANG, DONE);
        DoScriptText(-1631106,m_creature);
        if (MarkTarget && MarkTarget->HasAura(SPELL_MARK))
             MarkTarget->RemoveAurasDueToSpell(SPELL_MARK);
    }

    void doBloodPower()
    {
       m_creature->ModifyPower(m_creature->getPowerType(), +1);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_BLOOD_POWER))
            bsw->doCast(SPELL_BLOOD_POWER);

        switch(stage)
        {
            case 0:
                    if (m_creature->GetHealthPercent() <= 30.0f) stage = 1;
                    break;

            case 1: 
                        bsw->doCast(SPELL_FRENZY);
                        stage = 2;
                        DoScriptText(-1631101,m_creature);
                    break;

            case 2:
                    break;

            default:
                    break;
        }

            if (bsw->timedQuery(SPELL_MARK, diff))
            {
                if (MarkTarget && MarkTarget->HasAura(SPELL_MARK))
                   MarkTarget->RemoveAurasDueToSpell(SPELL_MARK);

                if (MarkTarget = bsw->SelectRandomPlayerAtRange(120.0f))
                   if (bsw->doCast(SPELL_MARK, MarkTarget) == CAST_OK) 
                       doBloodPower();
            }

            if (bsw->timedCast(SPELL_BLOOD_NOVA, diff) == CAST_OK) doBloodPower();

            if (bsw->timedCast(SPELL_BOILING_BLOOD, diff) == CAST_OK) doBloodPower();

            if (bsw->timedCast(SPELL_RUNE_OF_BLOOD, diff) == CAST_OK) doBloodPower();

            if (bsw->timedQuery(SPELL_CALL_BLOOD_BEASTS, diff))
                    {
                        if (Difficulty == RAID_DIFFICULTY_25MAN_NORMAL
                            || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC) beasts = 4;
                            else beasts = 2;

                        DoScriptText(-1631102,m_creature);
                     };

                     if (beasts > 0)
                        if (bsw->doCast(SPELL_CALL_BLOOD_BEASTS) == CAST_OK)
                           {
                               doBloodPower();
                               --beasts;
                           };

        if (bsw->timedQuery(SPELL_BERSERK, diff)){
                 bsw->doCast(SPELL_BERSERK);
                 DoScriptText(-1631108,m_creature);
                 };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_deathbringer_saurfang(Creature* pCreature)
{
    return new boss_deathbringer_saurfangAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_blood_beastAI : public ScriptedAI
{
    mob_blood_beastAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    Creature* pOwner;
    bool scentcasted;
    BossSpellWorker* bsw;

    void Reset()
    {
         pOwner = (Creature*)Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_DEATHBRINGER_SAURFANG));
         bsw->resetTimers();
         bsw->doCast(SPELL_BLOOD_LINK_BEAST);
         scentcasted = false;
    }
    void KilledUnit(Unit* pVictim)
    {
        if (pOwner && pOwner->isAlive())
            pOwner->ModifyHealth(pOwner->GetMaxHealth() * 0.05f);
        if (pVictim && pVictim->HasAura(SPELL_MARK))
            pVictim->RemoveAurasDueToSpell(SPELL_MARK);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!pInstance || pInstance->GetData(TYPE_SAURFANG) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_RESISTANT_SKIN))
            bsw->doCast(SPELL_RESISTANT_SKIN);

        if (!scentcasted && (m_creature->GetHealthPercent() <= 20.0f))
           {
               if (urand(0,1))                            //50%
                   bsw->doCast(SPELL_SCENT_OF_BLOOD);
               scentcasted = true;
           }

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_blood_beast(Creature* pCreature)
{
    return new mob_blood_beastAI(pCreature);
}

void AddSC_boss_deathbringer_saurfang()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_deathbringer_saurfang";
    newscript->GetAI = &GetAI_boss_deathbringer_saurfang;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_blood_beast";
    newscript->GetAI = &GetAI_mob_blood_beast;
    newscript->RegisterSelf();
}
