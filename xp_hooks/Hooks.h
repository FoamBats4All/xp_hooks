/*++

Copyright (c) Michael Wallar (NullCascade). All rights reserved.

Module Name:

	Hooks.h

Abstract:

	Defines each patch implementation, and accompanying functions.

--*/

#ifndef _SOURCE_PROGRAMS_XPHOOKS_HOOKS_H
#define _SOURCE_PROGRAMS_XPHOOKS_HOOKS_H

#ifdef _MSC_VER
#pragma once
#endif

#ifdef _M_IX86
#define CLIEXT_DECLSPEC_NAKED __declspec( naked )
#else
#define CLIEXT_DECLSPEC_NAKED
#endif

class HooksPlugin;
struct CNWSCreature;

void SetPlugin( HooksPlugin* );

//
// LORE IDENTIFY ITEM
//

void SetItemBeingIdentified( DWORD );
void SetItemIdentifier( DWORD );
void IdentifyItemWithLoreInternal( CNWSCreature*, DWORD );
void IdentifyItemWithLore();


//
// PICKPOCKET
//

void SetLastPickpocketed( DWORD );
void SetLastPickpocketer( DWORD );
void SetLastPickpocketedGoldCount( unsigned int );
void SetLastPickpocketedItem( unsigned int );
void OnPickPocket();


#endif