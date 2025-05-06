#include "TPIStream.h"

namespace LightweightPDB {

    void TPIStream::Serialize(unsigned char* buffer) {
        TpiStreamHeader* header = reinterpret_cast<TpiStreamHeader*>(buffer);
        version = header->Version;

        UINT typeIndex = header->TypeIndexBegin;
        for (UINT offset = 0; offset < header->TypeRecordBytes; ) {
            BYTE* recordPointer = buffer + header->HeaderSize + offset;
            RecordHeader* recordHeader = reinterpret_cast<RecordHeader*>(recordPointer);

            if (Record* record = RecordFactory::ByKind(recordHeader->RecordKind, typeIndex, recordPointer + sizeof(RecordHeader)))
                records.Add(record);

            offset += recordHeader->RecordLen + sizeof(recordHeader->RecordLen);
            typeIndex++;
        }
    }

    TpiStreamVersion TPIStream::Version() const {
        return version;
    }

    List<Record*> TPIStream::Records() const {
        return records;
    }
}
