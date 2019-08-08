#ifndef RO_RECT_HPP
#define RO_RECT_HPP

#include <ostream>

template <typename T>
struct Rect {
    explicit Rect(T a = T{}, T b = T{}, T c = T{}, T d = T{}) : a{ a }, b{ b }, c{ c }, d{ d } {}

    T a, b, c, d;
};

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Rect<T>& rect)
{
    return out << "rect(" << rect.a << ", " << rect.b << ", " << rect.c << ", " << rect.d << ')';
}

#endif // RO_RECT_HPP