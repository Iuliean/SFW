#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <concepts>
#include <vector>
#include <type_traits>
#include <stdint.h>

namespace iu
{
    template<typename T>
    struct Serializer : std::false_type
    {
    };

    template<typename U, template<typename> typename SerializerT = Serializer>
    concept Serializable = requires (SerializerT<U> serializer, std::vector<uint8_t>& buffer, const U& toSerialize)
    {
        {serializer.Serialize(buffer, toSerialize)} -> std::same_as<void>;
    };

    template<typename T>
    concept SizeSerializable = requires(Serializer<T> s, const T& toSize)
    {
        {s.GetSize(toSize)} -> std::same_as<size_t>;
    };
    
}

#endif //SERIALIZABLE_H