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
SDName: boss_deathbringer_saurfang
SD%Complete:
SDComment: by /dev/rsa, IOV
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    // Spells
    SPELL_BERSERK                           = 26662,
    SPELL_BLOOD_LINK                        = 72178,
    SPELL_BLOOD_LINK_BEAST                  = 72176,
    SPELL_BLOOD_POWER                       = 72371,
    SPELL_MARK                              = 72293,
    SPELL_MARK_SELF                         = 72256,
    SPELL_MARK_DAMAGE_10_N                  = 72255,
    SPELL_MARK_DAMAGE_10_H                  = 72445,
    SPELL_MARK_DAMAGE_25_N                  = 72444,
    SPELL_MARK_DAMAGE_25_H                  = 72446,
    SPELL_FRENZY                            = 72737,
    SPELL_BOILING_BLOOD_10_N                = 72385,
    SPELL_BOILING_BLOOD_10_H                = 72442,
    SPELL_BOILING_BLOOD_25_N                = 72441,
    SPELL_BOILING_BLOOD_25_H                = 72443,
    SPELL_BLOOD_NOVA_10                     = 72380,
    SPELL_BLOOD_NOVA_25                     = 73058,
    SPELL_RUNE_OF_BLOOD                     = 72410,
    SPELL_CALL_BLOOD_BEAST_1                = 72172,
    SPELL_CALL_BLOOD_BEAST_2                = 72173,
    SPELL_CALL_BLOOD_BEAST_3                = 72356,
    SPELL_CALL_BLOOD_BEAST_4                = 72357,
    SPELL_CALL_BLOOD_BEAST_5                = 72358,
    SPELL_RESISTANT_SKIN                    = 72723,
    SPELL_SCENT_OF_BLOOD_BUFF               = 72771,
    SPELL_SCENT_OF_BLOOD_DEBUFF             = 72769,
    SPELL_ZERO_REGEN                        = 72242,

    // NPCs
    NPC_BLOOD_BEASTS                        = 38508,

    // Texts
    SAY_AGGRO                               = -1631100,
    SAY_FALLENCHAMPION                      = -1631101,
    SAY_BLOODBEASTS                         = -1631102,
    SAY_KILL1                               = -1631103,
    SAY_KILL2                               = -1631104,
    SAY_FRENZY                              = -1631105,
    SAY_DEATH                               = -1631106,
    SAY_INTRO                               = -1631107,
    SAY_BERSERK                             = -1631108,
    SAY                                     = -1631109, // not used

    // Achievements
    ACHIEV_GONE_MESS_10                     = 4537,
    ACHIEV_GONE_MESS_25                     = 4613,
};

enum Equipment
{
    EQUIP_MAIN           = 50798,
    EQUIP_OFFHAND        = 50798,
    EQUIP_RANGED         = EQUIP_NO_CHANGE,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

struct MANGOS_DLL_DECL boss_deathbringer_saurfangAI : public ScriptedAI
{
    boss_deathbringer_saurfangAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_uiMode = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 m_uiMode;

    bool m_bIntro;
    bool m_bIsFenzy;
    uint8 m_uiBeastCount;
    uint8 m_uiMarkCount;
    uint32 m_uiBerserkTimer;
    uint32 m_uiBloodNovaTimer;
    uint32 m_uiBloodBeastTimer;
    uint32 m_uiBoilingBloodTimer;
    uint32 m_uiBoilingBloodDamageTimer;
    uint32 m_uiRuneofBloodTimer;
    uint64 m_uiBloodBoilTargetGuid;

    int32 oldPower;

    void Reset()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);

        m_creature->SetPower(POWER_ENERGY, 0);
        DoCast(m_creature, SPELL_ZERO_REGEN);

        m_bIntro = false;
        m_bIsFenzy = false;

        m_uiBeastCount = 0;
        m_uiMarkCount = 0;
        m_uiBerserkTimer = 6*MINUTE*IN_MILLISECONDS;
        m_uiBloodNovaTimer = 20*IN_MILLISECONDS;
        m_uiBloodBeastTimer = 40*IN_MILLISECONDS;
        m_uiBoilingBloodTimer = 1*IN_MILLISECONDS;
        m_uiBoilingBloodDamageTimer = 0;
        m_uiRuneofBloodTimer = 0;
        m_uiBloodBoilTargetGuid = 0;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        ScriptedAI::MoveInLineOfSight(pWho);

