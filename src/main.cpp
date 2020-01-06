#include <sp2/engine.h>
#include <sp2/window.h>
#include <sp2/logging.h>
#include <sp2/io/resourceProvider.h>
#include <sp2/audio/sound.h>
#include <sp2/audio/music.h>
#include <sp2/graphics/gui/scene.h>
#include <sp2/graphics/gui/theme.h>
#include <sp2/graphics/gui/loader.h>
#include <sp2/graphics/scene/graphicslayer.h>
#include <sp2/graphics/scene/basicnoderenderpass.h>
#include <sp2/graphics/scene/collisionrenderpass.h>
#include <sp2/graphics/textureManager.h>
#include <sp2/scene/scene.h>
#include <sp2/scene/node.h>
#include <sp2/scene/camera.h>
#include <sp2/io/keybinding.h>

#include "main.h"
#include "mainScene.h"


sp::P<sp::Window> window;
sp::io::Keybinding escape_key("ESCAPE", {"Escape", "AC Back"});
Controller controller;

static void openOptionsMenu();
static void openCreditsMenu();
void openMainMenu()
{
    sp::P<sp::gui::Widget> menu = sp::gui::Loader::load("gui/main_menu.gui", "MAIN_MENU");
    menu->getWidgetWithID("START")->setEventCallback([=](sp::Variant v) mutable {
        menu.destroy();
        new Scene();
    });
    menu->getWidgetWithID("OPTIONS")->setEventCallback([=](sp::Variant v) mutable {
        menu.destroy();
        openOptionsMenu();
    });
    menu->getWidgetWithID("CREDITS")->setEventCallback([=](sp::Variant v) mutable {
        menu.destroy();
        openCreditsMenu();
    });
    menu->getWidgetWithID("QUIT")->setEventCallback([](sp::Variant v){
        sp::Engine::getInstance()->shutdown();
    });
#ifdef EMSCRIPTEN
    menu->getWidgetWithID("QUIT")->hide();
#endif
}

static void openOptionsMenu()
{
    sp::P<sp::gui::Widget> menu = sp::gui::Loader::load("gui/main_menu.gui", "OPTIONS_MENU");
    menu->getWidgetWithID("EFFECT_VOLUME")->getWidgetWithID("VALUE")->setAttribute("caption", sp::string(int(sp::audio::Sound::getVolume())) + "%");
    menu->getWidgetWithID("EFFECT_VOLUME")->getWidgetWithID("SLIDER")->setAttribute("value", sp::string(sp::audio::Sound::getVolume()));
    menu->getWidgetWithID("EFFECT_VOLUME")->getWidgetWithID("SLIDER")->setEventCallback([=](sp::Variant v) mutable {
        menu->getWidgetWithID("EFFECT_VOLUME")->getWidgetWithID("VALUE")->setAttribute("caption", sp::string(v.getInteger()) + "%");
        sp::audio::Sound::setVolume(v.getInteger());
    });
    menu->getWidgetWithID("MUSIC_VOLUME")->getWidgetWithID("VALUE")->setAttribute("caption", sp::string(int(sp::audio::Music::getVolume())) + "%");
    menu->getWidgetWithID("MUSIC_VOLUME")->getWidgetWithID("SLIDER")->setAttribute("value", sp::string(sp::audio::Music::getVolume()));
    menu->getWidgetWithID("MUSIC_VOLUME")->getWidgetWithID("SLIDER")->setEventCallback([=](sp::Variant v) mutable {
        menu->getWidgetWithID("MUSIC_VOLUME")->getWidgetWithID("VALUE")->setAttribute("caption", sp::string(v.getInteger()) + "%");
        sp::audio::Music::setVolume(v.getInteger());
    });
    menu->getWidgetWithID("BACK")->setEventCallback([=](sp::Variant v) mutable {
        menu.destroy();
        openMainMenu();
    });
}

class AutoCreditScroller : public sp::Node
{
public:
    using sp::Node::Node;

    virtual void onUpdate(float delta) override
    {
        sp::P<sp::gui::Widget> w = getParent();
        w->layout.position.y -= delta * 30.0f;
        if (w->layout.position.y < -w->layout.size.y)
            w->layout.position.y = 480.0f;
    }
};

static void openCreditsMenu()
{
    sp::P<sp::gui::Widget> menu = sp::gui::Loader::load("gui/main_menu.gui", "CREDITS_MENU");
    new AutoCreditScroller(menu->getWidgetWithID("CREDITS"));
    menu->getWidgetWithID("BACK")->setEventCallback([=](sp::Variant v) mutable {
        menu.destroy();
        openMainMenu();
    });
}

int main(int argc, char** argv)
{
    sp::P<sp::Engine> engine = new sp::Engine();

    //Create resource providers, so we can load things.
    sp::io::ResourceProvider::createDefault();

    //Disable or enable smooth filtering by default, enabling it gives nice smooth looks, but disabling it gives a more pixel art look.
    sp::texture_manager.setDefaultSmoothFiltering(false);

    //Create a window to render on, and our engine.
    window = new sp::Window();
#if !defined(DEBUG) && !defined(EMSCRIPTEN)
    window->setFullScreen(true);
#endif

    sp::gui::Theme::loadTheme("default", "gui/theme/basic.theme.txt");
    new sp::gui::Scene(sp::Vector2d(640, 480));

    sp::P<sp::SceneGraphicsLayer> scene_layer = new sp::SceneGraphicsLayer(1);
    scene_layer->addRenderPass(new sp::BasicNodeRenderPass());
#ifdef DEBUG
    scene_layer->addRenderPass(new sp::CollisionRenderPass());
#endif
    window->addLayer(scene_layer);

    openMainMenu();

    engine->run();

    return 0;
}
