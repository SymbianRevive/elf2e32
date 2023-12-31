// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// Copyright (c) 2017 Strizhniou Fiodar.
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors: Strizhniou Fiodar - fix build and runtime errors.
//
// Description:
// Implementation of the Class SymbolAttrib for the elf2e32 tool
// @internalComponent
// @released
//
//

#if !defined _PL_COMMON_H_
#define _PL_COMMON_H_

#include <cstdint>
#include <string>

#ifdef _MSC_VER
	#pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
	#pragma warning(disable: 4514) // unreferenced inline function has been removed
	#pragma warning(disable: 4702) // unreachable code
	#pragma warning(disable: 4710) // function not inlined
#endif

typedef uint32_t	PLULONG;
typedef uint32_t	PLUINT32;
typedef uint16_t	PLUINT16;
typedef unsigned char	PLUCHAR;
typedef	int32_t			PLINT32;
typedef int16_t			PLINT16;
typedef unsigned char	PLUINT8;
typedef char			PLCHAR;
typedef uint32_t	PLMemAddr32;
typedef uint32_t	PLOffset32;
typedef uint16_t	PLOffset16;

#define DELETE_PTR(aPtr) delete aPtr; aPtr = nullptr;
#define DELETE_PTR_ARRAY(aPtr) delete[] aPtr; aPtr = nullptr;

#define ELF_ENTRY_PTR(ptype, base, offset) \
	((ptype*)((char*)base + offset))

//enum for version category
enum VER_CATEGORY{
	VER_CAT_NONE = 0,
	VER_CAT_DEFINED,
	VER_CAT_NEEDED

};

uint32_t elf_hash(const unsigned char *name);

/**
struct for Version info
@internalComponent
@released
*/
struct VersionInfo {
	char*	iSOName = nullptr;
	char*	iLinkAs = nullptr;
	char	iVerCategory = VER_CAT_NONE;
};

#endif //_PL_COMMON_H_

