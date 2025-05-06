#pragma once

#include "RecordKind.h"

namespace LightweightPDB {

    struct RecordHeader {
        WORD RecordLen;
        RecordKind RecordKind;
    };
}
