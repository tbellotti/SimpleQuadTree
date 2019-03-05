#ifndef REFINEMENTCRITERION_H
#define REFINEMENTCRITERION_H

#include <memory>
#include <cmath>
#include "cell.h"
#include "lipschitzfunction.h"

template <typename T>
class RefinementCriterion
{

public:
    RefinementCriterion() = default;
    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const = 0;
};




template <typename T>
class LevelSetCriterion : public RefinementCriterion<T>
{
protected:
    LipschitzFunction<T> * level_set;

public:
    LevelSetCriterion(LipschitzFunction<T> * ls) : RefinementCriterion<T>(), level_set(ls) {}
    /*
    void setLevelSet(const LipschitzFunction<T> & func)
    {
        level_set = func;
    }
    */
    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        return (std::abs((*level_set)(arg->getCenter())) <= level_set->getLipschitzConstant() * arg->getDiagonal());
    }
};


#endif
