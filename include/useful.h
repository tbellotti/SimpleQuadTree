#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include "point.h"



// Think it again.
/*
template <typename T>
std::vector<std::shared_ptr<T>> vectorOfSharedPointerStaticCast(const std::vector<std::shared_ptr<P>> & start_vc)
{
    std::vector<std::shared_ptr<T>> to_return;

    for (auto el : start_vc)    {
        to_return.push_back(std::pointer_static_cast<T>(el));
    }

    return to_return;

}

*/

std::string beginTikzFigure()
{
    std::string to_return("\\RequirePackage{luatex85} \n \\documentclass{standalone}\n \\usepackage{tikz} \n \\begin{document}\n \\begin{tikzpicture}[define rgb/.code={\\definecolor{mycolor}{RGB}{#1}},rgb color/.style={define rgb={#1},mycolor}]\n");
    return to_return;
}

std::string endTikzFigure()
{
    std::string to_return("\\end{tikzpicture} \n \\end{document} \n");
    return to_return;
}



// !!! We need to put them in a namespace in order to avoid the bordel
// with their simple names
namespace gaussian {
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
            //std::cout<<dp_dx<T>(pt, vc)*dq_dy<T>(pt, vc)-dq_dx<T>(pt, vc)*dp_dy<T>(pt, vc)<<std::endl;
            return (dp_dx<T>(pt, vc)*dq_dy<T>(pt, vc)-dq_dx<T>(pt, vc)*dp_dy<T>(pt, vc));
        };
}