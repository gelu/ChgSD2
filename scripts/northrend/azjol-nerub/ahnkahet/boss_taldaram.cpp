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
SDName: Boss_Taldaram
SD%Complete: 90%
SDAuthor: based on rsa's(tassadar), updated&fixed by orangevirus(kelthuzad)
SDComment: Timers, maybe wrong orb behavior
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SPELL_BEAM_VISUAL               = 60342,      // Used when taldram levitates before encounter
    SPELL_CONJURE_FLAME_ORB         = 55931,      // Dummy spell, dont do anything except cast
    SPELL_BLOODTHIRST               = 55968,
    SPELL_VANISH                    = 55964,      // Does not work...?
    SPELL_EMBRACE_OF_THE_VAMPYR     = 55959,
    SPELL_EMBRACE_OF_THE_VAMPYR_H   = 59513,

    SPELL_FLAME_ORB_SPAWN_EFFECT    = 55891, // Orb Grow up
    SPELL_FLAME_ORB_VISUAL          = 55928, // Flame orb effect
    SPELL_FLAME_ORB_DEATH           = 55947, // Despawn effect
    SPELL_FLAME_ORB                 = 57750, // Flame orb damage    
    SPELL_FLAME_ORB_H               = 58937,

    NPC_FLAME_ORB                   = 30702,

    SAY_AGGRO                       = -1619008,
    SAY_VANISH_1                    = -1619009,
    SAY_VANISH_2                    = -1619010,
    SAY_FEED_1                      = -1619011,
    SAY_FEED_2                      = -1619012,
    SAY_SLAY_1                      = -1619013,
    SAY_SLAY_2                      = -1619014,
    SAY_SLAY_3                      = -1619015,
    SAY_DEATH                       = -1619016,

    FLAME_ORB_Z                     = 17,

    FLAME_ORB_UP_X                  = 383,
    FLAME_ORB_UP_Y                  = -984,

    FLAME_ORB_DOWN_X                = 632,
    FLAME_ORB_DOWN_Y                = -684,

    FLAME_ORB_RIGHT_X               = 350,
    FLAME_ORB_RIGHT_Y               = -705,

    FLAME_ORB_LEFT_X                = 613,
    FLAME_ORB_LEFT_Y                = -966,
};

/*######
## boss_taldaram
######*/

struct MANGOS_DLL_DECL boss_taldaramAI : public ScriptedAI
{
    boss_taldaramAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool isInVanish;
    bool isInVampyrMode;

    uint32 summonFlameOrbTimer;
    bool orbSummoned;
    uint32 orbstartTimer;
    uint32 vanishTimer;
    uint32 bloodthirstTimer;
    uint32 embraceOfTheVampyrTimer;
    uint32 embraceOfTheVampyrInterruptDamage;
    uint32 embraceOfTheVampyrInterruptMaxDamage;
    uint32 embraceOfTheVampyrFinishedTimer;

    Creature* pFlameOrb;
    Creature* pFlameOrb2;
    Creature* pFlameOrb3;

