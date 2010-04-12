/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

/* ScriptData
SDName: faction_champions
SD%Complete: 60%
SDComment: Scripts by Selector, modified by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"

#define AI_MELEE    0
#define AI_RANGED   1
#define AI_HEALER   2

#define SPELL_ANTI_AOE     68595
#define SPELL_PVP_TRINKET  65547

struct MANGOS_DLL_DECL boss_faction_championsAI : public ScriptedAI
{
    boss_faction_championsAI(Creature* pCreature, uint32 aitype) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
        mAIType = aitype;
        bsw = new BossSpellWorker(this);
        Init();
    }

    ScriptedInstance* m_pInstance;
    BossSpellWorker* bsw;

    uint32 mAIType;
    uint32 ThreatTimer;
    uint32 CCTimer;

    void Init()
    {
        CCTimer = rand()%10000;
        ThreatTimer = 5000;
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_CRUSADERS, FAIL);
            m_creature->ForcedDespawn();
    }

    float CalculateThreat(float distance, float armor, uint32 health)
    {
        float dist_mod = (mAIType == AI_MELEE) ? 15.0f/(15.0f + distance) : 1.0f;
        float armor_mod = (mAIType == AI_MELEE) ? armor / 16635.0f : 0.0f;
        float eh = (health+1) * (1.0f + armor_mod);
        return dist_mod * 30000.0f / eh;
    }

    void UpdateThreat()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator itr;
        bool empty = true;
        for(itr = tList.begin(); itr!=tList.end(); ++itr)
        {
            Unit* pUnit = Unit::GetUnit((*m_creature), (*itr)->getUnitGuid());
            if (pUnit && m_creature->getThreatManager().getThreat(pUnit))
            {
                if(pUnit->GetTypeId()==TYPEID_PLAYER)
                {
                    float threat = CalculateThreat(m_creature->GetDistance2d(pUnit), (float)pUnit->GetArmor(), pUnit->GetHealth());
                    m_creature->getThreatManager().modifyThreatPercent(pUnit, -100);
                    m_creature->AddThreat(pUnit, 1000000.0f * threat);
                    empty = false;
                }
            }
        }
    }

    void UpdatePower()
    {
        if(m_creature->getPowerType() == POWER_MANA)
            m_creature->ModifyPower(POWER_MANA, m_creature->GetMaxPower(POWER_MANA) / 3);
        //else if(m_creature->getPowerType() == POWER_ENERGY)
        //    m_creature->ModifyPower(POWER_ENERGY, 100);
    }

    void RemoveCC()
    {
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STUN);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_FEAR);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_ROOT);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_PACIFY);
        m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_CONFUSE);
        //DoCast(m_creature, SPELL_PVP_TRINKET);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) m_pInstance->SetData(TYPE_CHAMPIONS_COUNT, m_pInstance->GetData(TYPE_CHAMPIONS_COUNT)-1);
    }

    void Aggro(Unit *who)
    {
        DoCast(m_creature, SPELL_ANTI_AOE, true);
        if(m_pInstance) m_pInstance->SetData(TYPE_CRUSADERS, IN_PROGRESS);
    }

    void Reset()
    {
        if(m_pInstance) m_pInstance->SetData(TYPE_CRUSADERS, NOT_STARTED);
    }

    Creature* SelectRandomFriendlyMissingBuff(uint32 spell)
    {
        std::list<Creature *> lst = DoFindFriendlyMissingBuff(40.0f, spell);
        std::list<Creature *>::const_iterator itr = lst.begin();
        if(lst.empty()) 
            return NULL;
        advance(itr, rand()%lst.size());
        return (*itr);
    }

    Unit* SelectEnemyCaster(bool casting)
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator iter;
        for(iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            Unit *target;
            if(target = Unit::GetUnit((*m_creature),(*iter)->getUnitGuid()))
                if(target->getPowerType() == POWER_MANA)
                    return target;
        }
        return NULL;
    }

    uint32 EnemiesInRange(float distance)
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator iter;
        uint32 count = 0;
        for(iter = tList.begin(); iter!=tList.end(); ++iter)
        {
            Unit *target;
            if(target = Unit::GetUnit((*m_creature),(*iter)->getUnitGuid()))
                if(m_creature->GetDistance2d(target) < distance)
                    ++count;
        }
        return count;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho) return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if(mAIType==AI_MELEE)
                DoStartMovement(pWho);
            else
                DoStartMovement(pWho, 40.0f);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(ThreatTimer < diff)
        {
            UpdatePower();
            UpdateThreat();
            ThreatTimer = 4000;
        }
        else ThreatTimer -= diff;

        if(CCTimer < diff)
        {
            RemoveCC();
            CCTimer = 8000+rand()%2000;
        }
        else CCTimer -= diff;

        if(mAIType == AI_MELEE) DoMeleeAttackIfReady();
    }
};

/********************************************************************
                            HEALERS
********************************************************************/

#define SPELL_LIFEBLOOM        66093
#define SPELL_NOURISH          66066
#define SPELL_REGROWTH         66067
#define SPELL_REJUVENATION     66065
#define SPELL_TRANQUILITY      66086
#define SPELL_BARKSKIN         65860 //1 min cd
#define SPELL_THORNS           66068
#define SPELL_NATURE_GRASP     66071 //1 min cd, self buff

struct MANGOS_DLL_DECL mob_toc_druidAI : public boss_faction_championsAI
{
    mob_toc_druidAI(Creature* pCreature) : boss_faction_championsAI(pCreature, AI_HEALER) {Init();}

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

          bsw->timedCast(SPELL_NATURE_GRASP, diff);

