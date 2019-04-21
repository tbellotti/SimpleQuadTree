#ifndef REFINEALWAYSCRITERION_H
#define REFINEALWAYSCRITERION_H

#include "refinementcriterion.h"

template <typename T>
class RefineAlwaysCriterion : public RefinementCriterion<T>
{
public:
    RefineAlwaysCriterion(void) = default;
    virtual ~RefineAlwaysCriterion() = default;

    virtual bool operator()(std::shared_ptr<Cell<T>> arg) const override 
    {
        arg->getLevel(); // JUst to do something with arg in order to make
                        // the compiler not complaining..
        return true;
    }    
};

#endif