// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// Copyright (c) 2017-2018 Strizhniou Fiodar
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
// Implementation of the Class DLLTarget for the elf2e32 tool
// @internalComponent
// @released
//
//

#include "dll_type_target.h"

/**
Constructor for the DLLTarget Class

@internalComponent
@released
*/
DLLTarget::DLLTarget(ParameterManager* aParams) : ElfFileSupplied(aParams), iDefExports(nullptr) {
}

/**
Destructor for the DLLTarget Class

@internalComponent
@released
*/
DLLTarget::~DLLTarget()
{
    if(iDefExports)
	{
		for(auto x: *iDefExports)
		{
			delete x;
		}
		iDefExports->clear();
	}
}

/**
Function to check if the provided input is a DLL.

@internalComponent
@released

@result True for DLL Type Targets.
*/
bool DLLTarget::ImageIsDll()
{
	return ((iParameterManager->TargetTypeName() == EDll)||
	(iParameterManager->TargetTypeName() == EPolyDll));
}
