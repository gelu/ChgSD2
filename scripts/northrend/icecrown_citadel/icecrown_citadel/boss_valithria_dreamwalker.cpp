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
SDName: boss_valithria
SD%Complete: 5%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

static Locations SpawnLoc[]=
{
    {4203.470215f, 2484.500000f, 364.872009f},  // 0 Valithria
    {4240.688477f, 2405.794678f, 364.868591f},  // 1 Valithria Room 1
    {4165.112305f, 2405.872559f, 364.872925f},  // 2 Valithria Room 2
    {4166.216797f, 2564.197266f, 364.873047f},  // 3 Valithria Room 3
    {4239.579102f, 2566.753418f, 364.868439f},  // 4 Valithria Room 4
    {4228.589844f, 2469.110107f, 364.868988f},  // 5 Mob 1
    {4236.000000f, 2479.500000f, 364.869995f},  // 6 Mob 2
    {4235.410156f, 2489.300049f, 364.872009f},  // 7 Mob 3
    {4228.509766f, 2500.310059f, 364.876007f},  // 8 Mob 4
};

enum BossSpells
{
//    SPELL_NIGHTMARE_PORTAL       = 72481, // Not worked yet. Use 71977 (visual effect) instead?
    SPELL_NIGHTMARE_PORTAL       = 71977,
    SPELL_EMERALD_VIGOR          = 70873,
    SPELL_DREAMWALKER_RAGE       = 71189,
    SPELL_IMMUNITY               = 72724,
    SPELL_CORRUPTION             = 70904,
    SPELL_DREAM_SLIP             = 71196,
    SPELL_ICE_SPIKE              = 70702,

// Summons
    NPC_RISEN_ARCHMAGE           = 37868,
    NPC_SUPPRESSOR               = 37863,
    NPC_BLASING_SKELETON         = 36791,
    NPC_BLISTERING_ZOMBIE        = 37934,
    NPC_GLUTTONOUS_ABOMINATION   = 37886,
    NPC_NIGHTMARE_PORTAL         = 38429, // Not realized yet
};

struct MANGOS_DLL_DECL boss_valithria_dreamwalkerAI : public ScriptedAI
{
    boss_valithria_dreamwalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        pMap = m_creature->GetMap();
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool battlestarted;
    bool intro;
    uint8 currentDoor;
    int8 portalscount;
    Map* pMap;


