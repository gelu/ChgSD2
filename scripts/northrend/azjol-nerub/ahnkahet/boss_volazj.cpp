/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SD%Complete: 60-90%
SDComment: todo: clone spells? timers..
SDAuthor: based on Tassadar, heavily modded by kelthuzad
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

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

    // phase shift spells
    SPELL_INSANITY_PHASE_16			= 57508,
    SPELL_INSANITY_PHASE_32			= 57509,
    SPELL_INSANITY_PHASE_64			= 57510,
    SPELL_INSANITY_PHASE_128		= 57511,
    SPELL_INSANITY_PHASE_256		= 57512,

    SPELL_INSANITY_CHANNEL          = 57561, //Just for visual, Volazj cast this when players are in insanity

    /*
    http://www.wowhead.com/?spell=57507 Twisted visage visual
    http://www.wowhead.com/?spells=0&filter=na=twisted+visage so many spells?!
    */

    SPELL_TWISTED_VISAGE_MIRROR     = 57507, 

    SAY_AGGRO                      = -1619033, 
    SAY_INSANITY                   = -1619034, 
    SAY_SLAY_1                     = -1619035, 
    SAY_SLAY_2                     = -1619036, 
    SAY_SLAY_3                     = -1619037, 
    SAY_DEATH                      = -1619038, 
    WHISPER_AGGRO                  = -1619039, 
    WHISPER_INSANITY               = -1619040, 
    WHISPER_SLAY_1                 = -1619041, 
    WHISPER_SLAY_2                 = -1619042, 
    WHISPER_SLAY_3                 = -1619043, 
    WHISPER_DEATH                  = -1619044, 

    PHASE_NOSTART                   = 0,  // before fight starts
    PHASE_FIGHT                     = 1,  // fight (no insanity)
    PHASE_INSANITY_1                = 2,  // Wait five seconds until cast is complete
    PHASE_INSANITY_2                = 3,  // is in insanity
    PHASE_INSANITY_3                = 4,  // insanity finished

    // twisted images
    NPC_TWISTED_IMAGE_1         = 30621,
    NPC_TWISTED_IMAGE_2         = 30622,
    NPC_TWISTED_IMAGE_3         = 30623,
    NPC_TWISTED_IMAGE_4         = 30624,
    NPC_TWISTED_IMAGE_5         = 30625,

    // clone spells - clone images use these
    SPELL_DRUID_1               = 69882, 
    SPELL_DRUID_1_H             = 71141, 
    SPELL_DRUID_2               = 69898, 
    SPELL_DRUID_2_H             = 71142, 
    SPELL_DRUID_3               = 38658, 
    SPELL_HUNT                  = 59604, 
    SPELL_DK                    = 55978, 
    SPELL_MAGE                  = 69869, 
    SPELL_MAGE_H                = 71130, 
    SPELL_ROGUE_1               = 37331, 
    SPELL_ROGUE_2               = 1330, 
    SPELL_SHAMAN                = 68113, 
    SPELL_SHAMAN_H              = 64213, 
    SPELL_PRIEST_1              = 71932, 
    SPELL_PRIEST_2              = 30854, 
    SPELL_PRIEST_1_H            = 43575, 
    SPELL_PRIEST_2_H            = 68089, 
    SPELL_WARRIOR               = 46271, 
    SPELL_WARRIOR_H             = 41399, 
    SPELL_WARLOCK               = 37668, 
    SPELL_WARLOCK_H             = 75383, 
    SPELL_PALA                  = 46029, 
    SPELL_PALA_H                = 41541, 

    FAC_HOSTILE                 = 16,

    ACHIEVEMENT_QUICK_DEMISE    = 1862
};

