/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#include "precompiled.h"
#include "Config/Config.h"
#include "config.h"
#include "Database/DatabaseEnv.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "ProgressBar.h"
#include "../system/ScriptLoader.h"
#include "../system/system.h"
#include "../../../game/ScriptMgr.h"
#include "World.h"
#include "sd2_projects.h"

int num_sc_scripts;
Script *m_scripts[MAX_SCRIPTS];

Config SD2Config;

/**********************************************************************
additions for windows compiler
**********************************************************************/
template<> bool MaNGOS::Singleton<World>::si_destroyed;
template<> bool MaNGOS::Singleton<ObjectMgr>::si_destroyed;
template<> World *MaNGOS::Singleton<World>::si_instance;
World::World()
{
}
World::~World()
{
}
ObjectMgr::~ObjectMgr()
{
}
/***********************************************************************/

QueryResult* strSD2Pquery(char* str)
{
return SD2Database.Query(str);
}
// Not registered scripts storage
std::map<std::string, Script*> m_scriptStorage;

void FillSpellSummary();

void LoadDatabase()
{

    std::string strSD2DBinfo = SD2Config.GetStringDefault("ScriptDev2DatabaseInfo", "");

    if (strSD2DBinfo.empty())
    {
        error_log("脚本库：无法从配置文件中加载数据库信息，请确认您的配置文件是有效的！ ");
        return;
    }

    //Initialize connection to DB
    if (SD2Database.Initialize(strSD2DBinfo.c_str()))
    {
        outstring_log("脚本库： 初始化脚本库 ：%s 。", strSD2DBinfo.c_str());
        outstring_log("");

        pSystemMgr.LoadVersion();
        pSystemMgr.LoadScriptTexts();
        pSystemMgr.LoadScriptTextsCustom();
        pSystemMgr.LoadScriptGossipTexts();
        pSystemMgr.LoadScriptWaypoints();
		SD2P_NAMESPACE::LoadDatabase();
    }
    else
    {
        error_log("脚本库： 连接数据库失败！ 脚本库加载已终止。 ");
        return;
    }

}

struct TSpellSummary {
    uint8 Targets;                                          // set of enum SelectTarget
    uint8 Effects;                                          // set of enum SelectEffect
}extern *SpellSummary;

MANGOS_DLL_EXPORT
void FreeScriptLibrary()
{
    // Free Spell Summary
    delete []SpellSummary;

    // Free resources before library unload
    for(int i=0; i<MAX_SCRIPTS; ++i)
        delete m_scripts[i];

    for (std::map<std::string, Script*>::iterator itr = m_scriptStorage.begin(); itr != m_scriptStorage.end(); ++itr)
        delete itr->second;

    num_sc_scripts = 0;
    SD2Database.HaltDelayThread();
}

