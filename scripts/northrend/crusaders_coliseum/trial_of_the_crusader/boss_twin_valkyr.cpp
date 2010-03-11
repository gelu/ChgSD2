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

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Equipment
{
    EQUIP_MAIN_1         = 47266,
    EQUIP_OFFHAND_1      = 47146,
    EQUIP_RANGED_1       = 47267,
    EQUIP_MAIN_2         = 47266,
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
    SPELL_TWIN_POWER,
    SPELL_LIGHT_VORTEX,
    SPELL_LIGHT_ESSENCE,
    SPELL_LIGHT_TOUCH,
    SPELL_TWIN_SPIKE_H,
    SPELL_DARK_SURGE,
    SPELL_SHIELD_DARK,
    SPELL_TWIN_PACT_H,
    SPELL_DARK_VORTEX,
    SPELL_DARK_ESSENCE,
    SPELL_DARK_TOUCH,
    SPELL_BERSERK,
    BOSS_SPELL_COUNT
};

static SpellTable m_BossSpell[]=
{
// Name                  10     25     10H    25H
{SPELL_TWIN_SPIKE_L,     66075, 67312, 67313, 67314, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_SURGE,      65766, 67270, 67271, 67272, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHIELD_LIGHT,     65858, 67259, 67260, 67261, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_PACT_L,      65876, 67306, 67307, 67308, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_POWER,       65916, 67248, 67249, 67250, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_VORTEX,     66046, 67206, 67207, 67208, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_ESSENCE,    65686, 65686, 65686, 65686, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_LIGHT_TOUCH,      67297, 67297, 67298, 67298, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_SPIKE_H,     66069, 67309, 67310, 67311, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_SURGE,       65768, 67262, 67263, 67264, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_RANDOM, false, false},
{SPELL_SHIELD_DARK,      65874, 67256, 67257, 67258, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_TWIN_PACT_H,      65879, 67244, 67245, 67246, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_VORTEX,      66058, 67182, 67183, 67184, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_ESSENCE,     65684, 65684, 65684, 65684, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_DARK_TOUCH,       67282, 67282, 67283, 67283, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 30000, 65535, CAST_ON_SELF, false, false},
{SPELL_BERSERK,          64238, 64238, 64238, 64238, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 600000, 65535, CAST_ON_SELF, false, false},
};

/*######
## boss_fjola
######*/

struct MANGOS_DLL_DECL boss_fjolaAI : public ScriptedAI
{
    boss_fjolaAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    ScriptedInstance* m_pInstance;

    void Reset() {}

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

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
    boss_eydisAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() {}

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

bool GossipHello_mob_light_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    uint8 Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        pCreature->CastSpell(player,m_BossSpell[SPELL_LIGHT_ESSENCE].m_uiSpellEntry[Difficulty],false);
        player->CLOSE_GOSSIP_MENU();
    return true;
}

bool GossipHello_mob_dark_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    uint8 Difficulty = pInstance->GetData(TYPE_DIFFICULTY);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
        pCreature->CastSpell(player,m_BossSpell[SPELL_DARK_ESSENCE].m_uiSpellEntry[Difficulty],false);
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
    newscript->GetAI = &GetAI_boss_fjola;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_light_essence";
    newscript->pGossipHello = &GossipHello_mob_light_essence;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_essence";
    newscript->pGossipHello = &GossipHello_mob_dark_essence;
    newscript->RegisterSelf();

}
