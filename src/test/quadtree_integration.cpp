#include "../../include/quadtree.h"



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



int main()
{

    QuadTree<double> my_tree(Point<double>(0.0, 0.0), 1.0, 1.0, 1, 8);
    my_tree.buildUniform(3);
    LevelSet<double> my_level_set(1.2);
    my_tree.updateWithLevelSet(my_level_set);


    std::cout<<"[1] - Integrating sin(exp(x)*y) over [0,1]^2"<<std::endl;

    std::function<double(Point<double>)> to_integrate1 = [](Point<double> pt) { 
        return sin(exp(pt.getX())*pt.getY());
    };

    std::cout<<"Exact result = 0.631367"<<std::endl;
    std::cout<<"Integral with simple formula = "<<my_tree.simpleIntegration(to_integrate1)<<std::endl;
    std::cout<<"Integral with 3rd order gaussian formula = "<<my_tree.thirdOrderGaussianIntegration(to_integrate1)<<std::endl;




    std::cout<<"[2] - Integrating Exp[x^2+y^2] over [0,1]^2"<<std::endl;

    std::function<double(Point<double>)> to_integrate2 = [](Point<double> pt) { 
        return exp(pow(pt.getX(),2)+pow(pt.getY(),2));
    };

    std::cout<<"Exact result = 2.13935"<<std::endl;
    std::cout<<"Integral with simple formula = "<<my_tree.simpleIntegration(to_integrate2)<<std::endl;
    std::cout<<"Integral with 3rd order gaussian formula = "<<my_tree.thirdOrderGaussianIntegration(to_integrate2)<<std::endl;


    return 0;
}