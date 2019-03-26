#ifndef REFINEMENTCRITERION_H
#define REFINEMENTCRITERION_H

#include <memory>
#include <cmath>
#include "cell.h"

template <typename T>
class RefinementCriterion
{

public:
    RefinementCriterion() = default;
    virtual ~RefinementCriterion() = default;

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const = 0;    
};

#endif

