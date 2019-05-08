#include <functional>
#include <mpi.h>
#include "quadtree.h"


/*
This does not contain a class, but just a parallel MPI function
which can be used to integrate a scalar function on an adaptive 
quadtree.
*/
double parallel_integration(const std::function<double(Point<double>)> &, unsigned,
                       const Point<double> &, const Point<double> &, 
                       unsigned char, unsigned char, const RefinementCriterion<double> &);
