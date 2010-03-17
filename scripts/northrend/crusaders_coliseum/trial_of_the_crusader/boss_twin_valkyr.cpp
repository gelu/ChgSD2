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
SD%Complete: 0
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

// Twin pact && shields not implemented
// Shared health not fully correct worked
// timers need correct
// Portals not moving

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Equipment
{
    EQUIP_MAIN_1         = 49303,
    EQUIP_OFFHAND_1      = 47146,
    EQUIP_RANGED_1       = 47267,
    EQUIP_MAIN_2         = 45990,
    EQUIP_OFFHAND_2      = 47470,
    EQUIP_RANGED_2       = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_DARK_ESSENCE     = 34567,
    NPC_LIGHT_ESSENCE    = 34568,
};

enum BossSpells
{
    SPELL_TWIN_SPIKE_L,
    SPELL_LIGHT_SURGE,
    SPELL_SHIELD_LIGHT,
    SPELL_TWIN_PACT_L,
    SPELL_LIGHT_VORTEX,
    SPELL_LIGHT_TOUCH,
    SPELL_TWIN_SPIKE_H,
    SPELL_DARK_SURGE,
    SPELL_SHIELD_DARK,
    SPELL_TWIN_PACT_H,
    SPELL_DARK_VORTEX,
    SPELL_DARK_TOUCH,
    SPELL_TWIN_POWER,
    SPELL_LIGHT_ESSENCE,
    SPELL_DARK_ESSENCE,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_TWIN_SPIKE_L,     66075, 67312, 67313, 67314, 10000, 10000, 10000, 10000, 20000, 20000, 20000, 20000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_LIGHT_SURGE,      65766, 67270, 67271, 67272, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHIELD_LIGHT,     65858, 67259, 67260, 67261, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_PACT_L,      65876, 67306, 67307, 67308, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_VORTEX,     66046, 67206, 67207, 67208, 30000, 30000, 30000, 30000, 90000, 90000, 90000, 90000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_TOUCH,      67297, 67297, 67298, 67298, 10000, 10000, 10000, 10000, 15000, 15000, 15000, 15000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_SPIKE_H,     66069, 67309, 67310, 67311, 10000, 10000, 10000, 10000, 20000, 20000, 20000, 20000, 65535, CAST_ON_VICTIM, false, false},
{SPELL_DARK_SURGE,       65768, 67262, 67263, 67264, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHIELD_DARK,      65874, 67256, 67257, 67258, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_PACT_H,      65879, 67244, 67245, 67246, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_VORTEX,      66058, 67182, 67183, 67184, 30000, 30000, 30000, 30000, 90000, 90000, 90000, 90000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_TOUCH,       67282, 67282, 67283, 67283, 10000, 10000, 10000, 10000, 15000, 15000, 15000, 15000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_TWIN_POWER,       65916, 67248, 67249, 67250, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_ESSENCE,    65686, 65686, 65686, 65686, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_ESSENCE,     65684, 65684, 65684, 65684, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_BERSERK,          64238, 64238, 64238, 64238, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
};

/*######
## boss_fjola
######*/

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    uint8 stage;
    Unit* currentTarget;

#include "sc_boss_spell_worker.cpp"

    void Reset() {
        if(!m_pInstance) return;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_OFFHAND_1, EQUIP_RANGED_1);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_LIGHT_ESSENCE, SpawnLoc[24].x, SpawnLoc[24].y, SpawnLoc[24].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_LIGHT_ESSENCE, SpawnLoc[25].x, SpawnLoc[25].y, SpawnLoc[25].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
    }
    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713547,m_creature);
            m_pInstance->SetData(TYPE_VALKIRIES, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(-1713544,pVictim);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_VALKIRIES, IN_PROGRESS);
        DoScriptText(-1713541,m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;
            m_pInstance->SetData(DATA_DAMAGE_FJOLA, uiDamage);
            uiDamage += m_pInstance->GetData(DATA_DAMAGE_EYDIS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_LIGHT_SURGE, uiDiff))
                    CastBossSpell(SPELL_LIGHT_SURGE);
        if (QuerySpellPeriod(SPELL_LIGHT_TOUCH, uiDiff))
                    CastBossSpell(SPELL_LIGHT_TOUCH);
        if (QuerySpellPeriod(SPELL_LIGHT_VORTEX, uiDiff)) {
            if (m_pInstance->GetData(DATA_CASTING_EYDIS) != SPELL_DARK_VORTEX) {
                    DoScriptText(-1713538,m_creature);
                    CastBossSpell(SPELL_LIGHT_VORTEX);
                    m_pInstance->SetData(DATA_CASTING_FJOLA, SPELL_LIGHT_VORTEX);
                    } else m_pInstance->SetData(DATA_CASTING_EYDIS, 0);
                    }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fjola(Creature* pCreature)
{
    return new boss_fjolaAI(pCreature);
}

/*######
## boss_eydis
######*/