          bsw->timedCast(SPELL_TRANQUILITY, diff);

          if(bsw->timedQuery(SPELL_BARKSKIN, diff))
                if(m_creature->GetHealthPercent() < 50.0f)
                    bsw->doCast(SPELL_BARKSKIN);

        if(bsw->timedQuery(SPELL_LIFEBLOOM, diff))
            switch(urand(0,4))
            {
                case 0:
                    if(Unit* target = DoSelectLowestHpFriendly(40.0f))
                        bsw->doCast(SPELL_LIFEBLOOM, target);
                    break;
                case 1:
                    if(Unit* target = DoSelectLowestHpFriendly(40.0f))
                        bsw->doCast(SPELL_NOURISH, target);
                    break;
                case 2:
                    if(Unit* target = DoSelectLowestHpFriendly(40.0f))
                        bsw->doCast(SPELL_REGROWTH, target);
                    break;
                case 3:
                    if(Unit* target = DoSelectLowestHpFriendly(40.0f))
                        bsw->doCast(SPELL_REJUVENATION, target);
                    break;
                case 4:
                    if(Creature* target = SelectRandomFriendlyMissingBuff(SPELL_THORNS))
                        bsw->doCast(SPELL_THORNS, target);
                    break;
            }

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_HEALING_WAVE         66055
#define H_SPELL_HEALING_WAVE       68115
#define SPELL_RIPTIDE              66053
#define H_SPELL_RIPTIDE            68118
#define SPELL_SPIRIT_CLEANSE       66056 //friendly only
#define SPELL_HEROISM              65983
#define SPELL_BLOODLUST            65980
#define SPELL_HEX                  66054
#define SPELL_EARTH_SHIELD         66063
#define SPELL_EARTH_SHOCK          65973
#define H_SPELL_EARTH_SHOCK        68100

struct MANGOS_DLL_DECL mob_toc_shamanAI : public boss_faction_championsAI
{
    mob_toc_shamanAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_HEALER) {Init();}

    uint32 HeroTimer;
    uint32 HexTimer;
    uint32 GCDTimer;

    void Init()
    {
        HeroTimer = 30000 + rand()%30000;
        HexTimer = 10000 + rand()%30000;
        GCDTimer = 2500;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(HeroTimer < diff)
        {
            DoCast(m_creature, SPELL_HEROISM);
            HeroTimer = 600000;
        }
        else HeroTimer -= diff;

        if(HexTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(target, SPELL_HEX);
            HexTimer = 20000 + rand()%20000;
        }
        else HexTimer -= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,5))
            {
                case 0: case 1:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_HEALING_WAVE);
                    break;
                case 2:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_RIPTIDE);
                    break;
                case 3:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_EARTH_SHOCK);
                    break;
                case 4:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_SPIRIT_CLEANSE);
                    break;
                case 5:
                    if(Unit *target = SelectRandomFriendlyMissingBuff(SPELL_EARTH_SHIELD))
                                                DoCast(target, SPELL_EARTH_SHIELD);
                    break;
            }
            GCDTimer = 2000;
        }
        else GCDTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_HAND_OF_FREEDOM    68757 //25 sec cd
#define SPELL_BUBBLE             66010 //5 min cd
#define SPELL_CLEANSE            66116
#define SPELL_FLASH_OF_LIGHT     66113
#define H_SPELL_FLASH_OF_LIGHT   68008
#define SPELL_HOLY_LIGHT         66112
#define H_SPELL_HOLY_LIGHT       68011
#define SPELL_HAND_OF_PROTECTION 66009
#define SPELL_HOLY_SHOCK         66114
#define H_SPELL_HOLY_SHOCK       68015
#define SPELL_HAND_OF_PROTECTION 66009
#define SPELL_HAMMER_OF_JUSTICE  66613

struct MANGOS_DLL_DECL mob_toc_paladinAI : public boss_faction_championsAI
{
    mob_toc_paladinAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_HEALER) {Init();}

    bool BubbleUsed;
    bool ProtectionUsed;
    uint32 FreedomTimer;
    uint32 GCDTimer;
    uint32 ShockTimer;
    uint32 HammerOfJusticeTimer;

    void Init()
    {
        GCDTimer = 3000;
        ShockTimer = 6000 + rand()%4000;
        BubbleUsed = false;
        ProtectionUsed = false;
        FreedomTimer = 10000 + rand()%15000;
        HammerOfJusticeTimer = 15000 + rand()%10000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        //cast bubble at 20% hp
        if(!BubbleUsed && m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
        {
            DoCast(m_creature, SPELL_BUBBLE);
            BubbleUsed = true;
        }

        if(!ProtectionUsed)
        {
            if(Unit *target = DoSelectLowestHpFriendly(40.0f))
            {
                if(target->GetHealth() * 100 / target->GetMaxHealth() < 15)
                {
                    DoCast(target, SPELL_HAND_OF_PROTECTION);
                    ProtectionUsed = true;
                }
            }
        }

        if(ShockTimer < diff)
        {
            if(Unit *target = DoSelectLowestHpFriendly(40.0f))
            {
                DoCast (target, SPELL_HOLY_SHOCK);
                ShockTimer = 6000 + rand()%4000;
            }
            else
                ShockTimer = 1000;
        }
        else ShockTimer -=diff;

        if(FreedomTimer < diff)
        {
            if(Unit *target = SelectRandomFriendlyMissingBuff(SPELL_HAND_OF_FREEDOM))
                DoCast(target, SPELL_HAND_OF_FREEDOM);
            FreedomTimer = 25000 + rand()%10000;
        }
        else FreedomTimer -= diff;

        if(HammerOfJusticeTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                if(m_creature->GetDistance2d(target) < 15)
                {
                    DoCast(target, SPELL_HAMMER_OF_JUSTICE);
                    HammerOfJusticeTimer=40000+rand()%5000;
                }
                else
                    HammerOfJusticeTimer = 1000;
            }
        }
        else HammerOfJusticeTimer -= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,4))
            {
                case 0: case 1:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_FLASH_OF_LIGHT);
                    break;
                case 2: case 3:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_HOLY_LIGHT);
                    break;
                case 4:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_CLEANSE);
                    break;
            }
            GCDTimer = 2500;
        }
        else GCDTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_RENEW            66177
