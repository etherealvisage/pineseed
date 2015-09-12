#include <iostream>

#include "interface/base.hpp"
#include "interface/sprite.hpp"
#include "interface/input.hpp"
#include "api/root.hpp"
#include "event/context_registry.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

bool quit = false;

void set_quit(SDL_Event *event) {
    quit = true;
}

void test_listener(int i) {
    std::cout << "test_listener(" << i << ")\n";
}


int main() {
    event::proxy_context<> con;

    con.add_listener("test", std::function<void (int)>(test_listener));
    con.queue("test", std::make_tuple(42));
    con.queue("test", std::make_tuple(42));
    con.fire_queued();

    return 0;
#if 0
    interface::base interface_base;

    interface_base.setup();

    api::root root;

    auto video = root.get_interface()->get_video();
    video->setup();
    video->set_video_mode(640, 480, 32);

    auto sprite = new interface::sprite();
    sprite->add_frame(1, IMG_Load("data/simple_image.png"));

    sprite->set_current_frame(1);

    interface::viewport vp;

    interface::event event;
    interface::input input;

    input.setup(&event);

    event.register_listener(SDL_QUIT,
        std::bind(set_quit, std::placeholders::_1));

    event::context_registry context_registry;

    while(!quit) {
        video->clear_screen();

        sprite->paint(&vp);

        video->update_screen();
        context_registry.fire_queued();
        event.poll_events();
    }

    interface_base.teardown();
    return 0;
#endif
}
