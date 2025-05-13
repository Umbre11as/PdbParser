#pragma once

#include <PDB/Typedefs/List.h>

namespace LightweightPDB {

    class PDB {
    public:
        PDB(PCSTR originalFile) : originalFile(originalFile) {}
        virtual ~PDB() {}
    public:
        virtual List<BYTE> Stream(SIZE_T index) const = 0;

        virtual BYTE* RawStream(SIZE_T index) const = 0;

        virtual List<List<BYTE>> Streams() const = 0;
    public:
        PCSTR OriginalFile() const {
            return originalFile;
        }
    protected:
        PCSTR originalFile;
    };
}