MANGOS_DLL_EXPORT
void InitScriptLibrary()
{
    //ScriptDev2 startup
    outstring_log("");
    outstring_log("===========================================================================");
    outstring_log("          P   R   O   J   E   C   T - The ChgMangos Project by Chglove     ");
    outstring_log("                     Wish everyone can enjoy my work!                      ");
	outstring_log("ChgMangos v1.3.0 beta                                                      ");
	outstring_log("Mangos 11304,SD2 2016, ChgMangosDB v1.3.0 with ACID 3.0.8 full             ");
	outstring_log("===========================================================================");

    //Get configuration file
    if (!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
        error_log("脚本库： 打开配置文件失败，系统将使用默认配置！ ");
    else
        outstring_log("脚本库： 使用配置文件 %s",_SCRIPTDEV2_CONFIG);

    //Check config file version
    if (SD2Config.GetIntDefault("ConfVersion", 0) != SD2_CONF_VERSION)
        error_log("脚本库： 配置文件版本不正确，可能过期了？一些功能将无法使用或不正常。 ");

    outstring_log("");

    //Load database (must be called after SD2Config.SetSource).
    LoadDatabase();

    outstring_log("脚本库： 加载C++脚本数据 ");
    barGoLink bar(1);
    bar.step();
    outstring_log("");

    for(int i=0; i<MAX_SCRIPTS; ++i)
        m_scripts[i]=NULL;

    m_scriptStorage.clear();

    FillSpellSummary();

    AddScripts();
	SD2P_NAMESPACE::LoadScripts();

    outstring_log(">> 加载了 %i 个脚本数据", num_sc_scripts);
}

//*********************************
//*** Functions used globally ***

void DoScriptText(int32 iTextEntry, WorldObject* pSource, Unit* pTarget)
{
    if (!pSource)
    {
        error_log("脚本库： DoScriptText entry %i, invalid Source pointer.", iTextEntry);
        return;
    }

    if (iTextEntry >= 0)
    {
        error_log("脚本库： DoScriptText with source entry %u (TypeId=%u, guid=%u) attempts to process text entry %i, but text entry must be negative.",
            pSource->GetEntry(), pSource->GetTypeId(), pSource->GetGUIDLow(), iTextEntry);

        return;
    }

    const StringTextData* pData = pSystemMgr.GetTextData(iTextEntry);

    if (!pData)
    {
        error_log("脚本库： DoScriptText with source entry %u (TypeId=%u, guid=%u) could not find text entry %i.",
            pSource->GetEntry(), pSource->GetTypeId(), pSource->GetGUIDLow(), iTextEntry);

        return;
    }

    debug_log("脚本库： DoScriptText: text entry=%i, Sound=%u, Type=%u, Language=%u, Emote=%u",
        iTextEntry, pData->uiSoundId, pData->uiType, pData->uiLanguage, pData->uiEmote);

    if (pData->uiSoundId)
    {
        if (GetSoundEntriesStore()->LookupEntry(pData->uiSoundId))
            pSource->PlayDirectSound(pData->uiSoundId);
        else
            error_log("脚本库： DoScriptText entry %i tried to process invalid sound id %u.", iTextEntry, pData->uiSoundId);
    }

    if (pData->uiEmote)
    {
        if (pSource->GetTypeId() == TYPEID_UNIT || pSource->GetTypeId() == TYPEID_PLAYER)
            ((Unit*)pSource)->HandleEmote(pData->uiEmote);
        else
            error_log("脚本库： DoScriptText entry %i tried to process emote for invalid TypeId (%u).", iTextEntry, pSource->GetTypeId());
    }

    switch(pData->uiType)
    {
        case CHAT_TYPE_SAY:
            pSource->MonsterSay(iTextEntry, pData->uiLanguage, pTarget);
            break;
        case CHAT_TYPE_YELL:
            pSource->MonsterYell(iTextEntry, pData->uiLanguage, pTarget);
            break;
        case CHAT_TYPE_TEXT_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget);
            break;
        case CHAT_TYPE_BOSS_EMOTE:
            pSource->MonsterTextEmote(iTextEntry, pTarget, true);
            break;
        case CHAT_TYPE_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget);
            else
                error_log("脚本库： DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_BOSS_WHISPER:
        {
            if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                pSource->MonsterWhisper(iTextEntry, pTarget, true);
            else
                error_log("脚本库： DoScriptText entry %i cannot whisper without target unit (TYPEID_PLAYER).", iTextEntry);

            break;
        }
        case CHAT_TYPE_ZONE_YELL:
            pSource->MonsterYellToZone(iTextEntry, pData->uiLanguage, pTarget);
            break;
    }
}

//*********************************
//*** Functions used internally ***

void Script::RegisterSelf(bool bReportError)
{
    int id = GetScriptId(Name.c_str());
    if (id != 0)
    {
        m_scripts[id] = this;
        ++num_sc_scripts;
    }
    else
    {
        //if (bReportError)
        //    error_db_log("脚本库： Script registering but ScriptName %s is not assigned in database.", (this)->Name.c_str());

        m_scriptStorage.insert(std::make_pair(Name.c_str(), this));
    }
}

//********************************
//*** Functions to be Exported ***

MANGOS_DLL_EXPORT
char const* GetScriptLibraryVersion()
{
    return strSD2Version.c_str();
}

MANGOS_DLL_EXPORT
bool GossipHello(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipHello)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipHello(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
bool GOGossipHello(Player *pPlayer, GameObject *pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipHelloGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipHelloGO(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool GossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    //debug_log("脚本库： Gossip selection, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipSelect)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();
//    this expression is wrong, where 'return false' from script's GossipSelect
//    not return menu ID (cleared in this string) and not allow to work with database-based menus

    return tmpscript->pGossipSelect(pPlayer, pCreature, uiSender, uiAction);
}

MANGOS_DLL_EXPORT
bool GOGossipSelect(Player *pPlayer, GameObject *pGo, uint32 sender, uint32 action)
{
    //debug_log("脚本库： GO Gossip selection, sender: %u, action: %u", sender, action);

    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipSelectGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectGO(pPlayer, pGo, sender, action);
}

MANGOS_DLL_EXPORT
bool GossipSelectWithCode(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* sCode)
{
    //debug_log("脚本库： Gossip selection with code, sender: %u, action: %u", uiSender, uiAction);

    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pGossipSelectWithCode)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectWithCode(pPlayer, pCreature, uiSender, uiAction, sCode);
}

MANGOS_DLL_EXPORT
bool GOGossipSelectWithCode(Player *pPlayer, GameObject *pGo, uint32 sender, uint32 action, const char* sCode)
{
    //debug_log("脚本库： GO Gossip selection with code, sender: %u, action: %u", sender, action);

    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGossipSelectGOWithCode)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectGOWithCode(pPlayer, pGo, sender, action, sCode);
}

