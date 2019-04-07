#ifndef PIXEL_H
#define PIXEL_H

#include "cell.h"
#include "rgbcolor.h"

template <typename T>
class Pixel : public Cell<T>
{
protected:
    RGBColor field;
    // Avoids to repeat long computations
    // when compressing the image, while generating
    // only a small overhead of data.
    bool flag = false;


public:

    // To be done better
    bool getFlag() const
    {
        return flag;
    }

    void setFlag(bool nf)
    {
        flag = nf;
    }

    Pixel(Point<T> b_p, T new_dx, T new_dy) : Cell<T>(b_p, new_dx, new_dy), field() {} 
    Pixel(Point<T> b_p, T new_dx, T new_dy, RGBColor n_col) : Cell<T>(b_p, new_dx, new_dy), field(n_col) {} 
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
            this->l_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point, 0.5*this->dx, 0.5*this->dy, field));
            // Lower right son
            this->l_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.5*this->dx, 0.0), 0.5*this->dx, 0.5*this->dy, field));
            // Upper left son
            this->u_l = std::shared_ptr<Cell<T>>(new Pixel<T>(this->base_point + Point<T>(0.0, 0.5*this->dy), 0.5*this->dx, 0.5*this->dy, field));
            // Upper right son
            this->u_r = std::shared_ptr<Cell<T>>(new Pixel<T>(this->getCenter(), 0.5*this->dx, 0.5*this->dy, field));
        }
    }


    std::vector<std::shared_ptr<Pixel<T>>> getLeavesCast() const
    {
        std::vector<std::shared_ptr<Pixel<T>>> to_return;
 
        // As we have implemented it, it will return only the base cell
        // if it does not contain children (it is the leaf)
        /*
        if (this->isLeaf())
        {
            to_return.push_back(this->shared_from_this());
        }
        else{
        */
        std::vector<std::shared_ptr<Cell<T>>> ch_l_l = this->l_l->getLeaves();//getLeaves(this->l_l);
        std::vector<std::shared_ptr<Cell<T>>> ch_l_r = this->l_r->getLeaves();//getLeaves(this->l_r);
        std::vector<std::shared_ptr<Cell<T>>> ch_u_l = this->u_l->getLeaves();//getLeaves(this->u_l);
        std::vector<std::shared_ptr<Cell<T>>> ch_u_r = this->u_r->getLeaves();//getLeaves(this->u_r);
        
        std::vector<std::shared_ptr<Cell<T>>> leaves;
        leaves.reserve(ch_l_l.size()+ch_l_r.size()+ch_u_l.size()+ch_u_r.size());
        leaves.insert(leaves.end(), ch_l_l.begin(), ch_l_l.end());
        leaves.insert(leaves.end(), ch_l_r.begin(), ch_l_r.end());
        leaves.insert(leaves.end(), ch_u_l.begin(), ch_u_l.end());
        leaves.insert(leaves.end(), ch_u_r.begin(), ch_u_r.end());
        

        // Casting the pointers
        for (auto el : leaves)  {
            to_return.push_back(std::static_pointer_cast<Pixel<T>>(el));
        }   
        return to_return;
    }


    // We suppose that what matters is 
    RGBColor meanField() const
    {   
        if (this->isLeaf())
            return field;

        else    {
            std::vector<std::shared_ptr<Pixel<T>>> leaves_cast = getLeavesCast();

            double prefactor = 1.0/((double) leaves_cast.size());
            auto it = leaves_cast.cbegin();

            RGBColor sum = (*it)->getField();

            it++;


            for ( ; it < leaves_cast.cend(); it++)   {

                sum += (*it)->getField();

            }

            return prefactor * sum;
            }
        }

    double stdDevField() const
    {
        

        if (this->isLeaf()) {
            // No variance inside
            return 0.0;
        }
        else    {
            RGBColor mean_color = meanField();

            std::vector<std::shared_ptr<Pixel<T>>> leaves = getLeavesCast();
            double prefactor = 1.0/((double) leaves.size());
            auto it = leaves.cbegin();

            //RGBColor diff = (*it)->getField() - mean_color; 
            //double sum = diff.abs();
            double sum = mean_color.distanceEuclidianCorrected((*it)->getField());
            it++;
            for ( ; it < leaves.cend(); it++)   {
                //diff = (*it)->getField() - mean_color; 
                sum += mean_color.distanceEuclidianCorrected((*it)->getField());
            }   
            return sqrt(prefactor * sum);
        }
    }

    virtual void mergeCell() override
    {
        setField(meanField());

        this->l_l = nullptr;
        this->l_r = nullptr;
        this->u_l = nullptr;
        this->u_r = nullptr;
    }


};


#endif
