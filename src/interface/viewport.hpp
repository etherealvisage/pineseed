#ifndef INTERFACE_VIEWPORT_HPP
#define INTERFACE_VIEWPORT_HPP

namespace interface {

class viewport {
private:
    double m_xorigin, m_yorigin;
    double m_xscale, m_yscale;
public:
    void setup();

    void transform(double &x, double &y);

    double get_xscale() const { return m_xscale; }
    double get_yscale() const { return m_yscale; }
};

} // namespace interface

#endif