#define H_SPELL_RENEW          68035
#define SPELL_SHIELD           66099
#define H_SPELL_SHIELD         68032
#define SPELL_FLASH_HEAL       66104
#define H_SPELL_FLASH_HEAL     68023
#define SPELL_DISPEL           65546
#define SPELL_PSYCHIC_SCREAM   65543
#define SPELL_MANA_BURN        66100
#define H_SPELL_MANA_BURN      68026

struct MANGOS_DLL_DECL mob_toc_priestAI : public boss_faction_championsAI
{
    mob_toc_priestAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_HEALER) {Init();}

    uint32 GCDTimer;
    uint32 ScreamTimer;

    void Init()
    {
        GCDTimer = 2500;
        ScreamTimer = 5000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(ScreamTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                DoCast(m_creature, SPELL_PSYCHIC_SCREAM);
                ScreamTimer = 30000;
            }
            else
                ScreamTimer = 3000;
        }
        else ScreamTimer -= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,5))
            {
                case 0:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_RENEW);
                    break;
                case 1:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_SHIELD);
                    break;
                case 2: case 3:
                    if(Unit *target = DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_FLASH_HEAL);
                    break;
                case 4:
                    if(Unit *target = urand(0,1) ? SelectUnit(SELECT_TARGET_RANDOM,0) : DoSelectLowestHpFriendly(40.0f))
                        DoCast(target, SPELL_DISPEL);
                    break;
                case 5:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_MANA_BURN);
                    break;
            }
            GCDTimer = 2000;
        }
        else GCDTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

/********************************************************************
                            RANGED
********************************************************************/

#define SPELL_SILENCE          65542
#define SPELL_VAMPIRIC_TOUCH   65490
#define H_SPELL_VAMPIRIC_TOUCH 68091
#define SPELL_SW_PAIN          65541
#define H_SPELL_SW_PAIN        68088
#define SPELL_MIND_FLAY        65488
#define H_SPELL_MIND_FLAY      68042
#define SPELL_MIND_BLAST       65492
#define H_SPELL_MIND_BLAST     68038
#define SPELL_HORROR           65545
#define SPELL_DISPERSION       65544
#define SPELL_SHADOWFORM       16592

struct MANGOS_DLL_DECL mob_toc_shadow_priestAI : public boss_faction_championsAI
{
    mob_toc_shadow_priestAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_RANGED) {Init();}

    uint32 SilenceTimer;
    uint32 PsychicScreamTimer;
    uint32 MindBlastTimer;
    uint32 GCDTimer;
    uint32 DispersionTimer;

    void Init()
    {
        SilenceTimer = 15000;
        PsychicScreamTimer = 8000;
        MindBlastTimer = 15000;
        GCDTimer = 3000;
        DispersionTimer = 1000;
    }

    void Aggro(Unit *who)
    {
        boss_faction_championsAI::Aggro(who);
        DoCast(m_creature, SPELL_SHADOWFORM, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(PsychicScreamTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                DoCast(m_creature, SPELL_PSYCHIC_SCREAM);
                PsychicScreamTimer = 24000;
            }
            else
                PsychicScreamTimer = 3000;
        }
        else PsychicScreamTimer -= diff;

        if(DispersionTimer < diff)
        {
            if(m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_DISPERSION);
                DispersionTimer = 180000;
            }
            else DispersionTimer = 1000;
        }
        else DispersionTimer -= diff;

        if(SilenceTimer < diff)
        {
            if(Unit *target = SelectEnemyCaster(false))
                DoCast(target, SPELL_SILENCE);
            SilenceTimer = 45000;
        }
        else SilenceTimer -= diff;

        if(MindBlastTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_MIND_BLAST);
            MindBlastTimer = 8000;
        }
        else MindBlastTimer -= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,4))
            {
                case 0: case 1:
                    DoCast(m_creature->getVictim(), SPELL_MIND_FLAY);
                    break;
                case 2:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_VAMPIRIC_TOUCH);
                    break;
               case 3:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_SW_PAIN);
                    break;
               case 4:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_DISPEL);
                    break;
            }
            GCDTimer = 2000;
        }
        else GCDTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_HELLFIRE             65816
#define H_SPELL_HELLFIRE           68146
#define SPELL_CORRUPTION           65810
#define H_SPELL_CORRUPTION         68134
#define SPELL_Curse_of_Agony       65814
#define H_SPELL_Curse_of_Agony     68137
#define SPELL_Curse_of_Exhaustion  65815
#define SPELL_Fear                 65809 //8s
#define SPELL_Searing_Pain         65819
#define H_SPELL_Searing_Pain       68149
#define SPELL_Shadow_Bolt          65821
#define H_SPELL_Shadow_Bolt        68152
#define SPELL_Summon_Felhunter     67514
#define SPELL_Unstable_Affliction  65812
#define H_SPELL_Unstable_Affliction 68155 //15s

