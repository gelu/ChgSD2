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
SDName: Boss_Volazj
SD%Complete: 20%
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

//TODO: fill in texts in database. Also need to add text for whisper.
enum
{
    SPELL_MIND_FLAY                 = 57941,
    SPELL_MIND_FLAY_H               = 59974,
    SPELL_SHADOW_BOLT               = 57942,
    SPELL_SHADOW_BOLT_H             = 59975,

    //Shiver - horrible ability
    SPELL_SHIVER                    = 57949, //Jump Aura 
    SPELL_SHIVER_H                  = 59978,
    SPELL_SHIVER_DMG                = 57952, //Damage
    SPELL_SHIVER_DMG_H              = 59979,
    SPELL_SHIVER_DUMMY              = 57951, //What is this? 

    //This is little complicated:
    //When volajz cast this, on every player is cast different invisibility spell,
    //so they dont see together, but they see four Twisted Visages - images of other
    //four party members, which cast spell like their class.
    SPELL_INSANITY                  = 57496, //This is what volajz casts, it should trigger Twisted Visage spawn spells
    SPELL_INSANITY_PHASE            = 57507, //For use in code
    SPELL_INSANITY_PHASE_1          = 57508, //invis spells
    SPELL_INSANITY_PHASE_2          = 57509,
    SPELL_INSANITY_PHASE_3          = 57510,
    SPELL_INSANITY_PHASE_4          = 57511,
    SPELL_INSANITY_PHASE_5          = 57512,
    SPELL_INSANITY_CHANNEL          = 57561, //Just for visual, Volazj cast this when players are in insanity

    SPELL_TWISTED_VISAGE_MIRROR     = 57507, //Not implented in mangos, but I have patch :)

    /*
    http://www.wowhead.com/?spell=57507 Twisted visage visual
    http://www.wowhead.com/?spells=0&filter=na=twisted+visage so many spells?!
    */

    NPC_TWISTED_VISAGE              = 30621,
    //NPC_TWISTED_VISAGE              = 20058, //Bloodmaul wolf, for testing

    SAY_AGGRO                       = -1619033,
    SAY_INSANITY                    = -1619034,
    SAY_SLAY_1                      = -1619035,
    SAY_SLAY_2                      = -1619036,
    SAY_SLAY_3                      = -1619037,
    SAY_DEATH_1                     = -1619038,
    SAY_DEATH_2                     = -1619039,

    PHASE_NOSTART                   = 0,
    PHASE_FIGHT                     = 1,
    PHASE_INSANITY_1                = 2,  // Wait five seconds until cast is complete, set unattackable
    PHASE_INSANITY_2                = 3, 
    PHASE_INSANITY_3                = 4
};
/*######
## mob_twisted_visage
######*/
struct MANGOS_DLL_DECL mob_twisted_visageAI : public ScriptedAI
{
    mob_twisted_visageAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8 m_uiMyPhase;
    Player *m_pAttackPlayer;
    Player *m_pMirrorPlayer;

    void Reset()
    {
        if(m_pMirrorPlayer)
            m_creature->CastSpell(m_creature, 57507, false);

    }
    void Aggro(Unit *pWho)
    {
        if(m_pMirrorPlayer)
            m_creature->CastSpell(m_creature, 57507, false);
    }
    void EnterEvadeMode()
    {
        if (m_creature->IsInEvadeMode() || !m_creature->isAlive())
            return;
    }
    void JustDied(Unit *pWho)
    {
        pWho->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE+m_uiMyPhase);
    }
     
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();       
    }
};

/*######
## boss_volazj
######*/

struct MANGOS_DLL_DECL boss_volazjAI : public ScriptedAI
{
    boss_volazjAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsDebugMode;  //if only one player(GM) in instance
    uint8 m_uiPhase;
    Player *m_pLastShiverTarget;
    uint8 m_uiShiverJumpTimer;
    uint8 m_uiLastSacrifaceHP;

    uint32 m_uiMindFlayTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiShiverTimer;
    uint32 m_uiCheckTimer;

    //Insanity
    uint32 m_uiInsanityCastTimer;

