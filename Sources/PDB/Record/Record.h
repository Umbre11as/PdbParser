#pragma once

#include <PDB/Typedefs/Serializable.h>
#include <PDB/Typedefs/RecordKind.h>

namespace LightweightPDB {

    class Record : public Serializable {
    public:
        Record(RecordKind kind, UINT index) : kind(kind), index(index) {}
        virtual ~Record() {}
    public:
        RecordKind Kind() const {
            return kind;
        }

        UINT Index() const {
            return index;
        }
    private:
        RecordKind kind;
        UINT index;
    };
}
