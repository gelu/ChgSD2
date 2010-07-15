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
SDName: ruby_sanctum mobs
SD%Complete: 10%
SDComment: by notagain, corrected by /dev/rsa
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "def_ruby_sanctum.h"

struct MANGOS_DLL_DECL mob_xerestraszaAI : public ScriptedAI
{
    mob_xerestraszaAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* pInstance;

    void Reset()
    {
        if(!pInstance)
            return;
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void JustRespawned()
    {
        Reset();
    }

    void MoveInLineOfSight(Unit *who)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //TODO
        //YELL on area trigger
        //MOVE on baltharus death
        //TALK once arrived at end WP
    }
};

CreatureAI* GetAI_mob_xerestrasza(Creature* pCreature)
{
    return new mob_xerestraszaAI(pCreature);
}

void AddSC_ruby_sanctum()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_xerestrasza";
    newscript->GetAI = &GetAI_mob_xerestrasza;
    newscript->RegisterSelf();
}
