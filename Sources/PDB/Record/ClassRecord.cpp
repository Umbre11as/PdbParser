#include "ClassRecord.h"

namespace LightweightPDB {

    void ClassRecord::Serialize(unsigned char* buffer) {
        memberCount = *reinterpret_cast<WORD*>(buffer);
        options = *reinterpret_cast<ClassOptions*>(buffer + 0x2);
        fieldList = *reinterpret_cast<UINT*>(buffer + 0x4);
        derivationList = *reinterpret_cast<UINT*>(buffer + 0x8);
        vtableShape = *reinterpret_cast<UINT*>(buffer + 0xC);
        size = *reinterpret_cast<WORD*>(buffer + 0x10);
        name = reinterpret_cast<PCSTR>(buffer + 0x12);
    }

    WORD ClassRecord::MemberCount() const {
        return memberCount;
    }

    ClassOptions ClassRecord::Options() const {
        return options;
    }

    UINT ClassRecord::FieldListIndex() const {
        return fieldList;
    }

    PCSTR ClassRecord::Name() const {
        return name;
    }
}
