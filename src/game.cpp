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
#include "kriti/gui/Button.h"
#include "kriti/gui/Panel.h"
#include "kriti/gui/PackedLayout.h"
#include "kriti/gui/MouseCursor.h"
#include "kriti/gui/MouseInteractor.h"
#include "kriti/gui/ScrollArea.h"

#include "kriti/ResourceRegistry.h"

#include "game/UI.h"

boost::shared_ptr<Kriti::Render::Stage> stage;
boost::shared_ptr<Kriti::Render::Pipeline> pipeline;
boost::shared_ptr<Kriti::Render::Renderable> tri, text;
boost::shared_ptr<Kriti::Scene::Camera> camera;

boost::shared_ptr<Kriti::GUI::Font> font;

boost::shared_ptr<Kriti::GUI::OutlineRegistry> outlineRegistry;
boost::shared_ptr<Kriti::GUI::MouseInteractor> mouseInteractor;
boost::shared_ptr<Kriti::GUI::MouseCursor> mouseCursor;
boost::shared_ptr<Kriti::GUI::ScrollArea> scroll;
boost::shared_ptr<Kriti::GUI::Panel> panel;
boost::shared_ptr<Kriti::GUI::Button> button, button2;
boost::shared_ptr<Kriti::GUI::Label> label;

Pineseed::Game::UI *ui;

void frame_handler() {
    //mouseInteractor->updateMouseActivation(outlineRegistry);

    /*panel->update(outlineRegistry, Kriti::Math::Vector(),
        Kriti::Math::Vector(0.5, 0.5), Kriti::Math::Vector(1.0, 1.0));
    panel->fill(stage->renderables());*/
    /*scroll->update(outlineRegistry, Kriti::Math::Vector(0.0, 0.0),
        Kriti::Math::Vector(0.5, 0.5), Kriti::Math::Vector(1.0, 1.0));
    scroll->fill(stage->renderables());

    scroll->scrollOffset() -= Kriti::Math::Vector(0.0, 0.001, 0.0);*/

    //ui->update(outlineRegistry);

    label->fill(stage->renderables());
    label->update(outlineRegistry, Kriti::Math::Vector(),
        Kriti::Math::Vector(0.5, 0.5), Kriti::Math::Vector(1.0, 1.0));

    pipeline->render();
    Kriti::Interface::Video::instance()->swapBuffers();
}

void pop(SDL_Keycode key) {
    if(key == SDLK_ESCAPE) {
        auto cr = Kriti::Interface::ContextRegistry::instance();

        cr->pop();
    }
    else if(key == SDLK_SPACE) {
        ui->addText("Another entry!");
    }
}

void gameEntryPoint() {
    Message3(Game, Debug, "Game entry point reached.");

    stage = boost::make_shared<Kriti::Render::Stage>(1, 800, 600, "");

    pipeline = boost::make_shared<Kriti::Render::Pipeline>();
    pipeline->setLastStage(stage);

    camera = boost::make_shared<Kriti::Scene::Camera>();
    camera->setProjection(Kriti::Math::ViewGenerator().orthogonal(
        0.0, Kriti::Interface::Video::instance()->aspectRatio(), 1.0, 0.0,
        -10.0, 10.0));
    stage->addUniformHook(camera);

    Kriti::Render::RenderableFactory rf;
    std::vector<Kriti::Math::Vector> v, n;
    v.push_back(Kriti::Math::Vector(0.0, 0.0, 5.0));
    v.push_back(Kriti::Math::Vector(0.0, 1.0, 5.0));
    v.push_back(Kriti::Math::Vector(1.0, 1.0, 5.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    n.push_back(Kriti::Math::Vector(0.0, 0.0, -1.0));
    std::vector<unsigned int> t;
    t.push_back(0);
    t.push_back(1);
    t.push_back(2);
    tri = rf.fromTriangleGeometry(v, n, t, "simple");
    //stage->renderables()->add(tri);

    font = Kriti::ResourceRegistry::get<Kriti::GUI::Font>("Ubuntu-B.ttf");

    /*auto tr = boost::make_shared<Kriti::GUI::TextRenderer>();
    text = tr->renderString(font->getInstance(12), "Testing!",
        Kriti::Math::Vector(1.0, 1.0));
    text->location() += Kriti::Math::Vector(0.3, 0.3);
    stage->renderables()->add(text);*/

    auto gcon = boost::make_shared<Kriti::State::Context>();

    gcon->addListener("key_down", boost::function<void (SDL_Keycode)>(pop));
    //gcon->addListener("mouse_moved", boost::function<void (double, double)>([](double x, double y){ mouseInteractor->updateMouseCoordinates(x, y); }));
    //gcon->addListener("mouse_moved", boost::function<void (double, double)>([](double x, double y){ mouseCursor->updateMouseCoordinates(x, y); }));
    //gcon->addListener("mouse_down", boost::function<void (int)>([](int b){ mouseInteractor->updateMouseButton(b, true); }));
    //gcon->addListener("mouse_up", boost::function<void (int)>([](int b){ mouseInteractor->updateMouseButton(b, false); }));
    gcon->addListener("new_frame", boost::function<void ()>(frame_handler));

    /* GUI stuff! */
    outlineRegistry = boost::make_shared<Kriti::GUI::OutlineRegistry>();
    mouseInteractor = boost::make_shared<Kriti::GUI::MouseInteractor>();
    mouseCursor = boost::make_shared<Kriti::GUI::MouseCursor>();
    panel = boost::make_shared<Kriti::GUI::Panel>(
        Kriti::Math::Vector(0.1, 0.1), Kriti::Math::Vector(1.0, 1.0),
        boost::make_shared<Kriti::GUI::PackedLayout>(Kriti::Math::Vector(1.0, 1.0)));
    button = boost::make_shared<Kriti::GUI::Button>(Kriti::Math::Vector(0.1, 0.1), Kriti::Math::Vector(1.0, 1.0), font->getInstance(36), "Toggle visible");
    panel->layout()->addItem(button);
    button2 = boost::make_shared<Kriti::GUI::Button>(Kriti::Math::Vector(0.1, 0.1), Kriti::Math::Vector(1.0, 1.0), font->getInstance(36), "Quit");
    panel->layout()->addItem(button2);
    stage->renderables()->add(mouseCursor->renderable());

    auto qevent = gcon->addEvent<>("button_clicked");
    button2->setClickEvent(qevent);
    gcon->addListener("button_clicked", boost::function<void ()>(boost::bind(pop, SDLK_ESCAPE)));
    gcon->addListener("button_clicked", boost::function<void ()>([](){ Message3(Game, Debug, "Button clicked!"); }));

    scroll = boost::make_shared<Kriti::GUI::ScrollArea>(Kriti::Math::Vector(), Kriti::Math::Vector(1.0, 1.0), panel, Kriti::Math::Vector(1.0, 1.0));

    //label = boost::make_shared<Kriti::GUI::Label>(Kriti::Math::Vector(), Kriti::Math::Vector(1.0, 1.0), font->getInstance(36), "Testing!");
    label = boost::make_shared<Kriti::GUI::Label>(Kriti::Math::Vector(), Kriti::Math::Vector(1.0, 1.0), font->getInstance(36), "Testing!");

    /*ui = new Pineseed::Game::UI();
    stage->renderables()->add(ui->renderables());
    ui->addText("This is a test! #1");
    ui->addText("This is a test! #2");
    ui->addText("This is a test! #3");*/

    auto cr = Kriti::Interface::ContextRegistry::instance();

    cr->push(gcon);

    cr->run();
}
