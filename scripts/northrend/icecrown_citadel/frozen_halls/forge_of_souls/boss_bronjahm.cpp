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
SDName: boss_bronjahm
SD%Complete: 10%
SDComment: by /dev/rsa
SDCategory: Icecrown - forge of souls
EndScriptData */

#include "precompiled.h"
#include "def_forge.h"

enum BossSpells
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_SOUL_FRAGMENT                       = 36535,
        //Abilities
        SPELL_MAGIC_BANE                        = 68793,
        SPELL_CORRUPT_SOUL                      = 68839,
        SPELL_CONSUME_SOUL                      = 68858,
        SPELL_TELEPORT                          = 68988,
        SPELL_SOULSTORM                         = 68872,
        SPELL_SOULSTORM_2                       = 68921,
        SPELL_FEAR                              = 68950,
        SPELL_SHADOW_BOLT                       = 70043,
};

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, NOT_STARTED);
        bsw->resetTimers();
        stage = 0;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
            DoScriptText(-1632001,m_creature,who);
            SetCombatMovement(true);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_BRONJAHM, DONE);
        bsw->doRemove(SPELL_SOULSTORM);
               DoScriptText(-1632004,m_creature,killer);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1632002,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1632003,m_creature,pVictim);
               break;
        };
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                    if  (bsw->timedQuery(SPELL_CORRUPT_SOUL, diff))
                        {
                            if (Unit* pTarget= m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                {
                                if (bsw->doCast(SPELL_CORRUPT_SOUL, pTarget) == CAST_OK)
                                    {
                                    DoScriptText(-1632006,m_creature,pTarget);
                                    float fPosX, fPosY, fPosZ;
                                    pTarget->GetPosition(fPosX, fPosY, fPosZ);
                                    bsw->doSummon(NPC_SOUL_FRAGMENT,fPosX, fPosY, fPosZ);
                                    }
                                }
                        }
                    break;
            case 1: 
                        if (bsw->timedCast(SPELL_TELEPORT, diff) == CAST_OK) stage = 2;
                    break;
            case 2:
                        if (bsw->timedCast(SPELL_SOULSTORM, diff) == CAST_OK) {
                               DoScriptText(-1632005,m_creature);
                               SetCombatMovement(false);
                               stage = 3;
                               }
                    break;
            case 3: 
                        bsw->timedCast(SPELL_FEAR, diff);
                    break;
        }

        bsw->timedCast(SPELL_SHADOW_BOLT, diff);

        bsw->timedCast(SPELL_MAGIC_BANE, diff);

        if (m_creature->GetHealthPercent() <= 30.0f && stage == 0) stage = 1;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_soul_fragmentAI : public ScriptedAI
{
    mob_soul_fragmentAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    Creature* pBoss;
    uint32 m_uiRangeCheck_Timer;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        if (!m_pInstance) return;
        pBoss = (Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_BRONJAHM));
        m_creature->SetSpeedRate(MOVE_RUN, 0.2f);
        m_creature->GetMotionMaster()->MoveChase(pBoss);
        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit* pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_BRONJAHM) != IN_PROGRESS ) m_creature->ForcedDespawn();

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (pBoss->IsWithinDistInMap(m_creature, 2.0f))
            {
                pBoss->CastSpell(pBoss, SPELL_CONSUME_SOUL, false);
                m_creature->ForcedDespawn();
            } else m_creature->GetMotionMaster()->MoveChase(pBoss);

            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

CreatureAI* GetAI_mob_soul_fragment(Creature* pCreature)
{
    return new mob_soul_fragmentAI (pCreature);
}


void AddSC_boss_bronjahm()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_bronjahm";
    newscript->GetAI = &GetAI_boss_bronjahm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soul_fragment";
    newscript->GetAI = &GetAI_mob_soul_fragment;
    newscript->RegisterSelf();

}
