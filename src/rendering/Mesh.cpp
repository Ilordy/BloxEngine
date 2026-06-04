#include "Mesh.h"

namespace BlxRendering
{
    Mesh::Mesh() : vertices(), indices() {}

    Mesh::~Mesh() {}

    void Mesh::SetVertices(const BlxStl::List<Vertex>& vertices)
    {
        this->vertices = vertices;
    }

    void Mesh::SetVertices(BlxStl::List<Vertex>&& vertices)
    {
        this->vertices = std::move(vertices);
    }

    void Mesh::SetIndices(const BlxStl::List<u32>& indices)
    {
        this->indices = indices;
    }

    void Mesh::SetIndices(BlxStl::List<u32>&& indices)
    {
        this->indices = std::move(indices);
    }
} // namespace BlxRendering