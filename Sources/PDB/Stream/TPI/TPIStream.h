#pragma once

#include <iostream>
#include <PDB/Stream/PDBStream.h>
#include <PDB/Typedefs/RecordHeader.h>
#include <PDB/Record/Factory/RecordFactory.h>
#include <PDB/Typedefs/List.h>

namespace LightweightPDB {

    enum class TpiStreamVersion : UINT {
        None = 0,
        V40 = 19950410,
        V41 = 19951122,
        V50 = 19961031,
        V70 = 19990903,
        V80 = 20040203,
    };

    struct TpiRecordEntry {
        UINT TypeIndex;
        RecordHeader* RecordHeader;
    };

    class TPIStream final : public PDBStream {
    public:
        TPIStream(PDB* pdb) : PDBStream(pdb) {}
    public:
        void Serialize(unsigned char* buffer) override;
    public:
        TpiStreamVersion Version() const;

        List<Record*> Records() const;

        template<class RecordClass = Record>
        RecordClass* WithTypeIndex(UINT index) {
            Record** found = records.FindIf([index](const Record* record) {
                return record->Index() == index;
            });
            if (found)
                return dynamic_cast<RecordClass*>(*found);

            return nullptr;
        }

        ClassRecord* FindClass(PCSTR name) {
            Record** found = records.FindIf([name](const Record* record) {
                if (const ClassRecord* classRecord = dynamic_cast<const ClassRecord*>(record)) {
                    if (strcmp(classRecord->Name(), name) == 0)
                        return true;
                }

                return false;
            });
            if (found)
                return dynamic_cast<ClassRecord*>(*found);

            return nullptr;
        }
    public:
        static SIZE_T Index() {
            return 2;
        }
    private:
        TpiStreamVersion version = TpiStreamVersion::None;
        List<Record*> records{};

        struct TpiStreamHeader {
            TpiStreamVersion Version;
            UINT HeaderSize; // sizeof(TpiStreamHeader)
            UINT TypeIndexBegin; // First 0x1000 are basic types
            UINT TypeIndexEnd;
            UINT TypeRecordBytes;

            WORD HashStreamIndex;
            WORD HashAuxStreamIndex;
            UINT HashKeySize;
            UINT NumHashBuckets;

            INT HashValueBufferOffset;
            UINT HashValueBufferLength;

            INT IndexOffsetBufferOffset;
            UINT IndexOffsetBufferLength;

            INT HashAdjBufferOffset;
            UINT HashAdjBufferLength;
        };
    };
}
