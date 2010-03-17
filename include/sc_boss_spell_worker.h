/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BOSS_SPELL_WORKER_H
#define DEF_BOSS_SPELL_WORKER_H
enum
{
  DIFFICULTY_LEVELS = 4,
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

struct WayPoints
{
    WayPoints(uint32 _id, float _x, float _y, float _z)
    {
        id = _id;
        x = _x;
        y = _y;
        z = _z;
    }
    uint32 id;
    float x, y, z;
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
  CAST_ON_SELF,
  CAST_ON_SUMMONS,
  CAST_ON_VICTIM,
  CAST_ON_RANDOM,
  CAST_ON_BOTTOMAGGRO,
  CAST_ON_TARGET,
  APPLY_AURA_SELF,
  APPLY_AURA_TARGET,
  SUMMON_NORMAL,
  SUMMON_INSTANT,
  SUMMON_TEMP,
  SPELLTABLEPARM_NUMBER
};



struct SpellTable
{
    uint32 id;
    uint32 m_uiSpellEntry[DIFFICULTY_LEVELS];          // Stores spell entry for difficulty levels
    uint32 m_uiSpellTimerMin[DIFFICULTY_LEVELS];       // The timer (min) before the next spell casting, in milliseconds
    uint32 m_uiSpellTimerMax[DIFFICULTY_LEVELS];       // The timer (max) before the next spell casting
    uint32 m_uiStageMask;                              // The mask of stages where spell is casted
    BossSpellTableParameters  m_CastTarget;            // Target on casting spell
    bool   m_IsVisualEffect;                           // Spellcasting is visual effect or real effect
    bool   m_IsBugged;                                 // Need override for this spell
};

class MANGOS_DLL_DECL BossAura : public Aura
{
    public:
        BossAura(const SpellEntry *spell, SpellEffectIndex eff, int32 *bp, Unit *target, Unit *caster) : Aura(spell, eff, bp, target, caster, NULL)
            {}
};

#endif
