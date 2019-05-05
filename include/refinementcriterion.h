#ifndef REFINEMENTCRITERION_H
#define REFINEMENTCRITERION_H

#include <memory>
#include <cmath>
#include "cell.h"


/*
This pure virtual class implements the idea of refinement criterion
for a Cell of quadree. It is nothing but a class which as a () operator
on smart pointers to the cell, returning a boolean telling if we have 
refine it or not.
*/

template <typename T>
class RefinementCriterion
{

public:
    RefinementCriterion() = default;
    virtual ~RefinementCriterion() = default;
    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const = 0;    
};

#endif
