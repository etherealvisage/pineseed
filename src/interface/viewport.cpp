#include <cmath>

#include "viewport.hpp"

namespace interface {

void viewport::transform(double &x, double &y) {
    // order to apply things in:
    // - scaling
    // - translation
    // - rotation?

    x *= m_xscale;
    y *= m_yscale;

    x -= m_xorigin;
    y -= m_yorigin;
}

} // namespace interface
