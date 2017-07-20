// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// Copyright (c) 2017 Strizhniou Fiodar
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
// Class for E32 Image implementation and dump of the elf2e32 tool
// @internalComponent
// @released
//
//

#ifndef E32IMAGEFILE_H
#define E32IMAGEFILE_H

#include "pl_elfrelocation.h"
#include "pl_elfrelocations.h"
#include <portable.h>
#include <elfdefs.h>

#include <fstream>
#include <vector>
#include <iostream>

using std::vector;
using std::ifstream;
using std::string;

class ELFExecutable;
class ElfFileSupplied;

/**
Class E32ImageChunkDesc for different sections in the E32 image.
@internalComponent
@released
*/
class E32ImageChunkDesc {
    public:
        E32ImageChunkDesc(char * aData, size_t aSize, size_t aOffset, char * aDoc);
        virtual ~E32ImageChunkDesc();
        virtual void Write(char * aPlace);

    public:
        char * iData;
        size_t iSize;
        size_t iOffset;
        char * iDoc;
    };

/**
Class E32ImageChunks for a list of sections in the E32 image.
@internalComponent
@released
*/
class E32ImageChunks {
    public:

        typedef vector<E32ImageChunkDesc *> ChunkList;

    public:
        E32ImageChunks();
        virtual ~E32ImageChunks();

        void AddChunk(char * aData, size_t aSize, size_t aOffset, char * aDoc);
        size_t GetOffset();
        void SetOffset(size_t aOffset);
        ChunkList & GetChunks();

    public:
        ChunkList iChunks;

        size_t iOffset;

    };

typedef unsigned char uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

class ElfFileSupplied;

/**
Class E32ImageFile for fields of an E32 image.
@internalComponent
@released
*/
class E32ImageFile {
    public:
        struct E32RelocPageDesc {
        	uint32_t aOffset;
            uint32_t aSize;
            };

//public:
        E32ImageFile(ElfExecutable * aExecutable, ElfFileSupplied * aUseCase);
        virtual ~E32ImageFile();

        void GenerateE32Image();

        void ReadInputELFFile(string aName, size_t & aFileSize, Elf32_Ehdr * & aELFFile );

        void ProcessImports();
        string FindDSO(string aName);

        void ProcessRelocations();
        void ProcessCodeRelocations();
        void ProcessDataRelocations();
        void CreateRelocations(ElfRelocations::RelocationList & aRelocList, char * & aRelocs, size_t & aRelocSize);
        size_t RelocationsSize(ElfRelocations::RelocationList & aRelocList);

        uint16 GetE32RelocType(ElfRelocation * aReloc);

        void ConstructImage();

        virtual void InitE32ImageHeader();
        virtual size_t GetE32ImageHeaderSize();
        virtual size_t GetExtendedE32ImageHeaderSize();
        virtual void SetExtendedE32ImageHeaderSize(size_t aSize);

        virtual void ComputeE32ImageLayout();
        virtual size_t GetE32ImageSize();
        virtual size_t GetExportOffset();
        virtual void CreateExportBitMap();
        virtual void AddExportDescription();

        virtual void AllocateE32Image();
        virtual void FinalizeE32Image();
        virtual uint16_t GetCpuIdentifier();
        virtual uint32_t EntryPointOffset();

        enum EEntryPointStatus {
            EEntryPointOK,
            EEntryPointCorrupt,
            EEntryPointNotSupported
            };

        EEntryPointStatus ValidateEntryPoint();

        virtual void SetUpExceptions();
        virtual void SetUids();
        virtual void SetSecureId();
        virtual void SetVendorId();
        virtual void SetCallEntryPoints();
        virtual void SetCapability();
        virtual void SetPriority(bool isDllp);
        virtual void SetFixedAddress(bool isDllp);
        virtual void SetVersion();
        virtual void SetCompressionType();
        virtual void SetFPU();
        virtual void SetPaged();
        virtual void SetSymbolLookup();
        virtual void SetDebuggable();
        virtual void SetSmpSafe();

        void UpdateHeaderCrc();

        bool WriteImage(const char * aName);

    public:

        char * iE32Image;
        uint8 * iExportBitMap;
        ElfExecutable * iElfExecutable;

        char* iData;
        ElfFileSupplied * iUseCase;
        E32ImageHeaderV * iHdr;
        size_t iHdrSize;

        E32ImageChunks iChunks;

        uint32 iNumDlls;
        uint32 iNumImports;

        uint32 * iImportSection;
        size_t iImportSectionSize;

        char * iCodeRelocs;
        size_t iCodeRelocsSize;

        char * iDataRelocs;
        size_t iDataRelocsSize;

        size_t iExportOffset;
        bool   iLayoutDone;

        int iMissingExports;

        // This table carries the byte offsets in the import table entries corresponding
        // to the 0th ordinal entry of static dependencies.
        std::vector<int32_t>    iImportTabLocations;
        std::vector<uint32_t> iSymAddrTab;
        std::vector<uint32_t> iSymNameOffTab;
        string      iSymbolNames;
        uint32_t    iSymNameOffset;

    public:
        E32ImageFile();
        TInt ReadHeader(ifstream& is);
        TInt Open(const char* aFileName);
        void Adjust(int32_t aSize, bool aAllowShrink=true);

        TUint TextOffset();
        TUint DataOffset();
        TUint BssOffset();
        TUint32 Capability();
        TUint32 Format();

        void Dump(const char *aFileName,TInt aDumpFlags);
        void DumpHeader(TInt aDumpFlags);
        void DumpData(TInt aDumpFlags);
        void DumpSymbolInfo(E32EpocExpSymInfoHdr *aSymInfoHdr);
        void E32ImageExportBitMap();
        TInt CheckExportDescription();
        void ProcessSymbolInfo();
        char* CreateSymbolInfo(size_t aBaseOffset);
        void SetSymInfo(E32EpocExpSymInfoHdr& aSymInfo);
    public:
        inline TUint OffsetUnadjust(TUint a) const {
            return a ? a-iOrigHdrOffsetAdj : 0;
            }
        inline TUint OrigCodeOffset() const {
            return OffsetUnadjust(iOrigHdr->iCodeOffset);
            }
        inline TUint OrigDataOffset() const {
            return OffsetUnadjust(iOrigHdr->iDataOffset);
            }
        inline TUint OrigCodeRelocOffset() const {
            return OffsetUnadjust(iOrigHdr->iCodeRelocOffset);
            }
        inline TUint OrigDataRelocOffset() const {
            return OffsetUnadjust(iOrigHdr->iDataRelocOffset);
            }
        inline TUint OrigImportOffset() const {
            return OffsetUnadjust(iOrigHdr->iImportOffset);
            }
        inline TUint OrigExportDirOffset() const {
            return OffsetUnadjust(iOrigHdr->iExportDirOffset);
            }
        inline void OffsetAdjust(TUint& a) {
            if (a) a+=iOrigHdrOffsetAdj;
            }

    public:
        TInt iSize;
        E32ImageHeader* iOrigHdr;
        TInt iError;
        TUint iOrigHdrOffsetAdj;
        TInt iFileSize = 0;
    };

ifstream &operator>>(ifstream &is, E32ImageFile &aImage);
void InflateUnCompress(unsigned char* source, int sourcesize, unsigned char* dest, int destsize);


#endif // E32IMAGEFILE_H


