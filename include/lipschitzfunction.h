#ifndef LIPSCHITZFUNCTION_H
#define LIPSCHITZFUNCTION_H

#include "point.h"

// This class implements the concept of Lipschitz function
// which is nothing but a class with an operator () and a stored Lipschitz
// constant that can be used for many purposes.

template <typename T>
class LipschitzFunction
{
protected:
    T lip_const = 1.0;

public:
    LipschitzFunction() = default;
    LipschitzFunction(T lipschitz) : lip_const(lipschitz) {}
    virtual ~LipschitzFunction() = default;

    virtual T operator()(Point<T> arg) const = 0;

    T getLipschitzConstant() const { return lip_const; }
    void setLipschitzConstant(T new_c) { lip_const = new_c; }
};

#endif
