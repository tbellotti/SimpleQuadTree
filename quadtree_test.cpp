#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

#include "quadtree.h"
#include "lipschitzfunction.h"

template <typename T>
class LevelSet : public LipschitzFunction<T>
{
public:
    LevelSet() : LipschitzFunction<T>() {}
    LevelSet(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {
        Point<T> tmp = arg+Point<T>(-0.5, -0.5);
        return tmp.abs() - 0.15;
    }
};

template <typename T>
struct BooleanCriterion
{

    bool operator()(Point<T> arg) const 
    {
        return (arg.getX() > 0.5);
    }
};

int main () 
{

    QuadTree<double> my_tree(1.0, 1.0, 3, 10);
    
    my_tree.buildUniform();




    LevelSet<double> my_level_set;
    BooleanCriterion<double> test_boolean_criterion;

    //std::function<bool(double)> crit(test_boolean_criterion);
    //my_tree.refineWithCriterion([](Point<double> arg) {return (arg.getX() > 0.5);} );

    my_tree.refineWithLevelSet([](Point<double> arg) { 
        Point<double> tmp1 = arg+Point<double>(-0.5, -0.5);
        Point<double> tmp2 = arg+Point<double>(-0.25, -0.25);
        return std::min(tmp1.abs() - 0.05, tmp2.abs() - 0.1); } );

    //my_tree.exportCentersTikz(std::string("draw.tex"));
    my_tree.exportMeshTikz(std::string("draw.tex"));


/*
    std::vector<Point<double>> centers = my_tree.getCenters();

    for (auto ct : centers) {
        std::cout<<ct.print()<<std::endl;
    }

*/
    return 0;
}

