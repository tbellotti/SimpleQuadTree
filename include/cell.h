#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>
#include <sstream>
#include "point.h"
#include "rgbcolor.h"
#include "useful.h"

//#include "refinementcriterion.h"
template <typename T>   // This forward declaration is needed to avoid
class RefinementCriterion;  // double inclusions.

/*
This class implements the notion of 
rectangular cell of a QuadTree based on the generic 
type T.
*/

template <typename T>
class Cell : public std::enable_shared_from_this<Cell<T>>
{
protected:
    // The const is the reason why we do not have setters
    // it avoids problems when creating the whole tree.
    const Point<T> base_point;
    const T dx;
    const T dy;

    const unsigned char level;

    // They are not const because we need to modify them
    // for example when erasing a children cell.
    std::shared_ptr<Cell> l_l; // Lower-Left children
    std::shared_ptr<Cell> l_r; // Lower-Right children
    std::shared_ptr<Cell> u_l; // Upper-Left children
    std::shared_ptr<Cell> u_r; // Upper-Right children
    /*
    The scheme is the following
     -----------
    | U_L | U_R |
    |     |     |
    |-----|-----|
    | L_L | L_R |
    |     |     |
     -----------
    */

    // Think where to put it...
    /*
    bool simplifyCellHelp(const RefinementCriterion<T> & criterion, const unsigned char min_lev)
    {
        if (!isLeaf())  {
            if (getLevel() >= min_lev &&
                 l_l->isLeaf() && l_r->isLeaf() && u_l->isLeaf() && u_r->isLeaf())  {
                
                if (!criterion(this->shared_from_this()))   {
                    mergeCell();
                    return true;
                }
                else    {
                    return false;
                }
                
            }
            else    {
                return l_l->simplifyCellHelp(criterion, min_lev) ||
                       l_r->simplifyCellHelp(criterion, min_lev) ||
                       u_l->simplifyCellHelp(criterion, min_lev) ||
                       u_r->simplifyCellHelp(criterion, min_lev); 
            }
        } 
        else    {
            return false;
        }   
    }
*/

public:

    // Constructors and destructor
    Cell(Point<T> b_p, T new_dx, T new_dy, unsigned char lv) : base_point(b_p), dx(new_dx), dy(new_dy), level(lv) {} 
    virtual ~Cell() = default;

    Point<T> getBasePoint() const
    {
        return base_point;
    }
    
    unsigned char getLevel() const
    {
        return level;
    }

    bool isLeaf() const
    {
        return (!l_l && !l_r && !u_l && !u_r);
    }

    Point<T> getCenter() const 
    {
        return base_point + Point<T>(0.5*dx, 0.5*dy);
    }

    T getDx() const { return dx; }
    T getDy() const { return dy; }
    T getDiagonal() const { return std::sqrt(dx*dx + dy*dy); } 
    T cellSurface() const { return dx * dy; }

    virtual void splitCell()    
    {
        if (isLeaf())    {
            // Lower Left son
            l_l = std::shared_ptr<Cell<T>>(new Cell<T>(base_point, 0.5*dx, 0.5*dy, static_cast<unsigned char>(level+1)));
            // Lower right son
            l_r = std::shared_ptr<Cell<T>>(new Cell<T>(base_point + Point<T>(0.5*dx, 0.0), 0.5*dx, 0.5*dy, static_cast<unsigned char>(level+1)));
            // Upper left son
            u_l = std::shared_ptr<Cell<T>>(new Cell<T>(base_point + Point<T>(0.0, 0.5*dy), 0.5*dx, 0.5*dy, static_cast<unsigned char>(level+1)));
            // Upper right son
            u_r = std::shared_ptr<Cell<T>>(new Cell<T>(getCenter(), 0.5*dx, 0.5*dy, static_cast<unsigned char>(level+1)));
        }
    }

    virtual void mergeCell()
    {
        l_l = nullptr;
        l_r = nullptr;
        u_l = nullptr;
        u_r = nullptr;
    }


    void refineCell(const RefinementCriterion<T> & criterion, const unsigned char max_lev)
    {

        bool splitted = false;

        if (isLeaf() && getLevel() < max_lev)   {
            if (criterion(this->shared_from_this()))    {
                splitCell();
                splitted = true;
            }
        }
        if (splitted || !isLeaf())  {
            l_l->refineCell(criterion, max_lev);
            l_r->refineCell(criterion, max_lev);
            u_l->refineCell(criterion, max_lev);
            u_r->refineCell(criterion, max_lev);
        }
    }

    void simplifyCell(const RefinementCriterion<T> & criterion, const unsigned char min_lev)
    {
        bool updated = true;
        while (updated) {
            updated = false;
            std::vector<std::shared_ptr<Cell<T>>> pre_leaves;
            getPreLeaves(pre_leaves);
            for (auto pl : pre_leaves)  {
                if (!criterion(pl) && pl->getLevel() >= min_lev) {
                    pl->mergeCell();
                    updated = true;
                }
            }

        }
        /*
        bool something_changed = true;

        while (something_changed)   {
            something_changed = simplifyCellHelp(criterion, min_lev);
        }*/
    }