struct MANGOS_DLL_DECL mob_toc_warlockAI : public boss_faction_championsAI
{
    mob_toc_warlockAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_RANGED) {Init();}

        uint32 FearTimer;
        uint32 UnstableAfflictionTimer;
    uint32 HellfireTimer;
        uint32 GCDTimer;

    void Init()
    {
        FearTimer = 4000+rand()%1000;
            UnstableAfflictionTimer = 2000+rand()%1000;
        HellfireTimer = 15000;
            GCDTimer = 2500;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(FearTimer < diff)
                {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_Fear);
            FearTimer = 8000;
        }
                else FearTimer -= diff;

        if(HellfireTimer < diff)
        {
            if(EnemiesInRange(10.0f) > 2)
            {
                DoCast(m_creature, SPELL_HELLFIRE);
                HellfireTimer = 40000;
            }
            else
                HellfireTimer = 5000;
        }
        else HellfireTimer -= diff;

        if(UnstableAfflictionTimer  < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_Unstable_Affliction);
            UnstableAfflictionTimer = 15000;
        }
        else UnstableAfflictionTimer  -= diff;

                if(GCDTimer < diff)
        {
            switch(urand(0,5))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_Searing_Pain );
                                    break;
                case 1: case 2:
                    DoCast(m_creature->getVictim(), SPELL_Shadow_Bolt );
                                    break;
                                case 3:
                                        if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_CORRUPTION);
                                    break;
                                case 4:
                                        if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_Curse_of_Agony);
                    break;
                                case 5:
                                        if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_Curse_of_Exhaustion);
                                    break;
                         }
                         GCDTimer = 2500;
                }
                else GCDTimer -= diff;

                boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_Arcane_Barrage   65799 //3s
#define H_SPELL_Arcane_Barrage 67995
#define SPELL_Arcane_Blast 65791
#define H_SPELL_Arcane_Blast 67998
#define SPELL_Arcane_Explosion 65800
#define H_SPELL_Arcane_Explosion 68001
#define SPELL_Blink  65793 //15s
#define SPELL_Counterspell 65790 //24s
#define SPELL_Frost_Nova 65792 //25s
#define SPELL_Frostbolt 65807
#define H_SPELL_Frostbolt 68004
#define SPELL_Ice_Block  65802 //5min
#define SPELL_Polymorph 65801 //15s

struct MANGOS_DLL_DECL mob_toc_mageAI : public boss_faction_championsAI
{
    mob_toc_mageAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_RANGED) {Init();}

        uint32 ArcaneBarrageTimer;
        uint32 BlinkTimer;
        uint32 CounterspellTimer;
        uint32 FrostNovaTimer;
        bool IceBlockUsed;
        uint32 PolymorphTimer;
        uint32 GCDTimer;

    void Init()
    {
        ArcaneBarrageTimer = 2000+rand()%1000;
        BlinkTimer =  8000+rand()%1000;
        CounterspellTimer = 5000+rand()%1000;
        FrostNovaTimer =  7000+rand()%2000;
        IceBlockUsed = false;
        PolymorphTimer = 3000+rand()%3000;
        GCDTimer = 2000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

                if(CounterspellTimer < diff)
                {
            if(Unit *target = SelectEnemyCaster(false))
                            DoCast (target, SPELL_Counterspell);
                        CounterspellTimer = 24000;
                }
                else CounterspellTimer -= diff;

                if(BlinkTimer < diff) // escape
                {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth() && EnemiesInRange(10.0f)>3)
            {
                                DoCast(m_creature, SPELL_Frost_Nova, true);
                            DoCast (m_creature , SPELL_Blink);
                BlinkTimer = 25000;
            }
            else
                BlinkTimer = 5000;
                }
                else BlinkTimer -= diff;

                if(!IceBlockUsed && m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
                {
                        DoCast(m_creature, SPELL_Ice_Block);
                        IceBlockUsed = true;
                }

        if(PolymorphTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(target, SPELL_Polymorph);
            PolymorphTimer = 15000;
        }
        else PolymorphTimer -= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,2))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_Arcane_Barrage);
                    break;
                case 1:
                    DoCast(m_creature->getVictim(), SPELL_Arcane_Blast);
                    break;
                case 2:
                    if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                        DoCast(target, SPELL_Frostbolt);
                    break;
            }
            GCDTimer = 2500;
        }
        else GCDTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};


#define SPELL_AIMED_SHOT 65883
#define H_SPELL_AIMED_SHOT 67978//10s
#define SPELL_Call_Pet 67777//perm
#define SPELL_Deterrence 65871 //90s
#define SPELL_Disengage 65869 //30s
#define SPELL_EXPLOSIVE_SHOT   65866
#define H_SPELL_EXPLOSIVE_SHOT 67984 //6s
#define SPELL_Frost_Trap 65880 //30s
#define SPELL_SHOOT 65868 //1.7s
#define H_SPELL_SHOOT 67989
#define SPELL_Steady_Shot  65867 //3s
#define SPELL_WING_CLIP 66207 //6s
#define SPELL_Wyvern_Sting 65877 //60s

