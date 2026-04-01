#include "GameObject.h"
#include "Component.h"
#include "Scene.h"
#include "blx_application.h"
#include "blx_defines.h"

namespace BlxCore
{
    GameObject::GameObject() : _transform(AddComponent<Transform>())
    {
        _transform->owner = this;
        _transform->Init();
    }

    void GameObject::Update(f32 deltaTime)
    {
        for (u64 i = 0; i < components.Count(); i++)
        {
            components[i]->Update(deltaTime);
        }
    }

    GameObject* GameObject::Create(Scene* scene)
    {
        void* mem = blxAllocate(sizeof(GameObject), BLXMEMORY_TAG_GAME);
        GameObject* newObject = BLX_CONSTRUCT(mem, GameObject);
        scene->gameObjects.Add(newObject);
        return newObject;
    }

    GameObject* GameObject::Create()
    {
        return Create(blxGetMainScene());
    }

    GameObject::~GameObject()
    {
        for(u64 i = 0; i < components.Count(); i++)
        {
            BlxMem::DestructAndFree(components[i], BLXMEMORY_TAG_GAME);
        }
    }
} // namespace BlxCore