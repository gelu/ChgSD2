/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Hall Of Reflection
SD%Complete: Who knows? :D
SDComment: event script!
SDErrors: They have, but i dont know were it! :D
SDCategory: hall_of_reflection
SDAuthor: MaxXx2021 aka Mioka
EndScriptData */

#include "precompiled.h"
#include "def_halls.h"
#include "escort_ai.h"

enum
{
  /*INTRO - Pre Uther*/
  SAY_JAINA_INTRO_01                 = -1594433,
  SAY_SYLVANA_INTRO_01               = -1594434,
  SAY_JAINA_INTRO_02                 = -1594435,
  SAY_JAINA_INTRO_03                 = -1594436,
  SAY_SYLVANA_INTRO_03               = -1594437,
  SAY_JAINA_INTRO_04                 = -1594438,
  SAY_SYLVANA_INTRO_04               = -1594439,

  /*INTRO - Uther Dialog*/
  SAY_UTHER_A_01                     = -1594440, 
  SAY_UTHER_H_01                     = -1594441,
  SAY_JAINA_02                       = -1594442,
  SAY_SYLVANA_02                     = -1594443,
  SAY_UTHER_A_03                     = -1594444,
  SAY_UTHER_H_03                     = -1594445,
  SAY_JAINA_04                       = -1594446,
  SAY_SYLVANA_04                     = -1594447,
  SAY_UTHER_A_05                     = -1594448,
  SAY_UTHER_H_05                     = -1594449,
  SAY_JAINA_06                       = -1594450,
  SAY_SYLVANA_06                     = -1594451,
  SAY_UTHER_A_07                     = -1594452,
  SAY_UTHER_H_07                     = -1594453,
  SAY_JAINA_08                       = -1594454,
  SAY_SYLVANA_08                     = -1594455,
  SAY_UTHER_A_09                     = -1594456,
  SAY_UTHER_H_09                     = -1594457,
  SAY_JAINA_10                       = -1594458,
  SAY_UTHER_A_11                     = -1594459,
  SAY_UTHER_H_11                     = -1594460,
  SAY_JAINA_12                       = -1594461,
  SAY_SYLVANA_12                     = -1594462,
  SAY_UTHER_A_13                     = -1594463,
  SAY_UTHER_A_14                     = -1594464,
  SAY_JAINA_15                       = -1594465,

  /*INTRO - Lich King Arrive*/
  SAY_UTHER_A_16                     = -1594466,
  SAY_UTHER_H_16                     = -1594467,
  SAY_LICH_KING_17                   = -1594468,
  SAY_LICH_KING_18                   = -1594469,
  SAY_LICH_KING_19                   = -1594470,
  SAY_JAINA_20                       = -1594471,
  SAY_SYLVANA_20                     = -1594472,
  SAY_LICH_KING_A_21                 = -1594473,
  SAY_LICH_KING_H_21                 = -1594474,
  SAY_FALRIC_INTRO                   = -1594475,
  SAY_MARWYN_INTRO                   = -1594476,
  SAY_FALRIC_INTRO2                  = -1594505,

  /*INTRO - Pre Escape*/
  SAY_LICH_KING_AGGRO_A              = -1594477,
  SAY_LICH_KING_AGGRO_H              = -1594478,
  SAY_JAINA_AGGRO                    = -1594479,
  SAY_SYLVANA_AGGRO                  = -1594480,

  /*ESCAPE*/
  SAY_JAINA_WALL_01                  = -1594487,
  SAY_SYLVANA_WALL_01                = -1594488,
  SAY_JAINA_WALL_02                  = -1594489,
  SAY_SYLVANA_WALL_02                = -1594490,
  SAY_LICH_KING_WALL_02              = -1594491,
  SAY_LICH_KING_WALL_03              = -1594492,
  SAY_LICH_KING_WALL_04              = -1594493,
  SAY_JAINA_WALL_03                  = -1594494,
  SAY_JAINA_WALL_04                  = -1594495,
  SAY_SYLVANA_WALL_03                = -1594496,
  SAY_SYLVANA_WALL_04                = -1594497,
  SAY_JAINA_ESCAPE_01                = -1594498,
  SAY_JAINA_ESCAPE_02                = -1594499,
  SAY_SYLVANA_ESCAPE_01              = -1594500,
  SAY_SYLVANA_ESCAPE_02              = -1594501,
  SAY_JAINA_TRAP                     = -1594502,
  SAY_SYLVANA_TRAP                   = -1594503,
  SAY_LICH_KING_END_01               = -1594506,
  SAY_LICH_KING_END_02               = -1594507,
  SAY_LICH_KING_END_03               = -1594508,

  SAY_ESCAPE_01                      = -1594531,
  SAY_ESCAPE_02                      = -1594532,
  SAY_ESCAPE_03                      = -1594533,


  /*SPELLS AND VISUAL EFFECTS*/
  SPELL_TAKE_FROSTMOURNE             = 72729,
  SPELL_FROSTMOURNE_DESPAWN          = 72726,
  SPELL_FROSTMOURNE_SOUNDS           = 70667,
  SPELL_CAST_VISUAL                  = 65633,  //Jaina And Sylavana cast this when summon uther.
  SPELL_BOSS_SPAWN_AURA              = 72712,  //Falric and Marwyn
  SPELL_UTHER_DESPAWN                = 70693,
  SPELL_WINTER                       = 69780,
  SPELL_FURY_OF_FROSTMOURNE          = 70063,
  SPELL_SOUL_REAPER                  = 73797,
  SPELL_RAISE_DEAD                   = 69818,
  SPELL_ICE_PRISON                   = 69708,
  SPELL_DARK_ARROW                   = 70194,
  SPELL_ICE_BARRIER                  = 69787,
  SPELL_DESTROY_ICE_WALL_01          = 69784, //Jaina
  SPELL_DESTROY_ICE_WALL_02          = 70224,
  SPELL_DESTROY_ICE_WALL_03          = 70225, //Sylvana
  SPELL_DESTRUCT_ICE_WALL            = 69784,
  SPELL_SUMMON_ICE_WALL              = 69768,
  SPELL_SYLVANA_JUMP                 = 68339,
  SPELL_SYLVANA_STEP                 = 69087,
  SPELL_SILENCE                      = 69413,
  SPELL_LICH_KING_CAST               = 57561,
  SPELL_FROSTMOURNE_VISUAL           = 73220,
  SPELL_SHIELD_DISRUPTION            = 58291,

