#pragma once
#include "blx_defines.h"

namespace BlxCore
{
    class GameObject;

    class BLXAPI Component
    {
      public:
        
        Component();
        virtual ~Component();
        
        virtual void Init() {}
        virtual void Update(f32 deltaTime) {}
        virtual void OnDestroy() {}

        inline GameObject* gameObject() const
        {
            return owner;
        }

      protected:
        friend class GameObject;
        GameObject* owner;
    };
} // namespace BlxCore