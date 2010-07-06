/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */
#include "precompiled.h"
#include "sc_boss_spell_worker.h"
#ifdef DEF_BOSS_SPELL_WORKER_H
#include "ace/Process_Mutex.h"

BSWScriptedAI::BSWScriptedAI(Creature* pCreature) : ScriptedAI(pCreature)
{
     pMap = m_creature->GetMap();
     if (pMap) currentDifficulty = pMap->GetDifficulty();
        else currentDifficulty = RAID_DIFFICULTY_10MAN_NORMAL;
     debug_log("BSW: Initializing BSWScriptedAI structure for creature %u difficulty %u",m_creature->GetEntry(),currentDifficulty);
     doReset();
};

BSWScriptedAI::~BSWScriptedAI()
{
     debug_log("BSW: Removing BSWScriptedAI structure for creature %u",m_creature->GetEntry());
};

void BSWScriptedAI::doReset()
{
     memset(&m_uiSpell_Timer, 0, sizeof(m_uiSpell_Timer));
     memset(&m_BossSpell,0,sizeof(m_BossSpell));
     _bossSpellCount = 0;
     _loadSpellTable();
     resetTimers();
};

void BSWScriptedAI::_resetTimer(uint8 m_uiSpellIdx)
{
    if (m_uiSpellIdx > bossSpellCount()) return;

    if (m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty] != m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty])
            m_uiSpell_Timer[m_uiSpellIdx] = urand(0,m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty]);
                else m_uiSpell_Timer[m_uiSpellIdx] = m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty];

    if (m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMin[currentDifficulty] == 0 
        && m_BossSpell[m_uiSpellIdx].m_uiSpellTimerMax[currentDifficulty] >= HOUR*IN_MILLISECONDS)
            m_uiSpell_Timer[m_uiSpellIdx] = 0;
};

