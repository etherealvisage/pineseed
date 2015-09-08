#include <iostream>

#include "interface/base.hpp"
#include "api/root.hpp"

int main() {
    interface::base interface_base;

    interface_base.setup();

    api::root root;

    auto video = root.get_interface()->get_video();
    video->setup();
    video->set_video_mode(800, 600, 32);

    interface_base.teardown();
    return 0;
}