    void Reset()
    {
        m_uiPhase = PHASE_NOSTART;
        m_bIsDebugMode = false;
        m_uiLastSacrifaceHP = 0;

        m_uiMindFlayTimer = 10000;
        m_uiShadowBoltTimer = 5000;
        m_uiShiverTimer = 18000;
        m_uiCheckTimer = 1000;
        m_uiShiverJumpTimer = 0;


        //Insanity
        m_uiInsanityCastTimer = 5000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        m_bIsDebugMode;
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, IN_PROGRESS);
        m_uiPhase = PHASE_FIGHT;
        
        Map* pMap = m_creature->GetMap();
        if(pMap)
        {
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            if(lPlayers.getSize() == 1)
                m_bIsDebugMode = true;
        }
    }
    void EnterEvadeMode()
    {
        if(m_uiPhase != PHASE_FIGHT)
            return;
    }
    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(urand(0, 1) ? SAY_DEATH_1 : SAY_DEATH_2, m_creature);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, DONE);
    }
    void DoShiver()
    {
        if(m_pLastShiverTarget && m_pLastShiverTarget->isAlive())
        {
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;
            Map::PlayerList const &lPlayers = pMap->GetPlayers();

            if (lPlayers.isEmpty())
                return;
            bool hasJumped = false;

            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if(m_pLastShiverTarget == itr->getSource())
                    continue;

                if(itr->getSource()->IsWithinDist(m_pLastShiverTarget, 20.0f, false))
                {
                    m_pLastShiverTarget->CastSpell(itr->getSource(),  m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H, true);
                    m_pLastShiverTarget = itr->getSource();
                    hasJumped = true;
                }
            }
            if(hasJumped == false)
            {
                if(m_uiShiverJumpTimer == 3)
                {
                    if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    {
                        DoCast(pTarget, m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H);
                        m_pLastShiverTarget = ((Player*)pTarget);
                    }
                    m_uiShiverJumpTimer = 0;
                }else m_uiShiverJumpTimer++;
            }
        }else{
            if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
            {
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H);
                m_pLastShiverTarget = ((Player*)pTarget);
            }        
        }
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiPhase == PHASE_FIGHT)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            //Spells
            //Mind Flay
            if(m_uiMindFlayTimer <= uiDiff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MIND_FLAY : SPELL_MIND_FLAY_H);
                m_uiMindFlayTimer = 10000 + rand()%10000;
            }else m_uiMindFlayTimer -= uiDiff;

            //Shadowbolt voley
            if(m_uiShadowBoltTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
                m_uiShadowBoltTimer = 8000 + rand()%5000;
            }else m_uiShadowBoltTimer -= uiDiff;

            //Shiver
            if(m_uiShiverTimer <= uiDiff)
            {
                //DoShiver();
                m_uiShiverTimer = 5000;
            }else m_uiShiverTimer -= uiDiff;

            //Health check
            if(m_uiCheckTimer <= uiDiff)
            {
                uint8 health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();                    
                if(m_uiLastSacrifaceHP == 0 && health <= 66)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    SetCombatMovement(false);    
                    m_uiLastSacrifaceHP = 66;
                    DoCast(m_creature, SPELL_INSANITY, false);
                    m_uiPhase = PHASE_INSANITY_1;
                    return;
                }
                else if(m_uiLastSacrifaceHP == 66 && health <= 33)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    SetCombatMovement(false);
                    DoCast(m_creature, SPELL_INSANITY, false);
                    m_uiLastSacrifaceHP = 33;
                    m_uiPhase = PHASE_INSANITY_1;
                    return;
                }
                m_uiCheckTimer = 1000;
            }else m_uiCheckTimer -= uiDiff; 
    
            DoMeleeAttackIfReady();
        }else if(m_uiPhase == PHASE_INSANITY_1)
        {
            //Wait until cast is complete
            if(m_uiInsanityCastTimer <= uiDiff)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCast(m_creature, SPELL_INSANITY_PHASE_1, true);
                DoCast(m_creature, SPELL_INSANITY_CHANNEL);
                DoInsanity();
                m_uiInsanityCastTimer = 5000;
                m_uiCheckTimer = 5000;
                m_uiPhase = PHASE_INSANITY_2;
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->MovementExpired(false);
            }else m_uiInsanityCastTimer -= uiDiff;
        }else if(m_uiPhase == PHASE_INSANITY_2)
        {
            if(m_uiCheckTimer <= uiDiff)
            {
                if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_TWISTED_VISAGE, 150.0f))
                {
                    if(!pTemp->isAlive())
                       m_uiPhase = PHASE_INSANITY_3;
                }else m_uiPhase = PHASE_INSANITY_3;
                m_uiCheckTimer = 1000;
            }else m_uiCheckTimer -= uiDiff; 
        }else if(m_uiPhase == PHASE_INSANITY_3)
        {
            m_creature->RemoveAurasDueToSpell(SPELL_INSANITY_CHANNEL);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_1);
            //RemoveInsanity();
            SetCombatMovement(true);
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            m_uiPhase = PHASE_FIGHT;
        }
        
    }
    //This do everything which is needed by Insanity spell
    void DoInsanity()
    {
        Map* pMap = m_creature->GetMap();
        if(!pMap)
            return;
        Map::PlayerList const &lPlayers = pMap->GetPlayers();

        if (lPlayers.isEmpty())
            return;
        int i = 1;
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if(!itr->getSource()->isAlive())
                continue;
            //Phase players, so they dont see together
            DoPhasePlayer(i, itr->getSource());
            //Spawn Visages and port them to phases
            DoSpawnTwistedVisages(itr->getSource(), i);
            i++;
        }
        //Set mirror image to twisted visages
        //DoInitializeVisages();
    }
    //Phase players, so they dont see together
    void DoPhasePlayer(uint8 count, Player *pPlayer)
    {
        if(pPlayer)
            pPlayer->CastSpell(pPlayer, SPELL_INSANITY_PHASE+count, false);
    }
    //Spawn Visages and port them to phases
    void DoSpawnTwistedVisages(Player *pPlayer, uint8 count)
    {
        if(!pPlayer)
            return;
        if(!pPlayer->isAlive())
            return;
        float x,y,z;
        m_creature->GetPosition(x, y, z);
        if(m_bIsDebugMode || count < 1)
        {
            if(Creature *pTemp = pPlayer->SummonCreature(NPC_TWISTED_VISAGE, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                 if(!pTemp->isAlive())
                    return;

                 pTemp->CastSpell(pTemp, SPELL_INSANITY_PHASE+1, true);
                 ((mob_twisted_visageAI*)pTemp->AI())->m_uiMyPhase = 1;
                 ((mob_twisted_visageAI*)pTemp->AI())->m_pMirrorPlayer = pPlayer;
                 pTemp->SetUInt32Value(UNIT_CREATED_BY_SPELL, 57500);
                 pTemp->SetCreatorGUID(pPlayer->GetGUID());
                 pPlayer->CastSpell(pTemp, SPELL_TWISTED_VISAGE_MIRROR, true);
            }
            return;
        }

        for(int i = 1; i <= 5; i++)
        {
            if(i == count)
                continue;

            if(Creature *pTemp = pPlayer->SummonCreature(NPC_TWISTED_VISAGE, x, y, z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                 if(!pTemp->isAlive())
                    return;

                 pTemp->CastSpell(pTemp, SPELL_INSANITY_PHASE+i, true);
                 ((mob_twisted_visageAI*)pTemp->AI())->m_uiMyPhase = i;
                 ((mob_twisted_visageAI*)pTemp->AI())->m_pMirrorPlayer = pPlayer;
                 pTemp->SetUInt32Value(UNIT_CREATED_BY_SPELL, 57500);
                 pTemp->SetCreatorGUID(pPlayer->GetGUID());
                 pPlayer->CastSpell(pTemp, SPELL_TWISTED_VISAGE_MIRROR, true);
            }
        }       
    }
};

CreatureAI* GetAI_boss_volazj(Creature* pCreature)
{
    return new boss_volazjAI(pCreature);
}

CreatureAI* GetAI_mob_twisted_visage(Creature* pCreature)
{
    return new mob_twisted_visageAI(pCreature);
}

void AddSC_boss_volazj()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_volazj";
    newscript->GetAI = &GetAI_boss_volazj;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twisted_visage";
    newscript->GetAI = &GetAI_mob_twisted_visage;
    newscript->RegisterSelf();
}
/* 
UPDATE creature_template SET ScriptName = "mob_twisted_visage" WHERE entry =30621;
*/
