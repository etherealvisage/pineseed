#include <iostream>

#include <SDL.h>

#include "kriti/MessageSystem.h"
#include "kriti/interface/ContextRegistry.h"
#include "kriti/interface/Video.h"

#include "kriti/render/Pipeline.h"
#include "kriti/render/RenderableFactory.h"

#include "kriti/scene/Camera.h"
#include "kriti/math/ViewGenerator.h"

#include "kriti/gui/TextRenderer.h"

#include "kriti/ResourceRegistry.h"

void pop(SDL_Keycode key) {
    if(key == SDLK_ESCAPE) {
        auto cr = Kriti::Interface::ContextRegistry::instance();

        cr->pop();
    }
}

boost::shared_ptr<Kriti::Render::Stage> stage;
boost::shared_ptr<Kriti::Render::Pipeline> pipeline;
boost::shared_ptr<Kriti::Render::Renderable> tri, text;
boost::shared_ptr<Kriti::Scene::Camera> camera;

boost::shared_ptr<Kriti::GUI::Font> font;

void frame_handler() {
    pipeline->render();
    Kriti::Interface::Video::instance()->swapBuffers();
}

void gameEntryPoint() {
    Message3(Game, Debug, "Game entry point reached.");

    stage = boost::make_shared<Kriti::Render::Stage>(1, 800, 600, "");

    pipeline = boost::make_shared<Kriti::Render::Pipeline>();
    pipeline->setLastStage(stage);

    camera = boost::make_shared<Kriti::Scene::Camera>();
    camera->setProjection(Kriti::Math::ViewGenerator().orthogonal(
        Kriti::Interface::Video::instance()->aspectRatio(), 1.0, 0.01, 10.0));
    stage->addUniformHook(camera);

    Kriti::Render::RenderableFactory rf;
    std::vector<Kriti::Math::Vector> v, n;
    v.push_back(Kriti::Math::Vector(0.0, 0.0, 1.0));
    v.push_back(Kriti::Math::Vector(-1.0, 1.0, 1.0));
    v.push_back(Kriti::Math::Vector(1.0, 1.0, 1.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    std::vector<unsigned int> t;
    t.push_back(0);
    t.push_back(1);
    t.push_back(2);
    tri = rf.fromTriangleGeometry(v, n, t, "simple");
    stage->renderables()->add(tri);

    font = Kriti::ResourceRegistry::get<Kriti::GUI::Font>("ubuntu");

    auto tr = boost::make_shared<Kriti::GUI::TextRenderer>();

    text = tr->render(font, "Testing!", Kriti::Math::Vector(0.2, 0.2));

    text->location() += Kriti::Math::Vector(0.0, -0.1);

    stage->renderables()->add(text);

    auto gcon = boost::make_shared<Kriti::State::Context>();

    gcon->addListener("key_down", boost::function<void (SDL_Keycode)>(pop));
    gcon->addListener("new_frame", boost::function<void ()>(frame_handler));

    auto cr = Kriti::Interface::ContextRegistry::instance();

    cr->push(gcon);

    cr->run();
}
