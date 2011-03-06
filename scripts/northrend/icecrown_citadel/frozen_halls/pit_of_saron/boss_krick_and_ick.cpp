/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_krick_and_ick
SD%Complete: 0%
SDComment: encounter need vehicle support; outro is missing; explosive barrage doesn't work fine; pursue doesn't work fine
SDCategory: Pit of Saron
EndScriptData */

#include "precompiled.h"
#include "pit_of_saron.h"

enum
{
    SAY_AGGRO                           = -1658024,
    SAY_SLAY_1                          = -1658025,
    SAY_SLAY_2                          = -1658026,
    SAY_ORDER_STOP                      = -1658027,
    SAY_ORDER_BLOW                      = -1658028,
    SAY_TARGET_1                        = -1658029,
    SAY_TARGET_2                        = -1658030,
    SAY_TARGET_3                        = -1658031,

    EMOTE_KRICK_MINES                   = -1658032,
    EMOTE_ICK_POISON                    = -1658033,
    EMOTE_ICK_CHASING                   = -1658034,

    SAY_OUTRO_1                         = -1658035,
    SAY_JAINA_KRICK_1                   = -1658036,
    SAY_SYLVANAS_KRICK_1                = -1658037,
    SAY_OUTRO_2                         = -1658038,
    SAY_JAINA_KRICK_2                   = -1658039,
    SAY_SYLVANAS_KRICK_2                = -1658040,
    SAY_OUTRO_3                         = -1658041,
    SAY_TYRANNUS_KRICK_1                = -1658042,
    SAY_OUTRO_4                         = -1658043,
    SAY_TYRANNUS_KRICK_2                = -1658044,
    SAY_JAINA_KRICK_3                   = -1658045,
    SAY_SYLVANAS_KRICK_3                = -1658046,

    // ick
    SPELL_POISON_NOVA                   = 68989,
    SPELL_POISON_NOVA_H                 = 70434,
    SPELL_MIGHTY_KICK                   = 69021,
    SPELL_PURSUED                       = 68987,

    // krick
    SPELL_TOXIC_WASTE                   = 69024,
    SPELL_TOXIC_WASTE_H                 = 70436,
    SPELL_STRANGULATING                 = 69413,        // spell used by tyrannus at the outro event
    SPELL_SHADOW_BOLT                   = 69028,
    SPELL_EXPLOSIVE_BARRAGE             = 69263,        // maybe 69012?

    NPC_EXPLOSIVE_ORB                   = 36610,
    SPELL_EXPLOSIVE_BARRAGE_ORB         = 69019,
};

struct MANGOS_DLL_DECL boss_ickAI : public ScriptedAI
{
    boss_ickAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPoisonNovaTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiMightKickTimer;

    void Reset()
    {
        m_uiPoisonNovaTimer     = 30000;
        m_uiPursueTimer         = 10000;
        m_uiMightKickTimer      = 20000;
    }

