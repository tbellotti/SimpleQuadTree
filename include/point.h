#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

/*
This class implements the notion of 
two dimensional point based on the generic 
type T.
*/

template <typename T>
class Point
{
protected:
    T x_coord = T(0.0);
    T y_coord = T(0.0);

public:
    Point() : x_coord(T(0.0)), y_coord(T(0.0)) {} 

    Point(T x, T y) : x_coord(x), y_coord(y) {}

    //explicit Point(T x) : x_coord(x), y_coord(x) {}
    // We do not want it to be explicit. Can be used for cast
    Point(T x) : x_coord(x), y_coord(x) {}

    virtual ~Point() = default;

    T getX() const 
    {
        return x_coord;
    }

    T getY() const 
    { 
        return y_coord; 
    }

    void setX(T x) 
    { 
        x_coord = x; 
    }

    void setY(T y) 
    { 
        y_coord = y; 
    }

    // Print (for debugging purpose)
    std::string print() const
    {
        std::ostringstream buffer;
        buffer<<"("<<std::to_string(x_coord)<<", "<<std::to_string(y_coord)<<")";
        return buffer.str();
    }

    T abs() const
    {
        return distance(Point<T>(0.0, 0.0));
    }

    T distance(const Point & rhs) const 
    {
        return sqrt(pow(x_coord - rhs.x_coord, 2.0) + pow(y_coord - rhs.y_coord, 2.0));
    }

};

// Userful operator overloading which we do 
// not define as members of the class.

// Multiplication by a scalar (cannot be class member)
template <typename T>
Point<T> operator* (const T & lhs, const Point<T> & rhs)
{
    return Point<T>(lhs*rhs.getX(), lhs*rhs.getY());
}

template <typename T>
std::ostream& operator<<(std::ostream & os, const Point<T> & pt)
{
    os<<"("<<std::to_string(pt.getX())<<", "<<std::to_string(pt.getY())<<")";
    return os;
}

template <typename T>
Point<T> operator+ (const Point<T> & lhs, const Point<T> & rhs)
{
    return Point<T>(lhs.getX() + rhs.getX(), lhs.getY() + rhs.getY());       
}

#endif
