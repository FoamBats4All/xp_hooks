/*++

Copyright (c) Michael Wallar (NullCascade). All rights reserved.

Module Name:

	NWN2.h

Abstract:

	Defines structures and operations found within nwn2main.exe

--*/

#include "Precomp.h"

#ifndef _SOURCE_PROGRAMS_XPHOOKS_NWN2_H
#define _SOURCE_PROGRAMS_XPHOOKS_NWN2_H

#ifdef _MSC_VER
#pragma once
#endif


//
// NWN2 data structure definitions.
//

struct CExoString
{
	const char * String;
	int Length;
};
C_ASSERT( sizeof( CExoString ) == 0x8 );

struct CExoArrayList
{
	void * Element;
	unsigned long Index;
	unsigned long ArraySize;
};
C_ASSERT( sizeof( CExoArrayList ) == 0xC );

struct CVirtualMachine
{
	unsigned char Fill0[0x40C];
};
C_ASSERT( sizeof( CVirtualMachine ) == 0x40C );

union CScriptParameterNumValue {
	float Float;
	int Int;
	unsigned long Object;
};
C_ASSERT( sizeof( CScriptParameterNumValue ) == 0x4 );

struct CScriptParameterWrapper
{
	unsigned char Fill0[0x4];
	CScriptParameterNumValue NumValue;
	CExoString StringValue;
	unsigned long Type;
};
C_ASSERT( sizeof( CScriptParameterWrapper ) == 0x14 );

struct CGameObject
{
	unsigned char Fill0[0x9F];
	DWORD m_ObjectId;
	BYTE m_nObjectType;
	unsigned char Fill1[0x1B];
};
C_ASSERT( offsetof( CGameObject, m_ObjectId ) == 0xA0 );
C_ASSERT( offsetof( CGameObject, m_nObjectType ) == 0xA4 );
C_ASSERT( sizeof( CGameObject ) == 0xC0 );

struct CNWSObject
{
	CGameObject AsCGameObject;
	unsigned char Fill0[0x250];
};
C_ASSERT( sizeof( CNWSObject ) == 0x310 );

struct CNWSCreature
{
	CNWSObject AsCNWSObject;
	unsigned char Fill0[0x1D80];
};
C_ASSERT( sizeof( CNWSCreature ) == 0x2090 );


//
// NWN2 function definitions.
//

typedef int (__stdcall * CVirtualMachine_RunScript)(
	CVirtualMachine * VM,
	CExoString * ScriptName,
	unsigned long ObjectId,
	CExoArrayList const & Params,
	int Unknown0,
	int Unknown1
	);

#endif