#include "PDB7.h"

#include <cstring>

namespace LightweightPDB {

#pragma pack(push, 1)
    constexpr char PDB7_SIGNATURE[] = {
        0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x2F,
        0x43, 0x2B, 0x2B, 0x20, 0x4D, 0x53, 0x46, 0x20, 0x37, 0x2E, 0x30, 0x30,
        0x0D, 0x0A, 0x1A, 0x44, 0x53, 0x00, 0x00, 0x00
    };

    // https://github.com/microsoft/llvm/blob/master/docs/PDB/MsfFile.rst
    struct SuperBlock {
        char FileMagic[sizeof(PDB7_SIGNATURE)];
        UINT BlockSize;
        UINT FreeBlockMapBlock;
        UINT NumBlocks;
        UINT NumDirectoryBytes;
        UINT Unknown;
        UINT BlockMapAddr;
    };
#pragma pack(pop)

    List<BYTE> GetStreamDirectory(PVOID base) {
        SuperBlock* super = static_cast<SuperBlock*>(base);
        UINT size = super->NumDirectoryBytes;
        UINT blockSize = super->BlockSize;
        UINT blockCount = (size + blockSize - 1) / blockSize;

        UINT* blockIdArray = reinterpret_cast<UINT*>(static_cast<BYTE*>(base) + blockSize * super->BlockMapAddr);

        List<BYTE> result;
        result.Reserve(blockCount * blockSize);
        for (UINT i = 0; i < blockCount; i++) {
            BYTE* block = static_cast<BYTE*>(base) + blockSize * blockIdArray[i];
            memcpy(result.Data() + i * blockSize, block, blockSize);
        }

        return result;
    }

    List<List<BYTE>> GetStreams(PVOID base) {
        List<List<BYTE>> streams;

        SuperBlock* super = static_cast<SuperBlock*>(base);
        UINT blockSize = super->BlockSize;

        List<BYTE> directory = GetStreamDirectory(base);
        UINT* cursor = reinterpret_cast<UINT*>(directory.Data());
        UINT streamNumber = *cursor++;
        streams.Reserve(streamNumber);

        UINT* streamSizes = cursor;
        cursor += streamNumber;

        for (UINT i = 0; i < streamNumber; i++) {
            UINT streamSize = streamSizes[i];
            UINT blockCount = (streamSize + blockSize - 1) / blockSize;

            List<BYTE> buffer;
            buffer.Reserve(blockCount * blockSize);

            for (UINT j = 0; j < blockCount; j++) {
                UINT blockId = *cursor++;
                BYTE* block = static_cast<BYTE*>(base) + blockSize * blockId;
                for (UINT b = 0; b < blockSize; ++b)
                    buffer.Add(block[b]);
            }

            if (buffer.Size() > streamSize) {
                List<BYTE> trimmed;
                trimmed.Reserve(streamSize);
                for (UINT b = 0; b < streamSize; b++)
                    trimmed.Add(buffer[b]);

                streams.Add(trimmed);
            } else
                streams.Add(buffer);
        }

        return streams;
    }

    PDB7::PDB7(PCSTR originalFile, PVOID base) : PDB(originalFile), streams(GetStreams(base)) {}

    List<BYTE> PDB7::Stream(SIZE_T index) const {
        return streams[index];
    }

    BYTE* PDB7::RawStream(SIZE_T index) const {
        return Stream(index).ConstData();
    }

    List<List<BYTE>> PDB7::Streams() const {
        return streams;
    }
}
