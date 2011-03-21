/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Thaddius
SD%Complete: 0
SDComment: Placeholder. Includes Feugen & Stalagg.
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    // Stalagg
    SAY_STAL_AGGRO                  = -1533023,
    SAY_STAL_SLAY                   = -1533024,
    SAY_STAL_DEATH                  = -1533025,

    SPELL_POWERSURGE                = 28134,
    SPELL_POWERSURGE_H              = 54529,

    //Feugen
    SAY_FEUG_AGGRO                  = -1533026,
    SAY_FEUG_SLAY                   = -1533027,
    SAY_FEUG_DEATH                  = -1533028,

    SPELL_STATICFIELD               = 28135,
    SPELL_STATICFIELD_H             = 54528,

    //both
    SPELL_WARSTOMP                  = 56427,   // workaround for damage when mobs are not on platforms
    SPELL_MAGNETIC_PULL             = 28337,   // not working

    //Thaddus
    SAY_GREET                       = -1533029,
    SAY_AGGRO1                      = -1533030,
    SAY_AGGRO2                      = -1533031,
    SAY_AGGRO3                      = -1533032,
    SAY_SLAY                        = -1533033,
    SAY_ELECT                       = -1533034, // when is this used?
    SAY_DEATH                       = -1533035,
    SAY_SCREAM1                     = -1533036, 
    SAY_SCREAM2                     = -1533037,
    SAY_SCREAM3                     = -1533038,
    SAY_SCREAM4                     = -1533039,

    SPELL_BALL_LIGHTNING            = 28299,

    EMOTE_POLARITY_SHIFT            = -1533151,

    SPELL_CHARGE_POSITIVE_DMGBUFF   = 29659,
    SPELL_CHARGE_POSITIVE_NEARDMG   = 28059,

    SPELL_CHARGE_NEGATIVE_DMGBUFF   = 29660,
    SPELL_CHARGE_NEGATIVE_NEARDMG   = 28084,

    SPELL_CHAIN_LIGHTNING           = 28167,
    SPELL_CHAIN_LIGHTNING_H         = 54531,

    SPELL_BERSERK                   = 26662,

    //generic
    NPC_TESLA_COIL                  = 16218,     // not used

    EMOTE_LOSING_LINK               = -1533149,
    EMOTE_TESLA_OVERLOAD            = -1533150, // emotes "Tesla Coil overloads!"
};

#define CHARGEDISTANCE 10.f

struct MANGOS_DLL_DECL boss_thaddiusAI : public ScriptedAI
{
    boss_thaddiusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStartTimer;
    uint32 m_uiScreamTimer;
    uint32 m_uiBallLightningTimer;
    uint32 m_uiChainLightningTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiPolarityShiftTimer;
    bool m_bStartsaid;


    void Reset()
    {
        m_uiStartTimer = 15000;
        m_uiScreamTimer = 10000;
        m_uiBallLightningTimer = 2000;
        m_uiChainLightningTimer = 15000;
        m_uiEnrageTimer = 6*MINUTE*IN_MILLISECONDS;
        m_uiPolarityShiftTimer = 30000;
        m_bStartsaid = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if(!m_bStartsaid)
        {
            DoScriptText(SAY_GREET,m_creature);
            m_bStartsaid = true;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, DONE);
        DoScriptText(SAY_DEATH, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0,2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void JustReachedHome()
    {
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_THADDIUS, FAIL);
            if(Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                pFeugen->Respawn();
            if(Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                pStalagg->Respawn();
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_pInstance->GetData(TYPE_THADDIUS) == SPECIAL) // make thaddius attackable 15 seconds after stalagg&feugen are dead
                if(m_uiStartTimer < uiDiff)
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                else
                    m_uiStartTimer -= uiDiff;
            return;
        }

        if(m_uiScreamTimer < uiDiff)
        {
            switch(urand(0,3))
            {
                case 0: DoScriptText(SAY_SCREAM1,m_creature); break;
                case 1: DoScriptText(SAY_SCREAM2,m_creature); break;
                case 2: DoScriptText(SAY_SCREAM3,m_creature); break;
                case 3: DoScriptText(SAY_SCREAM4,m_creature); break;
            }
            m_uiScreamTimer = urand(15000, 60000);
        }
        else
            m_uiScreamTimer -= uiDiff;

        if(!m_creature->IsWithinDistInMap(m_creature->getVictim(),5.f)) // this is not correct, spell should be cast when checked if no player is near Thaddius
        {
            if(m_uiBallLightningTimer < uiDiff)
            {
                m_creature->CastSpell(m_creature->getVictim(),SPELL_BALL_LIGHTNING,true);
                m_uiBallLightningTimer = urand(5000, 10000); // timers probably not right
            }
            else
                m_uiBallLightningTimer -= uiDiff;
        }

        if (m_uiChainLightningTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),m_bIsRegularMode?SPELL_CHAIN_LIGHTNING:SPELL_CHAIN_LIGHTNING_H,true);
            m_uiChainLightningTimer = urand(14000,16000);
        }
        else
            m_uiChainLightningTimer -= uiDiff;