struct MANGOS_DLL_DECL mob_toc_hunterAI : public boss_faction_championsAI
{
    mob_toc_hunterAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_RANGED) {Init();}

        uint32 AimedShotTimer;
    uint32 CallPetTimer;
        uint32 DeterrenceTimer;
        uint32 DisengageTimer;
        uint32 ExplosiveShotTimer;
        uint32 FrostTrapTimer;
        uint32 AutoShootTimer;
        uint32 SteadyShotTimer;
        uint32 WingClipTimer;
        uint32 WyvernStingTimer;
    uint32 GCDTimer;

    void Init()
    {
                AimedShotTimer = 1000+rand()%2000;
                DeterrenceTimer = 20000;
                DisengageTimer = 10000+rand()%3000;
                ExplosiveShotTimer = 2000+rand()%3000;
                FrostTrapTimer = 11000+rand()%5000;
                SteadyShotTimer = 3200+rand()%2000;
                WingClipTimer =  6000+rand()%2000;
                WyvernStingTimer = 7000+rand()%3000;
                GCDTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

                if(DisengageTimer < diff)
                {
            if(EnemiesInRange(10.0f) > 3)
            {
                            DoCast(m_creature , SPELL_Disengage);
                            DisengageTimer = 30000;
            }
            else
                DisengageTimer = 3000;
                }
                else DisengageTimer -= diff;

                if(DeterrenceTimer < diff)
                {
            if(m_creature->GetHealth()*5 < m_creature->GetMaxHealth())
            {
                            DoCast(m_creature, SPELL_Deterrence);
                            DeterrenceTimer = 90000;
            }
            else DeterrenceTimer = 3000;
                }
                else DeterrenceTimer -= diff;

        if(WyvernStingTimer < diff)
                {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,1))
                            DoCast(target, SPELL_Wyvern_Sting);
            WyvernStingTimer = 60000;
                }
                else WyvernStingTimer -= diff;

        if(FrostTrapTimer < diff)
                {
                        DoCast(m_creature, SPELL_Frost_Trap );
            FrostTrapTimer = 30000 + rand()%1000;
                }
                else FrostTrapTimer -= diff;

        if(WingClipTimer < diff)
        {
            if(m_creature->GetDistance2d(m_creature->getVictim()) < 5.0f)
                DoCast(m_creature->getVictim(), SPELL_WING_CLIP);
            WingClipTimer = 5000;
        }
        else WingClipTimer-= diff;

        if(GCDTimer < diff)
        {
            switch(urand(0,3))
            {
                case 0: case 1:
                    DoCast(m_creature->getVictim(), SPELL_SHOOT);
                    break;
                case 2:
                    DoCast(m_creature->getVictim(), SPELL_EXPLOSIVE_SHOT);
                    break;
                case 3:
                    DoCast(m_creature->getVictim(), SPELL_AIMED_SHOT);
                    break;
            }
                    GCDTimer = 2500;
                }
                else GCDTimer -= diff; 

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_Cyclone  65859 //6s
#define SPELL_Entangling_Roots 65857 //10s
#define SPELL_Faerie_Fire 65863
#define SPELL_Force_of_Nature 65861 //180s
#define SPELL_Insect_Swarm 65855
#define H_SPELL_Insect_Swarm 67942
#define SPELL_Moonfire  65856 //5s
#define H_SPELL_Moonfire 67945
#define SPELL_Starfire 65854
#define H_SPELL_Starfire 67948
#define SPELL_Wrath         65862
#define H_SPELL_Wrath       67952

struct MANGOS_DLL_DECL mob_toc_boomkinAI : public boss_faction_championsAI
{
    mob_toc_boomkinAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_RANGED) {Init();}

        uint32 BarkskinTimer;
        uint32 CycloneTimer;
        uint32 EntanglingRootsTimer;
        uint32 MoonfireTimer;
    uint32 FaerieFireTimer;
        uint32 GCDTimer;

        void Init()
    {
                BarkskinTimer = 6000;
                CycloneTimer = 5000;
                EntanglingRootsTimer = 2000+rand()%1000;
                MoonfireTimer = 1000+rand()%1000;
        FaerieFireTimer = 10000;
                GCDTimer = 1500;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(BarkskinTimer < diff)
                {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
                            DoCast (m_creature, SPELL_BARKSKIN);
                            BarkskinTimer = 60000;
            }
            else
                BarkskinTimer = 3000;
                }
                else BarkskinTimer -= diff;

        if(CycloneTimer < diff)
                {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,1))
                            DoCast (target, SPELL_Cyclone);
                        CycloneTimer = 6000+rand()%2000;
                }
                else CycloneTimer -= diff;

        if(EntanglingRootsTimer < diff)
                {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                            DoCast(target, SPELL_Entangling_Roots);
                        EntanglingRootsTimer = 10000 + rand()%2000;
                }
                else EntanglingRootsTimer -= diff;

        if(FaerieFireTimer < diff)
        {
            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(target, SPELL_Faerie_Fire);
            FaerieFireTimer = 10000 + rand()%8000;
        }
        else FaerieFireTimer -= diff;

                if(GCDTimer < diff)
        {
            switch(urand(0,6))
            {
                case 0: case 1:
                                DoCast(m_creature->getVictim(), SPELL_Moonfire);
                            break;
                                case 2:
                            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                                    DoCast(target,SPELL_Insect_Swarm);
                            break;
                                case 3:
                            if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                                    DoCast(m_creature->getVictim(), SPELL_Starfire);
                            break;
                case 4: case 5: case 6:
                                DoCast(m_creature->getVictim(), SPELL_Wrath);
                            break;
                        }
                    GCDTimer = 2000;
                }
                else GCDTimer -= diff; 

        boss_faction_championsAI::UpdateAI(diff);
    }
};

/********************************************************************
                            MELEE
********************************************************************/

#define SPELL_BLADESTORM           65947
#define SPELL_INTIMIDATING_SHOUT   65930
#define SPELL_MORTAL_STRIKE        65926
#define SPELL_CHARGE               68764
#define SPELL_DISARM               65935
#define SPELL_OVERPOWER            65924
#define SPELL_SUNDER_ARMOR         65936
#define SPELL_SHATTERING_THROW     65940
#define SPELL_RETALIATION          65932

struct MANGOS_DLL_DECL mob_toc_warriorAI : public boss_faction_championsAI
{
    mob_toc_warriorAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