  FACTION                            = 2076,
};

static _Locations WallLoc[]=
{
    {5540.39f, 2086.48f, 731.066f, 1.00057f},
    {5494.3f, 1978.27f, 736.689f, 1.0885f},
    {5434.27f, 1881.12f, 751.303f, 0.923328f},
    {5323.61f, 1755.85f, 770.305f, 0.784186f},
    {5239.01f, 1932.64f, 707.695f, 0.8f},       // Spawn point for Jaina && Silvana intro
    {5266.779785f, 1953.42f, 707.697f, 1.0f},
};


struct MANGOS_DLL_DECL npc_jaina_and_sylvana_HRintroAI : public ScriptedAI
{
    npc_jaina_and_sylvana_HRintroAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
   }

   ScriptedInstance* m_pInstance;

   uint32 StepTimer;
   uint32 Step;
   uint64 m_uiFrostmourneGUID;
   uint64 m_uiMainGateGUID;
   uint64 m_uiFalricGUID;
   uint64 m_uiMarwynGUID;
   Creature* pUther;
   Creature* pLichKing;
   bool Small;

   void Reset()
   {
      m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
      m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
      Small = false;
   }

   void StartEvent()
   {
      if(!m_pInstance) return
      debug_log("EventMGR: creature %u received signal %u ",m_creature->GetEntry(),m_pInstance->GetData(TYPE_EVENT));
         m_pInstance->SetData(TYPE_PHASE, 1);
         m_pInstance->SetData(TYPE_EVENT, 0);
      Step = 1;
      StepTimer = 100;
   }

   void JumpNextStep(uint32 Time)
   {
      StepTimer = Time;
      Step++;
   }

   void Event()
   {
         switch(Step)
         {
            case 1:
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                JumpNextStep(2000);
                break;
            case 2:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_INTRO_01, m_creature);
                   JumpNextStep(5000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_INTRO_01, m_creature);
                   JumpNextStep(8000);
                }
                break;
            case 3: 
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_INTRO_02, m_creature);
                   JumpNextStep(5000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                   JumpNextStep(500);
                break;
            case 4:
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                m_creature->GetMotionMaster()->MovePoint(0, 5307.031f, 1997.920f, 709.341f);
                JumpNextStep(10000);
                break;
            case 5:
                if(Creature* pTarget = m_creature->SummonCreature(NPC_ALTAR_TARGET,5309.374f,2006.788f,711.615f,1.37f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                {
                   m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pTarget->GetGUID());
                   pTarget->SetCreatorGuid(ObjectGuid());
                }
                JumpNextStep(1000);
                break;
            case 6:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_INTRO_03, m_creature);
                   JumpNextStep(5000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_INTRO_03, m_creature);
                   JumpNextStep(6000);
                }
                break;
            case 7:
                DoCast(m_creature, SPELL_CAST_VISUAL);
                if(m_creature->GetEntry() == NPC_JAINA)
                   DoScriptText(SAY_JAINA_INTRO_04, m_creature);
                if(m_creature->GetEntry() == NPC_SYLVANA)
                   DoScriptText(SAY_SYLVANA_INTRO_04, m_creature);
                JumpNextStep(3000);
                break;
            case 8:
                DoCast(m_creature, SPELL_FROSTMOURNE_SOUNDS);
                m_uiFrostmourneGUID = m_pInstance->GetData64(GO_FROSTMOURNE);
                if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                   pFrostmourne->SetGoState(GO_STATE_ACTIVE);
                if(m_creature->GetEntry() == NPC_JAINA)
                   JumpNextStep(12000);
                if(m_creature->GetEntry() == NPC_SYLVANA)
                   JumpNextStep(8000);
                break;
            case 9:
                if(Creature* Uther = m_creature->SummonCreature(NPC_UTHER,5308.228f,2003.641f,709.341f,4.17f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                {
                   pUther = Uther;
                   Uther->SetCreatorGuid(ObjectGuid());
                   Uther->SetUInt64Value(UNIT_FIELD_TARGET, m_creature->GetGUID());
                   m_creature->SetUInt64Value(UNIT_FIELD_TARGET, Uther->GetGUID());
                   if(m_creature->GetEntry() == NPC_JAINA)
                   {
                       DoScriptText(SAY_UTHER_A_01, Uther);
                       JumpNextStep(3000);
                   }
                   if(m_creature->GetEntry() == NPC_SYLVANA)
                   {
                       DoScriptText(SAY_UTHER_H_01, Uther);
                       JumpNextStep(10000);
                   }
                }
                break;
            case 10:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_02, m_creature);
                   JumpNextStep(5000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_02, m_creature);
                   JumpNextStep(3000);
                }
                break;
            case 11:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_03, pUther);
                   JumpNextStep(7000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                {
                   DoScriptText(SAY_UTHER_H_03, pUther);
                   JumpNextStep(6000);
                }
                if(Small)
                   Step = 24;
                break;
            case 12:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_04, m_creature);
                   JumpNextStep(2000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_04, m_creature);
                   JumpNextStep(5000);
                }
                break;
            case 13:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_05, pUther);
                   JumpNextStep(10000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                {
                   DoScriptText(SAY_UTHER_H_05, pUther); 
                   JumpNextStep(19000);
                }
                break;
            case 14:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_06, m_creature);
                   JumpNextStep(6000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_06, m_creature); 
                   JumpNextStep(2000);
                }
                break;
            case 15:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_07, pUther); 
                   JumpNextStep(12000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                {
                   DoScriptText(SAY_UTHER_H_07, pUther);
                   JumpNextStep(20000);
                }
                break;
            case 16:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_08, m_creature); 
                   JumpNextStep(6000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_08, m_creature);
                   JumpNextStep(3000);
                }
                break;
            case 17:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_09, pUther);
                   JumpNextStep(12000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                {
                   DoScriptText(SAY_UTHER_H_09, pUther);
                   JumpNextStep(11000);
                }
                break;
            case 18:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_10, m_creature);
                   JumpNextStep(11000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   JumpNextStep(100);
                }
                break;
            case 19:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_11, pUther);
                   JumpNextStep(24000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                {
                   DoScriptText(SAY_UTHER_H_11, pUther);
                   JumpNextStep(9000);
                }
                break;
            case 20:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_12, m_creature);
                   JumpNextStep(2000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   DoScriptText(SAY_SYLVANA_12, m_creature);
                   JumpNextStep(2100);
                }
                break;
            case 21:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_13, pUther); 
                   JumpNextStep(5000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   JumpNextStep(100);
                }
                break;
            case 22:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                {
                   DoScriptText(SAY_UTHER_A_14, pUther); 
                   JumpNextStep(12000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   JumpNextStep(100);
                }
                break;
            case 23:
                if(m_creature->GetEntry() == NPC_JAINA)
                {
                   DoScriptText(SAY_JAINA_15, m_creature);
                   JumpNextStep(2000);
                }
                if(m_creature->GetEntry() == NPC_SYLVANA)
                {
                   JumpNextStep(100);
                }
                break;
            case 24:
                if(m_creature->GetEntry() == NPC_JAINA && pUther)
                   DoScriptText(SAY_UTHER_A_16, pUther);
                if(m_creature->GetEntry() == NPC_SYLVANA && pUther)
                   DoScriptText(SAY_UTHER_H_16, pUther);
                m_uiMainGateGUID = m_pInstance->GetData64(GO_IMPENETRABLE_DOOR);
                if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                   pGate->SetGoState(GO_STATE_ACTIVE);
                if(Creature* LichKing = m_creature->SummonCreature(NPC_LICH_KING,5362.469f,2062.342f,707.695f,3.97f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,360000))
                {
                   LichKing->SetCreatorGuid(ObjectGuid());
                   pLichKing = LichKing;
                   LichKing->SetActiveObjectState(true);
                }
                JumpNextStep(1000);
                break;
            case 25:
                if(pUther)
                   pUther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                if(pLichKing)
                {
                   pLichKing->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                   pLichKing->GetMotionMaster()->MovePoint(0, 5314.881f, 2012.496f, 709.341f);
                }
                JumpNextStep(3000);
                break;
            case 26:
                if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                   pGate->SetGoState(GO_STATE_READY);
                JumpNextStep(7000);
                break;
            case 27:
                if(pUther)
                   pUther->CastSpell(pUther, SPELL_UTHER_DESPAWN, false);
                JumpNextStep(500);
                break;
            case 28:
                if(pLichKing)
                   DoScriptText(SAY_LICH_KING_17, pLichKing);
                JumpNextStep(10000);
                break;
            case 29:
                if(pLichKing)
                   DoScriptText(SAY_LICH_KING_18, pLichKing);
                JumpNextStep(5000);
                break;
            case 30:
                if(pLichKing)
                   pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, false);
                if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                   pFrostmourne->SetGoState(GO_STATE_READY);
                JumpNextStep(1500);
                break;
            case 31:
                if(GameObject* pFrostmourne = m_pInstance->instance->GetGameObject(m_uiFrostmourneGUID))
                   pFrostmourne->SetPhaseMask(0, true);
                if(pLichKing)
                   pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, false);
                   //pLichKing->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, uint32(36942));
                m_creature->RemoveAurasDueToSpell(SPELL_FROSTMOURNE_SOUNDS);
                JumpNextStep(5000);
                break;
            case 32:
                if(pLichKing)
                   DoScriptText(SAY_LICH_KING_19, pLichKing);
                JumpNextStep(9000);
                break;
            case 33:
                m_uiFalricGUID = m_pInstance->GetData64(NPC_FALRIC);
                m_uiMarwynGUID = m_pInstance->GetData64(NPC_MARWYN);
                if(Creature* Falric = (m_creature->GetMap()->GetCreature( m_uiFalricGUID)))
                { 
                   Falric->SetVisibility(VISIBILITY_ON);
                   Falric->CastSpell(Falric, SPELL_BOSS_SPAWN_AURA, false);
                   Falric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                }
                if(Creature* Marwyn = (m_creature->GetMap()->GetCreature( m_uiMarwynGUID)))
                {
                   Marwyn->SetVisibility(VISIBILITY_ON);
                   Marwyn->CastSpell(Marwyn, SPELL_BOSS_SPAWN_AURA, false);
                   Marwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                }
                if(pLichKing)
                   pLichKing->GetMotionMaster()->MovePoint(0, 5402.286f, 2104.496f, 707.695f);
                JumpNextStep(600);
                break;
            case 34:
                if(Creature* Falric = (m_creature->GetMap()->GetCreature( m_uiFalricGUID)))
                   DoScriptText(SAY_FALRIC_INTRO, Falric);
                if(Creature* Marwyn = (m_creature->GetMap()->GetCreature( m_uiMarwynGUID)))
                   DoScriptText(SAY_MARWYN_INTRO, Marwyn);
                JumpNextStep(3000);
                break;
            case 35:
                if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                   pGate->SetGoState(GO_STATE_ACTIVE);
                if(Creature* Falric = (m_creature->GetMap()->GetCreature( m_uiFalricGUID)))
                   DoScriptText(SAY_FALRIC_INTRO2, Falric);
                m_pInstance->SetData(TYPE_FALRIC, SPECIAL);
                JumpNextStep(4000);
                break;
            case 36:
                if(m_creature->GetEntry() == NPC_JAINA)
                   DoScriptText(SAY_JAINA_20, m_creature);
                if(m_creature->GetEntry() == NPC_SYLVANA)
                   DoScriptText(SAY_SYLVANA_20, m_creature);
                m_creature->GetMotionMaster()->MovePoint(0, 5443.880f, 2147.095f, 707.695f);
                JumpNextStep(4000);
                break;
            case 37:
                if(pLichKing)
                {
                   pLichKing->GetMotionMaster()->MovementExpired(false);
                   pLichKing->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                   pLichKing->GetMotionMaster()->MovePoint(0, 5443.880f, 2147.095f, 707.695f);
                }
                if(m_creature->GetEntry() == NPC_JAINA && pLichKing)
                   DoScriptText(SAY_LICH_KING_A_21, pLichKing);
                if(m_creature->GetEntry() == NPC_SYLVANA && pLichKing)
                   DoScriptText(SAY_LICH_KING_H_21, pLichKing);
                JumpNextStep(8000);
                break;
            case 38:
                if(GameObject* pGate = m_pInstance->instance->GetGameObject(m_uiMainGateGUID))
                   pGate->SetGoState(GO_STATE_READY); 
                JumpNextStep(5000);
                break;
            case 39:
                m_creature->SetVisibility(VISIBILITY_OFF);
                if(pLichKing)
                   pLichKing->SetVisibility(VISIBILITY_OFF);
                m_pInstance->SetData(TYPE_PHASE, 2);
                JumpNextStep(1000);
                break;
         }
    }

    void UpdateAI(const uint32 diff)
    {
         if(!m_pInstance)
             return;

        if(m_pInstance->GetData(TYPE_EVENT) == 1
           &&  m_pInstance->GetData64(DATA_ESCAPE_LIDER) == m_creature->GetGUID())
           StartEvent();

        if(m_pInstance->GetData(TYPE_EVENT) == 2
           &&  m_pInstance->GetData64(DATA_ESCAPE_LIDER) == m_creature->GetGUID())
        {
            Small = true;
            StartEvent();
        }

         if(StepTimer < diff && m_pInstance->GetData(TYPE_PHASE) == 1)
            Event();
         else StepTimer -= diff;

         return;
     }
};

