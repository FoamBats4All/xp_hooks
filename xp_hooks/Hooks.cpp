#include "Precomp.h"

#include "Hooks.h"

#include "Offsets.h"
#include "NWN2.h"
#include "HooksPlugin.h"

HooksPlugin            g_Plugin;
extern IDebugTextOut * g_TextOut;
FILE                 * g_Log;

Patch _patches[ ] =
{
	Patch( OFFS_USELOREONITEM_HOOK, "\xe9", 1 ),
	Patch( OFFS_USELOREONITEM_HOOK+1, (relativefunc)IdentifyItemWithLore ),

	// We include an end patch to ensure we fail the last one.
	Patch()
};

Patch * patches = _patches;


BOOL
WINAPI
DllMain(
	__in HINSTANCE hinstDLL,
	__in DWORD fdwReason,
	__in LPVOID lpvReserved
	)
/*++

Routine Description:

	This routine acts as the DLL entry point.  It simply turns off TLS callouts
	for process attach and returns TRUE.

Arguments:

	hinstDLL - Supplies the module instance handle.

	fdwReason - Supplies the reason code for the callout.

	lpvReserved - For process attach and detach, non-NULL for static attach,
	              else NULL for dynamic attach.

Return Value:

	The routine always returns TRUE to allow the module to load.

Environment:

	User mode.

--*/
{
	UNREFERENCED_PARAMETER( lpvReserved );

	switch (fdwReason)
	{

	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hinstDLL );
		break;

	}

	return TRUE;
}


NWNX4PluginBase *
WINAPI
GetPluginPointerV2(
	)
{
	return &g_Plugin;
}


bool
NWNXAPI
HooksPlugin::Init(
	__in const char * NWNXHome
	)
{
	FILE      * Log;
	char        LogFileName[ MAX_PATH ];

	StringCbPrintfA( LogFileName, sizeof( LogFileName ), "%s\\xp_hooks.log", NWNXHome);

	Log = fopen( LogFileName, "w");

	m_Log     = Log;
	g_Log     = Log;
	m_TextOut = g_TextOut;

	m_TextOut->WriteText( "xp_hooks plugin - built " __DATE__ " " __TIME__ "\n" );

	
	ApplyPatches();

	return true;
}


void
ExecuteScript(
	unsigned long Caller,
	std::string ScriptName,
	std::vector<CScriptParameterWrapper> Params
	)
{
#if defined(OFFS_CVirtualMachine) && defined(OFFS_CVirtualMachine_RunScript)
	g_TextOut->WriteText( "Calling script %s on %u with %u params.\n", ScriptName.c_str(), Caller, Params.size() );

	// Convert script name.
	CExoString NWScriptName;
	NWScriptName.String = ScriptName.c_str();
	NWScriptName.Length = ScriptName.length();

	// Convert params.
	CExoArrayList * NWParams = NULL;
	size_t ParamCount = Params.size();
	if ( ParamCount > 0 ) {
		NWParams = new CExoArrayList[ParamCount];
		for ( size_t i = 0; i < ParamCount; i++ ) {
			NWParams[i].Element = (void*)&Params[i];
			NWParams[i].Index = i;
			NWParams[i].ArraySize = ParamCount;
		}
	} else {
		NWParams = new CExoArrayList[1];
		NWParams[0].Element = NULL;
		NWParams[0].Index = 0;
		NWParams[0].ArraySize = 0;
	}

	CVirtualMachine_RunScript RunScript = (CVirtualMachine_RunScript)OFFS_CVirtualMachine_RunScript;
	CVirtualMachine* VM = (CVirtualMachine*)OFFS_CVirtualMachine;
	RunScript( VM, &NWScriptName, Caller, *NWParams, NULL, NULL );

	delete[] NWParams;
#else
	UNREFERENCED_PARAMETER( ScriptName );
	UNREFERENCED_PARAMETER( Params );
#endif
}

void
ExecuteScript(
	unsigned long Caller,
	std::string ScriptName
	)
{
	std::vector<CScriptParameterWrapper> Params;
	ExecuteScript( Caller, ScriptName, Params );
}

//
// LORE IDENTIFY ITEM
//


