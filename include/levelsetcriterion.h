#ifndef LEVELSETCRITERION_H
#define LEVELSETCRITERION_H

#include "refinementcriterion.h"
#include "lipschitzfunction.h"

// This is used just by the code. It is not used by the user 
// on general basis.
template <typename T>
class LevelSetCriterion : public RefinementCriterion<T>
{
protected:
    // !!!!! Think about the fact that we can like to
    // change the level-set without
    // having to declare a new class for the criterion
    // I guess it will have to be changed

    // Indeed no because this criterion is only used by
    // the tree which does not have the right and the necessity
    // to modify the level-set function.
    const LipschitzFunction<T> & level_set; // We need a pointer because
                        // the classes we use inherits from it
                        // and a reference is not assignable

public:
    LevelSetCriterion(const LipschitzFunction<T> & ls) : RefinementCriterion<T>(), level_set(ls) {}
    virtual ~LevelSetCriterion() = default;

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        return (std::abs((level_set)(arg->getCenter())) <= level_set.getLipschitzConstant() * arg->getDiagonal());
    }
};

#endif

