// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// Copyright (c) 2017-2018 Strizhniou Fiodar.
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
// Implementation of the Class ElfRelocation for the elf2e32 tool
// @internalComponent
// @released
//
//

#include "pl_elfrelocation.h"
#include "pl_elfimportrelocation.h"
#include "pl_elflocalrelocation.h"
#include "pl_symbol.h"

/**
Constructor for class ElfRelocation
@param aElfExec - Instance of class ELfExecutable
@param aAddr
@param aAddend
@param aIndex
@param aRelType - Relocation type
@param aRel
@internalComponent
@released
*/
ElfRelocation::ElfRelocation(ElfImage *aElfExec, PLMemAddr32 aAddr,
		PLUINT32 aAddend, PLUINT32 aIndex, PLUCHAR aRelType,
		Elf32_Rel* aRel) : iAddr(aAddr), iAddend(aAddend),
		iSymNdx(aIndex), iRelType(aRelType), iRel(aRel),
		iElfImage(aElfExec) {}


/**
This function verifies if the relocation entry is required to be
handled by the postlinker.
@param aType - type of relocation
@return - True if valid relocation type, otherwise false
@internalComponent
@released
*/
bool ElfRelocation::ValidRelocEntry(PLUCHAR aType) {

	switch(aType)
	{
	case R_ARM_ABS32:
	case R_ARM_GLOB_DAT:
	case R_ARM_JUMP_SLOT:
	case R_ARM_RELATIVE:
	case R_ARM_GOT_BREL:
		return true;
	case R_ARM_NONE:
		return false;
	default:
		return false;
	}
}
