#ifndef CELLWITHFATHER_H
#define CELLWITHFATHER_H
#include <memory>
#include <vector>
//#include "cell.h"
#include "point.h"
#include "rgbcolor.h"

template <typename T>
class CellWithFather
{
protected:

    const Point<T> base_point;
    const T dx;
    const T dy;

    RGBColor field;
    const CellWithFather * father;

    // We should not a shared pointer because the object it points to
    // is not created with the smart point with new or similar and
    // this gives rise to double free errors
    // https://stackoverflow.com/questions/39987897/double-free-or-corruption-with-shared-pointers
    // when deallocating.

    std::shared_ptr<CellWithFather> l_l; // Lower-Left children
    std::shared_ptr<CellWithFather> l_r; // Lower-Right children
    std::shared_ptr<CellWithFather> u_l; // Upper-Left children
    std::shared_ptr<CellWithFather> u_r; // Upper-Right children


public:
    CellWithFather(Point<T> b_p, T new_dx, T new_dy, const CellWithFather<T> * new_f) : base_point(b_p), dx(new_dx), dy(new_dy), field(), father(new_f)  {}
    virtual ~CellWithFather() = default;


    T getDx() const { return dx; }
    T getDy() const { return dy; }
    T getDiagonal() const { return std::sqrt(dx*dx + dy*dy); } 
    T cellSurface() const { return dx * dy; }

    const CellWithFather<T> getFather() const
    {
        return father;
    }

    void setField(const RGBColor & new_color)
    {
        field = new_color;
    } 

    RGBColor getField() const
    {
        return field;
    }

    bool isLeaf() const
    {
        return (!l_l && !l_r && !u_l && !u_r);
    }

    void splitCell()
    {
        if (isLeaf())    {
        // Lower Left son
        l_l = std::shared_ptr<CellWithFather<T>>(new CellWithFather<T>(base_point, 0.5*dx, 0.5*dy, this));
            // Lower right son
        l_r = std::shared_ptr<CellWithFather<T>>(new CellWithFather<T>(base_point + Point<T>(0.5*dx, 0.0), 0.5*dx, 0.5*dy, this));
            // Upper left son
        u_l = std::shared_ptr<CellWithFather<T>>(new CellWithFather<T>(base_point + Point<T>(0.0, 0.5*dy), 0.5*dx, 0.5*dy, this));
            // Upper right son
        u_r = std::shared_ptr<CellWithFather<T>>(new CellWithFather<T>(getCenter(), 0.5*dx, 0.5*dy, this));
        }
    }
    
    void mergeCell()
    {
        l_l = nullptr;
        l_r = nullptr;
        u_l = nullptr;
        u_r = nullptr;
    }

    Point<T> getCenter() const 
    {
        return base_point + Point<T>(0.5*dx, 0.5*dy);
    }

std::vector<std::shared_ptr<CellWithFather<T>>> getChildren() const
    {
        std::vector<std::shared_ptr<CellWithFather<T>>> to_return;
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
std::ostream & operator<<(std::ostream & os, const CellWithFather<T> & cl)
{
    os<<"Center: "<<cl.getCenter()<<", dx = "<<cl.getDx()<<", dy = "<<cl.getDy();
    return os;
}


#endif

