#include "Scene.h"
#include "GameObject.h"
#include "blx_application.h"

namespace BlxCore
{
    Scene::Scene()
    {
    }

    void Scene::Update(f32 deltaTime)
    {
        for (u64 i = 0; i < gameObjects.Count(); i++)
        {
            gameObjects[i]->Update(deltaTime);
        }
    }

    Scene::~Scene()
    {
        // for (u64 i = 0; i < gameObjects.Count(); i++)
        // {
        //     delete gameObjects[i];
        // }
        // gameObjects.Clear();
    }

    // GameObject* Scene::CreateGameObject()
    // {
    //     // TODO: idk if i like this idea, might be better for the gameobject class itself to have a create func....
    //     void* mem = blxAllocate(sizeof(GameObject), BLXMEMORY_TAG_GAME);
    //     GameObject* newObject = BLX_CONSTRUCT(mem, GameObject);
    //     //GameObject* newObject = BlxMem::New<GameObject>(BLXMEMORY_TAG_GAME);
    //     //blxGetMainScene()->gameObjects.Add(BlxMem::New<GameObject>(BLXMEMORY_TAG_GAME));
    //     //return newObject;
    // }
} // namespace BlxCore