        if (m_uiPolarityShiftTimer < uiDiff)
        {
            // get count of positive and negative charges
            uint32 m_uiNegativeCharge = m_creature->GetMap()->GetPlayersCountExceptGMs() / 2;
            uint32 m_uiPositiveCharge = m_creature->GetMap()->GetPlayersCountExceptGMs() - m_uiNegativeCharge;

            // split players into positive and negative charged
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const& pPlayers = pMap->GetPlayers();
            if (!pPlayers.isEmpty())
            {
                for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                {
                    if (itr->getSource()->isGameMaster())
                        continue;

                    Player *pTarget = itr->getSource();
                    pTarget->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_DMGBUFF);
                    pTarget->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_DMGBUFF);
                    pTarget->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_NEARDMG);
                    pTarget->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_NEARDMG);

                    if(((rand()%2) && m_uiNegativeCharge) || (!m_uiPositiveCharge))
                    {
                        pTarget->CastSpell(pTarget,SPELL_CHARGE_NEGATIVE_NEARDMG,true);
                        m_uiNegativeCharge--;
                    }
                    else
                    {
                        pTarget->CastSpell(pTarget,SPELL_CHARGE_POSITIVE_NEARDMG,true);
                        m_uiPositiveCharge--;
                    }
                }
            }

            m_uiPolarityShiftTimer = 30000;
        }
        else
            m_uiPolarityShiftTimer -= uiDiff;

        if (m_uiEnrageTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature,SPELL_BERSERK,true);
            m_uiEnrageTimer = 10*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thaddius(Creature* pCreature)
{
    return new boss_thaddiusAI(pCreature);
}

struct MANGOS_DLL_DECL boss_feugenAI : public ScriptedAI
{
    boss_feugenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStaticFieldTimer;
    uint32 m_uiAggroTimer;
    uint32 m_uiDeathTimer;
    uint32 m_uiCheckDistanceTimer;
    bool m_bOutOfRange;
    bool m_bStalaggDead;

