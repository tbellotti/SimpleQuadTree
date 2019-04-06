#ifndef CELLWITHPARENT_H
#define CELLWITHPARENT_H
#include <memory>
#include <vector>
#include "cell.h"
#include "point.h"
#include "rgbcolor.h"

template <typename T>
class CellWithParent : public Cell<T>
{
protected:
    RGBColor field;
    const CellWithParent * parent;

public:
    CellWithParent(Point<T> b_p, T new_dx, T new_dy, const CellWithParent<T> * new_f) : Cell<T>(b_p, new_dx, new_dy), parent(new_f)  {}
    virtual ~CellWithParent() override = default;

    const CellWithParent<T> * getFather() const
    {
        return parent;
    }

    void setField(const RGBColor & new_color)
    {
        field = new_color;
    } 

    RGBColor getField() const
    {
        return field;
    }


    virtual void splitCell() override
    {
        if (this->isLeaf())    {
            // Lower Left son
            this->l_l = std::shared_ptr<Cell<T>>(new CellWithParent<T>(this->base_point, 0.5*this->dx, 0.5*this->dy, this));
            // Lower right son
            this->l_r = std::shared_ptr<Cell<T>>(new CellWithParent<T>(this->base_point + Point<T>(0.5*this->dx, 0.0), 0.5*this->dx, 0.5*this->dy, this));
            // Upper left son
            this->u_l = std::shared_ptr<Cell<T>>(new CellWithParent<T>(this->base_point + Point<T>(0.0, 0.5*this->dy), 0.5*this->dx, 0.5*this->dy, this));
            // Upper right son
            this->u_r = std::shared_ptr<Cell<T>>(new CellWithParent<T>(this->getCenter(), 0.5*this->dx, 0.5*this->dy, this));
        }
    }

};


#endif

