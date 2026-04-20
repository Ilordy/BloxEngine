#pragma once
#include "core/blx_defines.h"
#include "rendering/Mesh.h"
#include "utils/Map.h"

namespace BlxCore
{
    class BLXAPI ObjLoader
    {
      public:
        bool Load(const char* objPath, BlxRendering::Mesh** outMeshPtr);

      private:
        struct VertexIndex;
        BlxStl::Map<VertexIndex, u32> vertexToIndexMap;

        bool ParseVertexPosition(const char* line, BlxMath::Vector3& outPosition);
        bool ParseVertexNormal(const char* line, BlxMath::Vector3& outNormal);
        bool ParseVertexTexCoord(const char* line, BlxMath::Vector2& outTexCoord);
        BlxStl::List<VertexIndex> ParseFace(char* line);
        void FanTriangulateFace(BlxStl::List<VertexIndex>& vertexIndices);
    };
} // namespace BlxCore