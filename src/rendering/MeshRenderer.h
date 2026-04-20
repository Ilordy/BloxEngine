#pragma once
#include "Material.h"
#include "Mesh.h"
#include "core/Component.h"
#include "core/blx_defines.h"
#include "utils/List.h"

namespace BlxRendering
{
    class BLXAPI MeshRenderer : public BlxCore::Component
    {
      public:
        MeshRenderer();
        ~MeshRenderer();

        void Update(f32 deltaTime) override;

        

      private:
        Mesh* mesh;
        // TODO: Add support for multiple materials in the future, but for now we will just use one material per mesh.
        // something like a list of material pointers that can be indexed by the mesh data to determine which material to use for each part of the mesh.
        Material* material;
    };
} // namespace BlxRendering