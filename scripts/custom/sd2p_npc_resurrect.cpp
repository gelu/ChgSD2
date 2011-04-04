/**
 *
 * @File : sd2p_npc_resurrect.cpp
 *
 * @Authors : Wilibald09
 *
 * @Date : 23/12/2009
 *
 * @Version : 1.0
 *
 * @Synopsis : PNJ semblable au PNJ des BG.
 *
 **/


#include "precompiled.h"

enum
{
    INTERVAL_RESURRECT = 20000,
};

namespace
{
    template<class Check>
    struct PlayerListSearcher
    {
        uint32 m_uiPhaseMask;
        std::list<Player*> & m_lObjects;
        Check & m_Check;

        PlayerListSearcher(WorldObject const * pSearcher, std::list<Player*> & lObjects, Check & _Check)
            : m_uiPhaseMask(pSearcher->GetPhaseMask()), m_lObjects(lObjects), m_Check(_Check)
        {
        }

        void Visit(PlayerMapType & _Map)
        {
            for(PlayerMapType::iterator It = _Map.begin(); It != _Map.end(); ++It)
            {
                if(!It->getSource()->InSamePhase(m_uiPhaseMask))
                    continue;

                if(m_Check(It->getSource()))
                    m_lObjects.push_back(It->getSource());
            }
        }

        template<class NOT_INTERESTED> void Visit(GridRefManager<NOT_INTERESTED> &) {}
    };

    class AnyDeadPlayerInRange
    {
        public:
            AnyDeadPlayerInRange(WorldObject const * pSource, float fRange)
                : m_pSource(pSource), m_fRange(fRange)
            {
            }

            bool operator()(Player * pPlayer)
            {
                return !pPlayer->isAlive() && m_pSource->IsWithinDistInMap(pPlayer, m_fRange);
            }

        private:
            WorldObject const * m_pSource;
            float m_fRange;
    };

    void GetDeadUnitInRange(std::list<Player*> & lList, WorldObject const * pSource, float fRange)
    {
        AnyDeadPlayerInRange u_check(pSource, fRange);
        PlayerListSearcher<AnyDeadPlayerInRange> searcher(pSource, lList, u_check);

        Cell::VisitWorldObjects(pSource, searcher, fRange);
    }
}

struct MANGOS_DLL_DECL npc_resurrect_ai : public ScriptedAI
{
    npc_resurrect_ai(Creature * pCreature)
        : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiIntervalCounter;
    uint32 m_uiIntervalRez;


    void Reset(void)
    {
        m_uiIntervalCounter = 1000;
        m_uiIntervalRez = INTERVAL_RESURRECT;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiIntervalCounter <= uiDiff)
        {
            m_uiIntervalCounter = 1000;

            std::list<Player*> lPlayers;
            GetDeadUnitInRange(lPlayers, m_creature, 45.0f);

            if (m_uiIntervalRez)
            {
                if (m_uiIntervalRez <= 5000 || m_uiIntervalRez % 5000 == 0)
                {
                    std::ostringstream oss;
                    oss << "Temps restant avant resurrection : " << m_uiIntervalRez / 1000 << " seconde(s)";

                    for (std::list<Player*>::const_iterator It = lPlayers.begin(); It != lPlayers.end(); ++It)
                        m_creature->MonsterSay(oss.str().c_str(), (*It)->GetGUID());
                }
                m_uiIntervalRez -= 1000;
            }
            else
            {
                for (std::list<Player*>::const_iterator It = lPlayers.begin(); It != lPlayers.end(); ++It)
                {
                    (*It)->ResurrectPlayer(100.0f);
                    (*It)->SpawnCorpseBones();
                }
                m_uiIntervalRez = INTERVAL_RESURRECT;
            }
        }
        else
            m_uiIntervalCounter -= uiDiff;
    }
    
    void Aggro(Unit * pWho) {}
    void MoveInLineOfSight(Unit * pWho) {}
};

CreatureAI* GetAI_npc_resurrect(Creature * pCreature)
{
    return new npc_resurrect_ai(pCreature);
}


void AddSC_npc_resurrect(void)
{
    Script * newscript;

    newscript = new Script;
    newscript->Name="sd2p_npc_resurrect";
    newscript->GetAI = &GetAI_npc_resurrect;
    newscript->RegisterSelf();
}
