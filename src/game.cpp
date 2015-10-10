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
#include "game/Conversation.h"

#include "conv/Node.h"
#include "conv/SpeechAction.h"

using namespace Pineseed;

boost::shared_ptr<Kriti::Render::Stage> stage;
boost::shared_ptr<Kriti::Render::Pipeline> pipeline;
boost::shared_ptr<Kriti::Render::Renderable> tri, text;
boost::shared_ptr<Kriti::Scene::Camera> camera;

boost::shared_ptr<Kriti::GUI::Font> font;

boost::shared_ptr<Kriti::GUI::OutlineRegistry> outlineRegistry;
boost::shared_ptr<Kriti::GUI::MouseInteractor> mouseInteractor;
boost::shared_ptr<Kriti::GUI::MouseCursor> mouseCursor;
boost::shared_ptr<Kriti::GUI::ScrollArea> scroll;

boost::shared_ptr<Game::UI> ui;
boost::shared_ptr<Game::Conversation> conversation;

void frame_handler() {
    mouseInteractor->updateMouseActivation(outlineRegistry);

    ui->update(outlineRegistry);

    pipeline->render();
    Kriti::Interface::Video::instance()->swapBuffers();
}

void pop(SDL_Keycode key) {
    if(key == SDLK_ESCAPE) {
        auto cr = Kriti::Interface::ContextRegistry::instance();

        cr->pop();
    }
    else if(key == SDLK_SPACE) {
        ui->addJournalText("Another entry!");
    }
    else if(key == SDLK_1) {
        conversation->takeOption(0);
    }
    else if(key == SDLK_2) {
        conversation->takeOption(1);
    }
    else if(key == SDLK_3) {
        conversation->takeOption(2);
    }
    else if(key == SDLK_4) {
        conversation->takeOption(3);
    }
}

void gameEntryPoint() {
    ui = Game::UI::instance();

    Message3(Game, Debug, "Game entry point reached.");

    stage = boost::make_shared<Kriti::Render::Stage>(1, 800, 600, "");

    pipeline = boost::make_shared<Kriti::Render::Pipeline>();
    pipeline->setLastStage(stage);

    camera = boost::make_shared<Kriti::Scene::Camera>();
    camera->setProjection(Kriti::Math::ViewGenerator().orthogonal(
        0.0, Kriti::Interface::Video::instance()->aspectRatio(), 1.0, 0.0,
        -10.0, 10.0));
    stage->addUniformHook(camera);

    font = Kriti::ResourceRegistry::get<Kriti::GUI::Font>("Ubuntu-B.ttf");

    auto gcon = boost::make_shared<Kriti::State::Context>();

    gcon->addListener("key_down", boost::function<void (SDL_Keycode)>(pop));
    gcon->addListener("mouse_moved", boost::function<void (double, double)>([](double x, double y){ mouseInteractor->updateMouseCoordinates(x, y); }));
    gcon->addListener("mouse_moved", boost::function<void (double, double)>([](double x, double y){ mouseCursor->updateMouseCoordinates(x, y); }));
    gcon->addListener("mouse_down", boost::function<void (int)>([](int b){ mouseInteractor->updateMouseButton(b, true); }));
    gcon->addListener("mouse_up", boost::function<void (int)>([](int b){ mouseInteractor->updateMouseButton(b, false); }));
    gcon->addListener("new_frame", boost::function<void ()>(frame_handler));

    /* GUI stuff! */
    outlineRegistry = boost::make_shared<Kriti::GUI::OutlineRegistry>();
    mouseInteractor = boost::make_shared<Kriti::GUI::MouseInteractor>();
    mouseCursor = boost::make_shared<Kriti::GUI::MouseCursor>();
    stage->renderables()->add(mouseCursor->renderable());

    stage->renderables()->add(ui->renderables());
    ui->addJournalText("This is a test! #1");
    ui->addJournalText("This is a test! #2");
    ui->addJournalText("This is a test! #3");

    conversation = boost::make_shared<Game::Conversation>();

    {
        auto convRoot = boost::make_shared<Conv::Node>();

        convRoot->addAction(boost::make_shared<Conv::SpeechAction>(nullptr,
            "Which conversation to test?"));

        auto convOneRoot = boost::make_shared<Conv::Node>();
        convRoot->addLink(Conv::Node::Link(convOneRoot, "Conversation #1"));
        convOneRoot->addAction(boost::make_shared<Conv::SpeechAction>(nullptr,
            "Hello there, stranger."));

        auto convOneRootResponse = boost::make_shared<Conv::Node>();
        convOneRoot->addLink(Conv::Node::Link(convOneRootResponse,
            "Hi there. My name's not known yet."));
        convOneRootResponse->addAction(boost::make_shared<Conv::SpeechAction>(nullptr,
            "Good to know!"));

        conversation->begin(convRoot);
    }

    auto cr = Kriti::Interface::ContextRegistry::instance();

    cr->push(gcon);

    cr->run();
}