    void updateCell(const RefinementCriterion<T> & criterion, const unsigned char min_lev, const unsigned char max_lev)   
    {
        refineCell(criterion, max_lev);
        simplifyCell(criterion, min_lev);
    }

    // For classes inheriting from Cell, these pointers must
    // be cast into the right type.
    std::vector<std::shared_ptr<Cell<T>>> getChildren() const
    {
    /* Scheme:
    3        2
     --------
    |        |
    |        | 
    |        |
    |        |
     --------
    0        1    
    */
        std::vector<std::shared_ptr<Cell<T>>> to_return;
        to_return.push_back(l_l);
        to_return.push_back(l_r);
        to_return.push_back(u_l);
        to_return.push_back(u_r);

        return to_return;
    }

    std::vector<Point<T>> getVertices() const
    {
        std::vector<Point<T>> to_return;

        to_return.push_back(base_point);
        to_return.push_back(base_point + Point<T>(dx, 0.0));
        to_return.push_back(base_point + Point<T>(dx, dy));
        to_return.push_back(base_point + Point<T>(0.0, dy));

        return to_return;
    }
 

    // This cannot be const because we put non-constant shared
    // pointers in the vector in order them to be useful
    // to eventually modify the objects they point to.
    void getLeaves(std::vector<std::shared_ptr<Cell<T>>> & to_fill)// const
    {
        if (isLeaf())   {
            to_fill.push_back(this->shared_from_this());
        }
        else    {
            l_l->getLeaves(to_fill);
            l_r->getLeaves(to_fill);
            u_l->getLeaves(to_fill);
            u_r->getLeaves(to_fill);

        }
    }


    void getPreLeaves(std::vector<std::shared_ptr<Cell<T>>> & to_fill)   
    {

        if (l_l->isLeaf() && l_r->isLeaf() && u_l->isLeaf() && u_r->isLeaf())   {
            to_fill.push_back(this->shared_from_this());
        }
        else    {
            if (!l_l->isLeaf()) 
                l_l->getPreLeaves(to_fill);

            if (!l_r->isLeaf()) 
                l_r->getPreLeaves(to_fill);

            if (!u_l->isLeaf()) 
                u_l->getPreLeaves(to_fill);

            if (!u_r->isLeaf()) 
                u_r->getPreLeaves(to_fill);
        }

    }

    std::string tikzDot() const
    {
        std::stringstream string_stream;
        string_stream<<"\\draw [fill=blue, line width = 0.1pt]"<<getCenter()<<" circle[radius= 0.5pt]; \n";
        return string_stream.str();
    }

    std::string tikzSquare(const RGBColor color = RGBColor(255, 255, 255), const bool outerline = false) const
    {
        std::vector<Point<T>> vertices = getVertices();
        std::stringstream string_stream;
        if (outerline)  {
            string_stream<<"\\draw [line width = 0.0mm, fill={rgb, 255 : red, "<<int(color.getRed())<<"; green, "<<int(color.getGreen())<<"; blue, "<<int(color.getBlue())<<"}]"<<vertices[0]<<" rectangle "<<vertices[2]<<"; \n";
        }
        else    {
            string_stream<<"\\fill [rgb color={"<<color<<"}]"<<vertices[0]<<" rectangle "<<vertices[2]<<"; \n";
        }
 
        return string_stream.str();

    }

    T zeroOrderIntegration(const std::function<T(Point<T>)> & f) const
    {
        return f(getCenter()) * cellSurface();
    }

    T thirdOrderGaussianIntegration(const std::function<T(Point<T>)> & f) const
    {
        T integral = 0.0;
        std::vector<T> w = {5.0/9.0, 8.0/9.0, 5.0/9.0};
        std::vector<T> xi = {-sqrt(3.0/5.0), 0.0, sqrt(3.0/5.0)};

        std::vector<Point<T>> vc = getVertices();
        
        for (size_t i = 0; i < xi.size(); i++)  {
            for (size_t j = 0; j < xi.size(); j++)  {
                Point<T> curr_point(xi[i], xi[j]);
                integral += w[i]*w[j]*f(Point<T>(gaussian::p<T>(curr_point, vc), gaussian::q<T>(curr_point, vc))) * std::abs(gaussian::Jacob<T>(curr_point, vc));
            }
        }
        return integral;
    }

};

// Mainly for debugging purposes
template <typename T>
std::ostream & operator<<(std::ostream & os, const Cell<T> & cl)
{
    os<<"Center: "<<cl.getCenter()<<", dx = "<<cl.getDx()<<", dy = "<<cl.getDy();
    return os;
}


#endif