        if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_bIntro)
            return;

        if (!m_creature->isInCombat() && pWho->IsWithinDistInMap(m_creature, 20.0f))
        {
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            DoScriptText(SAY_INTRO, m_creature);
            m_bIntro = true;
        }
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, IN_PROGRESS);

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        DoCast(m_creature, SPELL_BLOOD_POWER);
        DoCast(m_creature, SPELL_BLOOD_LINK);
        DoCast(m_creature, SPELL_MARK_SELF);

        SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, FAIL);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (!pVictim || pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (pVictim->HasAura(SPELL_MARK))
        {
            switch (m_uiMode)
            {
                case RAID_DIFFICULTY_10MAN_NORMAL:
                case RAID_DIFFICULTY_25MAN_NORMAL:
                    m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.05f);
                    pVictim->RemoveAurasDueToSpell(SPELL_MARK);
                    break;
                case RAID_DIFFICULTY_10MAN_HEROIC:
                case RAID_DIFFICULTY_25MAN_HEROIC:
                    m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.2f);
                    pVictim->RemoveAurasDueToSpell(SPELL_MARK);
                    break;
                default:
                    break;
            }
        }

        switch (urand(0,1)) 
        {
            case 0:
               DoScriptText(SAY_KILL1, m_creature,pVictim);
               break;
            case 1:
               DoScriptText(SAY_KILL2, m_creature,pVictim);
               break;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if(!m_pInstance || !pSummoned)
            return;

        pSummoned->SetOwnerGuid(m_creature->GetObjectGuid());

        float m_fPosX, m_fPosY, m_fPosZ;
        pSummoned->GetPosition(m_fPosX, m_fPosY, m_fPosZ);
        pSummoned->GetMotionMaster()->MovePoint(1, m_fPosX, m_fPosY, m_fPosZ);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAURFANG, DONE);

        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                if (pPlayer->HasAura(SPELL_MARK))
                    pPlayer->RemoveAurasDueToSpell(SPELL_MARK);
        }

        bool m_bIs10ManMode;
        bool m_bAchievFailed;
        switch (m_uiMode)
        {
            case RAID_DIFFICULTY_10MAN_NORMAL:
            case RAID_DIFFICULTY_10MAN_HEROIC:
                if (m_uiMarkCount < 3)
                    m_bAchievFailed = false;
                else
                    m_bAchievFailed = true;
                m_bIs10ManMode = true;
                break;
            case RAID_DIFFICULTY_25MAN_NORMAL:
            case RAID_DIFFICULTY_25MAN_HEROIC:
                if (m_uiMarkCount < 5)
                    m_bAchievFailed = false;
                else 
                    m_bAchievFailed = true;
                m_bIs10ManMode = false;
                break;
            default:
                break;
        }

        if (!m_bAchievFailed)
        {
            AchievementEntry const *AchievGoneMess = GetAchievementStore()->LookupEntry(m_bIs10ManMode ? ACHIEV_GONE_MESS_10 : ACHIEV_GONE_MESS_25);
            if (AchievGoneMess)
            {
                Map* pMap = m_creature->GetMap();
                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        itr->getSource()->CompletedAchievement(AchievGoneMess);
                }
            }
        }
    }

    void SpellHitTarget (Unit* pUnit, const SpellEntry* pSpellEntry)
    {
        if (pUnit->GetTypeId() != TYPEID_PLAYER)
           return;

        switch (pSpellEntry->Id)
        {
            case SPELL_BLOOD_NOVA_10:
            case SPELL_BLOOD_NOVA_25:
                m_creature->SetPower(POWER_ENERGY, m_creature->GetPower(POWER_ENERGY) + 2);
                break;
            case SPELL_MARK_DAMAGE_10_N:
            case SPELL_MARK_DAMAGE_10_H:
            case SPELL_MARK_DAMAGE_25_N:
            case SPELL_MARK_DAMAGE_25_H:
                m_creature->SetPower(POWER_ENERGY, m_creature->GetPower(POWER_ENERGY) + 1);
            default:
                break;
        }
    }

    Unit* BloodNovaTarget()
    {
        std::list<uint64> PlayerGuidList;
        std::list<uint64>::iterator itr;
        Unit* pTarget = NULL;

        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                if (pPlayer->isAlive())
                    if (!m_creature->IsWithinDistInMap(pPlayer, ATTACK_DISTANCE))
                        PlayerGuidList.push_back(pPlayer->GetGUID());
        }

        if (PlayerGuidList.empty())
        {
            if (pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                return pTarget;
        }
        else
        {
            itr = PlayerGuidList.begin();
            std::advance(itr, urand(0, PlayerGuidList.size() - 1));

            if (pTarget = m_creature->GetMap()->GetPlayer(*itr))
                return pTarget;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_BLOOD_POWER))
            DoCast(m_creature, SPELL_BLOOD_POWER);

        if (!m_creature->HasAura(SPELL_BLOOD_LINK))
            DoCast(m_creature, SPELL_BLOOD_LINK);

        if (!m_creature->HasAura(SPELL_MARK_SELF))
            DoCast(m_creature, SPELL_MARK_SELF);

        if (m_creature->GetPower(POWER_ENERGY) == 100)
        {
           if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
           {
               if (pTarget->GetTypeId() == TYPEID_PLAYER && !pTarget->HasAura(SPELL_MARK))
               {
                   DoScriptText(SAY_FALLENCHAMPION, m_creature);
                   m_creature->SetPower(POWER_ENERGY, 0);
                   DoCast(pTarget, SPELL_MARK);
                   ++m_uiMarkCount;
               }
           }
        }

        if (m_uiBloodNovaTimer < uiDiff)
        {
            switch (m_uiMode)
            {
                case RAID_DIFFICULTY_10MAN_NORMAL:
                case RAID_DIFFICULTY_10MAN_HEROIC:
                    DoCast(BloodNovaTarget(), SPELL_BLOOD_NOVA_10);
                    m_uiBoilingBloodDamageTimer = 3*IN_MILLISECONDS;
                    break;
                case RAID_DIFFICULTY_25MAN_NORMAL:
                case RAID_DIFFICULTY_25MAN_HEROIC:
                    DoCast(BloodNovaTarget(), SPELL_BLOOD_NOVA_25);
                    m_uiBoilingBloodDamageTimer = 3*IN_MILLISECONDS;
                    break;
                default:
                    break;
            }

            m_uiBloodNovaTimer = 20*IN_MILLISECONDS;
        }
        else
            m_uiBloodNovaTimer -= uiDiff;

        if (m_uiBoilingBloodTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                m_uiBloodBoilTargetGuid = pTarget->GetGUID();
                switch (m_uiMode)
                {
                    case RAID_DIFFICULTY_10MAN_NORMAL:
                        DoCast(pTarget, SPELL_BOILING_BLOOD_10_N);
                        break;
                    case RAID_DIFFICULTY_10MAN_HEROIC:
                        DoCast(pTarget, SPELL_BOILING_BLOOD_10_H);
                        break;
                    case RAID_DIFFICULTY_25MAN_NORMAL:
                        DoCast(pTarget, SPELL_BOILING_BLOOD_25_N);
                        break;
                    case RAID_DIFFICULTY_25MAN_HEROIC:
                        DoCast(pTarget, SPELL_BOILING_BLOOD_25_H);
                        break;
                    default:
                        break;
                }
            }
            
            m_uiBoilingBloodDamageTimer = 3*IN_MILLISECONDS;
            m_uiBoilingBloodTimer = 15*IN_MILLISECONDS;
        }
        else
            m_uiBoilingBloodTimer -= uiDiff;

        if (m_uiBoilingBloodDamageTimer < uiDiff)
        {
            if (m_uiBloodBoilTargetGuid)
                if (Player* pBloodBoilTarget = m_creature->GetMap()->GetPlayer(m_uiBloodBoilTargetGuid))
                    if (pBloodBoilTarget->HasAura(SPELL_BOILING_BLOOD_10_N) ||
                        pBloodBoilTarget->HasAura(SPELL_BOILING_BLOOD_10_H) ||
                        pBloodBoilTarget->HasAura(SPELL_BOILING_BLOOD_25_N) ||
                        pBloodBoilTarget->HasAura(SPELL_BOILING_BLOOD_25_H))
                    {
                        m_creature->SetPower(POWER_ENERGY, m_creature->GetPower(POWER_ENERGY) + 1);
                        m_uiBoilingBloodDamageTimer = 3*IN_MILLISECONDS;
                    }
        }
        else
            m_uiBoilingBloodDamageTimer -= uiDiff;

        if (m_uiRuneofBloodTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_RUNE_OF_BLOOD);
            m_uiRuneofBloodTimer = 25*IN_MILLISECONDS;
        }
        else
            m_uiRuneofBloodTimer -= uiDiff;

        if (m_uiBeastCount == 0)
        {
            if (m_uiBloodBeastTimer < uiDiff)
            {
                DoScriptText(SAY_BLOODBEASTS, m_creature);

                switch (m_uiMode)
                {
                    case RAID_DIFFICULTY_10MAN_NORMAL:
                        m_uiBeastCount = 2;
                        break;
                    case RAID_DIFFICULTY_10MAN_HEROIC:
                        m_uiBeastCount = 3;
                        break;
                    case RAID_DIFFICULTY_25MAN_NORMAL:
                        m_uiBeastCount = 4;
                        break;
                    case RAID_DIFFICULTY_25MAN_HEROIC:
                        m_uiBeastCount = 5;
                        break;
                    default:
                        break;
                }
            }
            else
                m_uiBloodBeastTimer -= uiDiff;
        }

        if (m_uiBeastCount > 0)
        {
            CanCastResult res = CAST_FAIL_OTHER;
            switch (m_uiBeastCount)
            {
                case 1:
                    res = DoCastSpellIfCan(m_creature, SPELL_CALL_BLOOD_BEAST_1);
                    break;
                case 2:
                    res = DoCastSpellIfCan(m_creature, SPELL_CALL_BLOOD_BEAST_2);
                    break;
                case 3:
                    res = DoCastSpellIfCan(m_creature, SPELL_CALL_BLOOD_BEAST_3);
                    break;
                case 4:
                    res = DoCastSpellIfCan(m_creature, SPELL_CALL_BLOOD_BEAST_4);
                    break;
                case 5:
                    res = DoCastSpellIfCan(m_creature, SPELL_CALL_BLOOD_BEAST_5);
                    break;
                default:
                    break;
            }

            if (res == CAST_OK)
            {
                --m_uiBeastCount;

                if (m_uiBeastCount == 0)
                    m_uiBloodBeastTimer = 40*IN_MILLISECONDS;
            }				
        }

        if (!m_bIsFenzy && m_creature->GetHealthPercent() < 30.0f)
        {
            DoScriptText(SAY_FRENZY, m_creature);
            DoCast(m_creature, SPELL_FRENZY);

            m_bIsFenzy = true;
        }

        if (m_uiBerserkTimer < uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);

            m_uiBerserkTimer = 6*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiBerserkTimer -= uiDiff;

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
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_uiMode = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 m_uiMode;

    Creature* pOwner;
    bool m_bHasAggro;
    uint32 m_uiAggroTimer;
    uint32 m_uiScentofBloodTimer;

    void Reset()
    {
        pOwner = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_DEATHBRINGER_SAURFANG));

        m_bHasAggro = false;
        m_uiAggroTimer = 2*IN_MILLISECONDS;
        m_uiScentofBloodTimer = 1*IN_MILLISECONDS;

        SetCombatMovement(false);
    }

    void JustDied(Unit* pKiller)
    {
        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                if (pPlayer->HasAura(SPELL_SCENT_OF_BLOOD_DEBUFF))
                    pPlayer->RemoveAurasDueToSpell(SPELL_SCENT_OF_BLOOD_DEBUFF);
        }
    }

    void DamageDeal(Unit* pVictim, uint32& uiDamage)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (pOwner)
            pOwner->SetPower(POWER_ENERGY, pOwner->GetPower(POWER_ENERGY) + 2);
    }

    void ApplyScentofBloodDebuff(Creature* pCreature)
    {
        std::list<uint64> PlayerGuidList;
        std::list<uint64>::iterator itr;

        Map::PlayerList const& players = m_creature->GetMap()->GetPlayers();
        for(Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
                if (pPlayer->isTargetableForAttack())
                    if (pPlayer->GetDistance(pCreature) <= ATTACK_DISTANCE)
                        if (!pPlayer->HasAura(SPELL_SCENT_OF_BLOOD_DEBUFF))
                            PlayerGuidList.push_back(pPlayer->GetGUID());
        }

        if (PlayerGuidList.empty())
            return;

        for (std::list<uint64>::iterator itr = PlayerGuidList.begin(); itr != PlayerGuidList.end(); ++itr)
        {
            if (Unit* pPlayer = pCreature->GetMap()->GetPlayer(*itr))
                pPlayer->_AddAura(SPELL_SCENT_OF_BLOOD_DEBUFF);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_SAURFANG) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(SPELL_BLOOD_LINK_BEAST))
            DoCast(m_creature, SPELL_BLOOD_LINK_BEAST);

        if (!m_creature->HasAura(SPELL_RESISTANT_SKIN))
            DoCast(m_creature, SPELL_RESISTANT_SKIN);

        switch (m_uiMode)
        {
            case RAID_DIFFICULTY_10MAN_HEROIC:
            case RAID_DIFFICULTY_25MAN_HEROIC:
                {
                    if (!m_creature->HasAura(SPELL_SCENT_OF_BLOOD_BUFF))
                        m_creature->_AddAura(SPELL_SCENT_OF_BLOOD_BUFF);

                    if (m_uiScentofBloodTimer < uiDiff)
                    {
                        ApplyScentofBloodDebuff(m_creature);
                        m_uiScentofBloodTimer = 1*IN_MILLISECONDS;
                    }
                    else
                        m_uiScentofBloodTimer -= uiDiff;
                }
                break;
            default:
                break;
        }

        if (!m_bHasAggro)
        {
            if (m_uiAggroTimer < uiDiff)
            {
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0));
                m_bHasAggro = true;
            }
            else 
                m_uiAggroTimer -= uiDiff;
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
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_deathbringer_saurfang";
    pNewScript->GetAI = &GetAI_boss_deathbringer_saurfang;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_blood_beast";
    pNewScript->GetAI = &GetAI_mob_blood_beast;
    pNewScript->RegisterSelf();
}
