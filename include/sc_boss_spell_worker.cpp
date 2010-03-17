/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifdef DEF_BOSS_SPELL_WORKER_H

    bool QuerySpellPeriod(uint32 m_uiSpellIdx, uint32 diff)
    {
    if(!m_pInstance) return false;
    bool result;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        if (m_uiSpellIdx != pSpell->id) return false;

        if (m_uiSpell_Timer[m_uiSpellIdx] < diff) {
            m_uiSpell_Timer[m_uiSpellIdx]=urand(pSpell->m_uiSpellTimerMin[Difficulty],pSpell->m_uiSpellTimerMax[Difficulty]);
            result = true;
            } else {
            m_uiSpell_Timer[m_uiSpellIdx] -= diff;
            result = false;
            };
        return result;
    }

    CanCastResult CastBossSpell(uint32 m_uiSpellIdx, BossSpellFlag spellfag = CAST_NORMAL)
    {
    if(!m_pInstance) return CAST_FAIL_OTHER;
    Unit* pTarget = NULL;
    Creature* pSummon = NULL;
    SpellEntry const *spell;
    bool isSummon = false;
    float fPosX, fPosY, fPosZ;
    int32 bp = 8;
    SpellTable* pSpell = &m_BossSpell[m_uiSpellIdx];
        // Find spell index - temporary direct insert from spelltable
        if (m_uiSpellIdx != pSpell->id) return CAST_FAIL_OTHER;

        switch (pSpell->m_CastTarget) {
            case CAST_ON_SELF:
                   pTarget = m_creature;
                   break;
            case CAST_ON_SUMMONS:
                   pTarget = m_creature->getVictim(); //CHANGE IT!!!
                   break;
            case CAST_ON_VICTIM:
                   pTarget = m_creature->getVictim();
                   break;
            case CAST_ON_TARGET:
                   pTarget = currentTarget;
                   if (!pTarget) pTarget = m_creature->getVictim();
                   break;
            case CAST_ON_RANDOM:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);
                   break;
            case CAST_ON_BOTTOMAGGRO:
                   pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1);
                   break;
            case APPLY_AURA_SELF:
                   if (spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[Difficulty]))
                          m_creature->AddAura(new BossAura(spell, EFFECT_INDEX_0, &bp, m_creature, m_creature));
                              return CAST_OK;
                   break;

            case APPLY_AURA_TARGET:
                   pTarget = currentTarget;
                   if (!pTarget) pTarget = m_creature->getVictim();
                   if (spell = (SpellEntry *)GetSpellStore()->LookupEntry(pSpell->m_uiSpellEntry[Difficulty]))
                          pTarget->AddAura(new BossAura(spell, EFFECT_INDEX_0, &bp, pTarget, pTarget));
                              return CAST_OK;
                   break;

            case SUMMON_NORMAL:
                   isSummon = true;
                   m_creature->GetPosition(fPosX, fPosY, fPosZ);
                   m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(30, 60), fPosX, fPosY, fPosZ);
                   pSummon = m_creature->SummonCreature(pSpell->m_uiSpellEntry[Difficulty], fPosX, fPosY, fPosZ, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
                   if(pSummon) pSummon->SetInCombatWithZone();
                   break;

            case SUMMON_TEMP:
                   isSummon = true;
                   m_creature->GetPosition(fPosX, fPosY, fPosZ);
                   m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(30, 60), fPosX, fPosY, fPosZ);
                   pSummon = m_creature->SummonCreature(pSpell->m_uiSpellEntry[Difficulty], fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,
                              urand(pSpell->m_uiSpellTimerMin[Difficulty],pSpell->m_uiSpellTimerMax[Difficulty]));
                   if(pSummon) pSummon->SetInCombatWithZone();
                   break;

            case SUMMON_INSTANT:
                   isSummon = true;
                   m_creature->GetPosition(fPosX, fPosY, fPosZ);
                   m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(30, 60), fPosX, fPosY, fPosZ);
                   pSummon = m_creature->SummonCreature(pSpell->m_uiSpellEntry[Difficulty], fPosX, fPosY, fPosZ, 0, TEMPSUMMON_MANUAL_DESPAWN,0);
                   break;

            default:
                   return CAST_FAIL_OTHER;
            };
            currentTarget = pTarget;
            if (pTarget && !isSummon && !pSpell->m_IsBugged) return DoCastSpellIfCan(pTarget,pSpell->m_uiSpellEntry[Difficulty]);
                else if (pTarget && !isSummon && pSpell->m_IsBugged) {DoCast(pTarget,pSpell->m_uiSpellEntry[Difficulty]); return CAST_OK;}
                     else if (isSummon) return pSummon ?  CAST_OK :  CAST_FAIL_OTHER;
            return CAST_FAIL_OTHER;
    }

#endif