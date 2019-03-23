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
        
       //return ((arg->getCenter().getY() - 0.4*sin(4*3.141*arg->getCenter().getX())) < 0.2);
        //return ((arg->getCenter().getY() - arg->getCenter().getX()) > 0.0);
    }
};


int main () 
{

    QuadTree<double> my_tree(1.0, 1.0, 1, 7);
    
    my_tree.buildUniform(3);

    LevelSet<double> my_level_set(1.2);
    TestCriterion<double> my_criterion;

    //my_tree.updateQuadTree(my_criterion);
    my_tree.refineWithLevelSet(my_level_set);
    //my_tree.updateQuadTree(my_criterion);

    my_tree.exportMeshTikz(std::string("draw1.tex"), 10.0);
    my_tree.exportCentersTikz(std::string("draw2.tex"), 10.0);



    std::function<double(Point<double>)> to_integrate = [](Point<double> pt) { 
        return pow(pt.getX(), 2.0) + 2.0*pow(pt.getY(), 2.0) + pt.getX()*pt.getY();
    };

    std::cout<<"Integral = "<<my_tree.simpleIntegration(to_integrate)<<std::endl;

    return 0;
}
