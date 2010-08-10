/* Copyright (C) 2009 - 2010 by /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef BSW_INSTANCE_H
#define BSW_INSTANCE_H
#define BSW_VERSION 0.6.12
#include "sc_instance.h"

class MANGOS_DLL_DECL BSWScriptedInstance : public ScriptedInstance
{
    public:

        BSWScriptedInstance(Map* pMap) : ScriptedInstance(pMap) {}
        ~BSWScriptedInstance() {}

        //sends completed achievments to all players in instance
        void DoCompleteAchievement(uint32 uiAchievmentId);
};
#endif
