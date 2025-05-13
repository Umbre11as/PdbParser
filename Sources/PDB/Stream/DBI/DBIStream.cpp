#include "DBIStream.h"

#include <cstdio>
#include <Windows.h>

namespace LightweightPDB {

    enum class PublicSymbols : WORD {
        S_PUB32 = 0x110e
    };

#pragma pack(push, 1)
    struct PUBSYM32 {
        WORD RecordLen;
        PublicSymbols RecordType;
        UINT Flags;
        UINT Offset;
        WORD Segment;
        char Name[1];
    };

#pragma pack(pop)

    void DBIStream::Serialize(unsigned char* buffer) {
        HANDLE fileHandle = CreateFile(pdb->OriginalFile(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (fileHandle == INVALID_HANDLE_VALUE)
            return;

        HANDLE mapping = CreateFileMapping(fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!mapping)
            return;

        BYTE* exeBase = static_cast<BYTE*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0));
        if (!exeBase)
            return;

        PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(exeBase);
        PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(exeBase + dosHeader->e_lfanew);

        PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(ntHeaders);

        DbiStreamHeader* header = reinterpret_cast<DbiStreamHeader*>(buffer);
        List<BYTE> symbolStream = pdb->Stream(header->SymRecordStream);

        BYTE* data = symbolStream.Data();
        BYTE* end = data + symbolStream.Size();
        while (data < end) {
            PUBSYM32* symbol = reinterpret_cast<PUBSYM32*>(data);
            if (symbol->RecordType == PublicSymbols::S_PUB32) {
                UINT offset = sections[symbol->Segment - 1].VirtualAddress + symbol->Offset;
                functions.Add(FunctionInfo{ symbol->Name, offset });
            }

            data += symbol->RecordLen + sizeof(symbol->RecordLen);
        }
    }

    List<FunctionInfo> DBIStream::Functions() const {
        return functions;
    }
}
