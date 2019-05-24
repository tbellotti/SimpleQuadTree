#include <functional>
#include <mpi.h>
#include "quadtree.h"


/*
This does not contain a class, but just a parallel MPI function
which can be used to integrate a scalar function on an adaptive 
quadtree.
*/

// The arguments are : function to integrate / order of integration (0: naive, !0: 3rd order gaussian)
// bottom left point of the square domain / top right point of the sqaure domain
// minimum level / maximum level / refinement criterion for the tree

double parallel_integration(const std::function<double(Point<double>)> &, unsigned,
                       const Point<double> &, const Point<double> &, 
                       unsigned char, unsigned char, const RefinementCriterion<double> &);
