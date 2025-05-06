#pragma once

#include <PDB/Typedefs/List.h>
#include "MemberRecord.h"

namespace LightweightPDB {

    class FieldListRecord final : public Record {
    public:
        FieldListRecord(UINT index) : Record(LF_FIELDLIST, index) {}
    public:
        List<MemberRecord*> Members() const;
    public:
        void Serialize(unsigned char* buffer) override;
    private:
        List<MemberRecord*> members{};
    };
}
