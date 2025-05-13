#pragma once

#include <cstdlib>
#include <PDB/Typedefs/Types.h>

namespace LightweightPDB {

    template<typename T>
    class List {
    public:
        List() : buffer(nullptr), size(0), capacity(0) {}
    public:
        T& operator[](SIZE_T index) {
            return buffer[index];
        }

        const T& operator[](SIZE_T index) const {
            return buffer[index];
        }
    public:
        T* begin() { return buffer; }
        T* end() { return buffer + size; }

        const T* begin() const { return buffer; }
        const T* end() const { return buffer + size; }
    public:
        BYTE* Data() {
            return buffer;
        }

        BYTE* ConstData() const {
            return buffer;
        }

        void Add(const T& value) {
            if (size >= capacity)
                Reserve(capacity == 0 ? 8 : capacity * 2);

            buffer[size++] = value;
        }

        void Set(T* data) {
            buffer = data;
        }

        void Reserve(SIZE_T newCapacity) {
            T* newBuffer = static_cast<T*>(realloc(buffer, newCapacity * sizeof(T)));
            if (!newBuffer)
                return;

            buffer = newBuffer;
            capacity = newCapacity;
        }

        template<typename Predicate>
        T* FindIf(Predicate predicate) {
            for (T& value : *this) {
                if (predicate(value))
                    return &value;
            }

            return nullptr;
        }

        SIZE_T Size() const {
            return size;
        }
    private:
        T* buffer;
        SIZE_T size;
        SIZE_T capacity;
    };
}
