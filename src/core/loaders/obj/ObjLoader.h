#pragma once
#include "ObjVertex.h"
#include "core/blx_defines.h"
#include "maths/Vector3.h"
#include "rendering/Mesh.h"
#include "utils/List.h"
#include "utils/Map.h"

namespace BlxCore
{
    class BLXAPI ObjLoader
    {
      public:
        ObjLoader(const char* objPath);
        ~ObjLoader();
        bool Load(const char* objPath, BlxRendering::Mesh** outMeshPtr);

      private:
        friend class BlxStl::Map<VertexIndex, u32>;
        friend struct BlxStl::Hash<VertexIndex>;
        // TODO: Make objpath a string variable type when we make our own string class.
        const char* objPath;
        BlxStl::Map<VertexIndex, u32> vertexToIndexMap;
        BlxStl::List<BlxMath::Vector3> positions;
        BlxStl::List<BlxMath::Vector2> texCoords;
        BlxStl::List<BlxMath::Vector3> normals;

        bool ParseVertexPosition(const char* line, BlxMath::Vector3& outPosition);
        bool ParseVertexNormal(const char* line, BlxMath::Vector3& outNormal);
        bool ParseVertexTexCoord(const char* line, BlxMath::Vector2& outTexCoord);
        i32 ResolveVertexIndex(i32 objIndex, u32 listSize);
        bool ParseFace(char* line, BlxStl::List<VertexIndex>& outVertexIndices);
        void FanTriangulateFace(BlxStl::List<VertexIndex>& vertexIndices);
    };
} // namespace BlxCore