    void KilledUnit(Unit* pVictim)
    {
        if(Creature* pKrick = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_KRICK)))
            DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, pKrick);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK, DONE);

        // ToDo - remove this when outro implemented
        if(Creature* pKrick = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_KRICK)))
            pKrick->DealDamage(pKrick, pKrick->GetMaxHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRICK, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiPoisonNovaTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_POISON_NOVA : SPELL_POISON_NOVA_H))
            {
                if(Creature* pKrick = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_KRICK)))
                    DoScriptText(SAY_ORDER_BLOW, pKrick);
                m_uiPoisonNovaTimer = 30000;
            }
        }
        else
            m_uiPoisonNovaTimer -= uiDiff;

        if (m_uiPursueTimer < uiDiff)
        {
            if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(pTarget, SPELL_PURSUED) == CAST_OK)
                {
                    if(Creature* pKrick = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_KRICK)))
                    {
                        switch (urand(0, 2))
                        {
                        case 0: 
                            DoScriptText(SAY_TARGET_1, pKrick);
                            break;
                        case 1: 
                            DoScriptText(SAY_TARGET_2, pKrick);
                            break;
                        case 2: 
                            DoScriptText(SAY_TARGET_3, pKrick);
                            break;
                        }
                    }

                    DoScriptText(EMOTE_ICK_CHASING, m_creature, pTarget);

                    m_uiPursueTimer = 13000;
                }
            }
        }
        else
            m_uiPursueTimer -= uiDiff;

        if (m_uiMightKickTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_MIGHTY_KICK) == CAST_OK)
                m_uiMightKickTimer = 25000;
        }
        else
            m_uiMightKickTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL boss_krickAI : public ScriptedAI
{
    boss_krickAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_pit_of_saron*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_pit_of_saron* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiToxicWasteTimer;
    uint32 m_uiShadowboltTimer;
    uint32 m_uiExplosivBarrageTimer;
    // workaround
    uint32 m_uiSummonOrbsTimer;
    uint32 m_uiSummonOverTimer;
    bool m_bIsSummoning;

    void Reset()
    {
        m_uiToxicWasteTimer         = 5000;
        m_uiShadowboltTimer         = 15000;
        m_uiExplosivBarrageTimer    = 35000;
        // workaround
        m_uiSummonOrbsTimer         = 600000;
        m_uiSummonOverTimer         = 600000;
        m_bIsSummoning              = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiToxicWasteTimer < uiDiff)
        {
            if(Creature* pIck = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ICK)))
            {
                if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_TOXIC_WASTE : SPELL_TOXIC_WASTE_H) == CAST_OK)
                        m_uiToxicWasteTimer = 10000;
                }
            }
        }
        else
            m_uiToxicWasteTimer -= uiDiff;

        if (m_uiShadowboltTimer < uiDiff)
        {
            if(Creature* pIck = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_ICK)))
            {
                if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_BOLT) == CAST_OK)
                        m_uiShadowboltTimer = 5000;
                }
            }
        }
        else
            m_uiShadowboltTimer -= uiDiff;

        if (m_uiExplosivBarrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_EXPLOSIVE_BARRAGE) == CAST_OK)
            {
                DoScriptText(SAY_ORDER_STOP, m_creature);
                DoScriptText(EMOTE_KRICK_MINES, m_creature);
                m_uiExplosivBarrageTimer = 45000;
            }

            // workaround
            m_uiSummonOrbsTimer = 3000;
            m_uiSummonOverTimer = 18000;
            m_bIsSummoning = true;
        }
        else
            m_uiExplosivBarrageTimer -= uiDiff;

        // workaround
        if (m_uiSummonOrbsTimer < uiDiff && m_bIsSummoning)
        {
            for(uint8 i = 0; i < 4; ++i)
            {
                if(Creature* pIck = GetClosestCreatureWithEntry(m_creature, NPC_ICK, 100.0f))
                {
                    if (Unit* pTarget = pIck->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->SummonCreature(NPC_EXPLOSIVE_ORB, pTarget->GetPositionX() + urand(0, 3), pTarget->GetPositionY() + urand(0, 3), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                }
            }
            m_uiSummonOrbsTimer = 1500;
        }
        else
            m_uiSummonOrbsTimer -= uiDiff;

        if (m_uiSummonOverTimer < uiDiff && m_bIsSummoning)
        {
            m_bIsSummoning = false;
            m_uiSummonOverTimer = 60000;
        }
        else
            m_uiSummonOverTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_ick(Creature* pCreature)
{
    return new boss_ickAI (pCreature);
}

CreatureAI* GetAI_boss_krick(Creature* pCreature)
{
    return new boss_krickAI (pCreature);
}
struct MANGOS_DLL_DECL mob_exploding_orbAI : public ScriptedAI
{
   mob_exploding_orbAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_creature->SetActiveObjectState(true);
        Reset();
   }

   ScriptedInstance* pInstance;
   uint32 ExplodeTimer;

    void Reset()
    {
      ExplodeTimer = 18000;
    }

    void AttackStart(Unit* who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!pInstance) return;

        if (ExplodeTimer < diff)
        {
           DoCast(m_creature, SPELL_EXPLOSIVE_BARRAGE_ORB);
           m_creature->ForcedDespawn();
        } else ExplodeTimer -= diff;
        return;
    }

};

CreatureAI* GetAI_mob_exploding_orb(Creature* pCreature)
{
    return new mob_exploding_orbAI(pCreature);
}

void AddSC_boss_krick_and_ick()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_krick";
    newscript->GetAI = &GetAI_boss_krick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_ick";
    newscript->GetAI = &GetAI_boss_ick;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_exploding_orb";
    newscript->GetAI = &GetAI_mob_exploding_orb;
    newscript->RegisterSelf();

}