struct MANGOS_DLL_DECL boss_eydisAI : public ScriptedAI
{
    boss_eydisAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 Difficulty;
    uint32 m_uiSpell_Timer[BOSS_SPELL_COUNT];
    uint8 stage;
    Unit* currentTarget;

#include "sc_boss_spell_worker.cpp"

    void Reset() 
    {
        if(!m_pInstance) return;
        Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
        for (uint8 i = 0; i < BOSS_SPELL_COUNT; ++i)
              m_uiSpell_Timer[i] = urand(m_BossSpell[i].m_uiSpellTimerMin[Difficulty],m_BossSpell[i].m_uiSpellTimerMax[Difficulty]);
        SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_OFFHAND_2, EQUIP_RANGED_2);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_DARK_ESSENCE, SpawnLoc[22].x, SpawnLoc[22].y, SpawnLoc[22].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_DARK_ESSENCE, SpawnLoc[23].x, SpawnLoc[23].y, SpawnLoc[23].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
    }
    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713547,m_creature);
            m_pInstance->SetData(TYPE_VALKIRIES, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(-1713543,pVictim);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_VALKIRIES, IN_PROGRESS);
        DoScriptText(-1713741,m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;
            m_pInstance->SetData(DATA_DAMAGE_EYDIS, uiDamage);
            uiDamage += m_pInstance->GetData(DATA_DAMAGE_FJOLA);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (QuerySpellPeriod(SPELL_DARK_SURGE, uiDiff))
                    CastBossSpell(SPELL_DARK_SURGE);
        if (QuerySpellPeriod(SPELL_DARK_TOUCH, uiDiff))
                    CastBossSpell(SPELL_DARK_TOUCH);
        if (QuerySpellPeriod(SPELL_DARK_VORTEX, uiDiff)) {
            if (m_pInstance->GetData(DATA_CASTING_FJOLA) != SPELL_LIGHT_VORTEX) {
                    DoScriptText(-1713540,m_creature);
                    CastBossSpell(SPELL_DARK_VORTEX);
                    m_pInstance->SetData(DATA_CASTING_EYDIS, SPELL_DARK_VORTEX);
                    } else m_pInstance->SetData(DATA_CASTING_FJOLA, 0);
                    };

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

struct MANGOS_DLL_DECL mob_light_essenceAI : public ScriptedAI
{
    mob_light_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }
    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset() 
    {
    Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == NOT_STARTED) {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive())
                             pPlayer->RemoveAurasDueToSpell(m_BossSpell[SPELL_LIGHT_ESSENCE].m_uiSpellEntry[Difficulty]);
                    }

            m_creature->ForcedDespawn();
            }
        return;
    }
};

CreatureAI* GetAI_mob_light_essence(Creature* pCreature)
{
    return new mob_light_essenceAI(pCreature);
};

bool GossipHello_mob_light_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    uint8 Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        player->RemoveAurasDueToSpell(m_BossSpell[SPELL_DARK_ESSENCE].m_uiSpellEntry[Difficulty]);
        player->CastSpell(player,m_BossSpell[SPELL_LIGHT_ESSENCE].m_uiSpellEntry[Difficulty],false);
        player->CLOSE_GOSSIP_MENU();
    return true;
};

struct MANGOS_DLL_DECL mob_dark_essenceAI : public ScriptedAI
{
    mob_dark_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    Reset();
    }
    ScriptedInstance* m_pInstance;
    uint8 Difficulty;

    void Reset() 
    {
    Difficulty = m_pInstance->GetData(TYPE_DIFFICULTY);
    m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == NOT_STARTED) {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive())
                             pPlayer->RemoveAurasDueToSpell(m_BossSpell[SPELL_DARK_ESSENCE].m_uiSpellEntry[Difficulty]);
                    }

            m_creature->ForcedDespawn();
            }
        return;
    }
};

CreatureAI* GetAI_mob_dark_essence(Creature* pCreature)
{
    return new mob_dark_essenceAI(pCreature);
};

bool GossipHello_mob_dark_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    uint8 Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        player->RemoveAurasDueToSpell(m_BossSpell[SPELL_LIGHT_ESSENCE].m_uiSpellEntry[Difficulty]);
        player->CastSpell(player,m_BossSpell[SPELL_DARK_ESSENCE].m_uiSpellEntry[Difficulty],false);
        player->CLOSE_GOSSIP_MENU();
    return true;
}

void AddSC_twin_valkyr()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_fjola";
    newscript->GetAI = &GetAI_boss_fjola;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_eydis";
    newscript->GetAI = &GetAI_boss_eydis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_light_essence";
    newscript->GetAI = &GetAI_mob_light_essence;
    newscript->pGossipHello = &GossipHello_mob_light_essence;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_essence";
    newscript->GetAI = &GetAI_mob_dark_essence;
    newscript->pGossipHello = &GossipHello_mob_dark_essence;
    newscript->RegisterSelf();

}