    void Reset()
    {
        summonFlameOrbTimer = 12000;
        vanishTimer = 14000;
        bloodthirstTimer = 10000;
        embraceOfTheVampyrFinishedTimer = 20000;
        embraceOfTheVampyrInterruptMaxDamage = m_bIsRegularMode ? 20000 : 40000;
        orbstartTimer = 500;
        orbSummoned = false;
        isInVanish = false;
        isInVampyrMode = false;
        pFlameOrb = NULL;
        pFlameOrb2 = NULL;
        pFlameOrb3 = NULL;
    
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->RemoveAurasDueToSpell(SPELL_BEAM_VISUAL);
        
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, IN_PROGRESS);
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
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage) 
    {
        // hack to prevent the orb damaging taldaram
        if(pDoneBy->GetEntry() == NPC_FLAME_ORB)
            uiDamage = 0;

        if (isInVampyrMode)
        {
            embraceOfTheVampyrInterruptDamage += uiDamage;

            if (embraceOfTheVampyrInterruptDamage > embraceOfTheVampyrInterruptMaxDamage)
            {
                m_creature->InterruptNonMeleeSpells(false);	
                isInVampyrMode = false;
                embraceOfTheVampyrInterruptDamage = 0;
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (pWho->IsWithinDist(m_creature, 10.0f, true))
            {
                if (m_creature->isAlive())
                {
                    if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    {
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                    }
                    AttackStart(pWho);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!isInVanish && !isInVampyrMode)
            // Summon Flame Orb
            if(summonFlameOrbTimer <= uiDiff)
            {
                for(int i = 0; i < 3; ++i)
                {
                    m_creature->SummonCreature(NPC_FLAME_ORB, m_creature->GetPositionX(), m_creature->GetPositionY(), FLAME_ORB_Z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    if(m_bIsRegularMode)
                        break;
                }
                DoCast(m_creature, SPELL_CONJURE_FLAME_ORB);
                vanishTimer += 10000;
                summonFlameOrbTimer = 16000 + rand()%10000;
            }else summonFlameOrbTimer -= uiDiff;


        if (!isInVampyrMode)
            if(vanishTimer < uiDiff)
            {
                m_creature->CastSpell(m_creature,SPELL_VANISH,true);
                isInVanish = true;
                embraceOfTheVampyrTimer = 2400;
                vanishTimer = 200000;
            }else vanishTimer -= uiDiff;
        
        if (isInVanish)
        {
            if (embraceOfTheVampyrTimer < uiDiff)
            {
                Player* pPlayer;

                // get player enemy
                do
                {
                    pPlayer = (Player*) m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                }
                while (!pPlayer);

                m_creature->NearTeleportTo(pPlayer->GetPositionX() + 3.0f, pPlayer->GetPositionY() + 3.0f, pPlayer->GetPositionZ(), pPlayer->GetOrientation());
                m_creature->RemoveAurasDueToSpell(SPELL_VANISH);
                DoCastSpellIfCan(pPlayer, m_bIsRegularMode ? SPELL_EMBRACE_OF_THE_VAMPYR : SPELL_EMBRACE_OF_THE_VAMPYR_H);
                isInVampyrMode = true;
                summonFlameOrbTimer += 20000;
                isInVanish = false;
                vanishTimer = 25000;
            }else embraceOfTheVampyrTimer -= uiDiff;
        }

        // set VampyrMode false when Spell not stopped by damage
        if(isInVampyrMode)
            if(embraceOfTheVampyrFinishedTimer < uiDiff)
            {
                isInVampyrMode = false;
                embraceOfTheVampyrFinishedTimer = 20000;
            } else embraceOfTheVampyrFinishedTimer -= uiDiff;
            
        if(!isInVampyrMode && !isInVanish)
            if (bloodthirstTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOODTHIRST); 
                bloodthirstTimer = m_bIsRegularMode ? 15000 : 8000;
            }else bloodthirstTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taldaram(Creature* pCreature)
{
    return new boss_taldaramAI(pCreature);
}

/*######
## mob_flame_orb
######*/

struct MANGOS_DLL_DECL mob_flame_orbAI : public ScriptedAI
{
    mob_flame_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsFlying;
    int8 direction;

    uint32 m_uiDespawn_Timer;
    uint32 m_uiCast_Timer;

    void Reset()
    {
        m_uiDespawn_Timer = 13000;
        m_uiCast_Timer = 3000;
        direction = -1;
        m_bIsFlying = false;
        m_creature->SetDisplayId(10045); //hack to set model invisible
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        DoCast(m_creature, SPELL_FLAME_ORB_VISUAL);
        DoCast(m_creature, SPELL_FLAME_ORB_SPAWN_EFFECT);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    void AttackStart(Unit* pWho)
    {
        return;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        // Despawn Timer
        if(m_uiDespawn_Timer <= uiDiff)
        {
            DoCast(m_creature, SPELL_FLAME_ORB_DEATH);
            m_creature->ForcedDespawn();
        }else m_uiDespawn_Timer -= uiDiff;

        // Fly timer
        if(m_uiCast_Timer <= uiDiff)
        {
            if(m_bIsFlying)
                return;

            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_ORB : SPELL_FLAME_ORB_H);
            direction = urand(0,3);
            switch(direction)
            {
                case 0: // Up
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_UP_X, FLAME_ORB_UP_Y, FLAME_ORB_Z);
                    break;
                case 1: // Down
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_DOWN_X, FLAME_ORB_DOWN_Y, FLAME_ORB_Z);
                    break;
                case 2: // Right
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_RIGHT_X, FLAME_ORB_RIGHT_Y, FLAME_ORB_Z);
                    break;
                case 3: // Left
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_LEFT_X, FLAME_ORB_LEFT_Y, FLAME_ORB_Z);
                    break;
                default:
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_UP_X, FLAME_ORB_UP_Y, FLAME_ORB_Z);
                    break;
            }
            m_bIsFlying = true;
        }else m_uiCast_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_flame_orb(Creature* pCreature)
{
    return new mob_flame_orbAI(pCreature);
}

/*######
## go_nerubian_device
######*/

bool GOUse_go_nerubian_device(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    pInstance->SetData(TYPE_TALDARAM, SPECIAL);
    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    return false;
}

void AddSC_boss_taldaram()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_taldaram";
    newscript->GetAI = &GetAI_boss_taldaram;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_nerubian_device";
    newscript->pGOUse = &GOUse_go_nerubian_device;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_orb";
    newscript->GetAI = &GetAI_mob_flame_orb;
    newscript->RegisterSelf();
}