bool GossipHello_npc_jaina_and_sylvana_HRintro(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if(pCreature->isQuestGiver())
       pPlayer->PrepareQuestMenu( pCreature->GetGUID());

    switch(pCreature->GetEntry())
    {
       case NPC_JAINA:
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3594536, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3594537, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            break;
       case NPC_SYLVANA: 
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3594538, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3594539, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            break;
    }

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_jaina_and_sylvana_HRintro(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!m_pInstance) return false;

    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
               pPlayer->CLOSE_GOSSIP_MENU();
               m_pInstance->SetData(TYPE_EVENT, 1);
               break;
        case GOSSIP_ACTION_INFO_DEF+2:
               pPlayer->CLOSE_GOSSIP_MENU();
               m_pInstance->SetData(TYPE_EVENT, 2);
               break;
    }

    if(pPlayer->GetTeam() == ALLIANCE)
            m_pInstance->SetData(DATA_LIDER, 1);
       else m_pInstance->SetData(DATA_LIDER, 2);

    m_pInstance->SetData64(DATA_ESCAPE_LIDER,pCreature->GetGUID());

    return true;
}

struct MANGOS_DLL_DECL npc_jaina_and_sylvana_HRextroAI : public npc_escortAI
{
   npc_jaina_and_sylvana_HRextroAI(Creature *pCreature) : npc_escortAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
   }

   ScriptedInstance* m_pInstance;

   uint32 CastTimer;
   uint32 StepTimer;
   uint32 Step;
   int32 HoldTimer;
   uint32 Count;
   bool Fight;
   bool Event;
   bool PreFight;
   bool WallCast;
   uint64 m_uiLichKingGUID;
   uint64 m_uiLiderGUID;
   ObjectGuid wallTarget;
   Creature* pLichKing;
   uint32    m_chestID;

   void Reset()
   {
       if(!m_pInstance) return;

       if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS) return;

       Step = 0;
       StepTimer = 500;
       Fight = true;
       wallTarget = ObjectGuid();
       m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
       m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

       if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
       {
          m_creature->CastSpell(m_creature, SPELL_ICE_BARRIER, false);
          m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
       }
       if(m_pInstance->GetData(TYPE_LICH_KING) == DONE)
          m_creature->SetVisibility(VISIBILITY_OFF);

    }

    void AttackStart(Unit* who)
    {
       if(!who)
           return;

       if(m_creature->GetEntry() != NPC_SYLVANA_OUTRO) return;

       if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS || Fight != true) return;

       npc_escortAI::AttackStart(who);

    }

    void JustDied(Unit* pKiller)
    {
        if(!m_pInstance) return;
        m_pInstance->SetData(TYPE_LICH_KING, FAIL);
    }

    void DoSummonWall(uint8 wallNum)
    {
        if(!m_pInstance || wallNum > 3)
            return;

        switch (wallNum)
        {
            case 0: m_pInstance->SetData(TYPE_ICE_WALL_01, IN_PROGRESS); break;
            case 1: m_pInstance->SetData(TYPE_ICE_WALL_02, IN_PROGRESS); break;
            case 2: m_pInstance->SetData(TYPE_ICE_WALL_03, IN_PROGRESS); break;
            case 3: m_pInstance->SetData(TYPE_ICE_WALL_04, IN_PROGRESS); break;
            default: break;
        }

        if (Creature* pWallTarget = m_creature->SummonCreature(NPC_ICE_WALL,WallLoc[wallNum].x,WallLoc[wallNum].y,WallLoc[wallNum].z,WallLoc[wallNum].o,TEMPSUMMON_MANUAL_DESPAWN,0, true))
        {
            pWallTarget->SetPhaseMask(65535, true);
            pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pWallTarget->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            wallTarget = pWallTarget->GetObjectGuid();
            pWallTarget->CastSpell(pWallTarget, SPELL_SUMMON_ICE_WALL, false);
        }
    }

    void DoDestructWall(uint8 wallNum)
    {
        if(!m_pInstance || wallNum > 3 )
            return;

        switch (wallNum)
        {
            case 0: m_pInstance->SetData(TYPE_ICE_WALL_01, DONE); break;
            case 1: m_pInstance->SetData(TYPE_ICE_WALL_02, DONE); break;
            case 2: m_pInstance->SetData(TYPE_ICE_WALL_03, DONE); break;
            case 3: m_pInstance->SetData(TYPE_ICE_WALL_04, DONE); break;
            default: break;
        }

        if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
        {
            pWallTarget->ForcedDespawn();
        }
        wallTarget =  ObjectGuid();
    }

    void WaypointReached(uint32 i)
    {
        switch(i)
        {
            case 2:
                DoSummonWall(0);
                break;
            case 4:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_JAINA_WALL_01, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_SYLVANA_WALL_01, m_creature);
                CastTimer = 1000;
                SetEscortPaused(true);
                if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                }
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_02, false);
                }
                WallCast = true;
                break;
            case 6:
                DoSummonWall(1);
                break;
            case 8:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_JAINA_WALL_02, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_SYLVANA_WALL_02, m_creature);
                CastTimer = 1000;
                SetEscortPaused(true);
                if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                }
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_02, false);
                }
                WallCast = true;
                break;
            case 9:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                   DoScriptText(SAY_JAINA_ESCAPE_01, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                   DoScriptText(SAY_SYLVANA_ESCAPE_01, m_creature);
                DoSummonWall(2);
                break;
            case 11:
                break;
            case 12:
                if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                   DoScriptText(SAY_JAINA_WALL_03, m_creature);
                if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                   DoScriptText(SAY_SYLVANA_WALL_03, m_creature);
                CastTimer = 1000;
                SetEscortPaused(true);
                if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                }
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_02, false);
                }
                WallCast = true;
                break;
            case 13:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_JAINA_ESCAPE_02, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_SYLVANA_ESCAPE_02, m_creature);
                DoSummonWall(3);
                break;
            case 15:
                break;
            case 16:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_JAINA_WALL_04, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_SYLVANA_WALL_04, m_creature);
                CastTimer = 1000;
                SetEscortPaused(true);
                if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_01, false);
                }
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                {
                    if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
                        m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_02, false);
                }
                WallCast = true;
                break;
            case 19:
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_JAINA_TRAP, m_creature);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_SYLVANA_TRAP, m_creature);
                break;
            case 20:
                SetEscortPaused(true);
                if (m_creature->GetEntry() == NPC_JAINA_OUTRO)
                   m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
                else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                   m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                break;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
       if(!who || !m_pInstance)
           return;

       if(who->GetTypeId() != TYPEID_PLAYER) return;

       Player* pPlayer = (Player *)who;

       if(pPlayer->GetTeam() == ALLIANCE && m_creature->GetEntry() == NPC_SYLVANA_OUTRO) return;

       if(pPlayer->GetTeam() == HORDE && m_creature->GetEntry() == NPC_JAINA_OUTRO) return;

       if(m_creature->IsWithinDistInMap(who, 50.0f)
          && m_pInstance->GetData(TYPE_FROST_GENERAL) == DONE
          && m_pInstance->GetData(TYPE_PHASE) == 3)
       {
          pPlayer = (Player *)who;
          Event = true;
          m_creature->setFaction(FACTION);
          m_pInstance->SetData(TYPE_PHASE, 4);
       }
   }

   void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
   {
        if(!m_pInstance) return;

        if(m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
        {
          uiDamage = 0;
          return;
        }

        if(m_pInstance->GetData(TYPE_LICH_KING) == IN_PROGRESS && WallCast == true)
        {
          HoldTimer = HoldTimer + 100;
          return;
        }
   }

   void JumpNextStep(uint32 Time)
   {
      StepTimer = Time;
      Step++;
   }

   void Intro()
   {
        switch(Step)
        {
           case 0:
              m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
              m_uiLichKingGUID = m_pInstance->GetData64(BOSS_LICH_KING);
              pLichKing = m_creature->GetMap()->GetCreature( m_uiLichKingGUID);
              JumpNextStep(100);
              break;
           case 1:
              if(pLichKing)
              {
                 pLichKing->SetPhaseMask(65535, true);
                 if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
                    DoScriptText(SAY_LICH_KING_AGGRO_A, pLichKing);
                 if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                    DoScriptText(SAY_LICH_KING_AGGRO_H, pLichKing);
                 pLichKing->AI()->AttackStart(m_creature);
                 m_creature->AI()->AttackStart(pLichKing);
              }
              JumpNextStep(3000);
              break; 
           case 2:
              if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
              {
                 Fight = false;
                 if(pLichKing)
                    m_creature->GetMotionMaster()->MovePoint(0, (m_creature->GetPositionX()-5)+rand()%10, (m_creature->GetPositionY()-5)+rand()%10, m_creature->GetPositionZ());
                 JumpNextStep(3000);
              }
              else
                  JumpNextStep(100);
              break;
           case 3:
              if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                 Fight = true;
              JumpNextStep(100);
              break;
           case 4:
              if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
              {
                 if(pLichKing)
                    m_creature->CastSpell(pLichKing, SPELL_SYLVANA_STEP, false);
                 JumpNextStep(3000);
              }
              else
                  JumpNextStep(100);
              break;
           case 5:
              if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
              {
                 Fight = false;
                 if(pLichKing)
                    m_creature->GetMotionMaster()->MovePoint(0, (m_creature->GetPositionX()-5)+rand()%10, (m_creature->GetPositionY()-5)+rand()%10, m_creature->GetPositionZ());
                 JumpNextStep(3000);
              }
              else 
                  JumpNextStep(12000);
              break;
           case 6:
              Fight = true;
              if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
              {
                 if(pLichKing)
                    m_creature->CastSpell(pLichKing,SPELL_ICE_PRISON,true);
              }
              if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
              {
                 if(pLichKing)
                    m_creature->CastSpell(pLichKing,SPELL_DARK_ARROW,true);
              }
              JumpNextStep(2500);
              break;
           case 7:
              if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
              {
                 if (pLichKing && !pLichKing->HasAura(SPELL_ICE_PRISON))
                    pLichKing->CastSpell(pLichKing,SPELL_ICE_PRISON,true);
              }
              else if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
              {
                 if (pLichKing && !pLichKing->HasAura(SPELL_DARK_ARROW))
                    pLichKing->CastSpell(pLichKing,SPELL_DARK_ARROW,true);
              }
              m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
              m_creature->AttackStop();

              if(m_creature->GetEntry() == NPC_JAINA_OUTRO)
              {
                 m_creature->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);
                 DoScriptText(SAY_JAINA_AGGRO, m_creature);
              }
              else if (m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
                 DoScriptText(SAY_SYLVANA_AGGRO, m_creature);
              JumpNextStep(3000);
              break;

           case 8:
              m_creature->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
              if(pLichKing)
                 m_creature->SetUInt64Value(UNIT_FIELD_TARGET, pLichKing->GetGUID());
              JumpNextStep(10000);
              break; 

           case 9:
              m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
              m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
              Count = 1;
              JumpNextStep(10000);
              break;
        }
   }

   void Outro()
   {
        switch(Step)
        {
           case 10:
              m_creature->CastSpell(m_creature, SPELL_SHIELD_DISRUPTION,false);
              m_creature->RemoveAurasDueToSpell(SPELL_SILENCE);
              m_creature->RemoveSplineFlag(SPLINEFLAG_FLYING);
              JumpNextStep(6000);
              break;
           case 11:
              if (GameObject* pCave = m_creature->SummonGameobject(GO_CAVE, 5275.28f, 1694.23f, 786.147f, 0.981225f, 0))
                  pCave->SetGoState(GO_STATE_READY);
              m_creature->CastSpell(m_creature, SPELL_SHIELD_DISRUPTION,false);
              m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
              m_creature->GetMotionMaster()->MovePoint(0, 5258.911328f,1652.112f,784.295166f);
              DoScriptText(SAY_ESCAPE_01, m_creature);
              JumpNextStep(10000);
              break;
           case 12:
              m_pInstance->SetData(TYPE_LICH_KING, DONE);
              DoScriptText(SAY_ESCAPE_02, m_creature);
              JumpNextStep(10000);
              break;
           case 13:
              DoScriptText(SAY_ESCAPE_03, m_creature);
              JumpNextStep(20000);
              break;
           case 14:
              m_creature->GetMotionMaster()->MovePoint(0, 5240.66f, 1646.93f, 784.302f);
              JumpNextStep(5000);
              break;
           case 15:
              m_creature->SetOrientation(0.68f);
              m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
              m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
              JumpNextStep(5000);
              break;
        }
   }

   void UpdateEscortAI(const uint32 diff)
   {
      if(!m_pInstance || !Event)
          return;

      DoMeleeAttackIfReady();

      if(m_pInstance->GetData(TYPE_PHASE) == 4)
      {
         if(StepTimer < diff)
            Intro();
         else StepTimer -= diff;
      }

      if(m_pInstance->GetData(TYPE_LICH_KING) == SPECIAL
         && m_pInstance->GetData(TYPE_PHASE) != 6)       //End Cinematic
      {
          m_pInstance->SetData(TYPE_PHASE, 6);
          Step = 10;
      }

      if (m_pInstance->GetData(TYPE_PHASE) == 6)
      {
         if(StepTimer < diff)
            Outro();
         else StepTimer -= diff;
         return;
      }

      if (WallCast == true && CastTimer < diff)
      {
         if (Creature* pWallTarget = m_creature->GetMap()->GetCreature(wallTarget))
         {
            if(m_creature->GetEntry() == NPC_SYLVANA_OUTRO)
               m_creature->CastSpell(pWallTarget, SPELL_DESTROY_ICE_WALL_03, false);
            CastTimer = 1000;
         }
      }
      else CastTimer -= diff;

      if (WallCast == true && HoldTimer < 10000 &&  m_pInstance->GetData(DATA_SUMMONS) == 0)
      {
         WallCast = false;
         m_creature->InterruptNonMeleeSpells(false);
         SetEscortPaused(false);
         ++Count;
         switch(Count)
         {
            case 2:
                DoDestructWall(0);
                if(pLichKing && pLichKing->isAlive())
                    DoScriptText(SAY_LICH_KING_WALL_02, pLichKing);
              break;
            case 3:
                DoDestructWall(0);
                if(pLichKing && pLichKing->isAlive())
                    DoScriptText(SAY_LICH_KING_WALL_03, pLichKing);
              break;
            case 4:
                DoDestructWall(0);
                if(pLichKing && pLichKing->isAlive())
                    DoScriptText(SAY_LICH_KING_WALL_04, pLichKing);
              break;
            case 5:
                DoDestructWall(0);
                if(pLichKing && pLichKing->isAlive())
                {
                    pLichKing->RemoveAurasDueToSpell(SPELL_WINTER);
                    pLichKing->SetSpeedRate(MOVE_WALK, 2.5f, true);
                    Step = 0;
                }
              break;
         }
         HoldTimer = 30000;
      }
      else
      {
         HoldTimer -= diff;
         if (HoldTimer <= 0) 
             HoldTimer = 0;
      }

      return;
   }
};