void BSWScriptedAI::_loadSpellTable()
{
    // mutex block for process-safe request execute
    ACE_Process_Mutex mMutex = ACE_Process_Mutex("BSW_Lock");

    debug_log("BSW: Loading table of creature %u spell on difficulty %u", m_creature->GetEntry(), currentDifficulty);

    char query[MAX_QUERY_LEN];

    sprintf(query, "SELECT entry, spellID_N10, spellID_N25, spellID_H10, spellID_H25, timerMin_N10, timerMin_N25, timerMin_H10, timerMin_H25, timerMax_N10, timerMax_N25, timerMax_H10, timerMax_H25, data1, data2, data3, data4, locData_x, locData_y, locData_z, varData, StageMask_N, StageMask_H, CastType, isVisualEffect, isBugged, textEntry FROM `boss_spell_table` WHERE entry = %u;\r\n", m_creature->GetEntry());

    mMutex.acquire();
       QueryResult* Result = strSD2Pquery(query);
    mMutex.release();

    if (Result)
    {
        uint32 uiCount = 0;
        do
        {
            Field* pFields = Result->Fetch();

            m_BossSpell[uiCount].id  = uiCount;

            uint32 m_creatureEntry          = pFields[0].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellEntry[j]  = pFields[1+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellTimerMin[j]  = pFields[1+DIFFICULTY_LEVELS+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellTimerMax[j]  = pFields[1+DIFFICULTY_LEVELS*2+j].GetUInt32();

            for (uint8 j = 0; j < DIFFICULTY_LEVELS; ++j)
                 m_BossSpell[uiCount].m_uiSpellData[j]  = pFields[1+DIFFICULTY_LEVELS*3+j].GetUInt32();

            m_BossSpell[uiCount].LocData.x  = pFields[1+DIFFICULTY_LEVELS*4].GetFloat();
            m_BossSpell[uiCount].LocData.y  = pFields[2+DIFFICULTY_LEVELS*4].GetFloat();
            m_BossSpell[uiCount].LocData.z  = pFields[3+DIFFICULTY_LEVELS*4].GetFloat();

            m_BossSpell[uiCount].varData    = pFields[4+DIFFICULTY_LEVELS*4].GetInt32();

            m_BossSpell[uiCount].StageMaskN = pFields[5+DIFFICULTY_LEVELS*4].GetUInt32();
            m_BossSpell[uiCount].StageMaskH = pFields[6+DIFFICULTY_LEVELS*4].GetUInt32();

            m_BossSpell[uiCount].m_CastTarget = _getBSWCastType(pFields[7+DIFFICULTY_LEVELS*4].GetUInt8());

            m_BossSpell[uiCount].m_IsVisualEffect = (pFields[8+DIFFICULTY_LEVELS*4].GetUInt8() == 0) ? false : true ;

            m_BossSpell[uiCount].m_IsBugged = (pFields[9+DIFFICULTY_LEVELS*4].GetUInt8() == 0) ? false : true ;

            m_BossSpell[uiCount].textEntry = pFields[10+DIFFICULTY_LEVELS*4].GetInt32();

            if (m_creatureEntry != m_creature->GetEntry()) error_log("BSW: Unknown error while load boss_spell_table");
               else ++uiCount;

        } while (Result->NextRow());

        _bossSpellCount = uiCount;

        delete Result;

        _fillEmptyDataField();

        debug_log("BSW: Loaded %u spell data records for creature %u", uiCount, m_creature->GetEntry());
    }
    else
    {
        error_log("BSW: Boss spell table for creature %u is empty.", m_creature->GetEntry());
        _bossSpellCount = 0;
    };
}

bool BSWScriptedAI::_QuerySpellPeriod(uint8 m_uiSpellIdx, uint32 diff)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (m_uiSpell_Timer[m_uiSpellIdx] < diff) 
    {
        if (pSpell->m_uiSpellTimerMax[currentDifficulty] >= HOUR*IN_MILLISECONDS) m_uiSpell_Timer[m_uiSpellIdx]=HOUR*IN_MILLISECONDS;
            else m_uiSpell_Timer[m_uiSpellIdx]=urand(pSpell->m_uiSpellTimerMin[currentDifficulty],pSpell->m_uiSpellTimerMax[currentDifficulty]);
        return true;
     } else {
                m_uiSpell_Timer[m_uiSpellIdx] -= diff;
                return false;
            };
};

CanCastResult BSWScriptedAI::_BSWSpellSelector(uint8 m_uiSpellIdx, Unit* pTarget)
{
    SpellEntry const *spell;

    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    Unit* pSummon = NULL;

        debug_log("BSW: Casting spell number %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

        switch (pSpell->m_CastTarget) {

            case DO_NOTHING: 
                   return CAST_OK;

            case CAST_ON_SELF:
                   if (!pSpell->m_IsBugged) return _DoCastSpellIfCan(m_creature, pSpell->m_uiSpellEntry[currentDifficulty]);
                   else return _BSWDoCast(m_uiSpellIdx, m_creature);
                   break;

            case CAST_ON_SUMMONS:
                   if (!pTarget) return CAST_FAIL_OTHER;
                   else return _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
                   break;

            case CAST_ON_VICTIM:
                   pTarget = m_creature->getVictim();
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_RANDOM:
                   pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_BOTTOMAGGRO:
                   pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_BOTTOMAGGRO,0);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_TARGET:
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case APPLY_AURA_SELF:
                   spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[currentDifficulty]);
                   if (spell)
                       if (m_creature->AddAura(new BossAura(spell, EFFECT_INDEX_0, &pSpell->varData, m_creature, m_creature)))
                              return CAST_OK;
                   return CAST_FAIL_OTHER;
                   break;

            case APPLY_AURA_TARGET:
                   if (!pTarget || !pTarget->IsInMap(m_creature)) return CAST_FAIL_OTHER;
                       _doAura(m_uiSpellIdx, pTarget, EFFECT_INDEX_0);
                       return CAST_OK;
                   break;

            case SUMMON_NORMAL:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case SUMMON_TEMP:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,
                                        urand(pSpell->m_uiSpellTimerMin[currentDifficulty],pSpell->m_uiSpellTimerMax[currentDifficulty]));
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case SUMMON_INSTANT:
                   pSummon = _doSummon(m_uiSpellIdx, TEMPSUMMON_MANUAL_DESPAWN,0);
                   if(pSummon) return CAST_OK;
                          else return CAST_FAIL_OTHER;
                   break;

            case CAST_ON_ALLPLAYERS:
            {
                    CanCastResult res1 = CAST_FAIL_OTHER;
                    Map::PlayerList const& pPlayers = pMap->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        pTarget = itr->getSource();
                        if (pTarget && pTarget->isAlive() && pTarget->IsWithinDistInMap(m_creature, pSpell->LocData.x))
                           {
                               if (!pSpell->m_IsBugged) 
                                   {
                                       res1 = _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
                                   }
                                   else 
                                   {
                                       _BSWDoCast(m_uiSpellIdx, pTarget);
                                       res1 = CAST_OK;
                                   };
                           };
                           return res1;
                     }
                   break;
            }

            case CAST_ON_FRENDLY:
                   pTarget = doSelectLowHPFriendly(pSpell->LocData.x);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_FRENDLY_LOWHP:
                   pTarget = doSelectLowHPFriendly(pSpell->LocData.x);
                   return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                   break;

            case CAST_ON_RANDOM_POINT:
                   if (!pTarget) pTarget = m_creature;
                   if (pSpell->LocData.z <= 1.0f) {
                         float fPosX, fPosY, fPosZ;
                         if (!pTarget->IsPositionValid() || !pTarget->IsInMap(m_creature))
                            {
                                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                                     error_log("BSW: CAST_ON_RANDOM_POINT FAILED: player has invalid position. SpellID is %u",pSpell->m_uiSpellEntry[currentDifficulty]);
                                else error_log("BSW: CAST_ON_RANDOM_POINT FAILED: creature has invalid position. SpellID is %u",pSpell->m_uiSpellEntry[currentDifficulty]);
                                return CAST_FAIL_OTHER;
                            }
                         pTarget->GetPosition(fPosX, fPosY, fPosZ);
                         pTarget->GetRandomPoint(fPosX, fPosY, fPosZ, urand((uint32)pSpell->LocData.x, (uint32)pSpell->LocData.y), fPosX, fPosY, fPosZ);
                                if ((int)fPosZ == 0)
                                {
                                    error_log("BSW: CAST_ON_RANDOM_POINT FAILED: Positon Z is NULL. Strange bug");
                                    return CAST_FAIL_OTHER;
                                 }
                         if (SpellEntry const *spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[currentDifficulty]))
                           if (SpellRangeEntry const *pSpellRange = GetSpellRangeStore()->LookupEntry(spell->rangeIndex))
                              if (m_creature->GetDistance(fPosX, fPosY, fPosZ) <= pSpellRange->maxRange)
                                 {
                                     m_creature->CastSpell(fPosX, fPosY, fPosZ, pSpell->m_uiSpellEntry[currentDifficulty], false);
                                     return CAST_OK;
                                 };
                                 return CAST_FAIL_TOO_FAR;
                         } else  return CAST_FAIL_OTHER;
                   break;

            case CAST_ON_RANDOM_PLAYER:
                   if ( pSpell->LocData.x < 1 ) pTarget = doSelectRandomPlayer();
                       else pTarget = doSelectRandomPlayerAtRange((float)pSpell->LocData.x);
                   if (pTarget && pTarget->IsInMap(m_creature)) return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                       else return CAST_FAIL_OTHER;
                   break;

            case APPLY_AURA_ALLPLAYERS:
                   {
                       Map::PlayerList const& pPlayers = pMap->GetPlayers();
                       for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                       {
                           pTarget = itr->getSource();
                           if (pTarget && pTarget->isAlive() && pTarget->IsWithinDistInMap(m_creature, pSpell->LocData.x))
                               _doAura(m_uiSpellIdx, pTarget, EFFECT_INDEX_0);
                       }
                   return CAST_OK;
                   }
                   break;

            case SPELLTABLEPARM_NUMBER:
            default:
                   return CAST_FAIL_OTHER;
                   break;
            };

    error_log("BSW: FAILED casting spell number %u type %u - type not exists",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

    return CAST_FAIL_OTHER;
};

