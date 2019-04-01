#ifndef REFINEMENTCRITERIONCOLOR_H
#define REFINEMENTCRITERIONCOLOR_H

#include <memory>
#include <cmath>
#include "cellwithfather.h"

template <typename T>
class RefinementCriterionColor
{
protected:
    double thr = 0.1;

public:
    RefinementCriterionColor() = default;
    RefinementCriterionColor(double n_thr) : thr(n_thr) {}

    bool operator()(std::shared_ptr<CellWithFather<T>> arg) const 
    {
        // Observe that 441.7 is the maximum value of 
        // the Euclidian difference between two colors.

        if (stdDevField(arg) / 441.7 > thr)
            return true;
        else
            return false;
    }    
};

#endif

