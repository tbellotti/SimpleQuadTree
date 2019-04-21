#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "quadtree.h"
#include "lipschitzfunction.h"

#include <mpi.h>

template <typename T>
class LevelSet : public LipschitzFunction<T>
{
public:
    LevelSet() : LipschitzFunction<T>() {}
    LevelSet(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {

        Point<double> tmp1 = arg+Point<double>(-0.5, -0.5);
        Point<double> tmp2 = arg+Point<double>(-0.25, -0.25);
        return std::min(tmp1.abs() - 0.075, tmp2.abs() - 0.12);
    }
};


template <typename T>
class LevelSetNew : public LipschitzFunction<T>
{
public:
    LevelSetNew() : LipschitzFunction<T>() {}
    LevelSetNew(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {

        Point<double> tmp1 = arg+Point<double>(-0.52, -0.55);
        Point<double> tmp2 = arg+Point<double>(-0.23, -0.25);
        return std::min(tmp1.abs() - 0.070, tmp2.abs() - 0.13);
    }
};

long long int factorial (long long int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int main () 
{

    MPI_Init(NULL, NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    unsigned int data_size;
    double time = MPI_Wtime();

    std::function<double(Point<double>)> to_integrate = [](Point<double> pt) { 
            //return pow(pt.getX(), 2.0) + 2.0*pow(pt.getY(), 2.0) + pt.getX()*pt.getY();
            //return pow(pt.getX()*sin(pt.getY()), 4.0) + exp(abs(cos(pt.getX())));
            
            double to_return_x = 0.0;
            double to_return_y = 0.0;
            for (int n = 0; n < 40; n++)   {
                to_return_x += pow(pt.getX(), n) / factorial(n);
                to_return_y += pow(2.0*pt.getY(), n) / factorial(n);
            }



            return to_return_x + to_return_y;

            

            //return pow(pt.getX(), 2.0) + pow(pt.getY(), 3.0) - 2.0*pt.getX()*pt.getY();
    };

    std::vector<double> pt_x;
    std::vector<double> pt_y;
    std::vector<double> weights;

    std::vector<int> offset(size);
    std::vector<int> chunk_size(size);

    if (rank == 0)  {
        QuadTree<double> my_tree(1.0, 1.0, 10, 13);
    
        my_tree.buildUniform(10);

        LevelSet<double> my_level_set(1.2);
        my_tree.updateWithLevelSet(&my_level_set);
        
        auto leaves = my_tree.getLeaves();
        for (auto leaf : leaves)    {
            pt_x.push_back(leaf->getCenter().getX());
            pt_y.push_back(leaf->getCenter().getY());
            weights.push_back(leaf->cellSurface());
        }

        data_size = weights.size();


        for (int i = 0; i < size; i++)  {
            int basic_chunk_size = data_size / size;
            int remainder = data_size % size;
            int start, stop;

            if (i < remainder)  {
                // The first 'remainder' ranks get 'count + 1' tasks each
                start = i * (basic_chunk_size + 1);
                stop = start + basic_chunk_size;
            }   
            else    {
                // The remaining 'size - remainder' ranks get 'count' task each
                start = i * basic_chunk_size + remainder;
                stop = start + (basic_chunk_size - 1);
            }         
            offset[i] = start;
            chunk_size[i] = stop - start + 1;
        }

    }
    /*
    if (rank == 0)  {
        
        for (int i = 0; i < size; i++)  {
            std::cout<<"Chunk size = "<<chunk_size[i]<<" Offset = "<<offset[i]<<std::endl;
        }
    }
    */
    int local_chunk_size;

    std::vector<double> local_x;
    std::vector<double> local_y;
    std::vector<double> local_weights;

    if (rank == 0)   {
        local_chunk_size = chunk_size[0];

        for (int i = 1; i < size; i++)  {
            int to_send = chunk_size[i];
            MPI_Send(&to_send, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            //std::cout<<"Sending to: "<<i<<std::endl;
        }
    }
    else    {
        MPI_Recv(&local_chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //std::cout<<"Rank "<<rank<<" - Chunk size = "<<local_chunk_size<<std::endl;
    }

    local_x.resize(local_chunk_size);
    local_y.resize(local_chunk_size);
    local_weights.resize(local_chunk_size);



    MPI_Scatterv(pt_x.data(), chunk_size.data(), offset.data(),MPI_DOUBLE, local_x.data(), local_chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(pt_y.data(), chunk_size.data(), offset.data(),MPI_DOUBLE, local_y.data(), local_chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(weights.data(), chunk_size.data(), offset.data(),MPI_DOUBLE, local_weights.data(), local_chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);



    double local_integral = 0.0;

    for (unsigned int i = 0; i < local_chunk_size; i++)   {
        Point<double> current_point (local_x[i], local_y[i]);
        local_integral += local_weights[i] * to_integrate(current_point);
    }


    double final_value = 0.0;

    MPI_Reduce(&local_integral, &final_value, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)  {
        std::cout<<"Integral = "<<final_value<<std::endl;
    }

    time = MPI_Wtime() - time;
    std::cout<<"Core "<<rank<<" Elapsed time = "<<time<<std::endl; 

    MPI_Finalize();


    return 0;
} 

