#include "MemberRecord.h"

namespace LightweightPDB {

    enum NumericLeaf : WORD {
        LF_CHAR = 0x8000,
        LF_SHORT = 0x8001,
        LF_USHORT = 0x8002,
        LF_LONG = 0x8003,
        LF_ULONG = 0x8004,
        LF_VARSTRING = 0x8010,

        LF_REAL32 = 0x8005,
        LF_REAL64 = 0x8006,
        LF_REAL80 = 0x8007,
        LF_REAL128 = 0x8008,
    };

    UINT ReadNumericLeafValue(const BYTE*& ptr) {
        if (BYTE first = *ptr; first < 0x80) {
            ++ptr;
            return first;
        }

        WORD kind = *reinterpret_cast<const WORD*>(ptr);
        ptr += sizeof(WORD);

        switch (kind) {
            case LF_CHAR:
                return *reinterpret_cast<const char*>(ptr) & 0xFF;
            case LF_SHORT:
                return *reinterpret_cast<const short*>(ptr);
            case LF_USHORT:
                return *reinterpret_cast<const USHORT*>(ptr);
            case LF_LONG:
                return *reinterpret_cast<const int*>(ptr);
            case LF_ULONG:
                return *reinterpret_cast<const UINT*>(ptr);
            default:
                ptr += 4;
                return 0;
        }
    }

    void MemberRecord::Serialize(unsigned char* buffer) {
        attributeMask = *reinterpret_cast<WORD*>(buffer);
        typeIndex = *reinterpret_cast<ULONG*>(buffer + 0x2);
        const BYTE* ptr = buffer + 6;
        offset = ReadNumericLeafValue(ptr);
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
