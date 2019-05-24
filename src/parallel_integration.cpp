#include "../include/parallel_integration.h"

// The arguments are : function to integrate / order of integration (0: naive, !0: 3rd order gaussian)
// bottom left point of the square domain / top right point of the sqaure domain
// minimum level / maximum level / refinement criterion for the tree

double parallel_integration(const std::function<double(Point<double>)> & func, unsigned order,
                       const Point<double> & l_left, const Point<double> & u_right, 
                       unsigned char min_lev, unsigned char max_lev, const RefinementCriterion<double> & criterion)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    unsigned char min_level_core; 
    unsigned char max_level_core;
    unsigned char nb_cores_x;
    double x_base;
    double y_base;

    double x_cell_size;
    double y_cell_size;


    unsigned level = unsigned(log(size)/log(4));


    if (rank == 0)  { // Performing preliminary controls.
        if (max_lev > 11)    {
            std::cerr<<"Be careful, the maximum level you have chose is very large. Possible memory overflow"<<std::endl;
            max_lev = 11;
        }
        if (!isPowerOf(size, 4))    {
            std::cerr<<"The number of cores is not a power of 4. We cannot proceed"<<std::endl;
            throw std::invalid_argument("The number of cores is not a power of 4. We cannot proceed");
        }

        if (min_lev < level)  {
            std::cerr<<"The minimum resolution is not enough to perform the splitting of the tree"<<std::endl;
            std::cerr<<"Changing minimum level accordingly"<<std::endl;
            min_lev = static_cast<unsigned char>(level);
        }
    }

    // This is the min level of the 
    // tree stored in each core
    min_level_core = static_cast<unsigned char>(min_lev-level); 
    max_level_core = static_cast<unsigned char>(max_lev-level);
    nb_cores_x = static_cast<unsigned char>(pow(2, level));

    x_cell_size = (u_right.getX()-l_left.getX()) / nb_cores_x;
    y_cell_size = (u_right.getY()-l_left.getY()) / nb_cores_x;

    x_base = l_left.getX();
    y_base = l_left.getY();
    
    // We are ready so that each one creates its Tree

    QuadTree<double> loc_tree(Point<double>(x_base + x_cell_size*(rank % nb_cores_x),
                                       y_base + y_cell_size*(rank / nb_cores_x)),
                                       x_cell_size, y_cell_size, min_level_core, max_level_core);

    loc_tree.buildUniform(max_level_core);
    loc_tree.updateQuadTree(criterion);


    double local_integral = 0;
    if(order == 0)  {
        local_integral = loc_tree.simpleIntegration(func);
    }
    else    {
        local_integral = loc_tree.thirdOrderGaussianIntegration(func);
    }
    
    double global_integral = 0.0;
    MPI_Allreduce(&local_integral, &global_integral, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); // We use it since 
                // its a parallel function.

    return global_integral;

}
