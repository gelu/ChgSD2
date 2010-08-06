/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BOSS_SPELL_WORKER_H
#define DEF_BOSS_SPELL_WORKER_H

#include "precompiled.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellMgr.h"
#include "Unit.h"
#include "Database/DatabaseEnv.h"
#include "../ScriptMgr.h"


enum
{
  DIFFICULTY_LEVELS    = 4,
  MAX_BOSS_SPELLS      = 32,
  SPELL_INDEX_ERROR    = 255,
  INSTANCE_MAX_PLAYERS = 40,
  EFFECT_INDEX_ALL     = 255,
};

enum BossSpellFlag
{
  CAST_NORMAL,
  CAST_REMOVE,
  CAST_OVERRIDE,
  SPELLFLAG_NUMBER
};

enum BossSpellTableParameters
{
  DO_NOTHING                =  0,
  CAST_ON_SELF              =  1,
  CAST_ON_SUMMONS           =  2,
  CAST_ON_VICTIM            =  3,
  CAST_ON_RANDOM            =  4,
  CAST_ON_BOTTOMAGGRO       =  5,
  CAST_ON_TARGET            =  6,
  APPLY_AURA_SELF           =  7,
  APPLY_AURA_TARGET         =  8,
  SUMMON_NORMAL             =  9,
  SUMMON_INSTANT            = 10,
  SUMMON_TEMP               = 11,
  CAST_ON_ALLPLAYERS        = 12,
  CAST_ON_FRENDLY           = 13,
  CAST_ON_FRENDLY_LOWHP     = 14,
  CAST_ON_RANDOM_POINT      = 15,
  CAST_ON_RANDOM_PLAYER     = 16,
  APPLY_AURA_ALLPLAYERS     = 17,
  SPELLTABLEPARM_NUMBER
};

struct Locations
{
    float x, y, z;
    int32 id;
};

struct WayPoints
{
    WayPoints(int32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    int32 id;
    float x, y, z;
};

struct SpellTable
{
    uint32 id;
    uint32 m_uiSpellEntry[DIFFICULTY_LEVELS];          // Stores spell entry for difficulty levels
    uint32 m_uiSpellTimerMin[DIFFICULTY_LEVELS];       // The timer (min) before the next spell casting, in milliseconds
    uint32 m_uiSpellTimerMax[DIFFICULTY_LEVELS];       // The timer (max) before the next spell casting
    uint32 m_uiSpellData[DIFFICULTY_LEVELS];           // Additional data for spell casting or summon
    Locations LocData;                                 // Float data structure for locations
    int   varData;                                     // Additional data for spell
    uint32 StageMaskN;                                 // Stage mask for this spell (normal)
    uint32 StageMaskH;                                 // Stage mask for this spell (heroic)
    BossSpellTableParameters  m_CastTarget;            // Target on casting spell
    bool   m_IsVisualEffect;                           // Spellcasting is visual effect or real effect
    bool   m_IsBugged;                                 // Need override for this spell
    int32  textEntry;                                  // Text entry from script_text for this spell
};

struct MANGOS_DLL_DECL BSWScriptedAI : public ScriptedAI
{
    public:
        explicit BSWScriptedAI(Creature* pCreature);

        ~BSWScriptedAI();

        void doReset();

        void resetTimer(uint32 SpellID)
             {
             uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
             if (!queryIndex(m_uiSpellIdx)) return;
             return _resetTimer(m_uiSpellIdx);
             };

        void resetTimers()
             {
             for (uint8 i = 0; i < bossSpellCount(); ++i)
                  _resetTimer(i);
             };

        bool timedQuery(uint32 SpellID, uint32 diff)
             {
             uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
             if (!queryIndex(m_uiSpellIdx)) return false;
             return _QuerySpellPeriod(m_uiSpellIdx, diff);
             };

        CanCastResult timedCast(uint32 SpellID, uint32 diff, Unit* pTarget = NULL)
             {
             uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
             if (!queryIndex(m_uiSpellIdx)) return CAST_FAIL_OTHER;
             if (!_QuerySpellPeriod(_findSpellIDX(SpellID), diff)) return CAST_FAIL_STATE;
                  else return _BSWSpellSelector(m_uiSpellIdx, pTarget);
             };

        CanCastResult doCast(uint32 SpellID, Unit* pTarget = NULL)
             {
                  uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                  if (queryIndex(m_uiSpellIdx)) return _BSWSpellSelector(m_uiSpellIdx, pTarget);
                      else return CAST_FAIL_OTHER;
             };

        CanCastResult doCast(Unit* pTarget, uint32 SpellID)
             {
                  if (!pTarget) return CAST_FAIL_OTHER;
                  uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                  if (queryIndex(m_uiSpellIdx)) return _BSWCastOnTarget(pTarget, m_uiSpellIdx);
                      else return CAST_FAIL_OTHER;
             };

        bool doRemove(uint32 SpellID, Unit* pTarget = NULL, uint8 index = EFFECT_INDEX_ALL)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return false;
                     return _doRemove(m_uiSpellIdx,pTarget, index);
             };

