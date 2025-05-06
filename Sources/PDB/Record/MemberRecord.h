#pragma once

#include "Record.h"

namespace LightweightPDB {

    class MemberRecord final : public Record {
    public:
        MemberRecord() : Record(LF_MEMBER, 0) {}
    public:
        ULONG TypeIndex() const;

        UINT Offset() const;

        PCSTR Name() const;
    public:
        void Serialize(unsigned char* buffer) override;
    private:
        WORD attributeMask;
        ULONG typeIndex;
        UINT offset;
        PCSTR name;
    };
}
