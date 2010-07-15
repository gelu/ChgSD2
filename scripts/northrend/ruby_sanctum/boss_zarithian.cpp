/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_zarithian
SD%Complete: 50%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

//TODO: Add spawn Locs, sql spells, sql npcs, TEST

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum
{
    SPELL_TWILIGHT_PRECISION         = 78243,
    SPELL_SUNDER_ARMOR               = 74367,
    SPELL_IMTIMIDATING_ROAR          = 74384,
    SPELL_LAVA_GOUT                  = 74395,
    SPELL_BLAST_NOVA                 = 74393,

    NPC_FLAMECALLER                  = 39814,
};

static float add[3][4]=
{
        {0.0f,0.0f,0.0f,0.0f}, //x y z o
        {0.0f,0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f,0.0f}, // HALION
};

struct MANGOS_DLL_DECL boss_zarithianAI : public BSWScriptedAI
{
    boss_zarithianAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance *pInstance;
    uint8 stage;
    uint32 uiTimer;

    void Reset()
    {
        if(!pInstance)
            return;

        pInstance->SetData(TYPE_ZARITHIAN, NOT_STARTED);
        resetTimers();
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;
    }

    void KilledUnit(Unit* pVictim)
    {
    switch (urand(0,1)) {
        case 0:
               DoScriptText(-1666201,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1666202,m_creature,pVictim);
               break;
        };
    }

    void JustReachedHome()
    {
        if (pInstance)
            pInstance->SetData(TYPE_ZARITHIAN, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
    }

    void Aggro(Unit *who) 
    {
        if(pInstance)
            pInstance->SetData(TYPE_ZARITHIAN, IN_PROGRESS);

        DoScriptText(-1666200,m_creature);
        uiTimer = 45000;
    }

    void JustDied(Unit *killer)
    {
        if(pInstance)
            pInstance->SetData(TYPE_ZARITHIAN, DONE);

        DoScriptText(-1666203,m_creature);
//        m_creature->SummonCreature(NPC_HALION_P, add[2][0], add[2][1], add[2][2], add[2][3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, HOUR*2*IN_MILLISECONDS);
// Don't summon Halion there! Xerestrasza can made this.
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //TODO every 45 secs summon adds

        if (uiTimer < diff)
        {
            DoScriptText(-1666204,m_creature);
            m_creature->SummonCreature(NPC_FLAMECALLER, add[0][0], add[0][1], add[0][2], add[0][3], TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 20000);
            m_creature->SummonCreature(NPC_FLAMECALLER, add[1][0], add[1][1], add[1][2], add[1][3], TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 20000);
            uiTimer = 45000;
        }
        timedCast(SPELL_TWILIGHT_PRECISION, diff);
        timedCast(SPELL_SUNDER_ARMOR, diff);
        timedCast(SPELL_IMTIMIDATING_ROAR, diff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zarithian(Creature* pCreature)
{
    return new boss_zarithianAI(pCreature);
}

struct MANGOS_DLL_DECL mob_flamecaller_rubyAI : public BSWScriptedAI
{
    mob_flamecaller_rubyAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *pInstance;

    void Reset()
    {
    }

    void AttackStart(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        timedCast(SPELL_LAVA_GOUT, diff);
        timedCast(SPELL_BLAST_NOVA, diff);
    }
};

CreatureAI* GetAI_mob_flamecaller_ruby(Creature* pCreature)
{
    return new mob_flamecaller_rubyAI(pCreature);
}

void AddSC_boss_zarithian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_zarithian";
    newscript->GetAI = &GetAI_boss_zarithian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flamecaller_ruby";
    newscript->GetAI = &GetAI_mob_flamecaller_ruby;
    newscript->RegisterSelf();
}
