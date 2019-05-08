#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include "point.h"

/*
This is not a class
It just contain some useful devices which are used throughout the project
and which are not logically linked to anything else, so we put them here.
*/

// Writes the preambule of a TikZ figure in order to print in LaTeX format
std::string beginTikzFigure();

// Gives all that is needed to finish a figure.
std::string endTikzFigure();


// We put them in a namespace in order to be clearer, when calling them
// in the class Cell, about their meaning and their origin, since their
// name is quite generic and not easily understandable.
namespace gaussian {

// Observe that this usage of variable which depends on a parameter
// from a template is allowed starting from C++14, which we thus need to employ.
template <typename T>
std::function<T(Point<T>)> n1 = [](Point<T> pt) { 
    return 1.0/4.0 * (1-pt.getX()) * (1-pt.getY());
};

template <typename T>
std::function<T(Point<T>)> n2 = [](Point<T> pt) { 
    return 1.0/4.0 * (1+pt.getX()) * (1-pt.getY());
    };

template <typename T>
std::function<T(Point<T>)> n3 = [](Point<T> pt) { 
    return 1.0/4.0 * (1+pt.getX()) * (1+pt.getY());
};

template <typename T>
std::function<T(Point<T>)> n4 = [](Point<T> pt) { 
    return 1.0/4.0 * (1-pt.getX()) * (1+pt.getY());
};

// ... and their partial derivatives
template <typename T>
std::function<T(Point<T>)> dn1_dx = [](Point<T> pt) { 
    return -1.0/4.0 * (1-pt.getY());
};

template <typename T>
std::function<T(Point<T>)> dn2_dx = [](Point<T> pt) { 
    return 1.0/4.0 * (1-pt.getY());
};

template <typename T>
std::function<T(Point<T>)> dn3_dx = [](Point<T> pt) { 
    return 1.0/4.0 * (1+pt.getY());
};

template <typename T>
std::function<T(Point<T>)> dn4_dx = [](Point<T> pt) { 
    return -1.0/4.0 * (1+pt.getY());
};

template <typename T>
std::function<T(Point<T>)> dn1_dy = [](Point<T> pt) { 
    return -1.0/4.0 * (1-pt.getX());
};

template <typename T>
std::function<T(Point<T>)> dn2_dy = [](Point<T> pt) { 
    return -1.0/4.0 * (1+pt.getX());
};

template <typename T>
std::function<T(Point<T>)> dn3_dy = [](Point<T> pt) { 
    return 1.0/4.0 * (1+pt.getX());
};

template <typename T>
std::function<T(Point<T>)> dn4_dy = [](Point<T> pt) { 
    return 1.0/4.0 * (1-pt.getX());
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> p = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return vc[0].getX() * n1<T>(pt) + vc[1].getX() * n2<T>(pt) + vc[2].getX() * n3<T>(pt) + vc[3].getX() * n4<T>(pt) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> q = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return vc[0].getY() * n1<T>(pt) + vc[1].getY() * n2<T>(pt) + vc[2].getY() * n3<T>(pt) + vc[3].getY() * n4<T>(pt) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> dp_dx = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return (vc[0].getX() * dn1_dx<T>(pt) + vc[1].getX() * dn2_dx<T>(pt) 
                  + vc[2].getX() * dn3_dx<T>(pt) + vc[3].getX() * dn4_dx<T>(pt)) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> dp_dy = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return (vc[0].getX() * dn1_dy<T>(pt) + vc[1].getX() * dn2_dy<T>(pt) 
                  + vc[2].getX() * dn3_dy<T>(pt) + vc[3].getX() * dn4_dy<T>(pt)) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> dq_dx = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return (vc[0].getY() * dn1_dx<T>(pt) + vc[1].getY() * dn2_dx<T>(pt) 
                  + vc[2].getY() * dn3_dx<T>(pt) + vc[3].getY() * dn4_dx<T>(pt)) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> dq_dy = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return (vc[0].getY() * dn1_dy<T>(pt) + vc[1].getY() * dn2_dy<T>(pt) 
                  + vc[2].getY() * dn3_dy<T>(pt) + vc[3].getY() * dn4_dy<T>(pt)) ;
};

template <typename T>
std::function<T(Point<T>, std::vector<Point<T>> &)> Jacob = [](Point<T> pt, std::vector<Point<T>> & vc) { 
    return (dp_dx<T>(pt, vc)*dq_dy<T>(pt, vc)-dq_dx<T>(pt, vc)*dp_dy<T>(pt, vc));
};
} // End of the gaussian namespace

// Verifies is a number is a power of another. Can be improved if one has 
// a better knowledge of floating point arithmetics....
bool isPowerOf(long long int n, int base);
