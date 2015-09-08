#include "video.hpp"

#include "message_system.hpp"

#include <SDL/SDL.h>

namespace interface {

void video::setup() {
    
}

void video::set_video_mode(int width, int height, int bpp) {
    if(SDL_SetVideoMode(width, height, bpp,
        SDL_HWSURFACE | SDL_DOUBLEBUF) == nullptr) {

        MSG3(interface, fatal, "Couldn't set video mode!");
    }
}

void video::clear_screen() {
    auto screen = SDL_GetVideoSurface();
    SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 32));
}

void video::update_screen() {
    SDL_Flip(SDL_GetVideoSurface());
}

} // namespace interface