bool GossipHello_npc_jaina_and_sylvana_HRextro(Player* pPlayer, Creature* pCreature)
{

    ScriptedInstance*   m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if(!m_pInstance) return false;

    if(m_pInstance->GetData(TYPE_LICH_KING) == DONE) return false;

    if(pCreature->isQuestGiver())
       pPlayer->PrepareQuestMenu( pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, -3594540, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_jaina_and_sylvana_HRextro(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    ScriptedInstance* m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
           pPlayer->CLOSE_GOSSIP_MENU();
           ((npc_jaina_and_sylvana_HRextroAI*)pCreature->AI())->Start(true);
           pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
           pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
           pCreature->SetUInt64Value(UNIT_FIELD_TARGET, 0);

           if(m_pInstance)
           {
              m_pInstance->SetData64(DATA_ESCAPE_LIDER, pCreature->GetGUID());
              m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
              m_pInstance->SetData(TYPE_PHASE, 5);
           }
           return true;
           break;
       default:  return false;
    }
}

struct MANGOS_DLL_DECL npc_lich_king_hrAI : public ScriptedAI
{
    npc_lich_king_hrAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    void Reset()
    {
    }

    void JustDied(Unit* pKiller)
    {
    }

    void AttackStart(Unit* who) 
    {
         return;
    }

    void UpdateAI(const uint32 diff)
    {
    }
};

CreatureAI* GetAI_npc_jaina_and_sylvana_HRintro(Creature* pCreature)
{
    return new npc_jaina_and_sylvana_HRintroAI(pCreature);
}

CreatureAI* GetAI_npc_jaina_and_sylvana_HRextro(Creature* pCreature)
{
    return new npc_jaina_and_sylvana_HRextroAI(pCreature);
}

CreatureAI* GetAI_npc_lich_king_hr(Creature* pCreature)
{
    return new npc_lich_king_hrAI(pCreature);
}

enum GENERAL_EVENT
{
   SAY_AGGRO                    = -1594519,
   SAY_DEATH                    = -1594520,

   SPELL_SHIELD_THROWN          = 69222,
};

struct MANGOS_DLL_DECL npc_frostworn_generalAI : public ScriptedAI
{
   npc_frostworn_generalAI(Creature *pCreature) : ScriptedAI(pCreature)
   {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
   }

   ScriptedInstance* m_pInstance;

   uint32 m_uiShieldTimer;

   void Reset()
   {
        if (!m_pInstance) return;
        m_uiShieldTimer = 5000;
        m_pInstance->SetData(TYPE_FROST_GENERAL, NOT_STARTED);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
   }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) 
            return;
        DoScriptText(SAY_DEATH, m_creature);
        m_pInstance->SetData(TYPE_FROST_GENERAL, DONE);
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (!m_pInstance) return;

        if (m_creature->getVictim()) return;

        if (pWho->GetTypeId() != TYPEID_PLAYER
            || m_pInstance->GetData(TYPE_MARWYN) != DONE
            || !m_creature->IsWithinDistInMap(pWho, 20.0f)
            ) return;

        if (Player* pPlayer = (Player*)pWho)
            if (pPlayer->isGameMaster()) return;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        AttackStart(pWho);
    }

    void Aggro(Unit* pVictim)
    {
        if (!m_pInstance) 
            return;
        DoScriptText(SAY_AGGRO, m_creature);
        m_pInstance->SetData(TYPE_FROST_GENERAL, IN_PROGRESS);

        Map::PlayerList const &pList = m_creature->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator i = pList.begin(); i != pList.end(); ++i)
        {
            if (Player* pPlayer = i->getSource())
            {
               if (pPlayer && pPlayer->isAlive() && pPlayer->IsInMap(m_creature))
               {
                   if (Creature* pMirror = m_creature->SummonCreature(NPC_SPIRITUAL_REFLECTION,0,0,0,0,TEMPSUMMON_DEAD_DESPAWN,0, true))
                   {
                      pMirror->SetPhaseMask(65535, true);
                      pMirror->SetInCombatWith(pPlayer);
                      pMirror->AddThreat(pPlayer, 1000.0f);
                   }

               }
            }
        };

    }

   void UpdateAI(const uint32 uiDiff)
   {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiShieldTimer < uiDiff)
        {
            if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
               DoCast(pTarget,SPELL_SHIELD_THROWN);
            m_uiShieldTimer = urand(4000, 8000);
        }
        else m_uiShieldTimer -= uiDiff;

        DoMeleeAttackIfReady();
   }
};

