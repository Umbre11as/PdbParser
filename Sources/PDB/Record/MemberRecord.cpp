#include "MemberRecord.h"

namespace LightweightPDB {

    void MemberRecord::Serialize(unsigned char* buffer) {
        attributeMask = *reinterpret_cast<WORD*>(buffer);
        typeIndex = *reinterpret_cast<ULONG*>(buffer + 0x2);
        offset = *reinterpret_cast<WORD*>(buffer + 0x6);
        name = reinterpret_cast<PCSTR>(buffer + 0x8);
    }

    ULONG MemberRecord::TypeIndex() const {
        return typeIndex;
    }

    UINT MemberRecord::Offset() const {
        return offset;
    }

    PCSTR MemberRecord::Name() const {
        return name;
    }
}
