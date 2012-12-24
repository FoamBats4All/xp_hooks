#include "Precomp.h"

#include "HooksPlugin.h"

#include "Offsets.h"
#include "Patch.h"
#include "Hooks.h"

HooksPlugin::HooksPlugin(
	) : NWNX4PluginBase(
			"HOOKS",
			"XP_HOOKS",
			"1.0.0",
			"Features hooks into various gameplay aspects."
		),
		m_OnLoreUseScript( "hook_lore" ),
		m_HookLoreIdentifiedItem( true ),
		m_LastLoreIdentifiedItem( NULL ),
		m_LastLoreIdentifier( NULL ),
		m_LastPickpocketSuccess( NULL ),
		m_LastPickpocketed( NULL ),
		m_LastPickpocketer( NULL ),
		m_LastPickpocketedGoldCount( NULL ),
		m_LastPickpocketedItem( NULL )
{

	ReadSettings();
}

HooksPlugin::~HooksPlugin(
	)
{

}

void
HooksPlugin::ReadSettings(
	)
{

}


void
HooksPlugin::ApplyPatches(
	)
{
	/*
	// Disable identify on successful lore check.
#if defined(OFFS_USELOREONITEM_HOOK)
	if ( m_HookLoreIdentifiedItem ) {
		m_Patches.push_back( Patch( OFFS_USELOREONITEM_HOOK, "\xe9", 1 ) );
		m_Patches.push_back( Patch( OFFS_USELOREONITEM_HOOK + 1, (relativefunc)IdentifyItemWithLore ) );
		m_TextOut->WriteText( "Loaded patch: UseLoreOnItem (2 patches)\n" );
	}
#endif

	// Apply patches.
	int applied = 0;
	for ( UINT32 i = 0; i < m_Patches.size(); i++ ) {
		UINT32 error = m_Patches[i].Apply();
		if ( error ) m_TextOut->WriteText( "ERROR: Patch at 0x%u failed, error code %u\n", m_Patches[i].offs, error );
		else applied++;
	}
	m_TextOut->WriteText( "Applied %u %s.\n", applied, applied == 1 ? "patch" : "patches" );
	*/
	
	
	int i = 0;

	extern Patch * patches;

	while( patches[ i ].Apply( ) )
		i++;
}


int
HooksPlugin::GetInt(
	__in const char* Function,
	__in const char* Param1,
	__in int Param2
	)
{
	if ( !strcmp( Function, "GetItemBeingIdentified" ) ) {
		return m_LastLoreIdentifiedItem;
	} else if ( !strcmp( Function, "GetItemIdentifier" ) ) {
		return m_LastLoreIdentifier;
	} else {
		return 0;
	}
}