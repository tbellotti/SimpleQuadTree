#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <string>

/*
This is a two dimensional point
*/

template <typename T>
class Point
{
protected:
    T x_coord;
    T y_coord;

public:
    // Constructors and destructor
    Point() = default;
    Point(T x, T y) : x_coord(x), y_coord(y) {}
    explicit Point(T x) : x_coord(x), y_coord(x) {}
    ~Point() = default;

    // Getters
    inline T getX() const { return x_coord; }
    inline T getY() const { return y_coord; }
    // Setters 
    inline void setX(T x) { x_coord = x; }
    inline void setY(T y) { y_coord = y; }

    // Print (for debugging purpose)
    std::string print() const
    {
        std::string tmp("(");
        tmp += std::to_string(x_coord);
        tmp += ", ";
        tmp += std::to_string(y_coord);
        tmp += ")";

        return tmp;
    }

    // Other operations
    bool operator==(Point & rhs)
    {   // TO BE COMPLETED
        return true;
    }

    Point<T> operator+ (Point<T> rhs)
    {
        return Point<T>(x_coord+rhs.x_coord, y_coord+rhs.y_coord);
    }


    T abs() const
    {
        Point<T> origin = Point<T>(0.0, 0.0);
        return distance(origin);
    }

    T distance(Point & rhs) const 
    {
        return sqrt(pow(x_coord - rhs.x_coord, 2.0) + pow(y_coord - rhs.y_coord, 2.0));
    }

};

template <typename T>
Point<T> operator* (T lhs, Point<T> rhs)
{
    return Point<T>(lhs*rhs.getX(), lhs*rhs.getY());
}


#endif


