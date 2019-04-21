#include "../../include/quadtree.h"
#include "../../include/refinementcriterion.h"

template <typename T>
class SpaceBasedCriterion : public RefinementCriterion<T>
{
public:
    SpaceBasedCriterion() : RefinementCriterion<T>()  {}
    ~SpaceBasedCriterion() = default;

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        T x = arg->getCenter().getX();
        T y = arg->getCenter().getY();

        return (y >= 2.0/5.0*pow(x,2.0)-4.0*x+20.0 && y <= 2.0*pow(x,2.0)-20.0*x+62.0) ||
            (y <= -40.0/3.0*x+20.0) || (x>=6.0 && y <= 3.0);
    }
};

template <typename T>
class MandelbrotCriterion : public RefinementCriterion<T>
{
public:
    MandelbrotCriterion() : RefinementCriterion<T>()  {}
    ~MandelbrotCriterion() = default;

    bool operator()(std::shared_ptr<Cell<T>> arg) const override
    {
        T x = arg->getCenter().getX()-2.0;
        T y = arg->getCenter().getY()-1.0;

        T max_abs = 10000.0;
        T curr_abs = 0.0;

        unsigned int it = 0;

        Point<T> p_n (0.0, 0.0);
        Point<T> c (x,y);

        while (it < 1000 && curr_abs < 2.0*max_abs)
        {
            Point<T> tmp = Point<T>(pow(p_n.getX(),2.0)-pow(p_n.getY(),2.0),2*p_n.getX()*p_n.getY()) + c;   
            p_n = tmp;
            curr_abs = p_n.abs();
            it++;
        }

        return (curr_abs < max_abs);

    }
};

int main()
{

    SpaceBasedCriterion<double> crit_space;
    MandelbrotCriterion<double> crit_mandelbrot;

    QuadTree<double> my_tree1(Point<double>(0.0, 0.0), 10.0, 20.0, 1.0, 7.0);
    my_tree1.buildUniform(4);
    my_tree1.updateQuadTree(crit_space);
    my_tree1.exportMeshTikz("/home/thomas/SimpleQuadTree/media/output/test/quadtree_other_meshes/space_criterion_color.tex", true);
    my_tree1.exportMeshTikz("/home/thomas/SimpleQuadTree/media/output/test/quadtree_other_meshes/space_criterion.tex", false);


    QuadTree<double> my_tree2(Point<double>(0.0, 0.0), 3.0, 2.0, 1, 8.0);
    my_tree2.buildUniform(6);
    my_tree2.updateQuadTree(crit_mandelbrot);
    my_tree2.exportMeshTikz("/home/thomas/SimpleQuadTree/media/output/test/quadtree_other_meshes/mandelbrot_criterion_color.tex", true);
    my_tree2.exportMeshTikz("/home/thomas/SimpleQuadTree/media/output/test/quadtree_other_meshes/mandelbrot_criterion.tex", false);


    return 0;
}