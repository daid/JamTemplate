#include "mainScene.h"
#include "main.h"


Scene::Scene()
: sp::Scene("MAIN")
{
}

void Scene::onUpdate(float delta)
{
    if (escape_key.getUp())
    {
        delete this;
        openMainMenu();
        return;
    }
}
