#include "precompiled.h"
#include "eye_of_eternity.h"

enum eGameObjects
{
    GO_Malygos_CHEST_HERO  = 193967,
	GO_Malygos_CHEST       = 193905
};

struct MANGOS_DLL_DECL instance_eye_of_eternity : public ScriptedInstance
{
    instance_eye_of_eternity(Map* pMap) : ScriptedInstance(pMap), MalygosChest(NULL) { Initialize(); };

	uint32 m_auiEncounter[MAX_ENCOUNTER];

    uint64 m_uiMalygosGUID;

    uint64 MalygosChest;

	void Initialize()
	{
        m_uiMalygosGUID = 0;
        MalygosChest = 0;
		memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    }

	void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_MALYGOS:
                m_uiMalygosGUID = pCreature->GetGUID();
                break;
        }

     }

	void OnObjectCreate(GameObject* pGo)
    {
        switch(pGo->GetEntry())
        {
            case GO_Malygos_CHEST_HERO: 
				MalygosChest = pGo->GetGUID();
				if (m_auiEncounter[TYPE_MALYGOS] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
                break;
            case GO_Malygos_CHEST:
				MalygosChest = pGo->GetGUID(); 
				if (m_auiEncounter[TYPE_MALYGOS] == DONE)
                    pGo->SetGoState(GO_STATE_ACTIVE);
				break;
        }
    }

	void SetData(uint32 type, uint32 data)
    {
        switch(type)
        {
            case TYPE_MALYGOS:
				m_auiEncounter[TYPE_MALYGOS] = data;
                if (data == DONE && MalygosChest)
				DoRespawnGameObject(MalygosChest, 30*MINUTE);
                break;
        }

		if (data == DONE)
        {
            OUT_SAVE_INST_DATA;
            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

	uint64 GetData64(uint32 data)
    {
        switch(data)
        {
            case TYPE_MALYGOS:
                return m_uiMalygosGUID;
        }

        return 0;
    }

	uint32 GetData(uint32 type)
    {
        switch(type)
        {
            case TYPE_MALYGOS:
                return m_auiEncounter[type];
        }

        return 0;
    }
};

InstanceData* GetInstanceData_instance_eye_of_eternity(Map* pMap)
{
    return new instance_eye_of_eternity(pMap);
}

void AddSC_instance_eye_of_eternity()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_eye_of_eternity";
    newscript->GetInstanceData = &GetInstanceData_instance_eye_of_eternity;
    newscript->RegisterSelf();
}
