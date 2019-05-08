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
        return std::max(abs(arg.getX()-0.25)-0.75,abs(arg.getY()-0.25)-0.75);
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

        return (x*y >= 0.0) && lsc(arg);
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

    std::function<double(Point<double>)> inertia_density = [](Point<double> pt) { 
        double x = pt.getX();
        double y = pt.getY();

        LevelSet<double> ls(1.2);
        return (x*x + y*y) * (cos(M_PI * x) + sin(M_PI * y))*(ls(pt)<=0);
    };
    
    // Paramters of the problem
    Point<double> lower_left_corner(-2.0, -2.0);
    Point<double> upper_right_corner(2.0, 2.0);

    LevelSet<double> ls(1.2);
    LevelSetCriterion<double> criterion_level_set(ls);
    QuadrantElimination<double> criterion_quadrant_elimination(ls);
    RefineAlwaysCriterion<double> criterion_uniform;

    double integral;

    switch (example)    {
        case 1 : { integral = parallel_integration(inertia_density, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_level_set); break; }

        case 2 : { integral = parallel_integration(inertia_density, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_quadrant_elimination); break;  }
        case 3 : { integral = parallel_integration(inertia_density, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_uniform); break;  }
    }

    if (rank == 0)  {
        double inmom = (3.0 * (-16.0 - 12.0 * M_PI + 7.0 * M_PI*M_PI))/(8.0 * M_PI*M_PI*M_PI);
        std::cout<<rank<<std::setprecision(10)<<"- Inertia moment is = "<<integral<<std::endl;
        std::cout<<rank<<"- The error on Inertia moment is = "<<abs((integral-inmom)/inmom*100)<<" \%"<<std::endl;
        time = MPI_Wtime() - time;
        std::cout<<"Core "<<rank<<" Elapsed time = "<<time<<std::endl; 
    }

    MPI_Finalize();

    return 0;
}
