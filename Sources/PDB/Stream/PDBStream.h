#pragma once

#include <PDB/Typedefs/Serializable.h>
#include <PDB/Typedefs/Types.h>
#include <PDB/PDB.h>

namespace LightweightPDB {

    class PDBStream : public Serializable {
    public:
        PDBStream(PDB* pdb) : pdb(pdb) {}
        virtual ~PDBStream() {}
    public:
        static SIZE_T Index() {
            return 0;
        }
    protected:
        PDB* pdb;
    };
}
