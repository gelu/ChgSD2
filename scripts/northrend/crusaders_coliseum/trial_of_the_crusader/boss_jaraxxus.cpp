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
SDName: trial_of_the_crusader
SD%Complete: 10%
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_crusader.h"
enum Default
{
    EQUIP_STAFF          = 40343,
};

enum Summons
{
    NPC_LEGION_FLAME     = 34784,
    NPC_INFERNAL_VOLCANO = 34813,
    NPC_FEL_INFERNAL     = 34815,
    NPC_NETHER_PORTAL    = 34825,
    NPC_MISTRESS         = 34826,
};

enum BossSpells
{
    SPELL_NETHER_POWER,
    SPELL_INFERNAL,
    SPELL_INFERNAL_ERUPTION,
    SPELL_FEL_FIREBALL,
    SPELL_FEL_LIGHTING,
    SPELL_INCINERATE_FLESH,
    SPELL_BURNING_INFERNO,
    SPELL_NETHER_PORTAL,
    SPELL_LEGION_FLAME,
    SPELL_SHIVAN_SLASH,
    SPELL_SPINNING_STRIKE,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_NETHER_POWER,     67108, 67108, 67108, 67108, 10000, 10000, 10000, 10000, 15000, 15000, 15000, 15000, 65535, CAST_ON_SELF, false, false},
{SPELL_INFERNAL,         66258, 66258, 66258, 66258, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, false},
{SPELL_INFERNAL_ERUPTION,66255, 66255, 66255, 66255, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_FEL_FIREBALL,     66532, 66963, 66964, 66965, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_FEL_LIGHTING,     66528, 66528, 67029, 67029, 15000, 15000, 15000, 15000, 25000, 25000, 25000, 25000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_INCINERATE_FLESH, 66237, 67049, 67050, 67051, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_BURNING_INFERNO,  66242, 67060, 67060, 67060, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_NETHER_PORTAL,    66264, 66264, 68405, 68405, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, true, true},
{SPELL_LEGION_FLAME,     68124, 68124, 68126, 68126, 30000, 30000, 30000, 30000, 45000, 45000, 45000, 45000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHIVAN_SLASH,     67098, 67098, 67098, 67098, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SPINNING_STRIKE,  66316, 66316, 66316, 66316, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_BERSERK,          26662, 26662, 26662, 26662, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
};




/*######
## boss_jaraxxus
######*/

struct MANGOS_DLL_DECL boss_jaraxxusAI : public ScriptedAI
{
    boss_jaraxxusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    uint8 stage;
    uint8 substage;
    uint8 m_portalsCount;

    void Reset() {
        if(!m_pInstance) return;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_pInstance->SetData(TYPE_JARAXXUS, NOT_STARTED);
        memset(&m_uiSpell_Timer, 0, sizeof(m_uiSpell_Timer));
        SetEquipmentSlots(false, EQUIP_STAFF, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
        m_portalsCount = 0;
    }

    bool QuerySpellPeriod(uint32 m_uiSpellIdx, uint32 diff)
    {
    if(!m_pInstance) return false;
    bool result;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        if (m_uiSpellIdx != pSpell->id) return false;

//        if (m_uiSpell_Timer[m_uiSpellIdx] == DAY) m_uiSpell_Timer[m_uiSpellIdx]=urand(0,pSpell->m_uiSpellTimerMax[Difficulty]);

        if (m_uiSpell_Timer[m_uiSpellIdx] < diff) {
            m_uiSpell_Timer[m_uiSpellIdx]=urand(pSpell->m_uiSpellTimerMin[Difficulty],pSpell->m_uiSpellTimerMax[Difficulty]);
            result = true;
            } else {
            m_uiSpell_Timer[m_uiSpellIdx] -= diff;
            result = false;
            };
        return result;
    }

    bool CastBossSpell(uint32 m_uiSpellIdx)
    {
    if(!m_pInstance) return false;
    Unit* pTarget;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        // Find spell index - temporary direct insert from spelltable
        if (m_uiSpellIdx != pSpell->id) return false;

        switch (pSpell->m_CastTarget) {
            case CAST_ON_SELF:
                   pTarget = m_creature;
                   break;
            case CAST_ON_SUMMONS:
                   pTarget = m_creature->getVictim(); //CHANGE IT!!!
                   break;
            case CAST_ON_VICTIM:
                   pTarget = m_creature->getVictim();
                   break;
            case CAST_ON_RANDOM:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
                   break;
            case CAST_ON_BOTTOMAGGRO:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
                   break;

            };
            if (pTarget) DoCastSpellIfCan(pTarget,pSpell->m_uiSpellEntry[Difficulty]);
    }

    uint64 CallGuard(uint64 npctype, TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(10, 50), fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        if(pSummon) pSummon->SetInCombatWithZone();
//        DoScriptText(EMOTE_SUMMON, m_creature);
        return pSummon ? pSummon->GetGUID() : 0;
    }


    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_JARAXXUS, DONE);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_JARAXXUS, IN_PROGRESS);
        DoScriptText(-1713514,m_creature);
        CastBossSpell(SPELL_NETHER_POWER);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_FEL_FIREBALL, uiDiff))
                    CastBossSpell(SPELL_FEL_FIREBALL);
        if (QuerySpellPeriod(SPELL_FEL_LIGHTING, uiDiff))
                    CastBossSpell(SPELL_FEL_LIGHTING);
        if (QuerySpellPeriod(SPELL_INCINERATE_FLESH, uiDiff))
                    CastBossSpell(SPELL_INCINERATE_FLESH);
        if (QuerySpellPeriod(SPELL_LEGION_FLAME, uiDiff))
                    CastBossSpell(SPELL_LEGION_FLAME);
        if (QuerySpellPeriod(SPELL_INFERNAL_ERUPTION, uiDiff))
                    CastBossSpell(SPELL_INFERNAL_ERUPTION);
        if (QuerySpellPeriod(SPELL_NETHER_PORTAL, uiDiff) && m_portalsCount < 2) {
                if (CallGuard(NPC_NETHER_PORTAL, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 5000) != 0)
                    ++m_portalsCount;
                };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_jaraxxus(Creature* pCreature)
{
    return new boss_jaraxxusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_legion_flameAI : public ScriptedAI
{
    mob_legion_flameAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) == DONE) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_legion_flame(Creature* pCreature)
{
    return new mob_legion_flameAI(pCreature);
}

