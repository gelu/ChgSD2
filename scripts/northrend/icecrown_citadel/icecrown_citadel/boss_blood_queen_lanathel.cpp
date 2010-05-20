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
SDName: boss_blood_queen_lanathel
SD%Complete: 20%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
        SPELL_BERSERK                           = 47008,
        SPELL_SHROUD_OF_SORROW                  = 72981,
        SPELL_DELRIOUS_SLASH                    = 71623,
        SPELL_BLOOD_MIRROR_1                    = 70821,
        SPELL_BLOOD_MIRROR_2                    = 71510,
        SPELL_VAMPIRIC_BITE                     = 71726,
        SPELL_ESSENCE_OF_BLOOD_QWEEN            = 70867,
        SPELL_FRENZIED_BLOODTHIRST              = 70877,
        SPELL_UNCONTROLLABLE_FRENZY             = 70923,
        SPELL_PACT_OF_DARKFALLEN                = 71340,
        SPELL_PACT_OF_DARKFALLEN_1              = 71341,
        SPELL_SWARMING_SHADOWS                  = 71264,
        SPELL_TWILIGHT_BLOODBOLT                = 71446,
        SPELL_BLOODBOLT_WHIRL                   = 71772,
        SPELL_PRESENCE_OF_DARKFALLEN            = 71952,

        NPC_SWARMING_SHADOWS                    = 38163,
        SPELL_SWARMING_SHADOWS_VISUAL           = 71267,
};

struct MANGOS_DLL_DECL boss_blood_queen_lanathelAI : public ScriptedAI
{
    boss_blood_queen_lanathelAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    BossSpellWorker* bsw;
    uint8 stage;
    uint32 UpdateTimer;

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LANATHEL, NOT_STARTED);
        stage = 0;
        UpdateTimer = 1000;
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_LANATHEL, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631330,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631331,m_creature,pVictim);
               break;
        }
    }

    void Aggro(Unit *who) 
    {
        if(!pInstance) return; 
        pInstance->SetData(TYPE_LANATHEL, IN_PROGRESS);
        bsw->doCast(SPELL_SHROUD_OF_SORROW);
               DoScriptText(-1631321,m_creature,who);
    }

    void JustDied(Unit *killer)
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LANATHEL, DONE);
               DoScriptText(-1631333,m_creature,killer);
    }

    void UpdateAI(const uint32 diff)
    {
        if (pInstance->GetData(TYPE_EVENT_NPC) == NPC_LANATHEL)
        {
            UpdateTimer = pInstance->GetData(TYPE_EVENT_TIMER);
            if (UpdateTimer <= diff)
            {
            debug_log("EventMGR: creature %u received signal %u ",m_creature->GetEntry(),pInstance->GetData(TYPE_EVENT));
            switch (pInstance->GetData(TYPE_EVENT))
                {
                case 800:
                          DoScriptText(-1631301, m_creature);
                          UpdateTimer = 2000;
                          pInstance->SetData(TYPE_EVENT,810);
                          break;
                default:
                          break;
                }
             } else UpdateTimer -= diff;
             pInstance->SetData(TYPE_EVENT_TIMER, UpdateTimer);
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                        bsw->timedCast(SPELL_VAMPIRIC_BITE,diff);
                        if (bsw->timedQuery(SPELL_BLOODBOLT_WHIRL,diff)) stage = 1;
                    break;
            case 1: 
                        DoScriptText(-1631327,m_creature);
                        bsw->doCast(SPELL_BLOODBOLT_WHIRL);
                        stage = 0;
                    break;
            default:
                    break;
        }

        bsw->timedCast(SPELL_DELRIOUS_SLASH, diff);

        bsw->timedCast(SPELL_TWILIGHT_BLOODBOLT, diff);

        bsw->timedCast(SPELL_PACT_OF_DARKFALLEN, diff);

        bsw->timedCast(SPELL_SWARMING_SHADOWS, diff);

        if (bsw->timedQuery(SPELL_BERSERK, diff)){
                 bsw->doCast(SPELL_BERSERK);
                 DoScriptText(-1631332,m_creature);
                 };

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}

struct MANGOS_DLL_DECL mob_swarming_shadowsAI : public ScriptedAI
{
    mob_swarming_shadowsAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
        SetCombatMovement(false); 
//        m_creature->SetDisplayId(29308);
        m_creature->GetMotionMaster()->MoveRandom();
//        m_ui_Timer = 30000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LANATHEL) != IN_PROGRESS) 
              m_creature->ForcedDespawn();

        if (!m_creature->HasAura(SPELL_SWARMING_SHADOWS_VISUAL))
              DoCast(m_creature, SPELL_SWARMING_SHADOWS_VISUAL);

    }
};

CreatureAI* GetAI_mob_swarming_shadows(Creature* pCreature)
{
     return new mob_swarming_shadowsAI (pCreature);
}


void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_swarming_shadows";
    newscript->GetAI = &GetAI_mob_swarming_shadows;
    newscript->RegisterSelf();
}
