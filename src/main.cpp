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

template<typename visitor, int v, typename P, class ...Ts>
class proxy_helper {
public:
    static void proxy(visitor &vis, std::string, const P &) {
        vis.done();
        std::cout << "Last parameter!" << std::endl;
    }
};

template<typename visitor, int v, typename P, class T, class ...Ts>
class proxy_helper<visitor, v, P, T, Ts...> {
public:
    static void proxy(visitor &vis, std::string name, const P &params) {
        vis.parameter(std::get<v>(params));
        proxy_helper<visitor, v+1, P, Ts...>::proxy(vis, name, params);
    }
};

class test_arg_visitor {
public:
    template<typename T>
    void parameter(const T &arg) {
        std::cout << __PRETTY_FUNCTION__ << " : value is " << arg << std::endl;
    }

    void done() {
        std::cout << "Done!" << std::endl;
    }
};

class test_proxy {
public:
    template<typename ...T>
    void proxy(std::string name, const std::tuple<T...> &params) {
        test_arg_visitor vis;
        proxy_helper<test_arg_visitor, 0, decltype(params), T...>::proxy(vis, name, params);
    }
};


int main() {
    event::proxy_context<test_proxy> con;

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
