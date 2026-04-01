#pragma once
#include "core/Component.h"
#include "core/blx_defines.h"

namespace BlxRendering
{
    class BLXAPI MeshRenderer : public BlxCore::Component
    {
      public:
        MeshRenderer();
        ~MeshRenderer();

        void Update(f32 deltaTime) override;

      private:
    };
} // namespace BlxRendering