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
SD%Complete: 1%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum BossSpells
{
        SPELL_SHROUD_OF_SORROW                  = 72981,
        SPELL_DELRIOUS_SLASH                    = 71623,
        SPELL_BLOOD_MIRROR_1                    = 70821,
        SPELL_BLOOD_MIRROR_2                    = 71510,
        SPELL_VAMPIRIC_BITE                     = 71726,
        SPELL_ESSENCE_OF_BLOOD_QWEEN            = 70867,
        SPELL_FRENZIED_BLOODTHIRST              = 70877,
        SPELL_UNCONTROLLABLE_FRENZY             = 70923,
        SPELL_PACT_OF_DARKFALLEN                = 71340,
        SPELL_SWARMING_SHADOWS                  = 71265,
        SPELL_TWILIGHT_BLOODBOLT                = 71446,
        SPELL_BLOODBOLT_WHIRL                   = 71772,
        SPELL_PRESENCE_OF_DARKFALLEN            = 71952,

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

    void Reset()
    {
        if(!pInstance) return;
        pInstance->SetData(TYPE_LANATHEL, NOT_STARTED);
        stage = 0;
    }

    void JustReachedHome()
    {
        if (pInstance) pInstance->SetData(TYPE_LANATHEL, FAIL);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_LANATHEL, IN_PROGRESS);
        bsw->doCast(SPELL_SHROUD_OF_SORROW);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_LANATHEL, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: 
                        bsw->timedCast(SPELL_VAMPIRIC_BITE,diff);
                        if (bsw->timedQuery(SPELL_BLOODBOLT_WHIRL,diff)) stage = 1;
                    break;
            case 1: 
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

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_blood_queen_lanathel(Creature* pCreature)
{
    return new boss_blood_queen_lanathelAI(pCreature);
}


void AddSC_boss_blood_queen_lanathel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_blood_queen_lanathel";
    newscript->GetAI = &GetAI_boss_blood_queen_lanathel;
    newscript->RegisterSelf();
}
