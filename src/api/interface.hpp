#ifndef API_INTERFACE_HPP
#define API_INTERFACE_HPP

#include "interface/video.hpp"

namespace api {

class interface {
private:
    ::interface::video m_video;
public:
    ::interface::video *get_video() { return &m_video; }
    const ::interface::video *get_video() const { return &m_video; }
};

} // namespace api

#endif