struct MANGOS_DLL_DECL mob_infernal_volcanoAI : public ScriptedAI
{
    mob_infernal_volcanoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) == DONE) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_infernal_volcano(Creature* pCreature)
{
    return new mob_infernal_volcanoAI(pCreature);
}

struct MANGOS_DLL_DECL mob_fel_infernalAI : public ScriptedAI
{
    mob_fel_infernalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) == DONE) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_fel_infernal(Creature* pCreature)
{
    return new mob_fel_infernalAI(pCreature);
}

struct MANGOS_DLL_DECL mob_nether_portalAI : public ScriptedAI
{
    mob_nether_portalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_Timer;
    uint8  m_Count;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        m_Timer = 5000;
        m_Count = 2;
        if (Difficulty != RAID_DIFFICULTY_10MAN_HEROIC && Difficulty != RAID_DIFFICULTY_25MAN_HEROIC) 
        {
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_Count = 1;
        };
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void AttackStart(Unit *who)
    {
            return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) == DONE) 
            m_creature->ForcedDespawn();

        if (m_Timer < diff && m_Count > 0) {
            DoCastSpellIfCan(m_creature,m_BossSpell[SPELL_NETHER_PORTAL].m_uiSpellEntry[Difficulty]);
            m_Timer=urand(m_BossSpell[SPELL_NETHER_PORTAL].m_uiSpellTimerMin[Difficulty],m_BossSpell[SPELL_NETHER_PORTAL].m_uiSpellTimerMax[Difficulty]);
            --m_Count;
            } else m_Timer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

    }
};

CreatureAI* GetAI_mob_nether_portal(Creature* pCreature)
{
    return new mob_nether_portalAI(pCreature);
}

struct MANGOS_DLL_DECL mob_mistress_of_painAI : public ScriptedAI
{
    mob_mistress_of_painAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset()
    {
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER) return;
    }

    void JustDied(Unit* Killer)
    {
    }

    void Aggro(Unit *who)
    {
        if (!m_pInstance) return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance->GetData(TYPE_JARAXXUS) == DONE) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_mistress_of_pain(Creature* pCreature)
{
    return new mob_mistress_of_painAI(pCreature);
}

void AddSC_boss_jaraxxus()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_jaraxxus";
    newscript->GetAI = &GetAI_boss_jaraxxus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_legion_flame";
    newscript->GetAI = &GetAI_mob_legion_flame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_infernal_volcano";
    newscript->GetAI = &GetAI_mob_infernal_volcano;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fel_infernal";
    newscript->GetAI = &GetAI_mob_fel_infernal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nether_portal";
    newscript->GetAI = &GetAI_mob_nether_portal;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_mistress_of_pain";
    newscript->GetAI = &GetAI_mob_mistress_of_pain;
    newscript->RegisterSelf();
}
