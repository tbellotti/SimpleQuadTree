#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include "point.h"

/*
This class implements the notion of 
rectangular cell of a QuadTree based on the generic 
type T.
*/

template <typename T>
class Cell
{
protected:
    // The const is the reason why we do not have setters
    // it avoids problems when creating the whole tree.
    const Point<T> base_point;
    const T dx;
    const T dy;

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

public:
    // Constructors and destructor
    Cell(Point<T> b_p, T new_dx, T new_dy) : base_point(b_p), dx(new_dx), dy(new_dy) {} 
    virtual ~Cell() = default;
    
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
            l_l = std::shared_ptr<Cell<T>>(new Cell<T>(base_point, 0.5*dx, 0.5*dy));
            // Lower right son
            l_r = std::shared_ptr<Cell<T>>(new Cell<T>(base_point + Point<T>(0.5*dx, 0.0), 0.5*dx, 0.5*dy));
            // Upper left son
            u_l = std::shared_ptr<Cell<T>>(new Cell<T>(base_point + Point<T>(0.0, 0.5*dy), 0.5*dx, 0.5*dy));
            // Upper right son
            u_r = std::shared_ptr<Cell<T>>(new Cell<T>(getCenter(), 0.5*dx, 0.5*dy));
        }
    }

    void mergeCell()
    {
        l_l = nullptr;
        l_r = nullptr;
        u_l = nullptr;
        u_r = nullptr;
    }

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

};

// Mainly for debugging purposes
template <typename T>
std::ostream & operator<<(std::ostream & os, const Cell<T> & cl)
{
    os<<"Center: "<<cl.getCenter()<<", dx = "<<cl.getDx()<<", dy = "<<cl.getDy();
    return os;
}

#endif

