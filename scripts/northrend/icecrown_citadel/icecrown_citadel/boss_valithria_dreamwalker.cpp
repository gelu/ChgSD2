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
    {4203.470215, 2484.500000, 364.872009},  // 0 Valithria
    {4240.688477, 2405.794678, 364.868591},  // 1 Valithria Room 1
    {4165.112305, 2405.872559, 364.872925},  // 2 Valithria Room 2
    {4166.216797, 2564.197266, 364.873047},  // 3 Valithria Room 3
    {4239.579102, 2566.753418, 364.868439},  // 4 Valithria Room 4
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
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    bool battlestarted;
    bool intro;

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
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void KilledUnit(Unit* pVictim)
    {
/*    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };*/
    }

    void JustSummoned(Creature* summoned)
    {
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
        DoScriptText(-1631000,m_creature);
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 diff)
    {
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