CanCastResult BSWScriptedAI::_BSWCastOnTarget(Unit* pTarget, uint8 m_uiSpellIdx)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive())
        {
           error_log("BSW: FAILED casting (on target) spell number %u - no target or target not in map",pSpell->m_uiSpellEntry[currentDifficulty]);
           return CAST_FAIL_OTHER;
        }

    debug_log("BSW: Casting (on target) spell %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

    if (!pSpell->m_IsBugged) return _DoCastSpellIfCan(pTarget, pSpell->m_uiSpellEntry[currentDifficulty]);
        else if (pSpell->m_IsBugged) return _BSWDoCast(m_uiSpellIdx, pTarget);
             else  return CAST_FAIL_OTHER;
};

bool BSWScriptedAI::_hasAura(uint8 m_uiSpellIdx, Unit* pTarget)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (!pTarget || !pTarget->IsInMap(m_creature)) 
        {
            error_log("BSW: FAILED Query aura for spell %u - no target or target not in map",pSpell->m_uiSpellEntry[currentDifficulty]);
            return false;
        }

    return (pTarget->HasAura(pSpell->m_uiSpellEntry[currentDifficulty]));

};

uint8 BSWScriptedAI::_auraCount(uint8 m_uiSpellIdx, Unit* pTarget, SpellEffectIndex index)
{
    if (!_hasAura(m_uiSpellIdx,pTarget)) return 0;

    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (Aura* aura = pTarget->GetAura(pSpell->m_uiSpellEntry[currentDifficulty], index))
        if (aura->GetStackAmount() > 0)
            return aura->GetStackAmount();
    return 0;

};

