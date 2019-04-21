#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>

#include "../../include/quadtree.h"

template <typename T>
class LevelSet : public LipschitzFunction<T>
{
public:
    LevelSet() : LipschitzFunction<T>() {}
    LevelSet(T lipschitz) : LipschitzFunction<T>(lipschitz) {}

    T operator()(Point<T> arg) const override
    {
        /*
        Circle of center (0.5, 0.5) with radius 0.075 and
        circle of center (0.25, 0.25) with radius 0.12
        */
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

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        std::vector<Point<T>> vt = arg->getVertices();
        
        return ((vt[0].getY() - pow(vt[0].getX(), 1.0) > 0.75) 
                || ((vt[1].getY() - pow(vt[1].getX(), 1.0)) > 0.75)
                || ((vt[2].getY() - pow(vt[2].getX(), 1.0)) > 0.75)
                || ((vt[3].getY() - pow(vt[3].getX(), 1.0)) > 0.75)
                || ((arg->getCenter().getY() - pow(arg->getCenter().getX(), 1.0) > 0.75))); 
    }
};


int main () 
{

    QuadTree<double> my_tree(Point<double>(0.0, 0.0), 1.0, 1.0, 1, 8);
    my_tree.buildUniform(3);

    LevelSet<double> my_level_set(1.2);
    TestCriterion<double> my_criterion;


    my_tree.updateWithLevelSet(my_level_set);
    my_tree.exportMeshTikz(std::string("/home/thomas/SimpleQuadTree/media/output/test/quadtree_highly_adaptive_mesh/level_set.tex"), false);
    my_tree.exportMeshTikz(std::string("/home/thomas/SimpleQuadTree/media/output/test/quadtree_highly_adaptive_mesh/level_set_color.tex"), true);

    my_tree.clear();
    my_tree.updateQuadTree(my_criterion);
    my_tree.exportMeshTikz(std::string("/home/thomas/SimpleQuadTree/media/output/test/quadtree_highly_adaptive_mesh/criterion.tex"), false);
    my_tree.exportMeshTikz(std::string("/home/thomas/SimpleQuadTree/media/output/test/quadtree_highly_adaptive_mesh/criterion_color.tex"), true);

    return 0;
}

