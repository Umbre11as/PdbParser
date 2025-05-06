#pragma once

#include <PDB/Record/ClassRecord.h>
#include <PDB/Record/FieldListRecord.h>
#include <PDB/Record/MemberRecord.h>

namespace LightweightPDB {

    class RecordFactory {
    public:
        static ClassRecord* MakeClassRecord(BYTE* buffer, UINT index);

        static FieldListRecord* MakeFieldListRecord(BYTE* buffer, UINT index);

        static MemberRecord* MakeMemberRecord(BYTE* buffer);

        static Record* ByKind(RecordKind kind, UINT index, BYTE* buffer);
    };
}
