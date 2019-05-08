#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <math.h>
#include <iomanip>

#include "../../include/parallel_integration.h"



template <typename T>
class Ellipse : public RefinementCriterion<T>
{
protected:
    T a;
    T b;

public:
    Ellipse(T an, T bn) : RefinementCriterion<T>(), a(an), b(bn)  {}
    ~Ellipse() = default;

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        T x = arg->getCenter().getX();
        T y = arg->getCenter().getY();

        return (pow(x/a, 2.0)+pow(y/b, 2.0)-1) <= 0;
    }
};

template <typename T>
class QuadrantElimination : public RefinementCriterion<T>
{
protected:
    const Ellipse<T> & ls;

public:
    QuadrantElimination(const Ellipse<T> & ls_n) : RefinementCriterion<T>(), ls(ls_n) {}
    ~QuadrantElimination() = default;

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        T x = arg->getCenter().getX();
        T y = arg->getCenter().getY();

        Ellipse<T> lsc(ls);

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

    double sigma_x = 0.1;
    double sigma_y = 0.05;

    std::function<double(Point<double>)> gaussian = [sigma_x, sigma_y](Point<double> pt) { 
        double x = pt.getX();
        double y = pt.getY();
        return 1/(2*M_PI*sigma_x*sigma_y)*exp(-1.0/2.0*(pow(x/sigma_x, 2.0) + pow(y/sigma_y, 2.0)));
    };

    // Paramters of the problem
    Point<double> lower_left_corner(-2.0, -2.0);
    Point<double> upper_right_corner(2.0, 2.0);

    Ellipse<double> ellipse(10.0 * sigma_x, 10.0 * sigma_y); 
    QuadrantElimination<double> criterion_quadrant_elimination(ellipse);
    RefineAlwaysCriterion<double> criterion_uniform;

    double integral;

    switch (example)    {
        case 1 : { integral = parallel_integration(gaussian, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, ellipse); break; }

        case 2 : { integral = parallel_integration(gaussian, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_quadrant_elimination); break;  }
        case 3 : { integral = parallel_integration(gaussian, order, lower_left_corner, 
                                           upper_right_corner, min_level, max_level, criterion_uniform); break;  }
    }

    if (rank == 0)  {
        std::cout<<rank<<std::setprecision(10)<<"- Normalization is = "<<integral<<std::endl;
        std::cout<<rank<<"- The error on Normalization is = "<<abs((integral-1.0)/1.0*100)<<" \%"<<std::endl;

        time = MPI_Wtime() - time;
        std::cout<<"Core "<<rank<<" Elapsed time = "<<time<<std::endl; 
    }

    MPI_Finalize();

    return 0;
}