        bool doRemoveFromAll(uint32 SpellID)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return false;
                     return _doRemoveFromAll(m_uiSpellIdx);
             };

        bool doAura(uint32 SpellID, Unit* pTarget = NULL, SpellEffectIndex index = EFFECT_INDEX_0)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return false;
                     return _doAura(m_uiSpellIdx,pTarget, index);
             };

        bool hasAura(uint32 SpellID, Unit* pTarget = NULL)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return false;
                 if (!pTarget) pTarget = m_creature;
                 return _hasAura(m_uiSpellIdx,pTarget);
             };

        uint8 auraCount(uint32 SpellID, Unit* pTarget = NULL, SpellEffectIndex index = EFFECT_INDEX_0)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return 0;
                 if (!pTarget) pTarget = m_creature;
                 return _auraCount(m_uiSpellIdx,pTarget,index);
             };

        Unit* doSummon(uint32 SpellID, TempSummonType type = TEMPSUMMON_CORPSE_TIMED_DESPAWN, uint32 delay = 60000)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return NULL;
                     return _doSummon(m_uiSpellIdx, type, delay);
             };

        Unit* doSelectRandomPlayer(uint32 SpellID = 0, bool spellsearchtype = false, float range = 100.0f)
             {
                 return _doSelect(SpellID, spellsearchtype, range);
             };

        Unit* doSelectRandomPlayerAtRange(float range)
             {
                 return _doSelect(0, false, range);
             };

        Unit* doSummon(uint32 SpellID, float fPosX, float fPosY, float fPosZ, TempSummonType type = TEMPSUMMON_CORPSE_TIMED_DESPAWN, uint32 delay = 60000)
             {
                 uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                 if (!queryIndex(m_uiSpellIdx)) return NULL;
                 return _doSummonAtPosition(m_uiSpellIdx, type, delay, fPosX, fPosY, fPosZ);
             };

        Unit* doSelectLowHPFriendly(float fRange = 40.0f, uint32 uiMinHPDiff = 0);

        uint8 bossSpellCount()
             {
                 return _bossSpellCount;
             };

        bool queryIndex(uint8 m_uiSpellIdx)
             {
                 if (    (m_uiSpellIdx >= 0)
                      && (m_uiSpellIdx <= bossSpellCount())
                      && (m_uiSpellIdx  != SPELL_INDEX_ERROR))
                      return true;
                 else return false;
             };

        Creature* doSelectNearestCreature(uint32 guid, float range = 120.0f);

        uint32 getSpellData(uint32 SpellID)
             {
                  uint8 m_uiSpellIdx = _findSpellIDX(SpellID);
                  return queryIndex(m_uiSpellIdx) ?  _getSpellData(m_uiSpellIdx) : 0;
             };

        bool doCastAll(uint32 diff);

        uint8 getStage() { return _stage; };

        void  setStage(uint8 stage) { _stage = stage; };

        bool isHeroic() { return pMap->IsRaid() ? _isDifficultyInMask(12) : _isDifficultyInMask(2); };

        bool isNormal() { return pMap->IsRaid() ? _isDifficultyInMask(3) : _isDifficultyInMask(1); };

        bool is25()     { return pMap->IsRaid() ? _isDifficultyInMask(10) : false; };

    protected:

        Map*          pMap;

        Difficulty    currentDifficulty;

    private:

        BossSpellTableParameters _getBSWCastType(uint32 pTemp);

        uint8         _findSpellIDX(uint32 SpellID);

        void          _loadSpellTable();

        void          _resetTimer(uint8 m_uiSpellIdx);

        Unit*         _doSelect(uint32 SpellID, bool spellsearchtype = false, float range = 100.0f);

        Unit*         _doSummon(uint8 m_uiSpellIdx, TempSummonType type = TEMPSUMMON_CORPSE_TIMED_DESPAWN, uint32 delay = 60000);

        Unit*         _doSummonAtPosition(uint8 m_uiSpellIdx, TempSummonType type, uint32 delay, float fPosX, float fPosY, float fPosZ);

        CanCastResult _BSWDoCast(uint8 m_uiSpellIdx, Unit* pTarget);

        CanCastResult _BSWSpellSelector(uint8 m_uiSpellIdx, Unit* pTarget = NULL);

        CanCastResult _BSWCastOnTarget(Unit* pTarget, uint8 m_uiSpellIdx);

        bool          _QuerySpellPeriod(uint8 m_uiSpellIdx, uint32 diff);

        CanCastResult _DoCastSpellIfCan(Unit* pTarget, uint32 uiSpell, uint32 uiCastFlags = 0, uint64 uiOriginalCasterGUID = 0);

        CanCastResult _CanCastSpell(Unit* pTarget, const SpellEntry *pSpell, bool isTriggered = false);

        bool          _doRemove(uint8 m_uiSpellIdx, Unit* pTarget = NULL, uint8 index = EFFECT_INDEX_ALL);

        bool          _doRemoveFromAll(uint8 m_uiSpellIdx);

        bool          _doAura(uint8 m_uiSpellIdx, Unit* pTarget = NULL, SpellEffectIndex index = EFFECT_INDEX_0);

        bool          _hasAura(uint8 m_uiSpellIdx, Unit* pTarget);

        uint8         _auraCount(uint8 m_uiSpellIdx, Unit* pTarget = NULL, SpellEffectIndex index = EFFECT_INDEX_0);

        void          _fillEmptyDataField();

        uint32        _getSpellData(uint8 m_uiSpellIdx);

        bool          _isDifficultyInMask(uint8 mask);

// Constants
        uint8         _bossSpellCount;
        uint8         _stage;
        uint32        m_uiSpell_Timer[MAX_BOSS_SPELLS];
        SpellTable    m_BossSpell[MAX_BOSS_SPELLS];
};

#endif