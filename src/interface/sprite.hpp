#ifndef INTERFACE_SPRITE_HPP
#define INTERFACE_SPRITE_HPP

#include <map>

#include "viewport.hpp"

#include <SDL/SDL.h>

namespace interface {

class sprite {
private:
    const int no_frame = -1;
private:
    double m_x, m_y;
    double m_rotation;

    std::map<int, SDL_Surface *> m_frames;
    int m_current_frame;
public:
    sprite() : m_current_frame(no_frame) {}

    void setup();

    void set_position(double x, double y);

    double get_x() const { return m_x; }
    double get_y() const { return m_y; }

    void paint(viewport *viewport);

    // takes ownership of the surface!
    void add_frame(int id, SDL_Surface *image);

    void set_current_frame(int id);
private:
    SDL_Surface *get_current_surface() { return m_frames[m_current_frame]; }
};

} // namespace interface

#endif