    void Reset()
    {
        if(!pInstance) return;
        m_creature->SetHealth(m_creature->GetMaxHealth()/2);
        pInstance->SetData(TYPE_VALITHRIA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
        bsw->doCast(SPELL_CORRUPTION);
        bsw->doCast(SPELL_IMMUNITY);
        stage = 0;
        portalscount = 0;
        battlestarted = false;
        intro = false;
        currentDoor = 0;
        if (Unit* pTemp = Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_VALITHRIA_QUEST)))
                if (pTemp->GetVisibility() == VISIBILITY_ON)
                            pTemp->SetVisibility(VISIBILITY_OFF);
    }

    uint64 GetDoor(uint8 doornum)
    {
        switch (doornum) {
            case 1:
               return pInstance->GetData64(GO_VALITHRIA_DOOR_1);
               break;
            case 2:
               return pInstance->GetData64(GO_VALITHRIA_DOOR_2);
               break;
            case 3:
               return pInstance->GetData64(GO_VALITHRIA_DOOR_3);
               break;
            case 4:
               return pInstance->GetData64(GO_VALITHRIA_DOOR_4);
               break;
            default:
               return 0;
               break;
        };
    }

    void OpenDoor(uint64 guid)
    {
        if(!guid) return;

        if (Map* pMap = m_creature->GetMap())
           if (GameObject* pGo = pMap->GetGameObject(guid))
              pGo->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
    }

    void CloseDoor(uint64 guid)
    {
        if(!guid) return;

        if (Map* pMap = m_creature->GetMap())
           if (GameObject* pGo = pMap->GetGameObject(guid))
              pGo->SetGoState(GO_STATE_READY);
    }

    void QueryEvadeMode()
    {
       Map::PlayerList const &players = pMap->GetPlayers();
       for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
              {
              if(Player* pPlayer = i->getSource())
                    if(pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 100.0f)) return;
              }

              pInstance->SetData(TYPE_VALITHRIA, FAIL);
              DoScriptText(-1631409,m_creature);
              m_creature->DeleteThreatList();
              m_creature->CombatStop(true);
              m_creature->LoadCreaturesAddon();
              if (m_creature->isAlive())
                  m_creature->GetMotionMaster()->MoveTargetedHome();
              m_creature->SetLootRecipient(NULL);
              Reset();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!pInstance || ( intro && battlestarted)) return;

        if (!intro)  {
                     DoScriptText(-1631401,m_creature,pWho);
                     intro = true;
                     }
        if (!battlestarted && pWho->isAlive() && pWho->IsWithinDistInMap(m_creature, 20.0f))  {
                     DoScriptText(-1631401,m_creature,pWho);
                     battlestarted = true;
                     pInstance->SetData(TYPE_VALITHRIA, IN_PROGRESS);
                     }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(!pInstance) return;

        switch (urand(0,1)) {
            case 0:
               DoScriptText(-1631403,m_creature,pVictim);
               break;
            case 1:
               DoScriptText(-1631404,m_creature,pVictim);
               break;
        };
    }

    void JustSummoned(Creature* summoned)
    {
        if(!pInstance || !summoned || !battlestarted) return;

        if ( summoned->GetEntry() == NPC_NIGHTMARE_PORTAL ) return;

        summoned->AddThreat(m_creature, 100.0f);

        m_creature->GetMotionMaster()->MoveChase(m_creature);

    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return
        pInstance->SetData(TYPE_VALITHRIA, FAIL);
        DoScriptText(-1631409,m_creature);
        m_creature->Respawn();
        Reset();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!battlestarted) return;

        QueryEvadeMode();

        switch(stage)
        {
            case 0: 
                    if ( m_creature->GetHealthPercent() > 90.0f ) stage = 2;
                    if ( m_creature->GetHealthPercent() < 10.0f ) stage = 3;
                    break;
            case 1: 
                    if ( m_creature->GetHealthPercent() < 90.0f && m_creature->GetHealthPercent() > 10.0f ) stage = 0;
                    if ( m_creature->GetHealthPercent() > 99.9f ) stage = 5;
                    if ( m_creature->GetHealthPercent() < 1.0f  ) stage = 4;
                    break;
            case 2: 
                    DoScriptText(-1631407,m_creature);
                    stage = 1;
                    break;
            case 3: 
                    DoScriptText(-1631406,m_creature);
                    stage = 1;
                    break;
            case 4: 
                    DoScriptText(-1631409,m_creature);
                    stage = 1;
                    pInstance->SetData(TYPE_VALITHRIA, FAIL);
                    DoScriptText(-1631409,m_creature);
                    m_creature->DeleteThreatList();
                    m_creature->CombatStop(true);
                    m_creature->LoadCreaturesAddon();

                    if (m_creature->isAlive())
                       m_creature->GetMotionMaster()->MoveTargetedHome();
                    m_creature->SetLootRecipient(NULL);
                    Reset();
                    break;
            case 5: 
                    DoScriptText(-1631408,m_creature);
                    if (m_creature->HasAura(SPELL_CORRUPTION)) bsw->doRemove(SPELL_CORRUPTION);
                    stage = 6;
                    return;
                    break;
            case 6: 
                    if (bsw->timedQuery(SPELL_CORRUPTION, diff)) stage = 7;
                    return;
                    break;
            case 7: 
                    bsw->doCast(SPELL_DREAMWALKER_RAGE);
                    stage = 8;
                    return;
                    break;
            case 8: 
                    if (bsw->timedQuery(SPELL_CORRUPTION, diff)) stage = 9;
                    return;
                    break;
            case 9: 
                    if (Unit* pTemp = Unit::GetUnit((*m_creature),pInstance->GetData64(NPC_VALITHRIA_QUEST)))
                    {
                        if (pTemp->HasAura(SPELL_CORRUPTION))
                             pTemp->RemoveAurasDueToSpell(SPELL_CORRUPTION);
                        if (pTemp->GetVisibility() == VISIBILITY_OFF)
                            pTemp->SetVisibility(VISIBILITY_ON);
                    }
                    pInstance->SetData(TYPE_VALITHRIA, DONE);
                    stage = 10;
                    m_creature->ForcedDespawn();
                    break;
            default:
                    break;
        } 


        if (bsw->timedQuery(NPC_RISEN_ARCHMAGE, diff)) {
                                                        CloseDoor(GetDoor(currentDoor));
                                                        currentDoor = urand(1,4);
                                                        OpenDoor(GetDoor(currentDoor));
                                                        DoScriptText(-1631402,m_creature);
                                                        };

        if (bsw->timedQuery(SPELL_NIGHTMARE_PORTAL, diff) || portalscount > 0) 
                                                        {
                                                        if (!portalscount) {
                                                                           portalscount = 3;
                                                                           DoScriptText(-1631405,m_creature);
                                                                           };
                                                        bsw->doCast(SPELL_NIGHTMARE_PORTAL);
                                                        --portalscount;
                                                        };

        bsw->timedCast(SPELL_ICE_SPIKE, diff);

        return;
    }
};

CreatureAI* GetAI_boss_valithria_dreamwalker(Creature* pCreature)
{
    return new boss_valithria_dreamwalkerAI(pCreature);
};

struct MANGOS_DLL_DECL mob_nightmare_portalAI : public ScriptedAI
{
    mob_nightmare_portalAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool portalcasted;
    BossSpellWorker* bsw;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false); 
        m_creature->GetMotionMaster()->MoveRandom();
        m_creature->SetDisplayId(29352);
        portalcasted = false;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!m_pInstance || portalcasted) return;

        if (pWho->isAlive() && pWho->GetTypeId() == TYPEID_PLAYER && pWho->IsWithinDistInMap(m_creature, 2.0f))
                 {
                 bsw->doCast(SPELL_EMERALD_VIGOR, pWho);
                 bsw->doAura(SPELL_EMERALD_VIGOR, pWho,EFFECT_INDEX_0);
                 bsw->doAura(SPELL_EMERALD_VIGOR, pWho,EFFECT_INDEX_1);
                 bsw->doAura(SPELL_EMERALD_VIGOR, pWho,EFFECT_INDEX_2);
                 portalcasted = true;
                 }
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_VALITHRIA) != IN_PROGRESS || portalcasted)
              m_creature->ForcedDespawn();
 
    }

};

CreatureAI* GetAI_mob_nightmare_portal(Creature *pCreature)
{
    return new mob_nightmare_portalAI(pCreature);
};


void AddSC_boss_valithria_dreamwalker()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_valithria_dreamwalker";
    newscript->GetAI = &GetAI_boss_valithria_dreamwalker;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_nightmare_portal";
    newscript->GetAI = &GetAI_mob_nightmare_portal;
    newscript->RegisterSelf();

}