    uint32 BladestormTimer;
    uint32 FearTimer;
    uint32 MortalStrikeTimer;
    uint32 ChargeTimer;
    uint32 SunderArmorTimer;
    uint32 RetaliationTimer;
    uint32 ShatteringThrowTimer;
    uint32 DisarmTimer;

    void Init()
    {
        BladestormTimer = 20000 + rand()%10000;
        FearTimer = 10000 + rand()%10000;
        MortalStrikeTimer = 6000 + rand()%4000;
        ChargeTimer = 1000;
        SunderArmorTimer = 5000;
        RetaliationTimer = 30000 + rand()%8000;
        ShatteringThrowTimer =  10000 + rand()%30000;
        DisarmTimer = 5000 + rand()%20000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(BladestormTimer < diff)
        {
            DoCast(m_creature, SPELL_BLADESTORM);
            BladestormTimer = 90000 + rand()%5000;
        }
        else BladestormTimer -= diff;

        if(FearTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_INTIMIDATING_SHOUT);
            FearTimer = 40000 + rand()%40000;
        }
        else FearTimer -= diff;

        if(MortalStrikeTimer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_MORTAL_STRIKE);
            MortalStrikeTimer = 6000 + rand()%2000;
        }
        else MortalStrikeTimer -= diff;

        if(SunderArmorTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_SUNDER_ARMOR);
                        SunderArmorTimer = 2000+rand()%5000;
                }
                else SunderArmorTimer -= diff;

        if(ChargeTimer < diff)
        {
            if(m_creature->IsInRange(m_creature->getVictim(), 8.0f, 25.0f, false))
            {
                DoCast(m_creature->getVictim(), SPELL_CHARGE);
                ChargeTimer = 12000;
            }
            else
                ChargeTimer = 2000;
        }
        else ChargeTimer -= diff;

        if(RetaliationTimer < diff)
                {
                        DoCast(m_creature, SPELL_RETALIATION);
                        RetaliationTimer = 300000 + rand()%10000;
                }
                else RetaliationTimer -= diff;

                if(ShatteringThrowTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_SHATTERING_THROW);
                        ShatteringThrowTimer = 300000 + rand()%30000;
                }
                else ShatteringThrowTimer -= diff;

        if(DisarmTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_DISARM);
            DisarmTimer = 60000 + rand()%6000;
                }
                else DisarmTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_Chains_of_Ice 66020 //8sec
#define SPELL_Death_Coil 66019    //5sec
#define H_SPELL_Death_Coil 67930
#define SPELL_Death_Grip  66017 //35sec
#define SPELL_Frost_Strike 66047  //6sec
#define H_SPELL_Frost_Strike 67936 
#define SPELL_Icebound_Fortitude 66023 //1min
#define SPELL_Icy_Touch 66021  //8sec
#define H_SPELL_Icy_Touch 67939
#define SPELL_Strangulate 66018 //2min

struct MANGOS_DLL_DECL mob_toc_dkAI : public boss_faction_championsAI
{
    mob_toc_dkAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

        uint32 ChainsOfIceTimer;
        uint32 DeathCoilTimer;
        uint32 DeathGripTimer;
        uint32 FrostStrikeTimer;
        uint32 IceboundFortitudeTimer;
        uint32 IcyTouchTimer;
        uint32 StrangulateTimer;

    void Init()
    {
        ChainsOfIceTimer = 2000+rand()%3000;
            DeathCoilTimer = 3000+rand()%4000;
            DeathGripTimer = 1000+rand()%2000;
            FrostStrikeTimer = 4000+rand()%2000;
            IceboundFortitudeTimer = 20000;
            IcyTouchTimer = 6000+rand()%2000;
            StrangulateTimer = 6000+rand()%10000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(IceboundFortitudeTimer < diff)
        {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_Icebound_Fortitude);
                IceboundFortitudeTimer = 60000 + rand()%10000;
            }
            else
                IceboundFortitudeTimer = 5000;
        }
        else IceboundFortitudeTimer -= diff;

                if(ChainsOfIceTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_Chains_of_Ice);
            ChainsOfIceTimer = 8000 + rand()%6000;
                }
                else ChainsOfIceTimer -= diff;

                if(DeathCoilTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_Death_Coil);
            DeathCoilTimer = 6000 + rand()%4000;
                }
                else DeathCoilTimer -= diff;

                if(StrangulateTimer < diff)
                {
                        if(Unit *target = SelectEnemyCaster(false))
                                DoCast(target , SPELL_Strangulate);
                        StrangulateTimer = 60000 + rand()%6000;
                }
                else StrangulateTimer -= diff;

                if(FrostStrikeTimer < diff)
                {
                        DoCast (m_creature->getVictim(), SPELL_Frost_Strike);
            FrostStrikeTimer = 5000 + rand()%5000;
                }
                else FrostStrikeTimer -= diff;

                if(IcyTouchTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_Icy_Touch);
            IcyTouchTimer = 8000 + rand()%4000;
                }
                else IcyTouchTimer -= diff;

                if(DeathGripTimer < diff)
                {
            if(m_creature->IsInRange(m_creature->getVictim(), 10.0f, 30.0f, false))
                        {
                            DoCast(m_creature->getVictim(), SPELL_Death_Grip); 
                            DeathGripTimer = 35000 + rand()%1000;
                        }
                        else DeathGripTimer = 3000;
                }
                else DeathGripTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};
 
