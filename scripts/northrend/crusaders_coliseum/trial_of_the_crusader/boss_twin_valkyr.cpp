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

// Twin pact - heal part not worked now by undefined reason. Need override?
// timers need correct

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
    SPELL_TWIN_SPIKE_L     = 66075,
    SPELL_LIGHT_SURGE      = 65766,
    SPELL_SHIELD_LIGHT     = 65858,
    SPELL_TWIN_PACT_L      = 65875,
    SPELL_LIGHT_VORTEX     = 66046,
    SPELL_LIGHT_TOUCH      = 67297,
    SPELL_TWIN_SPIKE_H     = 66069,
    SPELL_DARK_SURGE       = 65768,
    SPELL_SHIELD_DARK      = 65874,
    SPELL_TWIN_PACT_H      = 65876,
    SPELL_DARK_VORTEX      = 66058,
    SPELL_DARK_TOUCH       = 67282,
    SPELL_TWIN_POWER       = 65916,
    SPELL_LIGHT_ESSENCE    = 65686,
    SPELL_DARK_ESSENCE     = 65684,
    SPELL_BERSERK          = 64238,
    SPELL_NONE             = 0,
};

/*######
## boss_fjola
######*/

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    bsw = new BossSpellWorker(this);
    Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    BossSpellWorker* bsw;
    bool TwinPactCasted;

    void Reset() {
        if(!m_pInstance) return;
        SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_OFFHAND_1, EQUIP_RANGED_1);
        m_creature->SetRespawnDelay(DAY);
        stage = 1;
        TwinPactCasted = false;
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713547,m_creature);
            if (Creature* pSister = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_DARKBANE)))
               if (!pSister->isAlive())
                         m_pInstance->SetData(TYPE_VALKIRIES, DONE);
                else m_pInstance->SetData(TYPE_VALKIRIES, SPECIAL);
        m_pInstance->SetData(DATA_HEALTH_FJOLA, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (!m_pInstance) return;
        DoScriptText(-1713544,pVictim);
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_VALKIRIES, IN_PROGRESS);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_LIGHT_ESSENCE, SpawnLoc[24].x, SpawnLoc[24].y, SpawnLoc[24].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_LIGHT_ESSENCE, SpawnLoc[25].x, SpawnLoc[25].y, SpawnLoc[25].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        DoScriptText(-1713541,m_creature);
        m_pInstance->SetData(DATA_HEALTH_FJOLA, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_pInstance) return;
        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

        if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if(pDoneBy->HasAura(SPELL_LIGHT_ESSENCE))
                uiDamage /= 2;
            else if(pDoneBy->HasAura(SPELL_DARK_ESSENCE))
                uiDamage += uiDamage/2;
        }

        m_pInstance->SetData(DATA_HEALTH_FJOLA, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > m_pInstance->GetData(DATA_HEALTH_EYDIS) &&
                                      m_pInstance->GetData(DATA_HEALTH_EYDIS) != 0)
                m_creature->SetHealth(m_pInstance->GetData(DATA_HEALTH_EYDIS));

    switch (stage)
        {
         case 0:
                break;
         case 1:
                if (bsw->timedQuery(SPELL_LIGHT_VORTEX, uiDiff)) {
                    m_pInstance->SetData(DATA_CASTING_FJOLA, SPELL_LIGHT_VORTEX);
                    DoScriptText(-1713538,m_creature);
                    bsw->doCast(SPELL_LIGHT_VORTEX);
                    stage = 0;
                    };
                 break;
          case 2:
                 if (bsw->timedQuery(SPELL_TWIN_PACT_L, uiDiff) 
                     && bsw->doCast(SPELL_SHIELD_LIGHT) == CAST_OK ) 
                     {
                            m_pInstance->SetData(DATA_CASTING_FJOLA, SPELL_TWIN_PACT_L);
                            DoScriptText(-1713539,m_creature);
                            bsw->doCast(SPELL_TWIN_PACT_L);
                            stage = 0;
                            TwinPactCasted = true;
                      }
                 break;
          default:
                 break;
         }

    if (m_pInstance->GetData(DATA_CASTING_EYDIS) == SPELL_DARK_VORTEX && stage==0 )
    {
     m_pInstance->SetData(DATA_CASTING_EYDIS, SPELL_NONE);
        stage = 1;
    }

    if ( m_creature->GetHealthPercent() <= 50.0f 
    && stage == 0
    && !TwinPactCasted)
    {
        stage = 2;
    }

    if (m_pInstance->GetData(DATA_CASTING_EYDIS) == SPELL_TWIN_PACT_H)
    {
     bsw->doCast(SPELL_TWIN_POWER);
     m_pInstance->SetData(DATA_CASTING_EYDIS, SPELL_NONE);
    }

        bsw->timedCast(SPELL_LIGHT_SURGE, uiDiff);

        bsw->timedCast(SPELL_LIGHT_TOUCH, uiDiff);

        bsw->timedCast(SPELL_BERSERK, uiDiff);

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
    bsw = new BossSpellWorker(this);
    Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;
    BossSpellWorker* bsw;
    bool TwinPactCasted;

    void Reset() 
    {
        if(!m_pInstance) return;
        SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_OFFHAND_2, EQUIP_RANGED_2);
        m_creature->SetRespawnDelay(DAY);
        stage = 0;
        TwinPactCasted = false;
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1713547,m_creature);
            if (Creature* pSister = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LIGHTBANE)))
               if (!pSister->isAlive())
                         m_pInstance->SetData(TYPE_VALKIRIES, DONE);
                else m_pInstance->SetData(TYPE_VALKIRIES, SPECIAL);
        m_pInstance->SetData(DATA_HEALTH_EYDIS, 0);
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
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_DARK_ESSENCE, SpawnLoc[22].x, SpawnLoc[22].y, SpawnLoc[22].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        if (m_creature->isAlive()) m_creature->SummonCreature(NPC_DARK_ESSENCE, SpawnLoc[23].x, SpawnLoc[23].y, SpawnLoc[23].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
        m_pInstance->SetData(DATA_HEALTH_EYDIS, m_creature->GetMaxHealth());
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_pInstance) return;
        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

        if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if(pDoneBy->HasAura(SPELL_DARK_ESSENCE))
                uiDamage /= 2;
            else if(pDoneBy->HasAura(SPELL_LIGHT_ESSENCE))
                uiDamage += uiDamage/2;
        }

        m_pInstance->SetData(DATA_HEALTH_EYDIS, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > m_pInstance->GetData(DATA_HEALTH_FJOLA) &&
                                      m_pInstance->GetData(DATA_HEALTH_FJOLA) != 0)
                m_creature->SetHealth(m_pInstance->GetData(DATA_HEALTH_FJOLA));

    switch (stage)
        {
         case 0:
                break;
         case 1:
                if (bsw->timedQuery(SPELL_DARK_VORTEX, uiDiff)) {
                    m_pInstance->SetData(DATA_CASTING_EYDIS, SPELL_DARK_VORTEX);
                    DoScriptText(-1713540,m_creature);
                    bsw->doCast(SPELL_DARK_VORTEX);
                    stage = 0;
                    };
                 break;
          case 2:
                 if (bsw->timedQuery(SPELL_TWIN_PACT_H, uiDiff) 
                     && bsw->doCast(SPELL_SHIELD_DARK) == CAST_OK ) 
                     {
                            m_pInstance->SetData(DATA_CASTING_EYDIS, SPELL_TWIN_PACT_H);
                            DoScriptText(-1713539,m_creature);
                            bsw->doCast(SPELL_TWIN_PACT_H);
                            stage = 0;
                            TwinPactCasted = true;
                      }
                 break;
          default:
                 break;
         }

    if (m_pInstance->GetData(DATA_CASTING_FJOLA) == SPELL_LIGHT_VORTEX && stage==0 )
    {
     m_pInstance->SetData(DATA_CASTING_FJOLA, SPELL_NONE);
        stage = 1;
    }

    if ( m_creature->GetHealthPercent() <= 50.0f 
    && m_pInstance->GetData(DATA_CASTING_FJOLA) == SPELL_TWIN_PACT_L
    && !TwinPactCasted
    && stage == 0)
    {
     bsw->doCast(SPELL_TWIN_POWER);
     m_pInstance->SetData(DATA_CASTING_FJOLA, SPELL_NONE);
     stage = 2;
    }

        bsw->timedCast(SPELL_DARK_SURGE, uiDiff);

        bsw->timedCast(SPELL_DARK_TOUCH, uiDiff);

        bsw->timedCast(SPELL_BERSERK, uiDiff);

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

    void Reset() 
    {
    m_creature->SetRespawnDelay(DAY);
    m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    m_creature->GetMotionMaster()->MoveRandom();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) != IN_PROGRESS) {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive())
                             pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
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
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        player->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
        player->CastSpell(player,SPELL_LIGHT_ESSENCE,false);
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

    void Reset() 
    {
    m_creature->SetRespawnDelay(DAY);
    m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    m_creature->GetMotionMaster()->MoveRandom();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) != IN_PROGRESS) {
                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive())
                             pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
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
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        player->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
        player->CastSpell(player,SPELL_DARK_ESSENCE,false);
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
