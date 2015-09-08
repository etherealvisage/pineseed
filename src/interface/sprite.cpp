#include <cmath>

#include <SDL/SDL_rotozoom.h>

#include "sprite.hpp"

#include "message_system.hpp"

namespace interface {

void sprite::paint(viewport *viewport) {
    if(m_current_frame == no_frame) return;


    double x = m_x, y = m_y;
    viewport->transform(x, y);
    auto torender = rotozoomSurfaceXY(get_current_surface(), m_rotation,
        viewport->get_xscale(), viewport->get_yscale(), 1);

    SDL_Rect r;
    r.w = torender->w, r.h = torender->h;
    r.x = int(std::round(x)+1e-6), r.y = int(std::round(y)+1e-6);

    SDL_BlitSurface(torender, nullptr, SDL_GetVideoSurface(), &r);
}

void sprite::add_frame(int id, SDL_Surface *image) {
    if(m_frames[id]) SDL_FreeSurface(m_frames[id]);
    m_frames[id] = image;
}

void sprite::set_current_frame(int id) {
    if(!m_frames[id]) MSG3(interface, error, "No such frame " << id);
    else m_current_frame = id;
}

} // namespace interface
