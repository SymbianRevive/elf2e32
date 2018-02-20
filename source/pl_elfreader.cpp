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
// Implementation of the Class ElfReader for the elf2e32 tool
// @internalComponent
// @released
//
//

#include <string.h>

#include "pl_elfreader.h"
#include "errorhandler.h"

using std::min;
using std::list;


ElfReader::ElfReader(string aElfInput) :
    ElfExecutable(aElfInput)
{}


ElfReader::~ElfReader(){
	DELETE_PTR_ARRAY(iMemBlock);
}


PLUINT32 ElfReader::Read(){
	FILE*	aFd;

	if( (aFd = fopen(iElfInput.c_str(),"rb")) == nullptr) {
		throw Elf2e32Error(FILEOPENERROR, iElfInput);
	}

	fseek(aFd, 0, SEEK_END);

	PLUINT32 aSz = ftell(aFd);
	iMemBlock = new char[aSz];

	fseek(aFd, 0, SEEK_SET);

	// Certain Windows devices (e.g., network shares) limit the size of I/O operations to 64MB
	// or less.  We read all the data in individual KMaxWindowsIOSize (32MB) chunks to be safe.
	PLUINT32 chunkSize = 0;
	for( PLUINT32 bytesRead = 0; bytesRead < aSz; bytesRead += chunkSize) {

		chunkSize = (min)(aSz - bytesRead, PLUINT32(KMaxWindowsIOSize));

		if( fread(iMemBlock + bytesRead, chunkSize, 1, aFd) != 1) {
            fclose(aFd);
			throw Elf2e32Error(FILEREADERROR, iElfInput);
		}
	}
    fclose(aFd);
	return 0;
}


/**
Funtion for getting elf symbol list
@param aList - list of symbols found in elf files
@return - 0 for no exports in elf files, otherwise number of symbols found
@internalComponent
@released
*/
int ElfReader::GetElfSymbols(list<Symbol*>& aList){

	if( !iExports ) return 0;

	//Get the exported symbols
	vector<Symbol*> aTmpList = iExports->GetExports(true);

	typedef vector<Symbol*> List;
	List::iterator aItr = aTmpList.begin();
	while( aItr != aTmpList.end() ){
		aList.push_back((Symbol*) (*aItr));
		++aItr;
	}
	aTmpList.clear();
	return aList.size();
}

/**
Funtion for getting image details
@internalComponent
@released
*/
void ElfReader::GetImageDetails(/*E32ImageInterface aInterface*/){

}


/**
Funtion for processing elf file
@internalComponent
@released
*/
PLUINT32 ElfReader::ProcessElfFile(){

	Elf32_Ehdr *aElfHdr = ELF_ENTRY_PTR(Elf32_Ehdr, iMemBlock, 0);

	try
	{
		ElfExecutable::ProcessElfFile(aElfHdr);

		/* The following is a workaround for the ARM linker problem.
		 * Linker Problem: ARM linker generates Long ARM to Thumb veneers for which
		 * relocation entries are not generated.
		 * The linker problem is resolved in ARM Linker version RVCT 2.2 Build 616.
		 * Hence the workaround is applicable only for executables generated
		 * by ARM linker 2.2 and if build number is below 616.
		 */
		char const aARMCompiler[] = "ARM Linker, RVCT";
		int length = strlen(aARMCompiler);
		char * aCommentSection = ElfExecutable::FindCommentSection();
   		/* The .comment section in an elf file contains the compiler version information and
   		 * it is used to apply the fore mentioned workaround.
   		 * Some build tool chains generating elf file output without the .comment section,
   		 * just to save the disk space. In this case the variable aCommentSection gets the NULL value.
   		 * Solution: This workaround is only applicable for RVCT compiler. So if the .comment section
   		 * is not available in the elf file, then this workaround is no need to be applied.
   		 */
   		if(aCommentSection != NULL)
   		{
		if (!strncmp(aCommentSection, aARMCompiler, length))
		{
			int WorkAroundBuildNo = 616;
			int BuildNo = 0;
			char* RVCTVersion = aCommentSection+length;

			/* RVCTVersion contains the following string
			 * "<MajorVersion>.<MinorVersion> [Build <BuildNumber>]"
			 * Example: "2.2 [Build 616]"
			 */
			string Version(RVCTVersion);
			size_t pos = Version.find_last_of(' ');
			size_t size = Version.size();
			if (pos < size)
			{
				size_t index = pos + 1;
				if (index < size)
				{
					BuildNo = atoi(strtok(RVCTVersion+index, "]"));
				}
			}

			/* Workaround is applicable only when the version is 2.2 and if the
			 * build number is below 616.
			 */
			size_t minorVersionPos = Version.find_first_of('.');
			char RVCTMinorVersion='0';
			if (minorVersionPos < size)
			{
				size_t index = minorVersionPos + 1;
				if (index < size)
				{
					RVCTMinorVersion = *(RVCTVersion + index);
				}
			}

			if ((*RVCTVersion == '2') && (RVCTMinorVersion == '2') &&
				(BuildNo < WorkAroundBuildNo))
			{
				/* The static symbol table should be processed to identify the veneer symbols.
				 * Relocation entries should be generated for these symbols if the linker
				 * is not generating the same.
				 */
				ElfExecutable::FindStaticSymbolTable();
				ElfExecutable::ProcessVeneers();
			}
		}
		}
	}
	catch(ErrorHandler&)
	{
		throw;
	}
	/*catch(...) // If there are any other unhandled exception,they are handled here.
	{
		//Warning to indicate that there had been an exception at this point.
		MessageHandler::GetInstance()->ReportWarning(ELFFILEERROR,(char*)iPrmManager->ElfInput());
		throw;
	} */
	return 0;
}