#define SPELL_FAN_OF_KNIVES        52874 //correst is 65955 //2sec
#define SPELL_BLIND                43433 //correst is 65960 //2min
#define SPELL_CLOAK                65961 //90sec
#define SPELL_Blade_Flurry         65956 //2min
#define SPELL_SHADOWSTEP           66178 //30sec
#define SPELL_HEMORRHAGE            65897 //correct is65954
#define SPELL_EVISCERATE           71933 //correct is 65957
#define H_SPELL_EVISCERATE         71933 //correct is 68095
 
struct MANGOS_DLL_DECL mob_toc_rogueAI : public boss_faction_championsAI
{
    mob_toc_rogueAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

        uint32 FanOfKnivesTimer;
        uint32 BlindTimer;
        uint32 CloakTimer;
        uint32 BladeFlurryTimer;
        uint32 ShadowstepTimer;
        uint32 HemorrhageTimer;
        uint32 EviscerateTimer;

    void Init()
    {
        FanOfKnivesTimer = 7000 + rand()%1000;
            BlindTimer = 6000 + rand()%3000;
            CloakTimer = 20000 + rand()%20000;
            BladeFlurryTimer = 10000 + rand()%1000;
            ShadowstepTimer = 9000 + rand()%2000;
            HemorrhageTimer = 5000 + rand()%1000;
            EviscerateTimer = 14000 + rand()%2000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(FanOfKnivesTimer < diff)
        {
            if(EnemiesInRange(15.0f) > 2)
                DoCast(m_creature->getVictim(), SPELL_FAN_OF_KNIVES);
            FanOfKnivesTimer = 3000;
        }
        else FanOfKnivesTimer -= diff;

                if(HemorrhageTimer < diff)
                {
            DoCast(m_creature->getVictim(), SPELL_HEMORRHAGE);
            HemorrhageTimer = 5000 + rand()%2000;
                }
                else HemorrhageTimer -= diff;

                if(EviscerateTimer < diff)
        {
            DoCast(m_creature->getVictim(),  SPELL_EVISCERATE);
            EviscerateTimer = 7000 + rand()%3000;
        }
        else EviscerateTimer -= diff;

        if(ShadowstepTimer < diff)
                {
                        if(m_creature->IsInRange(m_creature->getVictim(), 10.0f, 40.0f))
                        {
                            DoCast(m_creature->getVictim(), SPELL_SHADOWSTEP);
                ShadowstepTimer = 30000 + rand()%6000;
                        }
                        else ShadowstepTimer = 2000;
                }
                else ShadowstepTimer -= diff;

                if(BlindTimer < diff)
                {
                        if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,1))
                        {
                if(m_creature->IsInRange(target, 0.0f, 15.0f, false))
                {
                                    DoCast(target, SPELL_BLIND);
                                    BlindTimer = 120000 + rand()%6000;
                }
                            else BlindTimer = 2000;
            }
            else BlindTimer = 2000;
                }
                else BlindTimer -= diff;

                if(CloakTimer < diff) //If rogue really damaged...
        {
            if(m_creature->GetHealth()*2 < m_creature->GetMaxHealth())
            {
                DoCast(m_creature, SPELL_CLOAK);  //...She will cloak.
                CloakTimer = 90000;
            }
            else CloakTimer = 5000;
        }
        else CloakTimer -= diff;

                if(BladeFlurryTimer < diff)
        {
            DoCast(m_creature, SPELL_Blade_Flurry);
            BladeFlurryTimer = 120000 + rand()%5000;
        }
        else BladeFlurryTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_EARTH_SHOCK      65973
#define SPELL_LAVA_LASH        65974
#define SPELL_STORMSTRIKE      65970

struct MANGOS_DLL_DECL mob_toc_enh_shamanAI : public boss_faction_championsAI
{
    mob_toc_enh_shamanAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

        uint32 HeroismTimer;
        uint32 ShockTimer;
        uint32 LavaLashTimer;
        uint32 StormstrikeTimer;

