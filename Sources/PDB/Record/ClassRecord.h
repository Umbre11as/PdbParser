#pragma once

#include "Record.h"

namespace LightweightPDB {

    enum class ClassOptions : WORD {
        None = 0x0000,
        Packed = 0x0001,
        HasConstructorOrDestructor = 0x0002,
        HasOverloadedOperator = 0x0004,
        Nested = 0x0008,
        ContainsNestedClass = 0x0010,
        HasOverloadedAssignmentOperator = 0x0020,
        HasConversionOperator = 0x0040,
        ForwardReference = 0x0080,
        Scoped = 0x0100,
        HasUniqueName = 0x0200,
        Sealed = 0x0400,
        Intrinsic = 0x2000
    };

    class ClassRecord final : public Record {
    public:
        ClassRecord(UINT index) : Record(LF_CLASS, index) {}
    public:
        void Serialize(unsigned char* buffer) override;
    public:
        WORD MemberCount() const;

        ClassOptions Options() const;

        UINT FieldListIndex() const;

        PCSTR Name() const;
    private:
        WORD memberCount = 0;
        ClassOptions options = ClassOptions::None;
        UINT fieldList;
        UINT derivationList;
        UINT vtableShape;
        WORD size;
        PCSTR name;
    };
}
