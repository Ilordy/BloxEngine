#pragma once
#include "core/blx_defines.h"
#include "utils/Map.h"
#include <functional>

namespace BlxCore
{
    struct VertexIndex
    {
        u32 posIndex, texIndex, normIndex;

        VertexIndex() : posIndex(0), texIndex(0), normIndex(0) {}

        bool operator==(const VertexIndex& other) const
        {
            return posIndex == other.posIndex && texIndex == other.texIndex && normIndex == other.normIndex;
        }
    };
} // namespace BlxCore

namespace BlxStl
{
    template <>
    struct Hash<BlxCore::VertexIndex>
    {
        u64 operator()(const BlxCore::VertexIndex& vertex)
        {
            u64 hash = 0;
            hash ^= std::hash<u32>()(vertex.posIndex) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<u32>()(vertex.texIndex) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<u32>()(vertex.normIndex) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
} // namespace BlxStl