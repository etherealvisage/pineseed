#ifndef SKELETON__STREAM_AS_STRING_H
#define SKELETON__STREAM_AS_STRING_H

#include <sstream>
#include <string>

/** A simple class which in effect that adds an insertion operator, <<, to the
    std::string class, as users of streams such as cout or stringstream expect.
    
    This class can be used as a temporary object in places where an ordinary
    string would be expected; for example:
        void print(std::string str);
        print(stream_as_string() << "Answer = " << 42);
    
    This implementation is quite efficient, only converting its internal
    stringstream to a string when operator std::string() is called.
*/
class stream_as_string {
private:
    /** The internal stringstream used to implement the insertion operator <<.
    */
    std::ostringstream stream;
protected:
    std::ostringstream &getStream() { return stream; }
    const std::ostringstream &getStream() const { return stream; }
public:
    /** Adds an object to the end of the internal stringstream.
    */
    template <typename T>
    stream_as_string &operator << (const T &data);
    
    /** Converts the internal stringstream to an std::string automatically.
    */
    operator std::string() const;
};

template <typename T>
stream_as_string &stream_as_string::operator << (const T &data) {
    getStream() << data;
    
    return *this;
}

inline stream_as_string::operator std::string() const {
    return getStream().str();
}

#endif
