#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <vector>
#include <memory>
#include "point.h"

template <typename T>
class Cell
{
protected:
    Point<T> base_point;
    T dx;
    T dy;
    bool is_leaf;

    std::shared_ptr<Cell> l_l;
    std::shared_ptr<Cell> l_r;
    std::shared_ptr<Cell> u_l;
    std::shared_ptr<Cell> u_r;


    //


public:
    // COnstructors and destructors
    Cell(Point<T> b_p, T new_dx, T new_dy) : base_point(b_p), dx(new_dx), dy(new_dy), is_leaf(true) {}


    bool isLeaf() const { return is_leaf; }

    Point<T> getCenter() const 
    {
        return Point<T>(base_point.getX()+0.5*dx, base_point.getY()+0.5*dy);
    }

    T getDx() const { return dx; }
    T getDy() const { return dy; }

    T getDiagonal() const { return std::sqrt(dx*dx + dy*dy); } 


    void splitCell()    
    {
        if (is_leaf)    {
            // Lower Left son
            l_l = std::shared_ptr<Cell<T>>(new Cell<T>(base_point, 0.5*dx, 0.5*dy));
            // Lower right son
            l_r = std::shared_ptr<Cell<T>>(new Cell<T>(Point<T>(base_point.getX() + 0.5*dx, base_point.getY()), 0.5*dx, 0.5*dy));
            // Upper left son
            u_l = std::shared_ptr<Cell<T>>(new Cell<T>(Point<T>(base_point.getX(), base_point.getY() + 0.5*dy), 0.5*dx, 0.5*dy));
            // Upper right son
            u_r = std::shared_ptr<Cell<T>>(new Cell<T>(getCenter(), 0.5*dx, 0.5*dy));
            // Since it now has children, we set this to false
            is_leaf = false;
        }
    }

    std::vector<std::shared_ptr<Cell<T>>> getChildren() const
    {
        std::vector<std::shared_ptr<Cell<T>>> to_return;
        to_return.push_back(l_l);
        to_return.push_back(l_r);
        to_return.push_back(u_l);
        to_return.push_back(u_r);

        return to_return;
    }

    /*
    3        2
     --------
    |        |
    |        | 
    |        |
    |        |
     --------
    0        1    
    */

    std::vector<Point<T>> getVertices() const
    {
        std::vector<Point<T>> to_return;

        Point<T> one = Point<T>(base_point.getX() + dx, base_point.getY());
        Point<T> two = Point<T>(base_point.getX() + dx, base_point.getY() + dy);
        Point<T> three = Point<T>(base_point.getX(), base_point.getY() + dy);

        to_return.push_back(base_point);
        to_return.push_back(one);
        to_return.push_back(two);
        to_return.push_back(three);

        return to_return;
    }


};


#endif


