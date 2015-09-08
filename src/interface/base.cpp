#include "base.hpp"

#include <SDL/SDL.h>

namespace interface {

void base::setup() {
    SDL_Init(SDL_INIT_VIDEO);
}

void base::teardown() {
    SDL_Quit();
}

} // namespace interface