    void Reset()
    {
        m_uiStaticFieldTimer = 3500;
        m_uiAggroTimer = 10000;
        m_uiDeathTimer = 5000;
        m_uiCheckDistanceTimer = 1000;
        m_bOutOfRange = false;
        m_bStalaggDead = false;

        if (m_pInstance)
            if (GameObject* pTesla = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_TESLA_COIL_FEUGEN)))
            {
                if (pTesla->GetGoState() == GO_STATE_ACTIVE)
                    pTesla->SetGoState(GO_STATE_READY);
            }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_FEUG_DEATH,m_creature);

        if(m_pInstance)
            if(Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                if(!pStalagg->isAlive())
                    m_pInstance->SetData(TYPE_THADDIUS, SPECIAL);
    }

    void Aggro(Unit* pWho)
    {
        if(m_pInstance)
            if(m_pInstance->GetData(TYPE_THADDIUS) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);

        DoScriptText(SAY_FEUG_AGGRO,m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_FEUG_SLAY,m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            return;

        if (m_uiDeathTimer < uiDiff) // check death of stalagg
        {
            if(Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                if (m_bStalaggDead)
                {
                    pStalagg->Respawn(); // maybe there is a better way, respawn does move him start position
                    m_uiDeathTimer = 500;
                    m_bStalaggDead = false;
                }
                else
                    if(!pStalagg->isAlive())
                    {
                        m_bStalaggDead = true;
                        m_uiDeathTimer = 5000;
                    }
        }
        else
            m_uiDeathTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance->GetData(TYPE_THADDIUS) == IN_PROGRESS) // prevent bosses from being killed separately
                if(m_uiAggroTimer < uiDiff)
                {
                    if(Creature* pStalagg = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_STALAGG)))
                        if(pStalagg->getVictim()) 
                        {
                            m_creature->Attack(pStalagg->getVictim(),true);
                            m_creature->GetMotionMaster()->MoveChase(pStalagg->getVictim());
                        }

                }
                else
                    m_uiAggroTimer -= uiDiff;
                   
            return;
        }

        if (m_uiCheckDistanceTimer < uiDiff) // check if still on platform
        {
            if (m_bOutOfRange) 
            {
                DoScriptText(EMOTE_TESLA_OVERLOAD,m_creature); 
                m_creature->CastSpell(m_creature, SPELL_WARSTOMP,true);

                Map* pMap = m_creature->GetMap();
                Map::PlayerList const& pPlayers = pMap->GetPlayers();  // do not know wich spell to use, this is hacked
                if (!pPlayers.isEmpty())
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        m_creature->DealDamage(itr->getSource(),urand(5500,10000), NULL, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NATURE, NULL, false);
                    }
            }
            else
                if (GameObject* pTesla = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_TESLA_COIL_FEUGEN)))
                    if (m_creature->GetDistance(pTesla) > 63.5f)
                    {
                        pTesla->UseDoorOrButton();
                        m_bOutOfRange = true;
                        DoScriptText(EMOTE_LOSING_LINK,m_creature);
                    }
            m_uiCheckDistanceTimer = 1000;
        }
        else
            m_uiCheckDistanceTimer -= uiDiff;

        if (m_uiStaticFieldTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,m_bIsRegularMode?SPELL_STATICFIELD:SPELL_STATICFIELD_H);
            m_uiStaticFieldTimer = urand(5200,6500);
        }
        else
            m_uiStaticFieldTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_feugen(Creature* pCreature)
{
    return new boss_feugenAI(pCreature);
}

struct MANGOS_DLL_DECL boss_stalaggAI : public ScriptedAI
{
    boss_stalaggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPowerSurgeTimer;
    uint32 m_uiMagneticPullTimer;
    bool m_bDoMagneticPull;
    uint32 m_uiAggroTimer;
    uint32 m_uiDeathTimer;
    uint32 m_uiCheckDistanceTimer;
    bool m_bOutOfRange;
    bool m_bFeugenDead;