static uint32 m_aPhasingSpells[] = {57508,57509,57510,57511,57512}; // spells to shift players to different phases
static uint32 m_uilMirrorNPCs[] = {NPC_TWISTED_IMAGE_1,NPC_TWISTED_IMAGE_2,NPC_TWISTED_IMAGE_3,NPC_TWISTED_IMAGE_4,NPC_TWISTED_IMAGE_5}; // entry of different NPCs
static uint64 m_uilPlayerGUIDs[] = {0,0,0,0,0}; // GUIDs of all players

struct MANGOS_DLL_DECL mob_volazj_cloneAI : public ScriptedAI 
{ 
    mob_volazj_cloneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData(); 
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty(); 
        Reset(); 
    } 
 
    ScriptedInstance* m_pInstance; 
 
    bool m_bIsRegularMode; 
 
    uint32 m_bOwnclass; // id of Clone's class

    uint32 spellPriestTimer; 
    uint32 spellRogueTimer; 
    uint32 spellMageTimer; 
    uint32 spellDeathKnightTimer; 
    uint32 spellShamanTimer; 
    uint32 spellWarlockTimer; 
    uint32 spellDruidTimer; 
    uint32 spellWarriorTimer; 
    uint32 spellPaladinTimer; 
    uint32 spellHunterTimer; 
        
    void Reset()
    {
        m_bOwnclass = 0;
        spellPriestTimer = urand(3500,6500); 
        spellRogueTimer = urand(3500,6500); 
        spellMageTimer = urand(3500,6500); 
        spellDeathKnightTimer = urand(3500,6500); 
        spellShamanTimer = urand(3500,6500); 
        spellWarlockTimer = urand(3500,6500); 
        spellDruidTimer = urand(3500,6500); 
        spellWarriorTimer = urand(3500,6500); 
        spellPaladinTimer = urand(3500,6500); 
        spellHunterTimer = urand(3500,6500); 
    } 
 
    void UpdateAI(const uint32 uiDiff) 
    { 
        if (m_bOwnclass == CLASS_DRUID) 
        { 
            if (spellDruidTimer < uiDiff) 
            { 
                int randomSpell = urand(0, 2); 
                switch (randomSpell) 
                { 
                    case 0: 
                        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_1 : SPELL_DRUID_1_H); 
                        break; 
                    case 1: 
                        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_2 : SPELL_DRUID_2_H); 
                        break; 
                    case 2: 
                        DoCastSpellIfCan(m_creature, SPELL_DRUID_3); 
                        break; 
                } 
                spellDruidTimer = urand(5000,12000); 
            }else spellDruidTimer -= uiDiff; 
 
        } 
        else if (m_bOwnclass == CLASS_MAGE) 
        { 
            if (spellMageTimer < uiDiff) 
            { 
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGE : SPELL_MAGE_H); 
 
                spellMageTimer = urand(5000, 7000); 
            }else spellMageTimer -= uiDiff; 
        } 
        else if (m_bOwnclass == CLASS_PALADIN) 
        { 
            if (spellPaladinTimer < uiDiff) 
            { 
                if (!m_bIsRegularMode) 
                { 
                    if (!urand(0, 2)) 
                        DoCastSpellIfCan(m_creature, SPELL_PALA_H); 
                    else 
                        DoCastSpellIfCan(m_creature, SPELL_PALA);  
                } 
                else 
                    DoCastSpellIfCan(m_creature, SPELL_PALA);    
 
                spellPaladinTimer = urand(5000, 8000); 
            }else spellPaladinTimer -= uiDiff; 
        } 
        else if (m_bOwnclass == CLASS_HUNTER) 
        { 
            if (spellHunterTimer < uiDiff) 
            { 
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_HUNT); 
 
                spellHunterTimer = m_bIsRegularMode ? urand(9000, 11000) : urand(6000, 8000); 
            }else spellHunterTimer -= uiDiff;  
        } 
        else if (m_bOwnclass == CLASS_WARLOCK) 
        { 
            if (spellWarlockTimer < uiDiff) 
            { 
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK : SPELL_WARLOCK_H); 
 
                spellWarlockTimer = urand(6000, 10000); 
            }else spellWarlockTimer -= uiDiff; 
        } 
        else if (m_bOwnclass == CLASS_DEATH_KNIGHT) 
        { 
            if (spellDeathKnightTimer < uiDiff) 
            { 
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_DK); 
 
                spellDeathKnightTimer = m_bIsRegularMode ? urand(9000, 12000) : urand(4000, 5000); 
            }else spellDeathKnightTimer -= uiDiff;  
        } 
        else if (m_bOwnclass == CLASS_WARRIOR) 
        { 
            if (spellWarriorTimer < uiDiff) 
            { 
                if (m_bIsRegularMode) 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_WARRIOR); 
                    spellWarriorTimer = urand(4000, 6000); 
                } 
                else 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_WARRIOR_H); 
                    spellWarriorTimer = urand(20000, 25000); 
                } 
            }else spellWarriorTimer -= uiDiff; 
        } 
        else if (m_bOwnclass == CLASS_PRIEST) 
        { 
            if (spellPriestTimer < uiDiff) 
            { 
                if (m_bIsRegularMode)
                {
                    if (urand(0, 1))
                        DoCastSpellIfCan(m_creature->getVictim(), SPELL_PRIEST_2);
                    else
                        DoCastSpellIfCan(m_creature, SPELL_PRIEST_1);
                }
                else 
                {
                    if (!urand(0, 3))
                        DoCastSpellIfCan(m_creature->getVictim(), SPELL_PRIEST_2_H);
                    else
                        DoCastSpellIfCan(m_creature, SPELL_PRIEST_1_H);
                }
                  
                spellPriestTimer = urand (5000, 7000);  
            }else spellPriestTimer -= uiDiff; 
        } 
        else if (m_bOwnclass == CLASS_SHAMAN) 
        { 
            if (spellShamanTimer < uiDiff) 
            { 
                if (m_bIsRegularMode) 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHAMAN); 
                    spellShamanTimer = urand(4000, 6000); 
                } 
                else 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHAMAN_H); 
                    spellShamanTimer = urand(10000, 11000); 
                } 
            }else spellShamanTimer -= uiDiff;  
        } 
        else if (m_bOwnclass == CLASS_ROGUE) 
        { 
            if (spellRogueTimer < uiDiff) 
            { 
                if (m_bIsRegularMode) 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_ROGUE_2); 
                    spellRogueTimer = urand(4000, 6000); 
                } 
                else 
                { 
                    DoCastSpellIfCan(m_creature->getVictim(), SPELL_ROGUE_1); 
                    spellRogueTimer = urand(10000, 14000); 
                } 
            }else spellRogueTimer -= uiDiff; 
        } 
 
        DoMeleeAttackIfReady(); 
    } 
}; 
 
