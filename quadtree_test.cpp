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

        Point<double> tmp1 = arg+Point<double>(-0.5, -0.5);
        Point<double> tmp2 = arg+Point<double>(-0.25, -0.25);
        return std::min(tmp1.abs() - 0.075, tmp2.abs() - 0.12);
    }
};

template <typename T>
class TestCriterion : public RefinementCriterion<T>
{
public:
    TestCriterion() : RefinementCriterion<T>()  {}

    bool operator()(Point<T> arg) const override
    {
        return ((arg.getY() - arg.getX()) > 0.0);
    }
};

template <typename T>
struct BooleanCriterion
{

    bool operator()(Point<T> arg) const 
    {
        return ((arg.getY() - arg.getX()) > 0.0);
    }
};

int main () 
{

    QuadTree<double> my_tree(1.0, 1.0, 2, 7);
    
    my_tree.buildUniform();


    LevelSet<double> my_level_set(1.2);
    BooleanCriterion<double> test_boolean_criterion;

    //std::function<bool(double)> crit(test_boolean_criterion);
    //my_tree.refineWithCriterion([](Point<double> arg) {return (arg.getX() > 0.5);} );

    /*
    my_tree.refineWithLevelSet([](Point<double> arg) { 
        Point<double> tmp1 = arg+Point<double>(-0.5, -0.5);
        Point<double> tmp2 = arg+Point<double>(-0.25, -0.25);
        return std::min(tmp1.abs() - 0.05, tmp2.abs() - 0.1); } );

    */
    my_tree.refineWithLevelSet(my_level_set);
    my_tree.exportMeshTikz(std::string("draw1.tex"));

    TestCriterion<double> my_criterion;
    //QuadTree<double> my_tree2(1.0, 1.0, 5, 7);
    //my_tree2.buildUniform();
    my_tree.updateQuadTree(my_criterion);
    my_tree.exportMeshTikz(std::string("draw2.tex"));

    //my_tree.stupidTest(5.2);

    //my_tree.exportCentersTikz(std::string("draw.tex"));


/*
    std::vector<Point<double>> centers = my_tree.getCenters();

    for (auto ct : centers) {
        std::cout<<ct.print()<<std::endl;
    }

*/
    return 0;
}

