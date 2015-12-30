#include <iostream>

#include <boost/shared_ptr.hpp>

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
#include "kriti/gui/OutlineRegistry.h"

#include "kriti/ResourceRegistry.h"

#include "game/UI.h"
#include "game/Conversation.h"

#include "conv/Node.h"
#include "conv/SpeechAction.h"
#include "conv/Root.h"

#include "level/Grid.h"
#include "level/Root.h"

#include "scene/Camera.h"
#include "scene/Geometry.h"
#include "scene/SceneCharacter.h"

using namespace Pineseed;

boost::shared_ptr<Kriti::Render::Stage> rootStage, guiStage, sceneStage;
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

boost::shared_ptr<Scene::Camera> sceneCamera;
boost::shared_ptr<Scene::SceneCharacter> playerCharacter;

Kriti::TimeValue lastFrame;

void frame_handler() {
    auto curtime = Kriti::TimeValue::current();
    if(!lastFrame.set()) {
        lastFrame = curtime;
    }
    auto timeDelta = (curtime - lastFrame);
    lastFrame = curtime;

    mouseInteractor->updateMouseActivation(outlineRegistry);

    ui->update(outlineRegistry);

    sceneCamera->setTarget(playerCharacter->location(),
        sceneCamera->targetOrientation());
    sceneCamera->step(timeDelta);

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
    else if(key == SDLK_UP) {
        playerCharacter->setLocation(
            playerCharacter->location() + Kriti::Math::Vector(0.0, -0.15));
    }
    else if(key == SDLK_DOWN) {
        playerCharacter->setLocation(
            playerCharacter->location() + Kriti::Math::Vector(0.0, 0.15));
    }
    else if(key == SDLK_LEFT) {
        playerCharacter->setLocation(
            playerCharacter->location() + Kriti::Math::Vector(-0.15, 0.0));
    }
    else if(key == SDLK_RIGHT) {
        playerCharacter->setLocation(
            playerCharacter->location() + Kriti::Math::Vector(0.15, 0.0));
    }
    else if(key == SDLK_q) {
        sceneCamera->setTarget(
            sceneCamera->targetPosition(),
            sceneCamera->targetOrientation() * Kriti::Math::Quaternion(Kriti::Math::Vector(0.0, 0.0, 1.0), M_PI/8));
    }
    else if(key == SDLK_e) {
        sceneCamera->setTarget(
            sceneCamera->targetPosition(),
            sceneCamera->targetOrientation() * Kriti::Math::Quaternion(Kriti::Math::Vector(0.0, 0.0, 1.0), -M_PI/8));
    }
}

void gameEntryPoint() {
    ui = Game::UI::instance();

    Message3(Game, Debug, "Game entry point reached.");

    rootStage = boost::make_shared<Kriti::Render::Stage>("root");
    guiStage = boost::make_shared<Kriti::Render::Stage>("gui");
    sceneStage = boost::make_shared<Kriti::Render::Stage>("scene");

    pipeline = boost::make_shared<Kriti::Render::Pipeline>();
    pipeline->setLastStage(rootStage);

    auto aratio = Kriti::Interface::Video::instance()->aspectRatio();
    camera = boost::make_shared<Kriti::Scene::Camera>();
    camera->setProjection(Kriti::Math::ViewGenerator().orthogonal(
        0.0, aratio, 1.0, 0.0,
        -10.0, 10.0));
    guiStage->addUniformHook(camera);
    rootStage->addUniformHook(camera);

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
    guiStage->renderables()->add(mouseCursor->renderable());

    guiStage->renderables()->add(ui->renderables());

    {
        auto initial = Kriti::ResourceRegistry::get<Level::Grid>("initial");
        auto geometry = boost::make_shared<Scene::Geometry>();
        geometry->addGrid(initial, Level::Pos(0.0, 0.3));
        sceneCamera = boost::make_shared<Scene::Camera>();
        sceneCamera->setInterpModes(Scene::Camera::ExponentialPosition,
            Scene::Camera::ExponentialOrientation);
        sceneCamera->setPositionInterpFactor(0.1);
        sceneCamera->setOrientationInterpFactor(0.1);
        sceneStage->addUniformHook(sceneCamera);

        sceneStage->renderables()->add(geometry->container());
    }

    {
        playerCharacter = boost::make_shared<Scene::SceneCharacter>();

        sceneStage->renderables()->add(playerCharacter->renderable());
    }

    {
        rootStage->addPrevious(sceneStage);
        rootStage->addPrevious(guiStage);

        auto overlayQuad = Kriti::Render::RenderableFactory().fromQuad(
            Kriti::Math::Vector(0.0, 0.0), Kriti::Math::Vector(0.0, 1.0),
            Kriti::Math::Vector(aratio, 1.0), Kriti::Math::Vector(aratio, 0.0),
            "overlay");
        auto overlayMaterial =
            Kriti::ResourceRegistry::get<Kriti::Render::Material>("overlay");
        rootStage->renderables()->add(overlayQuad);

        rootStage->addMapping(sceneStage,
            Kriti::Render::Framebuffer::ColourBuffer0,
            overlayMaterial, "baseStage");
        rootStage->addMapping(guiStage,
            Kriti::Render::Framebuffer::ColourBuffer0,
            overlayMaterial, "overlayStage");
    }

    auto cr = Kriti::Interface::ContextRegistry::instance();
    cr->push(gcon);
    cr->run();
}
