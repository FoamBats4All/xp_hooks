/*++

Copyright (c) Michael Wallar (NullCascade). All rights reserved.

Module Name:

	HooksPlugin.h

Abstract:

	This module defines interfaces related to the NWNX4 plugin API.

--*/

#include "NWNX4Plugin.h"

#include "Patch.h"

#ifndef _SOURCE_PROGRAMS_XPHOOKS_HOOKSPLUGIN_H
#define _SOURCE_PROGRAMS_XPHOOKS_HOOKSPLUGIN_H

#ifdef _MSC_VER
#pragma once
#endif

class HooksPlugin : public NWNX4PluginBase {

public:

	HooksPlugin(
		);

	~HooksPlugin(
		);

	void
	ReadSettings(
		);

	void
	ApplyPatches(
		);

	//
	// NWNX4 FUNCTIONS
	//

	bool
	NWNXAPI
	Init(
		__in const char * NWNXHome
		);

	int
	GetInt(
		__in const char* Function,
		__in const char* Param1,
		__in int Param2
		);

	//
	// MEMBER DATA
	//
	
	// Logging.
	FILE                        * m_Log;
	IDebugTextOut               * m_TextOut;
	bool                          m_Enabled;
	static HooksPlugin          * m_sPlugin;

	// Context data for lore identification events.
	std::string                             m_OnLoreUseScript;
	bool                                    m_HookLoreIdentifiedItem;
	DWORD                                   m_LastLoreIdentifiedItem;
	DWORD                                   m_LastLoreIdentifier;

	// Context data for pickpocket events.
	bool									m_HookPickpocket;
	unsigned long                           m_LastPickpocketSuccess;
	DWORD                                   m_LastPickpocketed;
	DWORD                                   m_LastPickpocketer;
	unsigned long                           m_LastPickpocketedGoldCount;
	unsigned long                           m_LastPickpocketedItem;

protected:

	std::vector<Patch>                      m_Patches;

};

#endif