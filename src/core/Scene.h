#pragma once
#include "GameObject.h"
#include "blx_defines.h"

// TODO: gameobject creation only allowed by scene should be reworked later...

namespace BlxCore
{
    class BLXAPI Scene
    {
      public:
        Scene();
        ~Scene();

        void Update(f32 deltaTime);

      private:
        BlxStl::List<GameObject*> gameObjects;
        friend GameObject* GameObject::Create();
        friend GameObject* GameObject::Create(Scene* scene);
    };
} // namespace BlxCore