#ifndef RO_POINT2D_HPP
#define RO_POINT2D_HPP

#include <ostream>

struct Point2D {
    Point2D(float x = 0, float y = 0) : x{ x }, y{ y } {}

    float x, y;
};

inline std::ostream& operator<<(std::ostream& out, const Point2D& point)
{
    return out << '(' << point.x << ", " << point.y << ')';
}

#endif // RO_POINT2D_HPP