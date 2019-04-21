#ifndef REFINEMENTCRITERIONCOLOR_H
#define REFINEMENTCRITERIONCOLOR_H

#include <memory>
#include <cmath>
#include "pixel.h"
#include "refinementcriterion.h"


template <typename T>
class CriterionVariance : public RefinementCriterion<T>
{
protected:
    double thr = 0.1;

public:
    CriterionVariance() = default;
    CriterionVariance(double n_thr) : thr(n_thr) {}

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {

        //We first have to cast the pointer
        std::shared_ptr<Pixel<T>> arg_cast = std::dynamic_pointer_cast<Pixel<T>>(arg);

        if (arg_cast->stdDevField() / 765.0 > thr)
            return true;
        else
            return false;
    }    
};

#endif