CreatureAI* GetAI_npc_frostworn_general(Creature* pCreature)
{
    return new npc_frostworn_generalAI(pCreature);
}

enum spiritual_reflection
{
   SPELL_REFLECTION_GHOST       = 69861,
   SPELL_CLONE                  = 69828,
   SPELL_CLONE2                 = 69837,

   SPELL_BALEFUL_STRIKE         = 69933,
   SPELL_SPIRIT_BURST           = 69900,

};

struct MANGOS_DLL_DECL npc_spiritual_reflectionAI : public BSWScriptedAI
{
    npc_spiritual_reflectionAI(Creature *pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool isMirror;
    ObjectGuid victimGuid;

    void Reset()
    {
        if (!m_pInstance) 
            return;
        isMirror = false;
        victimGuid = ObjectGuid();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pVictim)
    {
        if (!m_pInstance || !pVictim || pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (victimGuid.IsEmpty())
            victimGuid = pVictim->GetObjectGuid();

        DoStartMovement(pVictim);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_pInstance || !m_creature )
            return;

        if (uiDamage >= m_creature->GetHealth())
            doCast(SPELL_SPIRIT_BURST);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_FROST_GENERAL) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!isMirror)
        {
            if (Unit* pVictim = m_creature->GetMap()->GetUnit(victimGuid))
                if (m_creature->IsWithinDistInMap(pVictim, 5.0f))
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pVictim->CastSpell(m_creature, SPELL_CLONE, true);
                    pVictim->CastSpell(m_creature, SPELL_CLONE2, true);
                    m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID,   pVictim->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID));
                    m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, pVictim->GetUInt32Value(PLAYER_VISIBLE_ITEM_17_ENTRYID));
                    m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+2, pVictim->GetUInt32Value(PLAYER_VISIBLE_ITEM_18_ENTRYID));
                    pVictim->CastSpell(m_creature, SPELL_REFLECTION_GHOST, true);
                    isMirror = true;
                }
        }

        if (!isMirror)
            return;

        timedCast(SPELL_BALEFUL_STRIKE,uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_spiritual_reflection(Creature* pCreature)
{
    return new npc_spiritual_reflectionAI(pCreature);
}

struct MANGOS_DLL_DECL npc_queldelar_horAI : public ScriptedAI
{
    npc_queldelar_horAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool intro;
    Team team;
    uint32 newLeader;

    void Reset()
    {
        intro = false;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_pInstance || intro)
            return;

        if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER || !pWho->IsWithinDistInMap(m_creature, 20.0f))
            return;

        if (m_pInstance->GetData(TYPE_MARWYN) == DONE)
            return;

        if (Group* pGroup = ((Player*)pWho)->GetGroup())
        {
            ObjectGuid LeaderGuid = pGroup->GetLeaderGuid();
            if (!LeaderGuid.IsEmpty())
                if (Player* pLeader =m_creature->GetMap()->GetPlayer(LeaderGuid))
                    team = pLeader->GetTeam();
        }
        else
             team = ((Player*)pWho)->GetTeam();


        if (team == ALLIANCE)
            newLeader = NPC_JAINA;
        else
            newLeader = NPC_SYLVANA;

        debug_log("HOR event: team %u, leader %u ",team,newLeader);

        if (Creature* pNewLeader = m_creature->SummonCreature(newLeader,WallLoc[4].x,WallLoc[4].y,WallLoc[4].z,WallLoc[4].o,TEMPSUMMON_MANUAL_DESPAWN,0,true))
        {
             pNewLeader->SetCreatorGuid(ObjectGuid());
             pNewLeader->setFaction(35);
             pNewLeader->SetPhaseMask(65535, true);
             pNewLeader->GetMotionMaster()->MovePoint(0, WallLoc[5].x,WallLoc[5].y,WallLoc[5].z);
        }
        m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 63135);
    }

    void AttackStart(Unit* who) 
    {
         return;
    }

    void UpdateAI(const uint32 diff)
    {
    }
};
CreatureAI* GetAI_npc_queldelar_hor(Creature* pCreature)
{
    return new npc_queldelar_horAI(pCreature);
}


void AddSC_halls_of_reflection()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_jaina_and_sylvana_HRintro";
    newscript->GetAI = &GetAI_npc_jaina_and_sylvana_HRintro;
    newscript->pGossipHello = &GossipHello_npc_jaina_and_sylvana_HRintro;
    newscript->pGossipSelect = &GossipSelect_npc_jaina_and_sylvana_HRintro;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_jaina_and_sylvana_HRextro";
    newscript->GetAI = &GetAI_npc_jaina_and_sylvana_HRextro;
    newscript->pGossipHello = &GossipHello_npc_jaina_and_sylvana_HRextro;
    newscript->pGossipSelect = &GossipSelect_npc_jaina_and_sylvana_HRextro;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_lich_king_hr";
    newscript->GetAI = &GetAI_npc_lich_king_hr;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_frostworn_general";
    newscript->GetAI = &GetAI_npc_frostworn_general;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_spiritual_reflection";
    newscript->GetAI = &GetAI_npc_spiritual_reflection;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_queldelar_hor";
    newscript->GetAI = &GetAI_npc_queldelar_hor;
    newscript->RegisterSelf();
}