#ifndef REFINEMENTCRITERION_H
#define REFINEMENTCRITERION_H

#include "point.h"


template <typename T>
class RefinementCriterion
{

public:
    RefinementCriterion() = default;
    virtual bool operator()(Point<T> arg) const = 0;
};


#endif
