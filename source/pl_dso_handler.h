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
// Implementation of the Class DSOHandler for the elf2e32 tool
// @internalComponent
// @released
//
//

#if !defined(_PL_DSOHANDLER_H_)
#define _PL_DSOHANDLER_H_
#include "pl_common.h"
#include <list>

using std::list;

class Symbol;
class ElfExecutable;
class ElfProducer;
class ElfConsumer;
class ParameterManager;

/**
This class is for reading the input ELF file. If the input is of type ET_DYN, it
initiates writing the dso file.
@internalComponent
@released

*/
class DSOHandler
{

	typedef std::list<Symbol*>	SymbolList;

public:
	explicit DSOHandler(ParameterManager* aParameterManager);
	~DSOHandler();
	PLUINT32 ReadElfFile(char* aElfFile);
	void ProcessElfFile();
	PLUINT32 WriteElfFile(char* aDsoFullName, char* aDllName, char* aFile, SymbolList& aSymbolList);
	int GetElfExportSymbolList(SymbolList& aList);
	void GetImageDetails(/*E32ImageInterface aImageInterface*/);
	ElfExecutable * ElfExecutableP();

private:
	/** This member handles reading the ELF exucutable file. */
	ElfConsumer* iElfConsumer;
	/** This member is responsible for generating the proxy DSO file. */
	ElfProducer* iElfProducer;
	ParameterManager *iParameterListInterface;
};




#endif // !defined(_PL_DSOHANDLER_H_)
