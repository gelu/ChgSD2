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
SD%Complete: 1%
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
    SPELL_NIGHTMARE_POTAL        = 72482,
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
};

struct MANGOS_DLL_DECL boss_valithria_dreamwalkerAI : public ScriptedAI
{
    boss_valithria_dreamwalkerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
        pGuard1 = NULL;
        pGuard2 = NULL;
        pGuard3 = NULL;
        pGuard4 = NULL;
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool battlestarted;
    bool intro;
    uint8 currentDoor;
    Creature* pGuard1;
    Creature* pGuard2;
    Creature* pGuard3;
    Creature* pGuard4;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_VALITHRIA, NOT_STARTED);
        bsw->resetTimers();
        m_creature->SetRespawnDelay(7*DAY);
        bsw->doCast(SPELL_CORRUPTION);
        bsw->doCast(SPELL_IMMUNITY);
        stage = 0;
        battlestarted = false;
        intro = false;
        currentDoor = 0;
        if (!pGuard1) pGuard1 = (Creature*)bsw->doSummon(NPC_RISEN_ARCHMAGE, SpawnLoc[5].x, SpawnLoc[5].y, SpawnLoc[5].z);
            if (pGuard1 && !pGuard1->isAlive()) pGuard1->Respawn();
        if (!pGuard2) pGuard2 = (Creature*)bsw->doSummon(NPC_RISEN_ARCHMAGE, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z);
            if (pGuard2 && !pGuard2->isAlive()) pGuard2->Respawn();
        if (!pGuard3) pGuard3 = (Creature*)bsw->doSummon(NPC_RISEN_ARCHMAGE, SpawnLoc[7].x, SpawnLoc[7].y, SpawnLoc[7].z);
            if (pGuard3 && !pGuard3->isAlive()) pGuard3->Respawn();
        if (!pGuard4) pGuard4 = (Creature*)bsw->doSummon(NPC_RISEN_ARCHMAGE, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z);
            if (pGuard4 && !pGuard4->isAlive()) pGuard4->Respawn();
    }

    uint8 GetDoor(uint8 doornum)
    {
        switch (doornum) {
            case 1:
               return pInstance->GetData(GO_VALITHRIA_DOOR_1);
               break;
            case 2:
               return pInstance->GetData(GO_VALITHRIA_DOOR_2);
               break;
            case 3:
               return pInstance->GetData(GO_VALITHRIA_DOOR_3);
               break;
            case 4:
               return pInstance->GetData(GO_VALITHRIA_DOOR_4);
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

    void EnterEvadeMode()
    {
       Map* pMap = m_creature->GetMap();
       Map::PlayerList const &players = pMap->GetPlayers();
       for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
              {
              if(Player* pPlayer = i->getSource())
                    if(pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 100.0f)) return;
              }
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if(!pInstance || intro) return;

        DoScriptText(-1631401,m_creature,pWho);
        intro = true;
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
        summoned->AddThreat(m_creature, 100.0f);
        m_creature->GetMotionMaster()->MoveChase(m_creature);
    }

    void PlayersWin()
    {
        if(!pInstance) return
        DoScriptText(-1631000,m_creature);
        bsw->doCast(SPELL_DREAMWALKER_RAGE);
        if (Unit* pTemp = bsw->doSummon(NPC_VALITHRIA_QUEST, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z,TEMPSUMMON_MANUAL_DESPAWN))
           if (pTemp->HasAura(SPELL_CORRUPTION))
               pTemp->RemoveAurasDueToSpell(SPELL_CORRUPTION);
        pInstance->SetData(TYPE_VALITHRIA, DONE);
        m_creature->ForcedDespawn();
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return
        pInstance->SetData(TYPE_VALITHRIA, FAIL);
        DoScriptText(-1631409,m_creature);
            if (pGuard1 && pGuard1->isAlive()) pGuard1->ForcedDespawn();
            if (pGuard2 && pGuard2->isAlive()) pGuard2->ForcedDespawn();
            if (pGuard3 && pGuard3->isAlive()) pGuard3->ForcedDespawn();
            if (pGuard4 && pGuard4->isAlive()) pGuard4->ForcedDespawn();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!battlestarted) {
                                if (pGuard1 && !pGuard1->isAlive() &&
                                    pGuard2 && !pGuard2->isAlive() &&
                                    pGuard3 && !pGuard3->isAlive() &&
                                    pGuard4 && !pGuard4->isAlive()) {
                                                         battlestarted = true;
                                                         DoScriptText(-1631401,m_creature);
                                                         }
                            return;
                            }

        switch(stage)
        {
            case 0: 
                    break;
            case 1: 
                    break;
            default:
                    break;
        } 

        if ( stage ==0 && m_creature->GetHealthPercent() > 90.0f ) stage = 1;
        if ( stage ==2 && m_creature->GetHealthPercent() < 15.0f ) stage = 3;
        if ( m_creature->GetHealthPercent() > 99.9f ) stage = 4;

        if (bsw->timedQuery(NPC_RISEN_ARCHMAGE, diff)) {
                        CloseDoor(GetDoor(currentDoor));
                        currentDoor = urand(1,4);
                        OpenDoor(GetDoor(currentDoor));
                        DoScriptText(-1631402,m_creature);
                        };

        bsw->timedCast(SPELL_ICE_SPIKE, diff);

        return;
    }
};


CreatureAI* GetAI_boss_valithria_dreamwalker(Creature* pCreature)
{
    return new boss_valithria_dreamwalkerAI(pCreature);
}

void AddSC_boss_valithria_dreamwalker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_valithria_dreamwalker";
    newscript->GetAI = &GetAI_boss_valithria_dreamwalker;
    newscript->RegisterSelf();
}
