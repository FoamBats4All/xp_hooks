/*++

Copyright (c) Michael Wallar (NullCascade). All rights reserved.

Module Name:

	Offsets.h

Abstract:

    This module supplies the virtual address offsets that are used to patch the
	game server.  It abstracts these differences away from the main game server
	extension logic, so that in general, only changes to this module need be
	made to support a new game patch revision level.

--*/

//
// GLOBALS
//

#define OFFS_CAppManager                    (0x0086442C)    // 1.23.1763: CAppManager * g_pAppManager
#define OFFS_CVirtualMachine                (0x00864424)    // 1.23.1763: CVirtualMachine * g_pVirtualMachine

#define OFFS_CVirtualMachine_RunScript      (0x0072B050)    // 1.23.1763: CVirtualMachine::RunScript


//
// LORE IDENTIFY ITEM
//

#define OFFS_CNWSItem_SetIdentified         (0x005D2930)    // 1.23.1763: Offset to function CNWSItem::SetIdentified( ULONG )
#define OFFS_USELOREONITEM_HOOK             (0x005EC620)    // 1.23.1763: The place inside CNWSCreature::UseLoreOnItem where we palce our hook.
#define OFFS_USELOREONITEM_HOOK_RETURN      (0x005EC62B)    // 1.23.1763: 


//
// PICKPOCKET
//

#define OFFS_PICKPOCKET_ENTRY               (0x00000000)    // 1.23.1763: 
#define OFFS_PICKPOCKET_RESUME              (0x00000000)    // 1.23.1763: 

