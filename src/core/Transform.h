#pragma once
#include "Component.h"
#include "blx_defines.h"
#include "maths/Quaternion.h"
#include "maths/Vector3.h"

namespace BlxCore
{
    class BLXAPI Transform : public Component
    {
      public:
        Transform();
        ~Transform();

        void Update(f32 deltaTime) override;

      private:
        BlxMath::Vector3 position;
        BlxMath::Quaternion rotation;
        BlxMath::Vector3 scale;
    };
} // namespace BlxCore