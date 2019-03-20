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
    // !!!!! Think about the fact that we can like to
    // change the level-set without
    // having to declare a new class for the criterion
    // I guess it will have to be changed
    const LipschitzFunction<T> * level_set;

public:
    LevelSetCriterion(const LipschitzFunction<T> * ls) : RefinementCriterion<T>(), level_set(ls) {}
    ~LevelSetCriterion() = default;
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