    void Reset()
    {
        m_uiPowerSurgeTimer = urand(5000,7000);
        m_uiMagneticPullTimer = 30000;
        m_uiAggroTimer = 10000;
        m_uiDeathTimer = 5000;
        m_bOutOfRange = false;
        m_bFeugenDead = false;
        m_uiCheckDistanceTimer = 1000;

        if (m_pInstance)
            if (GameObject* pTesla = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_TESLA_COIL_STALAGG)))
            {
                if (pTesla->GetGoState() == GO_STATE_ACTIVE)
                    pTesla->SetGoState(GO_STATE_READY);
            }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_STAL_DEATH,m_creature);
        if(m_pInstance)
            if(Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                if(!pFeugen->isAlive())
                    m_pInstance->SetData(TYPE_THADDIUS, SPECIAL);
    }

    void Aggro(Unit* pWho)
    {
        if(m_pInstance)
            if(m_pInstance->GetData(TYPE_THADDIUS) != IN_PROGRESS)
                m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);

        DoScriptText(SAY_STAL_AGGRO,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_STAL_SLAY,m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if(!m_pInstance)
            return;

        if (m_uiDeathTimer < uiDiff) // check death of stalagg
        {
            if(Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                if (m_bFeugenDead)
                {
                    pFeugen->Respawn(); // maybe there is a better way, respawn does move him start position
                    m_uiDeathTimer = 500;
                    m_bFeugenDead = false;
                }
                else
                    if(!pFeugen->isAlive())
                    {
                        m_bFeugenDead = true;
                        m_uiDeathTimer = 5000;
                    }
        }
        else
            m_uiDeathTimer -= uiDiff;
        
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance->GetData(TYPE_THADDIUS) == IN_PROGRESS) // prevent bosses from being killed separately
                if(m_uiAggroTimer < uiDiff)
                {
                    if(Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN)))
                        if(pFeugen->getVictim()) 
                        {
                            m_creature->Attack(pFeugen->getVictim(),true);
                            m_creature->GetMotionMaster()->MoveChase(pFeugen->getVictim());
                        }

                }
                else
                    m_uiAggroTimer -= uiDiff;

            return;
        }

        if (m_uiCheckDistanceTimer < uiDiff) // check if still on platform
        {
            if (m_bOutOfRange) 
            {
                DoScriptText(EMOTE_TESLA_OVERLOAD,m_creature); 
                m_creature->CastSpell(m_creature, SPELL_WARSTOMP,true);

                Map* pMap = m_creature->GetMap();
                Map::PlayerList const& pPlayers = pMap->GetPlayers();  // do not know wich spell to use, this is hacked
                if (!pPlayers.isEmpty())
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        m_creature->DealDamage(itr->getSource(),urand(5500,10000), NULL, SPELL_DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NATURE, NULL, false);
                    }
            }
            else
                if (GameObject* pTesla = m_creature->GetMap()->GetGameObject(m_pInstance->GetData64(GO_TESLA_COIL_STALAGG)))
                    if (m_creature->GetDistance(pTesla) > 63.5f)
                    {
                        pTesla->UseDoorOrButton();
                        m_bOutOfRange = true;
                        DoScriptText(EMOTE_LOSING_LINK,m_creature);
                    }
            m_uiCheckDistanceTimer = 1000;
        }
        else
            m_uiCheckDistanceTimer -= uiDiff;

        if (m_uiPowerSurgeTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature,m_bIsRegularMode?SPELL_POWERSURGE:SPELL_POWERSURGE_H);
            m_uiPowerSurgeTimer = urand(26000,30000);
        }
        else
            m_uiPowerSurgeTimer -= uiDiff;

        if (m_uiMagneticPullTimer < uiDiff)
        {
            if (Creature* pFeugen = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_FEUGEN))) // get Feugan
                if (pFeugen->isAlive())
                {
                    Unit* pTargetFeugen = pFeugen->getVictim();
                    Unit* pTargetStalagg = m_creature->getVictim();
                    if (pTargetFeugen)
                    {
                        float m_fThreatStalaggTarget = m_creature->getThreatManager().getThreat(pTargetStalagg);
                        float m_fThreatFeugenTarget = pFeugen->getThreatManager().getThreat(pTargetFeugen);
                        // magnetic pull not working at the moment, use temporary hack
                        //pFeugen->CastSpell(pTargetStalagg, SPELL_MAGNETIC_PULL, false);
                        //m_creature->CastSpell(pTargetFeugen, SPELL_MAGNETIC_PULL, false);
                        pTargetStalagg->NearTeleportTo(pTargetFeugen->GetPositionX(),pTargetFeugen->GetPositionY(),pTargetFeugen->GetPositionZ(),pTargetFeugen->GetOrientation());
                        pTargetFeugen->NearTeleportTo(pTargetStalagg->GetPositionX(),pTargetStalagg->GetPositionY(),pTargetStalagg->GetPositionZ(),pTargetStalagg->GetOrientation());
                        if (HostileReference* ref = pFeugen->getThreatManager().getCurrentVictim())
                            ref->removeReference();
                        if (HostileReference* ref = m_creature->getThreatManager().getCurrentVictim())
                            ref->removeReference();
                        m_creature->AddThreat(pTargetFeugen,m_fThreatFeugenTarget);
                        pFeugen->AddThreat(pTargetStalagg,m_fThreatStalaggTarget);
                    }    
                }
            m_uiMagneticPullTimer = 30000;
        }
        else
            m_uiMagneticPullTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_stalagg(Creature* pCreature)
{
    return new boss_stalaggAI(pCreature);
}

void AddSC_boss_thaddius()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_stalagg";
    pNewScript->GetAI = &GetAI_boss_stalagg;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_feugen";
    pNewScript->GetAI = &GetAI_boss_feugen;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_thaddius";
    pNewScript->GetAI = &GetAI_boss_thaddius;
    pNewScript->RegisterSelf();
}
