#include "mainScene.h"
#include "main.h"


Scene::Scene()
: sp::Scene("MAIN")
{
}

void Scene::onUpdate(float delta)
{
    if (escape_key.getDown())
    {
        delete this;
        openMainMenu();
        return;
    }
}