uint8 BSWScriptedAI::_findSpellIDX(uint32 SpellID)
{
    if (bossSpellCount() >= 0)
        for(uint8 i = 0; i < bossSpellCount(); ++i)
            if (m_BossSpell[i].m_uiSpellEntry[RAID_DIFFICULTY_10MAN_NORMAL] == SpellID) return i;

    error_log("BSW: spell %u not found  in m_creature %u spelltable. Memory or database error?", SpellID, m_creature->GetEntry());

    return SPELL_INDEX_ERROR;
}

BossSpellTableParameters BSWScriptedAI::_getBSWCastType(uint32 pTemp)
{
    switch (pTemp) {
                case 0:  return DO_NOTHING;
                case 1:  return CAST_ON_SELF;
                case 2:  return CAST_ON_SUMMONS;
                case 3:  return CAST_ON_VICTIM;
                case 4:  return CAST_ON_RANDOM;
                case 5:  return CAST_ON_BOTTOMAGGRO;
                case 6:  return CAST_ON_TARGET;
                case 7:  return APPLY_AURA_SELF;
                case 8:  return APPLY_AURA_TARGET;
                case 9:  return SUMMON_NORMAL;
                case 10: return SUMMON_INSTANT;
                case 11: return SUMMON_TEMP;
                case 12: return CAST_ON_ALLPLAYERS;
                case 13: return CAST_ON_FRENDLY;
                case 14: return CAST_ON_FRENDLY_LOWHP;
                case 15: return CAST_ON_RANDOM_POINT;
                case 16: return CAST_ON_RANDOM_PLAYER;
                case 17: return APPLY_AURA_ALLPLAYERS;
                case 18: return SPELLTABLEPARM_NUMBER;
     default: return DO_NOTHING;
     };
};

CanCastResult BSWScriptedAI::_BSWDoCast(uint8 m_uiSpellIdx, Unit* pTarget)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive())
        {
           error_log("BSW: FAILED casting bugged spell number %u - no target or target not in map",pSpell->m_uiSpellEntry[currentDifficulty]);
           return CAST_FAIL_OTHER;
        }

    debug_log("BSW: Casting bugged spell number %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

    pTarget->InterruptNonMeleeSpells(false);

    pTarget->CastSpell(pTarget, pSpell->m_uiSpellEntry[currentDifficulty], false);
         return CAST_OK;
};

