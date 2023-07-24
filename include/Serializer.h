#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <vector>
#include <type_traits>
#include <stdint.h>

#include "utils.h"

namespace iu
{
    template<typename T>
    struct Serializer : std::false_type
    {
    };

    template<typename T>
    concept Serializable = requires(Serializer<T> serializer, std::vector<uint8_t>& buffer, T& toSerialize)
    {
        {serializer.Serialize(buffer, toSerialize)} -> std::same_as<void>;
    };

    template<typename T>
    concept SizeSerializable = requires(Serializer<T> s, T& toSize)
    {
        {s.GetSize(toSize)} -> std::same_as<size_t>;
    };
    
}

#endif //SERIALIZABLE_H