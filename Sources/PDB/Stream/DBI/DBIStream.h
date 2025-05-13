#pragma once

#include <PDB/Stream/PDBStream.h>
#include <PDB/Typedefs/List.h>

namespace LightweightPDB {

    struct FunctionInfo {
        PCSTR Name;
        UINT Offset;
    };

    class DBIStream final : public PDBStream {
    public:
        DBIStream(PDB* pdb) : PDBStream(pdb) {}
    public:
        void Serialize(unsigned char* buffer) override;
    public:
        List<FunctionInfo> Functions() const;
    public:
        static SIZE_T Index() {
            return 3;
        }
    private:
        enum class DbiStreamVersion : UINT {
            VC41 = 930803,
            V50 = 19960307,
            V60 = 19970606,
            V70 = 19990903,
            V110 = 20091201
        };

        struct DbiStreamHeader {
            INT VersionSignature;
            DbiStreamVersion VersionHeader;
            UINT Age;
            WORD GlobalStreamIndex;
            WORD BuildNumber;
            WORD PublicStreamIndex;
            WORD PdbDllVersion;
            WORD SymRecordStream;
            WORD PdbDllRbld;
            INT ModInfoSize;
            INT SectionContributionSize;
            INT SectionMapSize;
            INT SourceInfoSize;
            INT TypeServerMapSize;
            UINT MFCTypeServerIndex;
            INT OptionalDbgHeaderSize;
            INT ECSubstreamSize;
            WORD Flags;
            WORD Machine;
            UINT Padding;
        };

        // https://code.google.com/archive/p/syzygy/wikis/PdbFileFormat.wiki
        struct DbiDbgHeader {
            UINT FPO;
            UINT Exception;
            UINT FixUp;
            UINT OMapToSrc;
            UINT OMapFromSrc;
            UINT SectionHeader;
            UINT tokenRidMap;
            UINT XData;
            UINT PData;
            UINT NewFPO;
            UINT SectionHeaderOriginal;
        };

        List<FunctionInfo> functions;
    };
}