void BSWScriptedAI::_fillEmptyDataField()
{
    for (uint8 i = 0; i < bossSpellCount(); ++i)
        for (uint8 j = 1; j < DIFFICULTY_LEVELS; ++j)
        {
            if (m_BossSpell[i].m_uiSpellEntry[j] == 0)
                   m_BossSpell[i].m_uiSpellEntry[j] = m_BossSpell[i].m_uiSpellEntry[j-1];

            if (m_BossSpell[i].m_uiSpellTimerMin[j] == 0)
                   m_BossSpell[i].m_uiSpellTimerMin[j] = m_BossSpell[i].m_uiSpellTimerMin[j-1];

            if (m_BossSpell[i].m_uiSpellTimerMax[j] == 0)
                   m_BossSpell[i].m_uiSpellTimerMax[j] = m_BossSpell[i].m_uiSpellTimerMax[j-1];

            if (m_BossSpell[i].m_uiSpellData[j] == 0)
                   m_BossSpell[i].m_uiSpellData[j] = m_BossSpell[i].m_uiSpellData[j-1];
        };
};

Unit* BSWScriptedAI::_doSummon(uint8 m_uiSpellIdx, TempSummonType summontype, uint32 delay)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    debug_log("BSW: Summoning creature number %u type %u despawn delay %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget, delay);

    if (pSpell->LocData.z <= 1.0f) 
    {
        float fPosX, fPosY, fPosZ;
        if (!m_creature->IsPositionValid())
            {
               error_log("BSW: FAILED summoning creature, creature %u has invalid position",m_creature->GetEntry());
               return NULL;
            }
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand((uint32)pSpell->LocData.x, (uint32)pSpell->LocData.y), fPosX, fPosY, fPosZ);
        return m_creature->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], fPosX, fPosY, fPosZ, 0, summontype, delay);
    }
    else return m_creature->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], pSpell->LocData.x, pSpell->LocData.y, pSpell->LocData.z, 0, summontype, delay);
};

Unit* BSWScriptedAI::_doSummonAtPosition(uint8 m_uiSpellIdx, TempSummonType summontype, uint32 delay, float fPosX, float fPosY, float fPosZ)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    debug_log("BSW: Summoning creature number %u type %u despawn delay %u at position %f %f %f",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget, delay, fPosX, fPosY, fPosZ);

    return m_creature->SummonCreature(pSpell->m_uiSpellEntry[currentDifficulty], fPosX, fPosY, fPosZ, 0, summontype, delay);
};

bool BSWScriptedAI::_doRemove(uint8 m_uiSpellIdx, Unit* pTarget, uint8 index)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    debug_log("BSW: Removing effects of spell %u type %u",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);

        switch (pSpell->m_CastTarget) 
        {
                case DO_NOTHING: 
                                 return true;
                case SUMMON_NORMAL:
                case SUMMON_TEMP:
                case SUMMON_INSTANT:
                                 return false;

                case CAST_ON_SELF:
                case APPLY_AURA_SELF:
                         pTarget = m_creature;
                     break;

                case CAST_ON_SUMMONS:
                case CAST_ON_VICTIM:
                case CAST_ON_BOTTOMAGGRO:
                case CAST_ON_TARGET:
                case APPLY_AURA_TARGET:
                         if (!pTarget) return false;
                     break;

                case CAST_ON_RANDOM:
                case CAST_ON_RANDOM_PLAYER:
                case APPLY_AURA_ALLPLAYERS:
                case CAST_ON_ALLPLAYERS:
                     {
                         Map::PlayerList const& pPlayers = pMap->GetPlayers();
                         for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                         {
                             pTarget = itr->getSource();
                             if (_hasAura(m_uiSpellIdx,pTarget))
                                 pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
                          }
                          return true;
                      }
                      break;
                  default: 
                      debug_log("BSW: FAILED Removing effects of spell %u type %u - unsupported type",pSpell->m_uiSpellEntry[currentDifficulty], pSpell->m_CastTarget);
                      return false;
        }

    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive())
        {
           error_log("BSW: FAILED removing effects of spell number %u - no target or target not in map or target is dead",pSpell->m_uiSpellEntry[currentDifficulty]);
           return false;
        }

        if (index == EFFECT_INDEX_ALL)
        {
            pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
            return true;
        }

        if (_auraCount(m_uiSpellIdx,pTarget,(SpellEffectIndex)index) > 1)
        {
            if (pTarget->GetAura(pSpell->m_uiSpellEntry[currentDifficulty],(SpellEffectIndex)index)->modStackAmount(-1))
                return true;
            else return false;
        }
        else pTarget->RemoveAurasDueToSpell(pSpell->m_uiSpellEntry[currentDifficulty]);
    return true;
};

