#include "FieldListRecord.h"

#include <cstring>
#include <PDB/Typedefs/RecordHeader.h>
#include <PDB/Record/Factory/RecordFactory.h>

namespace LightweightPDB {

    void FieldListRecord::Serialize(unsigned char* buffer) {
        RecordHeader* fieldlistHeader = reinterpret_cast<RecordHeader*>(buffer - sizeof(RecordHeader));
        BYTE* data = buffer;
        BYTE* end = buffer + fieldlistHeader->RecordLen;

        while (data < end) {
            if (data + sizeof(RecordKind) > end)
                break;

            RecordKind kind = *reinterpret_cast<RecordKind*>(data);
            data += sizeof(RecordKind);

            switch (kind) {
                case LF_MEMBER: {
                    MemberRecord* memberRecord = RecordFactory::MakeMemberRecord(data);
                    members.Add(memberRecord);

                    break;
                }
                case LF_ENUMERATE:
                case LF_NESTTYPE:
                case LF_STMEMBER:
                case LF_METHOD:
                case LF_ONEMETHOD:
                case LF_BCLASS:
                case LF_VBCLASS:
                case LF_IVBCLASS:
                case LF_INDEX: {
                    WORD subRecordLen = *reinterpret_cast<WORD*>(data);
                    data += 2 + subRecordLen;
                    break;
                }
                default: break;
            }
        }
    }

    List<MemberRecord*> FieldListRecord::Members() const {
        return members;
    }
}
