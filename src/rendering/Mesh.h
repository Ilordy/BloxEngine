#include "core/blx_defines.h"
#include "maths/Vector3.h"
#include "maths/Vector2.h"
#include "utils/List.h"

namespace BlxRendering
{
    struct Vertex
    {
        BlxMath::Vector3 position;
        BlxMath::Vector3 normal;
        BlxMath::Vector2 uv;

        Vertex() : position(0.0f), normal(0.0f), uv(0.0f) {}
        Vertex(const BlxMath::Vector3& pos, const BlxMath::Vector3& norm, const BlxMath::Vector2& texCoord)
            : position(pos), normal(norm), uv(texCoord) {}
    };

    class BLXAPI Mesh
    {
      public:
        Mesh();
        ~Mesh();

        void SetVertices(const BlxStl::List<Vertex>& vertices);

        void SetVertices(BlxStl::List<Vertex>&& vertices);

        void SetIndices(const BlxStl::List<u32>& indices);

        void SetIndices(BlxStl::List<u32>&& indices);

      private:
      BlxStl::List<Vertex> vertices;
      BlxStl::List<u32> indices;
    };
} // namespace BlxRendering