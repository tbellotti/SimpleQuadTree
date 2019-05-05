#ifndef PIXEL_H
#define PIXEL_H

#include "cell.h"
#include "rgbcolor.h"

/*
This class implements the notion of generalized pixel of an image, inheriting
from the class Cell, adding only features necessary to what the class is designed
for and to correctly handle the cast of the smart pointers involved.
*/

template <typename T>
class Pixel : public Cell<T>
{
protected:
    RGBColor field; // This is the color of the pixel. We slightly leak
                    // in memory when storing the value also for non-leaf
                    // cells, but it is not easy to avoid this and this is 
                    // normal because we have to store the value for ex-non-leaf
                    // cells when their childrens are eliminated.

public:

    Pixel(Point<T> b_p, T new_dx, T new_dy, unsigned char lv) : Cell<T>(b_p, new_dx, new_dy, lv), field() {} 
    Pixel(Point<T> b_p, T new_dx, T new_dy, unsigned char lv, const RGBColor & n_col) : Cell<T>(b_p, new_dx, new_dy, lv), field(n_col) {} 
    virtual ~Pixel() = default;

    void setField(const RGBColor & new_color)
    {
        field = new_color;
    } 

    RGBColor getField() const
    {
        return field;
    }

    // This must be rewritten even if it is really close to what happens in the class Cell
    // because we must bind the pointer to the children (which remains of Cell type) to
    // new Pixels, because they need to store the RGBColor.
    virtual void splitCell() override 
    {
        if (this->isLeaf())    {
            this->l_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point, 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            this->l_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.5*this->dx, 0.0), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            this->u_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.0, 0.5*this->dy), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
            this->u_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->getCenter(), 0.5*this->dx, 0.5*this->dy, static_cast<unsigned char>(this->level+1), field));
        }
    }
    
    // Due to the way of building the vector of leaves with 
    // non constant pointers (see issue discussed in the comments for the class Cell),
    // we cannot have const
    // This computes the mean color field for the pixels contained in the current generalized pixel.
    RGBColor meanField() // const
    {   
        if (this->isLeaf()) // The mean is the value of the field itself.
            return field;
        else    {
            std::vector<std::shared_ptr<Cell<T>>> leaves;
            this->getLeaves(leaves);
            std::vector<std::shared_ptr<Pixel<T>>> leaves_cast;
            castVectorOfCellToVectorOfPixel(leaves, leaves_cast); // They need to be cast in order to  
                                                                  // recover the field on them.

            std::vector<RGBColor> leaves_colors; 
            for (auto lf : leaves_cast) {
                leaves_colors.push_back(lf->getField());
            }

            return meanColor(leaves_colors);
            }
        }

    // Same remark than for the previous function concerning the const qualifier.
    // Gives the stardard deviation of color inside the generalized pixel.
    double stdDevField() // const
    {
        if (this->isLeaf()) { // We have only one color inside thus the variance is zero.
            return 0.0;
        }
        else    {
            RGBColor mean_color = meanField();

            std::vector<std::shared_ptr<Cell<T>>> leaves;
            this->getLeaves(leaves);
            std::vector<std::shared_ptr<Pixel<T>>> leaves_cast;
            castVectorOfCellToVectorOfPixel(leaves, leaves_cast); // Again, the cast is necessary.

            double prefactor = 1.0/(double(leaves_cast.size()));
            double sum = 0.0;
            // Adds contributions to the stardard deviation in terms of colors inside
            // the cell.
            for (auto it = leaves_cast.cbegin() ; it < leaves_cast.cend(); it++)   {
                // We use the Euclidian distance with a correction to adapt it to the human
                // perception of colors (see discussion in https://en.wikipedia.org/wiki/Color_difference).
                sum += pow(mean_color.distanceEuclidianCorrected((*it)->getField()),2);
            }   

            return prefactor*sqrt(sum);
        }
    }

    // This needs to be overridden from Cell because before eliminating the children,
    // we have to save information and compute the mean of their colors.
    virtual void mergeCell() override
    {
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
