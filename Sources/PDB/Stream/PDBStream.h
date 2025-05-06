#pragma once

#include <PDB/Typedefs/Types.h>
#include <PDB/Typedefs/Serializable.h>

namespace LightweightPDB {

    class PDBStream : public Serializable {
    public:
        virtual ~PDBStream() {}
    public:
        static SIZE_T Index() {
            return 0;
        }
    };
}