void
SetItemBeingIdentified(
	__in DWORD ItemId
	)
/*++

Routine Description:

	Setter function for plugin->m_LastLoreIdentifiedItem, so that
	GetItemBeingIdentified can be called.

Arguments:

	CreatureId - Object ID of the item being identified.

Return Value:

	None.

Environment:

	User mode.

--*/
{
	g_TextOut->WriteText( "SetItemBeingIdentified( %u );\n", ItemId );
	g_Plugin.m_LastLoreIdentifiedItem = ItemId;
}


void
SetItemIdentifier(
	__in DWORD CreatureId
	)
/*++

Routine Description:

	Setter function for plugin->m_LastLoreIdentifier, so that
	GetItemIdentifier can be called.

Arguments:

	CreatureId - Object ID of the creature attempting to identify.

Return Value:

	None.

Environment:

	User mode.

--*/
{
	g_TextOut->WriteText( "SetItemIdentifier( %u );\n", CreatureId );
	g_Plugin.m_LastLoreIdentifier = CreatureId;
}


void
__stdcall
IdentifyItemWithLoreInternal(
	__in CNWSCreature * Creature,
	__in DWORD ItemId
	)
/*++

Routine Description:

	This function is the bridge between Lore Identification setter functions
	and the assembly code. It is responsible for getting our object Id for our
	indentifying creature and calling the script responsible for handling
	lore identification events.

Arguments:

	Creature - Pointer to a CNWSCreature, the creature responsible for
		identifying the item.
	ItemId - The object ID of the item to be identified.

Return Value:

	None.

Environment:

	User mode.

--*/
{
	g_TextOut->WriteText( "IdentifyItemWithLoreInternal( %u, %u );\n", Creature, ItemId );

	// Store our data values.
	SetItemBeingIdentified( ItemId );
	unsigned long CreatureId = Creature->AsCNWSObject.AsCGameObject.m_ObjectId;
	SetItemIdentifier( CreatureId );

	// Execute our callback function.
	ExecuteScript( CreatureId, g_Plugin.m_OnLoreUseScript );
}


CLIEXT_DECLSPEC_NAKED
void
IdentifyItemWithLore(
	)
/*++

Routine Description:

	This function is called whenever an item is about to be identified with the
	NWN2 'Lore' skill. When this function is enabled, it will overwrite the 
	normal function, allowing a user to specify their own.

Arguments:

	ecx - CNWSCreature's 'this' pointer.
	edx - Object ID of the item to be identified.

Return Value:

	None.

Environment:

	User mode, mid-function assembler hook.

--*/
{
#if defined(_M_IX86) && defined(OFFS_USELOREONITEM_HOOK_RETURN)
	__asm
	{
		mov     eax, OFFS_CAppManager
		mov     edx, [esp+4]
		push    ebx
		push    esi
		mov     ebx, OFFS_USELOREONITEM_HOOK_RETURN
		jmp     ebx
	}
#endif
}



//
// PICKPOCKET
//


void
SetLastPickpocketed(
	__in DWORD ID
	)
{
	g_Plugin.m_LastPickpocketed = ID;
}


void
SetLastPickpocketer(
	__in DWORD ID
	)
{
	g_Plugin.m_LastPickpocketer = ID;
}


void
SetLastPickpocketedGoldCount(
	__in unsigned int ID
	)
{
	g_Plugin.m_LastPickpocketedItem = 0;
	g_Plugin.m_LastPickpocketedGoldCount = ID;
}


void
SetLastPickpocketedItem(
	__in unsigned int ID
	)
{
	g_Plugin.m_LastPickpocketedItem = ID;
	g_Plugin.m_LastPickpocketedGoldCount = 0;

}


CLIEXT_DECLSPEC_NAKED
void
OnPickPocket(
	)
/*++

Routine Description:

	This function is called whenever a pickpocket event has been concluded.

Arguments:

	

Return Value:

	

Environment:

	User mode, mid-function assembler hook.

--*/
{
#if defined(_M_IX86) && defined(OFFS_PICKPOCKET_ENTRY) && defined(OFFS_PICKPOCKET_RESUME)
	__asm
	{

		

	}
#endif
}
