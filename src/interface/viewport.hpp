#ifndef INTERFACE_VIEWPORT_HPP
#define INTERFACE_VIEWPORT_HPP

namespace interface {

class viewport {
private:
    double m_xorigin, m_yorigin;
    double m_xscale, m_yscale;
public:
    viewport()
        : m_xorigin(0.0), m_yorigin(0.0), m_xscale(1.0), m_yscale(1.0) {}

    void setup();

    void transform(double &x, double &y);

    double get_xscale() const { return m_xscale; }
    void set_xscale(double x) { m_xscale = x; }
    double get_yscale() const { return m_yscale; }
    void set_yscale(double y) { m_yscale = y; }
};

} // namespace interface

#endif
