#ifndef PIXEL_H
#define PIXEL_H

#include "cell.h"
#include "rgbcolor.h"

template <typename T>
class Pixel : public Cell<T>
{
protected:
    RGBColor field;

public:

    Pixel(Point<T> b_p, T new_dx, T new_dy, unsigned char lv) : Cell<T>(b_p, new_dx, new_dy, lv), field() {} 
    Pixel(Point<T> b_p, T new_dx, T new_dy, unsigned char lv, RGBColor n_col) : Cell<T>(b_p, new_dx, new_dy, lv), field(n_col) {} 
    virtual ~Pixel() = default;

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
            this->l_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point, 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            // Lower right son
            this->l_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.5*this->dx, 0.0), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            // Upper left son
            this->u_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.0, 0.5*this->dy), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            // Upper right son
            this->u_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->getCenter(), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
        }
    }
    
    // Due to the way of building the vector of leaves with 
    // non constant pointers, we cannot have const (ASK)
    RGBColor meanField()// const
    {   
        if (this->isLeaf())
            return field;

        else    {
            std::vector<std::shared_ptr<Cell<T>>> leaves;// = this->getLeaves();
            this->getLeaves(leaves);
            std::vector<std::shared_ptr<Pixel<T>>> leaves_cast;// = getLeavesCast()
            castVectorOfCellToVectorOfPixel(leaves, leaves_cast);

            std::vector<RGBColor> leaves_colors;

            for (auto lf : leaves_cast) {
                leaves_colors.push_back(lf->getField());
            }
            return meanColor(leaves_colors);
            }
        }

    // We have to eliminate the const qualifier
    // otherwise we cannot get the leaves in a vector and
    // eventually modify them
    double stdDevField()// const
    {
        if (this->isLeaf()) {
            // No variance inside
            return 0.0;
        }
        else    {
            RGBColor mean_color = meanField();

            std::vector<std::shared_ptr<Cell<T>>> leaves;
            this->getLeaves(leaves);
            std::vector<std::shared_ptr<Pixel<T>>> leaves_cast;// = getLeavesCast();
            castVectorOfCellToVectorOfPixel(leaves, leaves_cast);

            double prefactor = 1.0/(double(leaves_cast.size()));
            double sum = 0.0;
            for (auto it = leaves_cast.cbegin() ; it < leaves_cast.cend(); it++)   {
                sum += pow(mean_color.distanceEuclidianCorrected((*it)->getField()),2);
            }   
            return prefactor*sqrt(sum);
        }
    }

    virtual void mergeCell() override
    {
        // We need to override since we have to take the mean field.
        setField(meanField());

        this->l_l = nullptr;
        this->l_r = nullptr;
        this->u_l = nullptr;
        this->u_r = nullptr;
    }


};

// Utility to cast the vector of pointers to the base class Cell
// to a vector of pointers to Pixel.
template <typename T>
void castVectorOfCellToVectorOfPixel(const std::vector<std::shared_ptr<Cell<T>>> & input,
                                           std::vector<std::shared_ptr<Pixel<T>>> & output)
{
    output.clear();
    for (auto cell : input) {
        output.push_back(std::dynamic_pointer_cast<Pixel<T>>(cell));
    }

}

#endif
