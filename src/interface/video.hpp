#ifndef INTERFACE_VIDEO_HPP
#define INTERFACE_VIDEO_HPP

namespace interface {

class video {
public:
    void setup();

    void set_video_mode(int width, int height, int bpp);

    void clear_screen();
    void update_screen();
};

} // namespace interface

#endif
