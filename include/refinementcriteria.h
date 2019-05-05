#ifndef REFINEMENTCRITERIA_H
#define REFINEMENTCRITERIA_H

#include <cmath>
#include "refinementcriterion.h"
#include "lipschitzfunction.h"
#include "pixel.h"

// In this file, we have several classes which pratically implement
// several version of the RefinementCriterion which are no longer
// pure virtual, since they are effectively used in the project in order
// to achieve some kind of refinement/simplification of the quadtree.

// Refine whatever happens in the cell.
template <typename T>
class RefineAlwaysCriterion : public RefinementCriterion<T>
{
public:
    RefineAlwaysCriterion(void) = default;
    virtual ~RefineAlwaysCriterion() = default;

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const override 
    {
        arg->getLevel(); // Just to do something with arg in order to make
                        // the compiler not complaining..
        return true;
    }    
};

// Refinement criterion based on a level-set function
template <typename T>
class LevelSetCriterion : public RefinementCriterion<T>
{
protected:
    // We do not need to touch it because this criterion is only used by
    // the tree which does not have the right and the necessity
    // to modify the level-set function.
    const LipschitzFunction<T> & level_set; 

public:
    LevelSetCriterion(const LipschitzFunction<T> & ls) : RefinementCriterion<T>(), level_set(ls) {}
    virtual ~LevelSetCriterion() = default;

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        return (std::abs((level_set)(arg->getCenter())) <= level_set.getLipschitzConstant() * arg->getDiagonal());
    }
};

// Criterion based on the color variance. It supposes that we are actually
// dealing with Pixels rather than simple Cells.
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
        // We first have to cast the pointer in order to retrieve the 
        // color field inside the cell.
        std::shared_ptr<Pixel<T>> arg_cast = std::dynamic_pointer_cast<Pixel<T>>(arg);

        if (arg_cast->stdDevField() / 765.0 > thr)
            return true;
        else
            return false;
    }    
};

#endif
