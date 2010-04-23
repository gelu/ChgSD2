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
        NPC_BONE_SPIKE                          = 38711,
        NPC_COLDFLAME                           = 36672,
        //Abilities
        SPELL_SABER_LASH                        = 71021,
        SPELL_CALL_COLD_FLAME                   = 69138,
        SPELL_COLD_FLAME                        = 69146,
        SPELL_COLD_FLAME_0                      = 69145,
        SPELL_BONE_STRIKE                       = 69057,
        SPELL_BONE_STORM                        = 69076,
        SPELL_BONE_STRIKE_IMPALE                = 69065,
        SPELL_BONE_STORM_STRIKE                 = 69075,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool intro;

    void Reset()
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
        stage = 0;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (intro) return;
        DoScriptText(-1631000,m_creature);
        intro = true;
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
        if(!pInstance) return;
        pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
        DoScriptText(-1631001,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
        DoScriptText(-1631009,m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    if (bsw->timedQuery(SPELL_BONE_STRIKE, diff))
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                            if (bsw->doCast(SPELL_BONE_STRIKE, pTarget) == CAST_OK)
                              {
                              switch (urand(0,1)) {
                                                   case 0:
                                                   DoScriptText(-1631003,m_creature,pTarget);
                                                   break;
                                                   case 1:
                                                   DoScriptText(-1631004,m_creature,pTarget);
                                                   break;
                                                   case 2:
                                                   DoScriptText(-1631005,m_creature,pTarget);
                                                   break;
                                                   };

                               float fPosX, fPosY, fPosZ;
                               pTarget->GetPosition(fPosX, fPosY, fPosZ);
                               if (Unit* pSpike = bsw->doSummon(NPC_BONE_SPIKE, fPosX, fPosY, fPosZ))
                                   pSpike->AddThreat(pTarget, 100.0f);
                              }
                    break;}

            case 1: {
                    bsw->doCast(SPELL_BONE_STORM);
                    stage = 2;
                    DoScriptText(-1631002,m_creature);
                    break;}

            case 2: {
                    if (!bsw->hasAura(SPELL_BONE_STORM, m_creature)) stage = 3;
//                             else bsw->timedCast(SPELL_BONE_STORM_STRIKE, diff);
// insert to this damage override from bone storm
                    break;}

            case 3: break;
            }

        bsw->timedCast(SPELL_SABER_LASH, diff);

        bsw->timedCast(SPELL_CALL_COLD_FLAME, diff);

//        if (bsw->timedQuery(NPC_COLDFLAME, diff))
//                   bsw->doSummon(NPC_COLDFLAME, TEMPSUMMON_TIMED_DESPAWN, 60000);

        if (m_creature->GetHealthPercent() <= 30.0f && stage == 0) stage = 1;

        if (bsw->timedQuery(SPELL_BERSERK, diff))
            {
            bsw->doCast(SPELL_BERSERK);
            DoScriptText(-1631008,m_creature);
            }

        DoMeleeAttackIfReady();

    }
};

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    BossSpellWorker* bsw;
    float fPosX, fPosY, fPosZ;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(150, 200), fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
        SetCombatMovement(false);
        m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
        bsw->doCast(SPELL_COLD_FLAME_0);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!m_pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
        if(id != 1)
             m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
             else m_creature->ForcedDespawn();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        bsw->timedCast(SPELL_COLD_FLAME_0, uiDiff);
//        bsw->timedCast(SPELL_COLD_FLAME, uiDiff);
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint64 m_uiVictimGUID;


    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
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
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
            m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
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
