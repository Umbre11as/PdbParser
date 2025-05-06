#pragma once

#include <cstdlib>
#include <cstring>
#include <PDB/Typedefs/Types.h>
#include <PDB/Stream/TPI/TPIStream.h>

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

    class PDB7 {
    public:
        PDB7(PVOID base) : streams(GetStreams(base)) {}
        ~PDB7() {
            delete[] streams;
        }
    public:
        BYTE* RawStream(SIZE_T index) const {
            return streams[index];
        }

        template<class StreamClass = PDBStream>
        StreamClass ReadStream() {
            StreamClass stream;
            stream.Serialize(streams[StreamClass::Index()]);

            return stream;
        }
    private:
        static BYTE* GetStreamDirectory(PVOID base) {
            SuperBlock* super = static_cast<SuperBlock*>(base);
            UINT size = super->NumDirectoryBytes;
            UINT blockSize = super->BlockSize;
            UINT blockCount = (size + blockSize - 1) / blockSize;

            UINT* blockIdArray = reinterpret_cast<UINT*>(static_cast<BYTE*>(base) + blockSize * super->BlockMapAddr);

            BYTE* result = new BYTE[blockCount * blockSize];
            if (!result)
                return nullptr;

            for (UINT i = 0; i < blockCount; i++) {
                BYTE* block = static_cast<BYTE*>(base) + blockSize * blockIdArray[i];
                memcpy(result + i * blockSize, block, blockSize);
            }

            return result;
        }

        static BYTE** GetStreams(PVOID base) {
            SuperBlock* super = static_cast<SuperBlock*>(base);
            UINT blockSize = super->BlockSize;

            BYTE* directory = GetStreamDirectory(base);
            if (!directory)
                return nullptr;

            UINT* cursor = reinterpret_cast<UINT*>(directory);
            UINT streamNumber = *cursor++;
            UINT* streamSizes = cursor;
            cursor += streamNumber;

            BYTE** streams = new BYTE*[streamNumber];
            if (!streams) {
                delete[] directory;
                return nullptr;
            }

            for (UINT i = 0; i < streamNumber; i++) {
                UINT streamSize = streamSizes[i];
                UINT blockCount = (streamSize + blockSize - 1) / blockSize;

                BYTE* buffer = new BYTE[blockCount * blockSize];
                if (!buffer) {
                    for (UINT j = 0; j < i; j++)
                        delete[] streams[j];

                    delete[] streams;
                    delete[] directory;
                    return nullptr;
                }

                for (UINT j = 0; j < blockCount; j++) {
                    UINT blockId = *cursor++;
                    BYTE* block = static_cast<BYTE*>(base) + blockSize * blockId;
                    memcpy(buffer + j * blockSize, block, blockSize);
                }

                streams[i] = buffer;
            }

            delete[] directory;
            return streams;
        }
    private:
        BYTE** streams;
    };
}
