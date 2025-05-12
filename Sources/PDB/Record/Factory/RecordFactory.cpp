#include "RecordFactory.h"

namespace LightweightPDB {

    ClassRecord* RecordFactory::MakeClassRecord(BYTE* buffer, UINT index) {
        ClassRecord* record = new ClassRecord(index);
        record->Serialize(buffer);
        if (record->FieldListIndex() == 0) {
            delete record;
            return nullptr;
        }

        return record;
    }

    FieldListRecord* RecordFactory::MakeFieldListRecord(BYTE* buffer, UINT index) {
        FieldListRecord* record = new FieldListRecord(index);
        record->Serialize(buffer);

        return record;
    }

    MemberRecord* RecordFactory::MakeMemberRecord(BYTE* buffer) {
        MemberRecord* record = new MemberRecord;
        record->Serialize(buffer);

        return record;
    }

    Record* RecordFactory::ByKind(RecordKind kind, UINT index, BYTE* buffer) {
        switch (kind) {
            case LF_CLASS:
            case LF_STRUCTURE:
            case LF_UNION:
            case LF_INTERFACE:
                return MakeClassRecord(buffer, index);
            case LF_FIELDLIST:
                return MakeFieldListRecord(buffer, index);
            default:
                return nullptr;
        }
    }
}
