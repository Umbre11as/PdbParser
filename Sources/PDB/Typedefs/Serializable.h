#pragma once

namespace LightweightPDB {

    class Serializable {
    public:
        virtual ~Serializable() {}
    public:
        virtual void Serialize(unsigned char* buffer) = 0;
    };
}
