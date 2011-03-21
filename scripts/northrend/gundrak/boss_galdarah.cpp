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
SDName: Boss_Galdarah
SD%Complete: 100%
SDComment:
SDCategory: Gundrak
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{
    SAY_AGGRO                       = -1604019,
    SAY_TRANSFORM_1                 = -1604020,
    SAY_TRANSFORM_2                 = -1604021,
    SAY_SUMMON_1                    = -1604022,
    SAY_SUMMON_2                    = -1604023,
    SAY_SUMMON_3                    = -1604024,
    SAY_SLAY_1                      = -1604025,
    SAY_SLAY_2                      = -1604026,
    SAY_SLAY_3                      = -1604027,
    SAY_DEATH                       = -1604028,

    SPELL_STAMPEDE                  = 55220,
    SPELL_STAMPEDE_H                = 59823,
    SPELL_CHARGE                    = 74399,
    SPELL_WHIRLING_SLASH            = 55250,
    SPELL_WHIRLING_SLASH_H          = 59824,
    SPELL_IMPALING_CHARGE           = 54956,
    SPELL_IMPALING_CHARGE_H         = 59827,
    SPELL_KNOCK_BACK                = 56104,
    SPELL_PUNCTURE                  = 55276,
    SPELL_PUNCTURE_H                = 59826,
    SPELL_STOMP                     = 55292,
    SPELL_STOMP_H                   = 59829,
    SPELL_ENRAGE                    = 55285,
    SPELL_ENRAGE_H                  = 59828,

    NPC_RHINO_SPIRIT                = 29791,

    MODELID_HUMAN                   = 27061,
    MODELID_RHINO                   = 26265,

    AURA_ECK_RESIDUE                = 55817,
    ACHIEV_WHAT_THE_ECK_H           = 1864
};

/*######
## boss_galdarah
######*/

struct MANGOS_DLL_DECL boss_galdarahAI : public ScriptedAI
{
    boss_galdarahAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_gundrak*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_gundrak* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bRhinoPhase;
    bool m_bUseStampede;

    uint32 m_uiChangePhaseTimer;
    uint32 m_uiStampedeTimer;
    uint32 m_uiWhirlingSlashTimer;
    uint32 m_uiStompTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiImpalingChargeTimer;
    uint32 m_uiPunctureTimer;

    uint64 m_uiRhinoGUID;

    void Reset()
    {
        m_bRhinoPhase = false;
        m_bUseStampede = false;

        m_uiChangePhaseTimer = 40000;
        m_uiStampedeTimer = 6000;
        m_uiWhirlingSlashTimer = 8000;
        m_uiStompTimer = 5000;
        m_uiEnrageTimer = 4000;
        m_uiImpalingChargeTimer = 7000;
        m_uiPunctureTimer = 10000;

        m_uiRhinoGUID = 0;

        m_creature->SetDisplayId(MODELID_HUMAN);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GALDARAH, DONE);

        DoScriptText(SAY_DEATH, m_creature);

        if (pKiller->HasAura(AURA_ECK_RESIDUE) && !m_bIsRegularMode)
        {
            Map* pMap = m_creature->GetMap();
            if (pMap && pMap->IsDungeon())
            {
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    itr->getSource()->CompletedAchievement(ACHIEV_WHAT_THE_ECK_H);
            }
        }
    }

    void ChangePhase()
    {
        m_bRhinoPhase ? m_bRhinoPhase = false : m_bRhinoPhase = true;
        m_creature->SetDisplayId(m_creature->GetDisplayId() == MODELID_HUMAN ? MODELID_RHINO : MODELID_HUMAN);
    }

    void SummonRhinoSpirit()
    {
        Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
        if(pTarget)
        {
            float x = pTarget->GetPositionX() + 20.0f;
            float y = pTarget->GetPositionY() + 20.0f;
            float z = pTarget->GetPositionZ();
            Creature* cRhino = m_creature->SummonCreature(NPC_RHINO_SPIRIT, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN, 1800);
            if (cRhino)
            {
                m_uiRhinoGUID = cRhino->GetGUID();
                cRhino->CastSpell(pTarget, SPELL_CHARGE, true);
                cRhino->SetInCombatWith(pTarget);
                cRhino->AddThreat(pTarget, 1000.0f);
            }
        }
        m_bUseStampede = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();

        if (m_bUseStampede)
        {
            if (Creature* cRhino = m_creature->GetMap()->GetCreature(m_uiRhinoGUID))
            {
                if (cRhino->getVictim())
                {
                    if (cRhino->getVictim()->GetDistance(cRhino) < ATTACK_DISTANCE)
                    {
                        cRhino->CastSpell(cRhino->getVictim(), m_bIsRegularMode ? SPELL_STAMPEDE : SPELL_STAMPEDE_H, true);
                        m_bUseStampede = false;
                    }
                }
            }
        }

        if (m_uiChangePhaseTimer < uiDiff)
        {
            ChangePhase();
            m_uiChangePhaseTimer = 40000;
        }
        else 
            m_uiChangePhaseTimer -= uiDiff;

        if (!m_bRhinoPhase)
        {
            if (m_uiStampedeTimer < uiDiff)
            {
                SummonRhinoSpirit();
                m_uiStampedeTimer = urand(8000, 11000);
            }
            else 
                m_uiStampedeTimer -= uiDiff;

            if (m_uiWhirlingSlashTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WHIRLING_SLASH : SPELL_WHIRLING_SLASH_H);
                m_uiWhirlingSlashTimer = urand(15000, 23000);
                if (m_uiChangePhaseTimer < 5000)
                m_uiChangePhaseTimer = 5000;
            }
            else 
                m_uiWhirlingSlashTimer -= uiDiff;
        }
        else
        {
            if (m_uiStompTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_STOMP : SPELL_STOMP_H);
                m_uiStompTimer = urand(12000, 15000);
            }
            else 
                m_uiStompTimer -= uiDiff;

            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
                m_uiEnrageTimer = urand(12000, 15000);
            }
            else 
                m_uiEnrageTimer -= uiDiff;

            if (m_uiImpalingChargeTimer < uiDiff)
            {
                if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoCastSpellIfCan(pVictim, m_bIsRegularMode ? SPELL_IMPALING_CHARGE : SPELL_IMPALING_CHARGE_H);
                    DoCastSpellIfCan(pVictim, SPELL_KNOCK_BACK);
                }
                m_uiImpalingChargeTimer = urand(7000, 9000);
            }
            else 
                m_uiImpalingChargeTimer -= uiDiff;

            if (m_uiPunctureTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_PUNCTURE : SPELL_PUNCTURE_H);
                m_uiPunctureTimer = urand(20000, 25000);
            }
            else 
                m_uiPunctureTimer -= uiDiff;
        }        
    }
};

CreatureAI* GetAI_boss_galdarah(Creature* pCreature)
{
    return new boss_galdarahAI(pCreature);
}

void AddSC_boss_galdarah()
{
    Script *pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_galdarah";
    pNewscript->GetAI = &GetAI_boss_galdarah;
    pNewscript->RegisterSelf();
}