MANGOS_DLL_EXPORT
bool QuestAccept(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pQuestAcceptNPC)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptNPC(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
bool QuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pQuestRewardedNPC)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestRewardedNPC(pPlayer, pCreature, pQuest);
}

MANGOS_DLL_EXPORT
uint32 GetNPCDialogStatus(Player* pPlayer, Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->pDialogStatusNPC)
        return 100;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pDialogStatusNPC(pPlayer, pCreature);
}

MANGOS_DLL_EXPORT
uint32 GetGODialogStatus(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pDialogStatusGO)
        return 100;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pDialogStatusGO(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool ItemQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pItem->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestAcceptItem)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptItem(pPlayer, pItem, pQuest);
}

MANGOS_DLL_EXPORT
bool GOUse(Player* pPlayer, GameObject* pGo)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pGOUse)
        return false;

    return tmpscript->pGOUse(pPlayer, pGo);
}

MANGOS_DLL_EXPORT
bool GOQuestAccept(Player* pPlayer, GameObject* pGo, const Quest* pQuest)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestAcceptGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestAcceptGO(pPlayer, pGo, pQuest);
}

MANGOS_DLL_EXPORT
bool GOQuestRewarded(Player* pPlayer, GameObject* pGo, Quest const* pQuest)
{
    Script *tmpscript = m_scripts[pGo->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pQuestRewardedGO)
        return false;

    pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pQuestRewardedGO(pPlayer, pGo, pQuest);
}

MANGOS_DLL_EXPORT
bool AreaTrigger(Player* pPlayer, AreaTriggerEntry const* atEntry)
{
    Script *tmpscript = m_scripts[GetAreaTriggerScriptId(atEntry->id)];

    if (!tmpscript || !tmpscript->pAreaTrigger)
        return false;

    return tmpscript->pAreaTrigger(pPlayer, atEntry);
}

MANGOS_DLL_EXPORT
bool ProcessEvent(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    Script *tmpscript = m_scripts[GetEventIdScriptId(uiEventId)];

    if (!tmpscript || !tmpscript->pProcessEventId)
        return false;

    // bIsStart may be false, when event is from taxi node events (arrival=false, departure=true)
    return tmpscript->pProcessEventId(uiEventId, pSource, pTarget, bIsStart);
}

MANGOS_DLL_EXPORT
CreatureAI* GetCreatureAI(Creature* pCreature)
{
    Script *tmpscript = m_scripts[pCreature->GetScriptId()];

    if (!tmpscript || !tmpscript->GetAI)
        return NULL;

    return tmpscript->GetAI(pCreature);
}

MANGOS_DLL_EXPORT
bool ItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    Script *tmpscript = m_scripts[pItem->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pItemUse)
        return false;

	pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pItemUse(pPlayer, pItem, targets);
}

MANGOS_DLL_EXPORT
bool GossipSelectItem(Player* pPlayer, Item* pItem, uint32 sender, uint32 action, SpellCastTargets const& targets)
{
    Script *tmpscript = m_scripts[pItem->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pItemUse)
        return false;

	pPlayer->PlayerTalkClass->ClearMenus();

    return tmpscript->pGossipSelectItem(pPlayer, pItem, sender, action, targets);
}

MANGOS_DLL_EXPORT
bool EffectDummyCreature(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Creature* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectDummyNPC)
        return false;

    return tmpscript->pEffectDummyNPC(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyGameObject(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, GameObject* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetGOInfo()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyGO)
        return false;

    return tmpscript->pEffectDummyGO(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool EffectDummyItem(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Item* pTarget)
{
    Script *tmpscript = m_scripts[pTarget->GetProto()->ScriptId];

    if (!tmpscript || !tmpscript->pEffectDummyItem)
        return false;

    return tmpscript->pEffectDummyItem(pCaster, spellId, effIndex, pTarget);
}

MANGOS_DLL_EXPORT
bool AuraDummy(Aura const* pAura, bool apply)
{
    Script *tmpscript = m_scripts[((Creature*)pAura->GetTarget())->GetScriptId()];

    if (!tmpscript || !tmpscript->pEffectAuraDummy)
        return false;

    return tmpscript->pEffectAuraDummy(pAura, apply);
}

MANGOS_DLL_EXPORT
InstanceData* CreateInstanceData(Map* pMap)
{
    Script *tmpscript = m_scripts[pMap->GetScriptId()];

    if (!tmpscript || !tmpscript->GetInstanceData)
        return NULL;

    return tmpscript->GetInstanceData(pMap);
}

Script* GetScriptByName(std::string scriptName)
{
    std::map<std::string, Script*>::const_iterator itr = m_scriptStorage.find(scriptName);
    if (itr != m_scriptStorage.end())
        return itr->second;
    else
        return NULL;
}
