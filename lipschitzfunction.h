#ifndef LIPSCHITZFUNCTION_H
#define LIPSCHITZFUNCTION_H

#include "point.h"


template <typename T>
class LipschitzFunction
{
protected:
    T lip_const = 1.0;

public:
    LipschitzFunction() = default;
    LipschitzFunction(T lipschitz) : lip_const(lipschitz) {}
    virtual T operator()(Point<T> arg) const = 0;

    T getLipschitzConstant() const { return lip_const; }
    void setLipschitzConstant(T new_c) { lip_const = new_c; }

};



#endif

