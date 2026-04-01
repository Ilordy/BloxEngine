#pragma once
#include "Component.h"
#include "Transform.h"
#include "blx_defines.h"
#include "utils/List.h"
#include "utils/MemHelper.h"
#include <type_traits>


namespace BlxCore
{
    class Scene;

    class BLXAPI GameObject
    {
      public:
        ~GameObject();

        template <typename T>
        T* AddComponent()
        {
            static_assert(std::is_base_of<BlxCore::Component, T>::value, "T must inherit from BlxCore::Component");

            // TODO: Component allocations should be handled by a memory pool or some sort of custom allocator for better performance and memory management.
            // TODO: this function or the component itself should be able to provide an allocator for better memory management. maybe we can have a tag for each component type and use that to allocate from the correct pool.
            T* component = BlxMem::New<T>(BLXMEMORY_TAG_GAME);
            component->owner = this;
            component->Init();
            components.Add(component);
            return component;
        }

        template <typename T>
        T* GetComponent()
        {
            for (u64 i = 0; i < components.Count(); i++)
            {
                T* casted = dynamic_cast<T*>(components[i]);
                if (casted)
                {
                    return casted;
                }
            }

            return nullptr;
        }

        template <typename T>
        bool TryGetComponent(T** outComponent)
        {
            T* component = GetComponent<T>();
            if (component)
            {
                *outComponent = component;
                return true;
            }
            else
            {
                *outComponent = nullptr;
                return false;
            }
        }

        inline Transform& transform()
        {
            return *_transform;
        }

        static GameObject* Create();
        static GameObject* Create(Scene* scene);

      private:
        BlxStl::List<Component*> components;
        Transform* _transform;

        GameObject();
        void Update(float deltaTime);
        friend class Scene;
    };
} // namespace BlxCore