#pragma once

#include "PDB.h"
#include "Stream/PDBStream.h"
#include "Stream/TPI/TPIStream.h"
#include "Stream/DBI/DBIStream.h"

namespace LightweightPDB {

    class PDB7 final : public PDB {
    public:
        PDB7(PCSTR originalFile, PVOID base);
    public:
        List<BYTE> Stream(SIZE_T index) const override;

        BYTE* RawStream(SIZE_T index) const override;

        List<List<BYTE>> Streams() const override;
    public:
        template<class StreamClass = PDBStream>
        StreamClass ReadStream() {
            StreamClass stream(this);
            stream.Serialize(RawStream(StreamClass::Index()));

            return stream;
        }
    private:
        List<List<BYTE>> streams;
    };
}