    void Init()
    {
        ShockTimer=3000+rand()%2000;
            LavaLashTimer=4000+rand()%2000;
            StormstrikeTimer=5000+rand()%2000;
            HeroismTimer=5000+rand()%20000;
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

        if(HeroismTimer < diff)
        {
            DoCast(m_creature, SPELL_HEROISM);
            HeroismTimer = 600000;
        }
        else HeroismTimer -= diff;

                if(ShockTimer < diff)
                {
                        if(Unit *target = SelectUnit(SELECT_TARGET_RANDOM,0))
                                DoCast(target,  SPELL_EARTH_SHOCK);
                    ShockTimer = 6000 + rand()%1000;
                }
                else ShockTimer -= diff;

                if(StormstrikeTimer < diff)
                {
                        DoCast(m_creature->getVictim(), SPELL_STORMSTRIKE);
            StormstrikeTimer = 8000 + rand()%4000;
                }
                else StormstrikeTimer -= diff;

                if(LavaLashTimer < diff)
                {
                        DoCast (m_creature->getVictim(), SPELL_LAVA_LASH);
            LavaLashTimer = 8000 + rand()%5000;
                }
                else LavaLashTimer -= diff;

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_Avenging_Wrath 66011 //3min cd
#define SPELL_Crusader_Strike 66003 //6sec cd
#define SPELL_Divine_Shield 66010 //5min cd
#define SPELL_Divine_Storm 66006 //10sec cd
#define SPELL_Hammer_of_Justice 66007 //40sec cd
#define SPELL_Hand_of_Protection 66009 //5min cd
#define SPELL_Judgement_of_Command 66005 //8sec cd
#define SPELL_REPENTANCE 66008 //60sec cd
#define SPELL_Seal_of_Command 66004 //no cd

struct MANGOS_DLL_DECL mob_toc_retro_paladinAI : public boss_faction_championsAI
{
    mob_toc_retro_paladinAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

    bool ShieldUsed;

    void Init()
    {
        ShieldUsed = false;
    }

    void Aggro(Unit *who)
    {
        boss_faction_championsAI::Aggro(who);

        bsw->doCast(SPELL_Seal_of_Command);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

            bsw->timedCast(SPELL_REPENTANCE, diff);

            bsw->timedCast(SPELL_Crusader_Strike, diff);

            bsw->timedCast(SPELL_Avenging_Wrath, diff);

            if(!ShieldUsed && m_creature->GetHealthPercent() < 20.0f)
            {
                 bsw->doCast(SPELL_Divine_Shield);
                 ShieldUsed = true;
            }

            bsw->timedCast(SPELL_Divine_Storm, diff);

            bsw->timedCast(SPELL_Judgement_of_Command, diff);

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_WPET0 67518
#define SPELL_WPET1 67519

struct MANGOS_DLL_DECL mob_toc_pet_warlockAI : public boss_faction_championsAI
{
    mob_toc_pet_warlockAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

    void Aggro(Unit *who)
    {
        boss_faction_championsAI::Aggro(who);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

            bsw->timedCast(SPELL_WPET0, diff);

            bsw->timedCast(SPELL_WPET1, diff);

        boss_faction_championsAI::UpdateAI(diff);
    }
};

#define SPELL_HPET0 67793
struct MANGOS_DLL_DECL mob_toc_pet_hunterAI : public boss_faction_championsAI
{
    mob_toc_pet_hunterAI(Creature *pCreature) : boss_faction_championsAI(pCreature, AI_MELEE) {Init();}

    void Aggro(Unit *who)
    {
        boss_faction_championsAI::Aggro(who);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim()) return;

            bsw->timedCast(SPELL_HPET0, diff);

        boss_faction_championsAI::UpdateAI(diff);
    }
};


/*========================================================*/
CreatureAI* GetAI_mob_toc_druid(Creature *pCreature) {
    return new mob_toc_druidAI (pCreature);
}
CreatureAI* GetAI_mob_toc_shaman(Creature *pCreature) {
    return new mob_toc_shamanAI (pCreature);
}
CreatureAI* GetAI_mob_toc_paladin(Creature *pCreature) {
    return new mob_toc_paladinAI (pCreature);
}
CreatureAI* GetAI_mob_toc_priest(Creature *pCreature) {
    return new mob_toc_priestAI (pCreature);
}
CreatureAI* GetAI_mob_toc_shadow_priest(Creature *pCreature) {
    return new mob_toc_shadow_priestAI (pCreature);
}
CreatureAI* GetAI_mob_toc_warlock(Creature *pCreature) {
    return new mob_toc_warlockAI (pCreature);
}
CreatureAI* GetAI_mob_toc_mage(Creature *pCreature) {
    return new mob_toc_mageAI (pCreature);
}
CreatureAI* GetAI_mob_toc_hunter(Creature *pCreature) {
    return new mob_toc_hunterAI (pCreature);
}
CreatureAI* GetAI_mob_toc_boomkin(Creature *pCreature) {
    return new mob_toc_boomkinAI (pCreature);
}
CreatureAI* GetAI_mob_toc_warrior(Creature *pCreature) {
    return new mob_toc_warriorAI (pCreature);
}
CreatureAI* GetAI_mob_toc_dk(Creature *pCreature) {
    return new mob_toc_dkAI (pCreature);
}
CreatureAI* GetAI_mob_toc_rogue(Creature *pCreature) {
    return new mob_toc_rogueAI (pCreature);
}
CreatureAI* GetAI_mob_toc_enh_shaman(Creature *pCreature) {
    return new mob_toc_enh_shamanAI (pCreature);
}
CreatureAI* GetAI_mob_toc_retro_paladin(Creature *pCreature) {
    return new mob_toc_retro_paladinAI (pCreature);
}
CreatureAI* GetAI_mob_toc_pet_warlock(Creature *pCreature) {
    return new mob_toc_pet_warlockAI (pCreature);
}
CreatureAI* GetAI_mob_toc_pet_hunter(Creature *pCreature) {
    return new mob_toc_pet_hunterAI (pCreature);
}

void AddSC_boss_faction_champions()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_toc_druid";
    newscript->GetAI = &GetAI_mob_toc_druid;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_shaman";
    newscript->GetAI = &GetAI_mob_toc_shaman;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_paladin";
    newscript->GetAI = &GetAI_mob_toc_paladin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_priest";
    newscript->GetAI = &GetAI_mob_toc_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_shadow_priest";
    newscript->GetAI = &GetAI_mob_toc_shadow_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_mage";
    newscript->GetAI = &GetAI_mob_toc_mage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_warlock";
    newscript->GetAI = &GetAI_mob_toc_warlock;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_hunter";
    newscript->GetAI = &GetAI_mob_toc_hunter;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_boomkin";
    newscript->GetAI = &GetAI_mob_toc_boomkin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_warrior";
    newscript->GetAI = &GetAI_mob_toc_warrior;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_dk";
    newscript->GetAI = &GetAI_mob_toc_dk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_rogue";
    newscript->GetAI = &GetAI_mob_toc_rogue;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_enh_shaman";
    newscript->GetAI = &GetAI_mob_toc_enh_shaman;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_retro_paladin";
    newscript->GetAI = &GetAI_mob_toc_retro_paladin;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_pet_warlock";
    newscript->GetAI = &GetAI_mob_toc_pet_warlock;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toc_pet_hunter";
    newscript->GetAI = &GetAI_mob_toc_pet_hunter;
    newscript->RegisterSelf();
}
