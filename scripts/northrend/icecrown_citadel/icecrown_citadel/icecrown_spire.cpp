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
SDName: icecrown_spire
SD%Complete: 10%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel - mobs
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"
enum
{
        SPELL_BERSERK                           = 47008,
        SPELL_FROST_BREATH                      = 70116,
        SPELL_BLIZZARD                          = 70362,
        SPELL_SOUL_FEAST                        = 71203,
        SPELL_CLEAVE                            = 70361,

        SPELL_STOMP                             = 64652,
        SPELL_DEATH_PLAGUE                      = 72865,
};

struct MANGOS_DLL_DECL mob_spire_frostwyrmAI : public ScriptedAI
{
    mob_spire_frostwyrmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    BossSpellWorker* bsw;

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        stage = 0;
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_SKULLS_PLATO, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_SKULLS_PLATO, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    bsw->timedCast(SPELL_SOUL_FEAST, diff);
                    break;}
            case 1: {
                    bsw->doCast(SPELL_BERSERK);
                    stage = 2;
                    break;}
            case 2: {
                    break;}
            }

                    bsw->timedCast(SPELL_CLEAVE, diff);
                    bsw->timedCast(SPELL_BLIZZARD, diff);
                    bsw->timedCast(SPELL_FROST_BREATH, diff);

        if (m_creature->GetHealthPercent() < 10.0f && stage == 0) stage = 1;

        bsw->timedCast(SPELL_BERSERK, diff);

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_spire_frostwyrm(Creature* pCreature)
{
    return new mob_spire_frostwyrmAI(pCreature);
}

struct MANGOS_DLL_DECL mob_frost_giantAI : public ScriptedAI
{
    mob_frost_giantAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        bsw = new BossSpellWorker(this);
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    BossSpellWorker* bsw;

    void Aggro(Unit *who)
    {
        if(pInstance) pInstance->SetData(TYPE_FLIGHT_WAR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_FLIGHT_WAR, DONE);
    }

    void Reset()
    {
        m_creature->SetRespawnDelay(DAY);
        stage = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch(stage)
        {
            case 0: {
                    bsw->timedCast(SPELL_SOUL_FEAST, diff);
                    break;}
            case 1: {
                    bsw->doCast(SPELL_BERSERK);
                    stage = 2;
                    break;}
            case 2: {
                    break;}
            }
                    bsw->timedCast(SPELL_STOMP, diff);
                    bsw->timedCast(SPELL_DEATH_PLAGUE, diff);

        if (m_creature->GetHealthPercent() < 2.0f && stage == 0) stage = 1;

        bsw->timedCast(SPELL_BERSERK, diff);

        DoMeleeAttackIfReady();

    }
};

CreatureAI* GetAI_mob_frost_giant(Creature* pCreature)
{
    return new mob_frost_giantAI(pCreature);
}

void AddSC_icecrown_spire()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_spire_frostwyrm";
    newscript->GetAI = &GetAI_mob_spire_frostwyrm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frost_giant";
    newscript->GetAI = &GetAI_mob_frost_giant;
    newscript->RegisterSelf();
}
