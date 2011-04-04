/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SD%Complete:
SDComment: by /dev/rsa, IOV
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // Spells
    SPELL_BONE_SLICE_10                     = 69055,
    SPELL_BONE_SLICE_25                     = 70814,
    SPELL_BONE_SPIKE_GRAVEYARD_10_N         = 69057,
    SPELL_BONE_SPIKE_GRAVEYARD_10_H_0       = 72088,
    SPELL_BONE_SPIKE_GRAVEYARD_10_H_1       = 73144,
    SPELL_BONE_SPIKE_GRAVEYARD_25_N         = 70826,
    SPELL_BONE_SPIKE_GRAVEYARD_25_H_0       = 72089,
    SPELL_BONE_SPIKE_GRAVEYARD_25_H_1       = 73145,
    SPELL_CALL_COLD_FLAME_N                 = 69138, // not used
    SPELL_CALL_COLD_FLAME_H                 = 71580, // not used
    SPELL_COLD_FLAME_VISUAL                 = 69145,
    SPELL_COLD_FLAME_1                      = 69147,
    SPELL_COLD_FLAME_10_N                   = 69146,
    SPELL_COLD_FLAME_10_H                   = 70824,
    SPELL_COLD_FLAME_25_N                   = 70823,
    SPELL_COLD_FLAME_25_H                   = 70825,
    SPELL_BONE_STORM                        = 69076,
    SPELL_BONE_STORM_STRIKE                 = 69075,
    SPELL_BONE_STRIKE_IMPALE                = 69065,
    SPELL_VEHICLE_HARDCODED                 = 46598,

    // NPCs
    NPC_BONE_SPIKE                          = 38711,
    NPC_COLD_FLAME                          = 36672,

    // Texts
    SAY_INTRO                               = -1631000,
    SAY_AGGRO                               = -1631001,
    SAY_BONESTORM                           = -1631002,
    SAY_BONESPIKE1                          = -1631003,
    SAY_BONESPIKE2                          = -1631004,
    SAY_BONESPIKE3                          = -1631005,
    SAY_KILL1                               = -1631006,
    SAY_KILL2                               = -1631007,
    SAY_ENRAGE                              = -1631008, // not used
    SAY_DEATH                               = -1631009,

    // Achievements
    ACHIEV_BONED_10                         = 4534,
    ACHIEV_BONED_25                         = 4610,
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_uiMode = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 m_uiMode;

    bool m_bIntro;
    bool m_bPhase1;
    bool m_bPhase2;
    bool m_bSummon;
    bool m_bAchievFailed;
    uint32 m_uiBoneSliceTimer;
    uint32 m_uiBoneSpikeGraveyardTimer;
    uint32 m_uiSummonBoneSpikeTimer;
    uint32 m_uiBoneStormTimer;
    uint32 m_uiTargetSwitchTimer;
    uint32 m_uiColdFlameTimer;
    uint32 m_uiColdFlameDespawnDelay;

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);

        m_creature->SetSpeedRate(MOVE_RUN, 1.5);
        m_creature->SetSpeedRate(MOVE_WALK, 1.5);

        m_bIntro = false;
        m_bPhase1 = true;
        m_bPhase2 = false;
        m_bSummon = false;
        m_bAchievFailed = false;
        m_uiBoneSliceTimer = urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS);
        m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
        m_uiSummonBoneSpikeTimer = 3*IN_MILLISECONDS;
        m_uiBoneStormTimer = 50*IN_MILLISECONDS;
        m_uiTargetSwitchTimer = 5*IN_MILLISECONDS;
        m_uiColdFlameTimer = 10*IN_MILLISECONDS;
        m_uiColdFlameDespawnDelay = 10*IN_MILLISECONDS;
    }

    void AchievFailed()
    {
        m_bAchievFailed = true;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        ScriptedAI::MoveInLineOfSight(pWho);

        if (m_bIntro)
            return;

        DoScriptText(SAY_INTRO, m_creature);
        m_bIntro = true;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1))
        {
            case 0:
               DoScriptText(SAY_KILL1, m_creature, pVictim);
               break;
            case 1:
               DoScriptText(SAY_KILL2, m_creature, pVictim);
               break;
        };
    }

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, DONE);

        DoScriptText(SAY_DEATH, m_creature);

        if (!m_bAchievFailed)
        {
            bool m_bIs10ManMode;
            switch (m_uiMode)
            {
                case RAID_DIFFICULTY_10MAN_NORMAL:
                case RAID_DIFFICULTY_10MAN_HEROIC:
                    m_bIs10ManMode = true;
                    break;
                case RAID_DIFFICULTY_25MAN_NORMAL:
                case RAID_DIFFICULTY_25MAN_HEROIC:
                    m_bIs10ManMode = false;
                    break;
                default:
                    break;
            }

            AchievementEntry const *AchievBoned = GetAchievementStore()->LookupEntry(m_bIs10ManMode ? ACHIEV_BONED_10 : ACHIEV_BONED_25);
            if (AchievBoned)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->CompletedAchievement(AchievBoned);
                }
            }
        }
    }


    void DoSummonSpike(Unit* pTarget)
    {
        if (!pTarget || !pTarget->isAlive())
            return;

        float m_fPosX, m_fPosY, m_fPosZ;
        pTarget->GetPosition(m_fPosX, m_fPosY, m_fPosZ);

        if (Creature* pSpike = m_creature->SummonCreature(NPC_BONE_SPIKE, m_fPosX, m_fPosY, m_fPosZ + 0.5f, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0))
        {
            pSpike->SetOwnerGuid(m_creature->GetObjectGuid());
            pSpike->SetInCombatWith(pTarget);
            pSpike->AddThreat(pTarget, 1000.0f);
        }
    }

    void MoveToFarthestTarget(Creature* pCreature)
    {
        float m_fMaxRange = 1.0f;
        float m_fRange;
        Player* pTarget = NULL;

        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                if (pPlayer->isTargetableForAttack())
                {
                    m_fRange = pPlayer->GetDistance(pCreature);
                    if (m_fRange > m_fMaxRange)
                    {
                        m_fMaxRange = m_fRange;
                        pTarget = pPlayer;
                    }
                }
            }
        }

        if (!pTarget)
            return;

        pCreature->GetMotionMaster()->MovePoint(0, pTarget->GetPositionX(),  pTarget->GetPositionY(),  pTarget->GetPositionZ());
    }

    void DoBoneSpikeAttack(Creature* pCreature, uint8 m_uiPlayerMode)
    {
        std::list<uint64> PlayerGuidList;
        std::list<uint64>::iterator itr;

        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                if (pPlayer->isTargetableForAttack() || !pPlayer->HasAura(SPELL_BONE_STRIKE_IMPALE))
                    if (pPlayer->GetGUID() != pCreature->getVictim()->GetGUID())
                        PlayerGuidList.push_back(pPlayer->GetGUID());
        }

        if (PlayerGuidList.empty())
            return;

        if (m_uiPlayerMode == 10)
        {
            itr = PlayerGuidList.begin();
            std::advance(itr, urand(0, PlayerGuidList.size() - 1));
            PlayerGuidList.remove(*itr);
            if (Unit * pPlayer = pCreature->GetMap()->GetPlayer(*itr))
                DoSummonSpike(pPlayer);
        }
        else if (m_uiPlayerMode == 25)
        {
            if (PlayerGuidList.size() < 3)
            {
                for (std::list<uint64>::iterator itr = PlayerGuidList.begin(); itr != PlayerGuidList.end(); ++itr)
                {
                    if (Unit* pPlayer = pCreature->GetMap()->GetPlayer(*itr))
                        DoSummonSpike(pPlayer);
                }
            }
            else
            {
                for (uint8 i = 0; i < 2; ++i)
                {
                    itr = PlayerGuidList.begin();
                    std::advance(itr, urand(0, PlayerGuidList.size() - 1));
                    PlayerGuidList.remove(*itr);
                    if (Unit * pPlayer = pCreature->GetMap()->GetPlayer(*itr))
                        DoSummonSpike(pPlayer);
                }
            }
        }

        PlayerGuidList.clear();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bPhase1)
        {
            if (m_uiBoneSpikeGraveyardTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->getVictim())
                {
                    if (!pTarget->isAlive())
                        return;

                    if (!m_bSummon)
                    {
                        switch (m_uiMode)
                        {
                            case RAID_DIFFICULTY_10MAN_NORMAL:
                                DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_10_N);
                                break;
                            case RAID_DIFFICULTY_25MAN_NORMAL:
                                DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_25_N);
                                break;
                            case RAID_DIFFICULTY_10MAN_HEROIC:
                                DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_10_H_0);
                                break;
                            case RAID_DIFFICULTY_25MAN_HEROIC:
                                DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_25_H_0);
                                break;
                            default:
                                break;
                        }

                        switch (urand(0,2))
                        {
                            case 0:
                                DoScriptText(SAY_BONESPIKE1, m_creature, pTarget);
                                break;
                            case 1:
                                DoScriptText(SAY_BONESPIKE2, m_creature, pTarget);
                                break;
                            case 2:
                                DoScriptText(SAY_BONESPIKE3, m_creature, pTarget);
                                break;
                        }

                        m_bSummon = true;
                        m_uiSummonBoneSpikeTimer = 3*IN_MILLISECONDS;
                    }

                    if (m_bSummon)
                    {
                        if (m_uiSummonBoneSpikeTimer < uiDiff)
                        {
                            switch (m_uiMode)
                            {
                                case RAID_DIFFICULTY_10MAN_NORMAL:
                                case RAID_DIFFICULTY_10MAN_HEROIC:
                                    DoBoneSpikeAttack(m_creature, 10);
                                    m_bSummon = false;
                                    m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
                                    break;
                                case RAID_DIFFICULTY_25MAN_NORMAL:
                                case RAID_DIFFICULTY_25MAN_HEROIC:
                                    DoBoneSpikeAttack(m_creature, 25);
                                    m_bSummon = false;
                                    m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
                                    break;
                                default:
                                    break;
                            }
                        }
                        else
                            m_uiSummonBoneSpikeTimer -= uiDiff;
                    }
                }
            }
            else
                m_uiBoneSpikeGraveyardTimer -= uiDiff;

            if (m_uiBoneSliceTimer < uiDiff)
            {
                switch (m_uiMode)
                {
                    case RAID_DIFFICULTY_10MAN_NORMAL:
                    case RAID_DIFFICULTY_10MAN_HEROIC:
                        DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_10);
                        break;
                    case RAID_DIFFICULTY_25MAN_NORMAL:
                    case RAID_DIFFICULTY_25MAN_HEROIC:
                        DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_25);
                        break;
                    default:
                        break;
                }
                
                m_uiBoneSliceTimer = urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS);
            }
            else
                m_uiBoneSliceTimer -= uiDiff;

            if (m_uiColdFlameTimer < uiDiff)
            {
                float m_fPosX, m_fPosY, m_fPosZ;
                m_creature->GetPosition(m_fPosX, m_fPosY, m_fPosZ);
                m_creature->SummonCreature(NPC_COLD_FLAME, m_fPosX, m_fPosY, m_fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);

                m_uiColdFlameTimer = 10*IN_MILLISECONDS;
            }
            else
                m_uiColdFlameTimer -= uiDiff;

            if (m_uiBoneStormTimer < uiDiff)
            {
                m_creature->InterruptNonMeleeSpells(true);

                DoCast(m_creature, SPELL_BONE_STORM);
                DoResetThreat();
                SetCombatMovement(false);
                m_creature->SetSpeedRate(MOVE_RUN, 3);
                m_creature->SetSpeedRate(MOVE_WALK, 3);

                m_uiBoneStormTimer = 23*IN_MILLISECONDS;
                m_uiTargetSwitchTimer = 3*IN_MILLISECONDS;
                m_bPhase1 = false;
                m_bPhase2 = true;

                DoScriptText(SAY_BONESTORM, m_creature);
                return;
            }
            else
                m_uiBoneStormTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        else if (m_bPhase2)
        {
            if (m_uiBoneStormTimer < uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(SPELL_BONE_STORM_STRIKE);
                m_creature->RemoveAurasDueToSpell(SPELL_BONE_STORM);

                m_bPhase1 = true;
                m_bPhase2 = false;

                SetCombatMovement(true);
                m_creature->SetSpeedRate(MOVE_RUN, 1.5);
                m_creature->SetSpeedRate(MOVE_WALK, 1.5);
                m_pInstance->SetData(DATA_DIRECTION, 0);

                m_uiBoneSliceTimer = urand(10*IN_MILLISECONDS, 25*IN_MILLISECONDS);
                m_uiBoneStormTimer = 50*IN_MILLISECONDS;
                m_uiColdFlameTimer = 10*IN_MILLISECONDS;

                switch (m_uiMode)
                {
                    case RAID_DIFFICULTY_10MAN_NORMAL:
                    case RAID_DIFFICULTY_25MAN_NORMAL:
                        m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
                        break;
                    default:
                        break;
                }

                DoResetThreat();

                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->AI()->AttackStart(m_creature->getVictim());
                return;
            }
            else
                m_uiBoneStormTimer -= uiDiff;

            switch (m_uiMode)
            {
                case RAID_DIFFICULTY_10MAN_HEROIC:
                case RAID_DIFFICULTY_25MAN_HEROIC:
                    if (m_uiBoneSpikeGraveyardTimer < uiDiff)
                    {
                        if (Unit* pTarget = m_creature->getVictim())
                        {
                            if (!pTarget->isAlive())
                                return;

                            if (!m_bSummon)
                            {
                                switch (m_uiMode)
                                {
                                    case RAID_DIFFICULTY_10MAN_HEROIC:
                                        //DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_10_H_1);
                                        break;
                                    case RAID_DIFFICULTY_25MAN_HEROIC:
                                        //DoCast(pTarget, SPELL_BONE_SPIKE_GRAVEYARD_25_H_1);
                                        break;
                                    default:
                                        break;
                                }

                                switch (urand(0,2))
                                {
                                    case 0:
                                        DoScriptText(SAY_BONESPIKE1, m_creature, pTarget);
                                        break;
                                    case 1:
                                        DoScriptText(SAY_BONESPIKE2, m_creature, pTarget);
                                        break;
                                    case 2:
                                        DoScriptText(SAY_BONESPIKE3, m_creature, pTarget);
                                        break;
                                }

                                m_bSummon = true;
                                m_uiSummonBoneSpikeTimer = 1*IN_MILLISECONDS;
                            }

                            if (m_bSummon)
                            {
                                if (m_uiSummonBoneSpikeTimer < uiDiff)
                                {
                                    switch (m_uiMode)
                                    {
                                        case RAID_DIFFICULTY_10MAN_HEROIC:
                                            DoBoneSpikeAttack(m_creature, 10);
                                            m_bSummon = false;
                                            m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
                                            break;
                                        case RAID_DIFFICULTY_25MAN_HEROIC:
                                            DoBoneSpikeAttack(m_creature, 25);
                                            m_bSummon = false;
                                            m_uiBoneSpikeGraveyardTimer = 20*IN_MILLISECONDS;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                else
                                    m_uiSummonBoneSpikeTimer -= uiDiff;
                            }
                        }
                    }
                    else
                        m_uiBoneSpikeGraveyardTimer -= uiDiff;
                    break;
                default:
                    break;
            }

            if (m_uiTargetSwitchTimer < uiDiff)
            {
                m_pInstance->SetData(DATA_DIRECTION, (uint32)(1000*2.0f*M_PI_F*((float)urand(1,16)/16.0f)));
                float m_fPosX, m_fPosY, m_fPosZ;
                m_creature->GetPosition(m_fPosX, m_fPosY, m_fPosZ);
                m_creature->SummonCreature(NPC_COLD_FLAME, m_fPosX, m_fPosY, m_fPosZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);

                DoResetThreat();
                MoveToFarthestTarget(m_creature);

                m_uiTargetSwitchTimer = 5*IN_MILLISECONDS;
            }
            else
                m_uiTargetSwitchTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

    ObjectGuid m_uiVictimGuid;
    uint32 m_uiAchievTimer;

    void Reset()
    {
        m_uiVictimGuid = ObjectGuid();
        m_uiAchievTimer = 8*IN_MILLISECONDS;

        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        if (m_uiVictimGuid.IsEmpty() && pWho && pWho->GetTypeId() == TYPEID_PLAYER)
        {
            m_uiVictimGuid = pWho->GetObjectGuid();
            m_creature->SetInCombatWith(pWho);
            DoCast(pWho, SPELL_BONE_STRIKE_IMPALE);
            DoCast(pWho, SPELL_VEHICLE_HARDCODED);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
            if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_uiVictimGuid))
                pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void AttackStart(Unit *who)
    {
    }

    void KilledUnit(Unit* pVictim)
    {
        if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_uiVictimGuid))
            pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void JustDied(Unit* pKiller)
    {
        if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_uiVictimGuid))
            pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_uiVictimGuid))
                pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
            m_creature->ForcedDespawn();
        }

        if (m_uiVictimGuid.IsEmpty())
            return;

        if (Player* pVictim = m_creature->GetMap()->GetPlayer(m_uiVictimGuid))
        {
            if(!pVictim->isAlive())
                m_creature->ForcedDespawn();
        }
        else
            m_creature->ForcedDespawn();

        if (m_uiAchievTimer < uiDiff)
        {
            if (Creature* pMarrowgar = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LORD_MARROWGAR)))
                if (boss_lord_marrowgarAI* pMarrowgarAI = dynamic_cast<boss_lord_marrowgarAI*>(pMarrowgar->AI()))
                    pMarrowgarAI->AchievFailed();
        }
        else
            m_uiAchievTimer -= uiDiff; 
    }
};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        m_uiMode = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 m_uiMode;

    bool m_bIsFirst;
    bool m_bIsXmode;
    bool m_bIsCreator;
    float m_fDirection;
    float m_fX, m_fY, m_fRadius;
    uint8 m_uiFlameCounter;
    uint8 m_uiFlameMaxCounter;
    uint32 m_uiColdFlameCreationTimer;
    uint32 m_uiColdFlameDespawnTimer;
    uint32 m_uiColdFlameDamageTimer;
    uint32 m_uiColdFlameDespawnDelay;

    void Reset()
    {
        if(!m_pInstance)
            return;

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);

        m_bIsCreator = false;
        m_uiFlameCounter = 0;
        m_uiFlameMaxCounter = 15;
        m_uiColdFlameCreationTimer = 1*IN_MILLISECONDS;
        m_uiColdFlameDamageTimer = 1*IN_MILLISECONDS;
        m_uiColdFlameDespawnDelay = 10*IN_MILLISECONDS;
        
        DoCast(m_creature, SPELL_COLD_FLAME_VISUAL);
    }

    void AttackStart(Unit *who)
    {
    }

    void JustSummoned(Creature* pSummoned)
    {
        if(!m_pInstance || !pSummoned)
            return;
        
        pSummoned->SetCreatorGuid(m_creature->GetObjectGuid());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (m_creature->GetCreatorGuid().IsEmpty())
            return;

        if (!m_bIsCreator)
        {
            if (m_creature->GetCreatorGuid() == m_pInstance->GetData64(NPC_LORD_MARROWGAR))
            {
                m_bIsFirst = true;

                uint32 m_uiTmpDirection = m_pInstance->GetData(DATA_DIRECTION);
                m_pInstance->SetData(DATA_DIRECTION, 0);

                if (m_uiTmpDirection)
                {
                    m_fDirection = m_uiTmpDirection/1000.0f;
                    m_bIsXmode = true;
                }
                else
                {
                    m_fDirection = 2.0f*M_PI_F*((float)urand(1,16)/16.0f);
                    m_bIsXmode = false;
                }
            }
            else
                m_bIsFirst = false;
            
            m_bIsCreator = true;
        }

        if (m_uiColdFlameDespawnTimer < uiDiff)
        {
            if (!m_bIsFirst)
                m_creature->ForcedDespawn();
        }
        else
            m_uiColdFlameDespawnTimer -= uiDiff;

        if (m_uiColdFlameCreationTimer < uiDiff)
        {
            if (m_bIsFirst)
            {
                if (m_uiFlameCounter < m_uiFlameMaxCounter)
                {
                    m_uiFlameCounter += 1;
                    m_fRadius = m_uiFlameCounter*5;

                    m_creature->GetNearPoint2D(m_fX, m_fY, m_fRadius, m_fDirection);
                    m_creature->SummonCreature(NPC_COLD_FLAME, m_fX, m_fY, m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);

                    if (m_bIsXmode)
                    {
                        m_creature->GetNearPoint2D(m_fX, m_fY, m_fRadius, m_fDirection + M_PI_F/2);
                        m_creature->SummonCreature(NPC_COLD_FLAME, m_fX, m_fY, m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);
                        m_creature->GetNearPoint2D(m_fX, m_fY, m_fRadius, m_fDirection + M_PI_F);
                        m_creature->SummonCreature(NPC_COLD_FLAME, m_fX, m_fY, m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);
                        m_creature->GetNearPoint2D(m_fX, m_fY, m_fRadius, m_fDirection + M_PI_F*1.5f);
                        m_creature->SummonCreature(NPC_COLD_FLAME, m_fX, m_fY, m_creature->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, m_uiColdFlameDespawnDelay);
                    }
                }
                else
                    m_creature->ForcedDespawn();
            }

            m_uiColdFlameCreationTimer = 2*IN_MILLISECONDS;
        }
        else
            m_uiColdFlameCreationTimer -= uiDiff;

        if (m_uiColdFlameDamageTimer < uiDiff)
        {
            switch (m_uiMode)
            {
                case RAID_DIFFICULTY_10MAN_NORMAL:
                    DoCast(m_creature->getVictim(), SPELL_COLD_FLAME_10_N);
                    break;
                case RAID_DIFFICULTY_25MAN_NORMAL:
                    DoCast(m_creature->getVictim(), SPELL_COLD_FLAME_25_N);
                    break;
                case RAID_DIFFICULTY_10MAN_HEROIC:
                    DoCast(m_creature->getVictim(), SPELL_COLD_FLAME_10_H);
                    break;
                case RAID_DIFFICULTY_25MAN_HEROIC:
                    DoCast(m_creature->getVictim(), SPELL_COLD_FLAME_25_H);
                    break;
                default:
                    break;
            }

            m_uiColdFlameDamageTimer = 1*IN_MILLISECONDS;
        }
        else
            m_uiColdFlameDamageTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_lord_marrowgar";
    pNewScript->GetAI = &GetAI_boss_lord_marrowgar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_bone_spike";
    pNewScript->GetAI = &GetAI_mob_bone_spike;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_coldflame";
    pNewScript->GetAI = &GetAI_mob_coldflame;
    pNewScript->RegisterSelf();
}
