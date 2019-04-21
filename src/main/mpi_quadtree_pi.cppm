#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <math.h>
#include <iomanip>

#include "../include/quadtree.h"
//#include "lipschitzfunction.h"
#include <mpi.h>


template <typename T>
class LevelSet : public LipschitzFunction<T>
{
public:
    LevelSet() : LipschitzFunction<T>() {}
    LevelSet(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {
        return (sqrt(pow(arg.getX(), 2.0) + pow(arg.getY(), 2.0)) - 1.0);
    }
};

std::function<double(Point<double>)> circle_indicator_function = [](Point<double> pt) { 
        
    if (pow(pt.getX(), 2.0) + pow(pt.getY(), 2.0) < 1.0)   {
        return 1.0;
    }
    else    {
        return 0.0;
    }
};


int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);

    double time = MPI_Wtime();


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

    unsigned char min_level = 3;
    unsigned char max_level = 10;

    if (rank == 0)  {

        if (argc <= 1)  {
            std::cerr<<"Too few arguments, tell me the minimum and the maximum level"<<std::endl;
            return -1;
        }

        if (argc == 2)  {
            std::cerr<<"You provided only the minimum level. The maximum will be chosen automatically"<<std::endl;
            min_level = std::atoi(argv[1]);
            max_level = 10;
        }
        else    {
            if (std::atoi(argv[2]) > 11)    {
                std::cerr<<"Be careful, the maximum level you have chose is very large. Possible memory overflow"<<std::endl;
                max_level = 11;
                min_level = std::atoi(argv[1]);
            }
            else    {
                max_level = std::atoi(argv[2]);
                min_level = std::atoi(argv[1]);
            }
        }




        // Paramters of the problem
        Point<double> lower_left_corner(-2.0, -2.0);
        Point<double> upper_right_corner(2.0, 2.0);


        // We check that the number of cores
        // is a power of 4.
        if (!isPowerOf(size, 4))    {
            std::cerr<<"The number of cores is not a power of 4. We cannot proceed"<<std::endl;
            return -1;
        }

        unsigned level = log(size)/log(4);

        if (min_level < level)  {
            std::cerr<<"The minimum resolution is not enough to perform the splitting of the tree"<<std::endl;
            std::cerr<<"Changing minimum level accordingly"<<std::endl;
            //return -1;

            min_level = level;
        }

        // This is the min level of the 
        // tree stored in each core
        min_level_core = min_level-level; 
        max_level_core = max_level-level;
        nb_cores_x = pow(2, level);

        x_cell_size = (upper_right_corner.getX()-lower_left_corner.getX()) / nb_cores_x;
        y_cell_size = (upper_right_corner.getY()-lower_left_corner.getY()) / nb_cores_x;

        x_base = lower_left_corner.getX();
        y_base = lower_left_corner.getY();
    }

    // This are common to all the process in terms of values
    MPI_Bcast(&min_level_core, 1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_level_core, 1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x_cell_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y_cell_size, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x_base, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y_base, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&nb_cores_x, 1, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    // We are ready so that each one creates its Tree

    QuadTree<double> loc_tree(Point<double>(x_base + x_cell_size*(rank % nb_cores_x),
                                       y_base + y_cell_size*(rank / nb_cores_x)),
                                       x_cell_size, y_cell_size, min_level_core, max_level_core);

    loc_tree.buildUniform(max_level_core);
    LevelSet<double> ls(1.2);
    loc_tree.updateWithLevelSet(ls);

    std::ostringstream outputname;
    outputname<<"./media/tree_core"<<rank<<".tex";
    //loc_tree.exportMeshTikz(outputname.str(), false);

    double local_integral = loc_tree.thirdOrderGaussianIntegration(circle_indicator_function);
    //double local_integral = loc_tree.simpleIntegration(circle_indicator_function);

    double global_integral = 0.0;
    MPI_Reduce(&local_integral, &global_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)  {
        std::cout<<rank<<std::setprecision(10)<<"- Pi is = "<<global_integral<<std::endl;
        std::cout<<rank<<"- The error on Pi is = "<<abs((global_integral-M_PI)/M_PI*100)<<" \%"<<std::endl;

        time = MPI_Wtime() - time;
        std::cout<<"Core "<<rank<<" Elapsed time = "<<time<<std::endl; 
    }




    MPI_Finalize();

    return 0;
}