CreatureAI* GetAI_mob_volazj_clone(Creature* pCreature) 
{ 
    return new mob_volazj_cloneAI(pCreature); 
} 

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

    uint8 m_uiPhase;

    // normal-phase spells
    uint64 m_uiLastShiverTargetGUID;
    uint8 m_uiShiverJumpTimer;
    uint32 m_uiShiverTimer;
    uint32 m_uiMindFlayTimer;
    uint32 m_uiShadowBoltTimer;

    //Insanity
    uint32 m_uiInsanityCastTimer;
    uint8 m_uiInsanityCount;
    uint32 m_uiCheckTimer;
    bool m_bIsInInsanity; 
    bool m_bClone16;
    bool m_bClone32;
    bool m_bClone64;
    bool m_bClone128;
    bool m_bClone256;
    std::list<uint64> m_lCloneGUIDList; 
    std::list<uint64> m_lClone16GUIDList;
    std::list<uint64> m_lClone32GUIDList;
    std::list<uint64> m_lClone64GUIDList;
    std::list<uint64> m_lClone128GUIDList;
    std::list<uint64> m_lClone256GUIDList;

    // achievement
    bool m_bStartAchievement;
    bool m_bGetsAchievement;
    uint32 m_uiAchievementTimer;

    void Reset()
    {
        m_uiPhase = PHASE_NOSTART;
        m_uiInsanityCount = 0;

        m_uiMindFlayTimer = 10000;
        m_uiShadowBoltTimer = 5000;
        m_uiShiverTimer = 18000;
        m_uiCheckTimer = 1000;
        m_uiShiverJumpTimer = 0;
        m_uiLastShiverTargetGUID = 0;
        
        //Insanity
        m_uiInsanityCastTimer = 5000;
        m_bIsInInsanity = false;
        m_lCloneGUIDList.clear();
        m_lClone16GUIDList.clear();
        m_lClone32GUIDList.clear();
        m_lClone64GUIDList.clear();
        m_lClone128GUIDList.clear();
        m_lClone256GUIDList.clear();
        m_bClone16 = false;
        m_bClone32 = false;
        m_bClone64 = false;
        m_bClone128 = false;
        m_bClone256 = false;

        // achievement
        m_bStartAchievement = false;
        m_bGetsAchievement = true;
        m_uiAchievementTimer = 120000; // 2minutes

        if (m_pInstance && m_pInstance->GetData(TYPE_VOLAZJ) != DONE)
            m_pInstance->SetData(TYPE_VOLAZJ, NOT_STARTED);

        m_creature->SetPhaseMask(511,true);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, IN_PROGRESS);
        m_uiPhase = PHASE_FIGHT;

        DoScriptText(SAY_AGGRO, m_creature);

        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
            if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                DoScriptText(WHISPER_AGGRO,m_creature,target);

        m_bStartAchievement = true;
    }

    void EnterEvadeMode()
    {
        if(m_uiPhase != PHASE_FIGHT)
            return;

        m_creature->GetMotionMaster()->MoveTargetedHome();
        m_pInstance->SetData(TYPE_VOLAZJ, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        int32 textId = 0;

        switch(urand(0, 2))
        {
            case 0: textId = SAY_SLAY_1; break;
            case 1: textId = SAY_SLAY_2; break;
            case 2: textId = SAY_SLAY_3; break;
        }

        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
            if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                DoScriptText(textId+6,m_creature,target);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        Map* pMap = m_creature->GetMap();
        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &players = pMap->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                if(!m_bIsRegularMode && m_bGetsAchievement)
                    itr->getSource()->CompletedAchievement(ACHIEVEMENT_QUICK_DEMISE);
                DoScriptText(WHISPER_DEATH,m_creature,itr->getSource());
            }
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, DONE);
    }

    void createClassMirrors() 
    {
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        uint8 m_uiCount = 0;

        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        { 
            if(Unit* target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID()))
            {
                m_uilPlayerGUIDs[m_uiCount] = target->GetGUID(); // save GUIDs
                target->CastSpell(target,m_aPhasingSpells[m_uiCount],true); // cast phase shifting spell
                m_uiCount++;
            }
        }

        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) // get every player
        { 
            if(Unit* target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
            { 
                for(uint8 i=0;i<m_uiCount;i++) // get all other players
                {
                    if (target->GetGUID() == m_uilPlayerGUIDs[i]) // get other players, but not self
                        continue;

                    if(Player* pOtherPlayer = m_creature->GetMap()->GetPlayer(m_uilPlayerGUIDs[i]))
                    {    
                        uint32 m_uiNpc = urand(0,4); // create mirror image with random weapon/model on other players position
                        Creature* pClone = m_creature->SummonCreature(m_uilMirrorNPCs[m_uiNpc], pOtherPlayer->GetPositionX(), pOtherPlayer->GetPositionY(), pOtherPlayer->GetPositionZ(), pOtherPlayer->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 90000);
                    
                        if (pClone) 
                        {
                            pClone->SetDisplayId(pOtherPlayer->GetNativeDisplayId());
                            pClone->SetName(pOtherPlayer->GetName());
                            pClone->setFaction(FAC_HOSTILE);
                            if (mob_volazj_cloneAI* pCloneAI = dynamic_cast<mob_volazj_cloneAI*>(pClone->AI()))
                            {
                                pCloneAI->m_bOwnclass = pOtherPlayer->getClass(); // set class to other players class
                            }

                            pClone->Attack(target, true); 
                            pClone->GetMotionMaster()->MoveChase(target);
                            pClone->AddThreat(target, 10.0f);
                            pClone->SetPhaseMask(target->GetPhaseMask(),true);
                            m_lCloneGUIDList.push_back(pClone->GetGUID()); 

                            switch(pClone->GetPhaseMask())
                            {
                                case 16:  m_lClone16GUIDList.push_back(pClone->GetGUID()); break;
                                case 32:  m_lClone32GUIDList.push_back(pClone->GetGUID()); break;
                                case 64:  m_lClone64GUIDList.push_back(pClone->GetGUID()); break;
                                case 128: m_lClone128GUIDList.push_back(pClone->GetGUID()); break;
                                case 256: m_lClone256GUIDList.push_back(pClone->GetGUID()); break;
                                default: break;
                            }
                        } 
                    } 
                }   
            }
        }
    } 

    bool cloneAlive(int phase)
    {
        std::list<uint64> cloneList;
        
        switch(phase)
        {
            case 16: cloneList = m_lClone16GUIDList; break;
            case 32: cloneList = m_lClone32GUIDList; break;
            case 64: cloneList = m_lClone64GUIDList; break;
            case 128: cloneList = m_lClone128GUIDList; break;
            case 256: cloneList = m_lClone256GUIDList; break;
            default: cloneList = m_lCloneGUIDList; break;
        }

        if (!cloneList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = cloneList.begin(); itr != cloneList.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    uint32 getSpellForPhase(uint32 phase)
    {
        switch(phase)
        {
            case 16: return SPELL_INSANITY_PHASE_16;
            case 32: return SPELL_INSANITY_PHASE_32;
            case 64: return SPELL_INSANITY_PHASE_64;
            case 128: return SPELL_INSANITY_PHASE_128;
            case 256: return SPELL_INSANITY_PHASE_256;
        }
        return 0;
    }

    void shiftPlayersInOtherPhase(uint32 phase)
    {
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
        { 
            if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
            { 
                if (target->GetPhaseMask() == phase)
                {
                    target->RemoveAurasDueToSpell(getSpellForPhase(phase));
                    if (!m_bClone16)
                        target->CastSpell(target, SPELL_INSANITY_PHASE_16, true);
                    else if (!m_bClone32)
                        target->CastSpell(target, SPELL_INSANITY_PHASE_32, true);
                    else if (!m_bClone64)
                        target->CastSpell(target, SPELL_INSANITY_PHASE_64, true);
                    else if (!m_bClone128)
                        target->CastSpell(target, SPELL_INSANITY_PHASE_128, true);
                    else if (!m_bClone256)
                        target->CastSpell(target, SPELL_INSANITY_PHASE_256, true);
                }
            } 
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if(m_bStartAchievement)
            if(m_uiAchievementTimer < uiDiff)
            {
                m_bStartAchievement = false;
                m_bGetsAchievement = false;
            } 
            else
                m_uiAchievementTimer -= uiDiff;

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
            }
            else 
                m_uiMindFlayTimer -= uiDiff;

            //Shadowbolt voley
            if(m_uiShadowBoltTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
                m_uiShadowBoltTimer = 8000 + rand()%5000;
            }
            else 
                m_uiShadowBoltTimer -= uiDiff;

            //Shiver
            if(m_uiShiverTimer <= uiDiff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H);
                m_uiShiverTimer = 30000;
            }
            else 
                m_uiShiverTimer -= uiDiff;

            //insanity check
            if(m_uiCheckTimer <= uiDiff)
            {            
                if((m_uiInsanityCount == 0 && m_creature->GetHealthPercent() <= 66) || (m_uiInsanityCount == 1 && m_creature->GetHealthPercent() <= 33))
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    SetCombatMovement(false);    
                    m_uiInsanityCount=1;
                    DoCast(m_creature, SPELL_INSANITY);
                    m_uiPhase = PHASE_INSANITY_1;

                    DoScriptText(SAY_INSANITY, m_creature);

                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                        if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                            DoScriptText(WHISPER_INSANITY,m_creature,target);

                    return;
                }
                m_uiCheckTimer = 1000;
            }
            else 
                m_uiCheckTimer -= uiDiff;  

            DoMeleeAttackIfReady();
        }
        else if(m_uiPhase == PHASE_INSANITY_1)
        {
            //Wait until cast is complete
            if(m_uiInsanityCastTimer <= uiDiff)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                DoCast(m_creature, SPELL_INSANITY_CHANNEL);
                m_uiInsanityCastTimer = 5000;
                m_uiCheckTimer = 5000;
                m_uiPhase = PHASE_INSANITY_2;
                SetCombatMovement(false);
                m_creature->GetMotionMaster()->MovementExpired(false);
            }
            else 
                m_uiInsanityCastTimer -= uiDiff;
        }
        else if(m_uiPhase == PHASE_INSANITY_2) // cast complete, is in insanity
        {
            if(!m_bIsInInsanity) // do once on start of insanity
            {
                createClassMirrors();
                m_bIsInInsanity = true;
            }
            else if(m_uiCheckTimer <= uiDiff) 
            {
                if (!cloneAlive(1024)) // no clones alive -> change phase of fight
                {  

                    Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &players = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                    { 
                        if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                        {
                            for(int i=0;i<5;i++)
                                target->RemoveAurasDueToSpell(m_aPhasingSpells[i]); // make sure all players are out of their phases
                        }
                    }

                    m_bClone16 = m_bClone32 = m_bClone64 = m_bClone128 = m_bClone256 = false;
                    m_uiPhase = PHASE_INSANITY_3;
                }
                else if (!cloneAlive(16) && !m_bClone16)
                {
                    m_bClone16 = true;
                    shiftPlayersInOtherPhase(16);
                }
                else if (!cloneAlive(32) && !m_bClone32)
                {
                    m_bClone32 = true;
                    shiftPlayersInOtherPhase(32);
                }
                else if (!cloneAlive(64) && !m_bClone64)
                {
                    m_bClone64 = true;
                    shiftPlayersInOtherPhase(64);
                }
                else if (!cloneAlive(128) && !m_bClone128)
                {
                    m_bClone64 = true;
                    shiftPlayersInOtherPhase(128);
                }
                else if (!cloneAlive(256) && !m_bClone256)
                {
                    m_bClone64 = true;
                    shiftPlayersInOtherPhase(256);
                }
                m_uiCheckTimer = 1000;
            }
            else 
                m_uiCheckTimer -= uiDiff; 
        }
        else if(m_uiPhase == PHASE_INSANITY_3) // insanity finished
        {
            m_bIsInInsanity = false;
            m_creature->RemoveAurasDueToSpell(SPELL_INSANITY_CHANNEL);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            SetCombatMovement(true);
            m_creature->Attack(m_creature->getVictim(), true); 
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            m_uiPhase = PHASE_FIGHT;
        }

    }
};

CreatureAI* GetAI_boss_volazj(Creature* pCreature)
{
    return new boss_volazjAI(pCreature);
}



void AddSC_boss_volazj()
{
    Script *pNewscript;

    pNewscript = new Script;
    pNewscript->Name = "boss_volazj";
    pNewscript->GetAI = &GetAI_boss_volazj;
    pNewscript->RegisterSelf();

    pNewscript = new Script; 
    pNewscript->Name = "mob_volazj_clone"; 
    pNewscript->GetAI = &GetAI_mob_volazj_clone; 
    pNewscript->RegisterSelf(); 
}
