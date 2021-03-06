#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <math.h>
#include <iomanip>

#include "../../include/parallel_integration.h"


template <typename T>
class LevelSet : public LipschitzFunction<T>
{
public:
    LevelSet() : LipschitzFunction<T>() {}
    LevelSet(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {
        double radius = 0.15;

        double value = 100000.0;


        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {


                Point<double> tmp1 = arg+Point<double>(-(double)(2*i+1), -(double)(2*j+1));

                value = std::min(tmp1.abs()-radius, value);

            }
        }
        return value;
    }
};



template <typename T>
class QuadrantElimination : public RefinementCriterion<T>
{
protected:
    const LevelSet<T> & ls;

public:
    QuadrantElimination(const LevelSet<T> & ls_n) : RefinementCriterion<T>(), ls(ls_n) {}
    ~QuadrantElimination() = default;

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        T x = arg->getCenter().getX();
        T y = arg->getCenter().getY();

        LevelSetCriterion<T> lsc(ls);

        return ((x-4.0)*(y-4.0) >= 0.0) && lsc(arg);
    }
};

// What we expect : 
// min_level | max_level | order | example
// where:
// * order : 0 -> simpleIntegration | 1 -> thirdOrderGaussian
// * example : 0, 1, 2 

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);

    double time = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned char min_level = 3;
    unsigned char max_level = 10;

    unsigned char order = 0;
    unsigned char example = 1;

    if (argc < 5)  {
        std::cerr<<"Too few arguments, tell me the minimum and the maximum level"<<std::endl;
        return -1;
    }
    else    {
        if (std::atoi(argv[2]) > 11)    {
            std::cerr<<"Be careful, the maximum level you have chose is very large. Possible memory overflow"<<std::endl;
            max_level = 11;
            min_level = static_cast<unsigned char>(std::atoi(argv[1]));
        }
        else    {
            example = static_cast<unsigned char>(std::atoi(argv[4]));
            order = static_cast<unsigned char>(std::atoi(argv[3]));
            max_level = static_cast<unsigned char>(std::atoi(argv[2]));
            min_level = static_cast<unsigned char>(std::atoi(argv[1]));
        }
    }

    std::function<double(Point<double>)> circle_indicator_function = [](Point<double> pt) { 
        LevelSet<double> ls(1.2); 

        return (ls(pt) <= 0.0);
    };


    // Paramters of the problem
    Point<double> lower_left_corner(0.0, 0.0);
    Point<double> upper_right_corner(8.0, 8.0);

    LevelSet<double> ls(1.2);
    LevelSetCriterion<double> criterion_level_set(ls);
    QuadrantElimination<double> criterion_quadrant_elimination(ls);
    RefineAlwaysCriterion<double> criterion_uniform;

    double integral;

    switch (example)    {
        case 1 : { integral = parallel_integration(circle_indicator_function, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_level_set); break; }

        case 2 : { integral = parallel_integration(circle_indicator_function, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_quadrant_elimination); break;  }
        case 3 : { integral = parallel_integration(circle_indicator_function, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_uniform); break;  }
    }

    if (rank == 0)  {
        std::cout<<rank<<std::setprecision(10)<<"- 9 Pi/25 is = "<<integral<<std::endl;
        std::cout<<rank<<"- The error on 9 Pi/25 is = "<<abs((integral-9*M_PI/25)/(9*M_PI/25)*100)<<" \%"<<std::endl;

        time = MPI_Wtime() - time;
        std::cout<<"Core "<<rank<<" Elapsed time = "<<time<<std::endl; 
    }

    MPI_Finalize();

    return 0;
}