bool BSWScriptedAI::_doAura(uint8 m_uiSpellIdx, Unit* pTarget, SpellEffectIndex index)
{
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];

    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive())
        {
           error_log("BSW: FAILED adding aura of spell number %u - no target or target not in map or target is dead",pSpell->m_uiSpellEntry[currentDifficulty]);
           return false;
        }

    SpellEntry const *spell;

    if (_hasAura(m_uiSpellIdx,pTarget))
         debug_log("BSW: adding aura stack from spell %u index %u",pSpell->m_uiSpellEntry[currentDifficulty], index);
    else debug_log("BSW: adding new aura from spell %u index %u",pSpell->m_uiSpellEntry[currentDifficulty], index);

    spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[currentDifficulty]);
    if (spell)
        {
            int32 basepoint = pSpell->varData ?  pSpell->varData - 1 : spell->EffectBasePoints[index] + 1;
            if (pTarget->AddAura(new BossAura(spell, index, &basepoint, pTarget, pTarget)))
                return true;
        };

    error_log("BSW: FAILED adding aura from spell %u index %u",pSpell->m_uiSpellEntry[currentDifficulty], index);

    return false;
};

// Copypasting from CreatureAI.cpp. if this called from m_creatureAI-> crashed :(

CanCastResult BSWScriptedAI::_CanCastSpell(Unit* pTarget, const SpellEntry *pSpell, bool isTriggered)
{
    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive()) return CAST_FAIL_OTHER;
    // If not triggered, we check
    if (!isTriggered)
    {
        // State does not allow
        if (m_creature->hasUnitState(UNIT_STAT_CAN_NOT_REACT))
            return CAST_FAIL_STATE;

        if (pSpell->PreventionType == SPELL_PREVENTION_TYPE_SILENCE && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED))
            return CAST_FAIL_STATE;

        if (pSpell->PreventionType == SPELL_PREVENTION_TYPE_PACIFY && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED))
            return CAST_FAIL_STATE;

        // Check for power (also done by Spell::CheckCast())
        if (m_creature->GetPower((Powers)pSpell->powerType) < pSpell->manaCost)
            return CAST_FAIL_POWER;
    }

    if (const SpellRangeEntry *pSpellRange = GetSpellRangeStore()->LookupEntry(pSpell->rangeIndex))
    {
        if (pTarget != m_creature)
        {
        if (!pTarget->IsPositionValid() || !m_creature->IsPositionValid()) return CAST_FAIL_OTHER;

            // pTarget is out of range of this spell (also done by Spell::CheckCast())
            float fDistance = m_creature->GetCombatDistance(pTarget);

            if (fDistance > (m_creature->IsHostileTo(pTarget) ? pSpellRange->maxRange : pSpellRange->maxRangeFriendly))
                return CAST_FAIL_TOO_FAR;

            float fMinRange = m_creature->IsHostileTo(pTarget) ? pSpellRange->minRange : pSpellRange->minRangeFriendly;

            if (fMinRange && fDistance < fMinRange)
                return CAST_FAIL_TOO_CLOSE;
        }

        return CAST_OK;
    }
    else
        return CAST_FAIL_OTHER;
}

