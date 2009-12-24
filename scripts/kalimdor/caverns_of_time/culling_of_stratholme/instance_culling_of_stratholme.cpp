#include "precompiled.h"
#include "def_culling_of_stratholme.h"

#define GO_SHKAF_GATE          188686
#define GO_MALGANIS_GATE1   187711    
#define GO_MALGANIS_GATE2   187723 
#define GO_MALGANIS_CHEST   190663 

struct MANGOS_DLL_DECL instance_culling_of_stratholme : public ScriptedInstance
{
    instance_culling_of_stratholme(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[1];
    std::string strInstData;

    uint64 m_uiShkafGateGUID;
    uint64 m_uiMalGate1GUID;
    uint64 m_uiMalGate2GUID;
    uint64 m_uiMalChestGUID;

    void Initialize()
    {
        m_uiShkafGateGUID = 0;
        m_uiMalGate1GUID = 0;
        m_uiMalGate2GUID = 0;
        m_uiMalChestGUID = 0;
    }

    void OnObjectCreate(GameObject* pGo)
    {
        if (pGo->GetEntry() == GO_SHKAF_GATE)
            m_uiShkafGateGUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_GATE1)
            m_uiMalGate1GUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_GATE2)
            m_uiMalGate2GUID = pGo->GetGUID();

        if (pGo->GetEntry() == GO_MALGANIS_CHEST)
            m_uiMalChestGUID = pGo->GetGUID();
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_ARTHAS_EVENT:
                m_auiEncounter[0] = uiData;
                break;

        }
    }

    uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_ARTHAS_EVENT:
                return m_auiEncounter[0];

        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_GO_SHKAF_GATE: return m_uiShkafGateGUID;
            case DATA_GO_MAL_GATE1: return m_uiMalGate1GUID;
            case DATA_GO_MAL_GATE2: return m_uiMalGate2GUID;
            case DATA_GO_MAL_CHEST: return m_uiMalChestGUID;
        }

        return 0;
    }

};

InstanceData* GetInstanceData_instance_culling_of_stratholme(Map* pMap)
{
    return new instance_culling_of_stratholme(pMap);
}

void AddSC_instance_culling_of_stratholme()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_culling_of_stratholme";
    newscript->GetInstanceData = &GetInstanceData_instance_culling_of_stratholme;
    newscript->RegisterSelf();
}