CanCastResult BSWScriptedAI::_DoCastSpellIfCan(Unit* pTarget, uint32 uiSpell, uint32 uiCastFlags, uint64 uiOriginalCasterGUID)
{
    Unit* pCaster = m_creature;
    if (!pTarget || !pTarget->IsInMap(m_creature) || !pTarget->isAlive()) return CAST_FAIL_OTHER;

    if (uiCastFlags & CAST_FORCE_TARGET_SELF)
        pCaster = pTarget;

    // Allowed to cast only if not casting (unless we interrupt ourself) or if spell is triggered
    if (!pCaster->IsNonMeleeSpellCasted(false) || (uiCastFlags & (CAST_TRIGGERED | CAST_INTERRUPT_PREVIOUS)))
    {
        if (const SpellEntry* pSpell = GetSpellStore()->LookupEntry(uiSpell))
        {
            // If cast flag CAST_AURA_NOT_PRESENT is active, check if target already has aura on them
            if (uiCastFlags & CAST_AURA_NOT_PRESENT)
            {
                if (pTarget->HasAura(uiSpell))
                    return CAST_FAIL_TARGET_AURA;
            }

            // Check if cannot cast spell
            if (!(uiCastFlags & (CAST_FORCE_TARGET_SELF | CAST_FORCE_CAST)))
            {
                CanCastResult castResult = _CanCastSpell(pTarget, pSpell, uiCastFlags & CAST_TRIGGERED);

                if (castResult != CAST_OK)
                    return castResult;
            }

            // Interrupt any previous spell
            if (uiCastFlags & CAST_INTERRUPT_PREVIOUS && pCaster->IsNonMeleeSpellCasted(false))
                pCaster->InterruptNonMeleeSpells(false);

            pCaster->CastSpell(pTarget, pSpell, uiCastFlags & CAST_TRIGGERED, NULL, NULL, uiOriginalCasterGUID);
            return CAST_OK;
        }
        else
        {
            error_log("BSW: FAILED DoCastSpellIfCan: creature %u attempt to cast spell %u but spell does not exist.", m_creature->GetEntry(), uiSpell);
            return CAST_FAIL_OTHER;
        }
    }
    else
        return CAST_FAIL_IS_CASTING;
}

// Copypasting from sc_creature.cpp :( Hung if call from m_creatureAI->

Unit* BSWScriptedAI::doSelectLowHPFriendly(float fRange, uint32 uiMinHPDiff)
{
    CellPair p(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    Unit* pUnit = NULL;

    MaNGOS::MostHPMissingInRange u_check(m_creature, fRange, uiMinHPDiff);
    MaNGOS::UnitLastSearcher<MaNGOS::MostHPMissingInRange> searcher(m_creature, pUnit, u_check);

    TypeContainerVisitor<MaNGOS::UnitLastSearcher<MaNGOS::MostHPMissingInRange>, GridTypeMapContainer >  grid_unit_searcher(searcher);

    cell.Visit(p, grid_unit_searcher, *(pMap), *m_creature, fRange);

    return pUnit;
}

// Not threat-based select random player function

Unit* BSWScriptedAI::_doSelect(uint32 SpellID, bool spellsearchtype, float range)
{
    Map::PlayerList const &pList = pMap->GetPlayers();
          if (pList.isEmpty()) return NULL;

#if defined( __GNUC__ )
    Unit* _list[pMap->GetMaxPlayers()];
#else
    Unit* _list[INSTANCE_MAX_PLAYERS];
#endif 
    uint8 _count = 0;

    memset(&_list, 0, sizeof(_list));

          for(Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
          {
              if (Player* player = i->getSource())
              {
                  if (player->isGameMaster()) continue;

                  if (!player->IsInMap(m_creature)) continue;

                  if (player->isAlive()
                       && player->IsWithinDistInMap(m_creature, range)
                       && (SpellID == 0 || (player->HasAura(SpellID) == spellsearchtype))
                     )
                  {
                      _list[_count] = (Unit*)player;
                      ++_count;
                  }
              }
           }

    debug_log("BSW: search result for random player, count = %u ",_count);

    if (_count == 0) return NULL;

    else return _list[urand(0,_count)];
};

Creature* BSWScriptedAI::doSelectNearestCreature(uint32 guid, float range)
{
    Creature* pTarget = NULL;

    MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck u_check(*m_creature, guid, true, range*2);
    MaNGOS::CreatureLastSearcher<MaNGOS::NearestCreatureEntryWithLiveStateInObjectRangeCheck> searcher(m_creature, pTarget, u_check);
    Cell::VisitGridObjects(m_creature, searcher, range*2);


    if (pTarget && pTarget->IsInMap(m_creature) && pTarget != m_creature && pTarget->isAlive())
    {
        debug_log("BSW: search creature %u in range %f - found it.",guid,range);
        return pTarget;
    }
        else return NULL;
}

#endif