#include "point.h"
#include "cell.h"
#include "lipschitzfunction.h"
#include "refinementcriterion.h"
#include "quadtree.h"
#include <string>
#include <cmath>

typedef double real;

int main () {

    // [1] Testing "point.h"
    std::cout<<"[1] Testing point.h"<<std::endl;
    // Testing constructors
    std::cout<<"(-) Testing constructors"<<std::endl;

    
    Point<real> pt1(); // (0.0, 0.0) !!!! UNDERSTAND WHY DOES NOT WORK
    Point<real> pt2(1.0, -2.0); // (1.0, -2.0)
    Point<real> pt3(-3.0);  // (-3.0, 3.0)
    Point<real> pt4(pt2);   // (1.0, -2.0)
    std::cout<<"(0.0, 0.0) vs "<<pt1<<std::endl;
    std::cout<<"(1.0, -2.0) vs "<<pt2<<std::endl;
    std::cout<<"(-3.0, -3.0) vs "<<pt3<<std::endl;
    std::cout<<"(1.0, -2.0) vs "<<pt4<<std::endl;


    // Testing copy assignment operator
    std::cout<<"(-) Testing copy assignment operator"<<std::endl;
    Point<real> pt5 = pt3;
    std::cout<<"(-3.0, -3.0) vs "<<pt5<<std::endl;

    // Testing getters
    std::cout<<"(-) Testing getters"<<std::endl;
    std::cout<<"1.0 vs "<<pt2.getX()<<std::endl;
    std::cout<<"-2.0 vs "<<pt2.getY()<<std::endl;

    // Testing setters
    std::cout<<"(-) Testing setters"<<std::endl;
    pt2.setX(3.0);
    pt2.setY(5.0);
    std::cout<<"(3.0, 5.0) vs "<<pt2<<std::endl;

    // Testing abs and distance
    std::cout<<"(-) Testing abs and distance"<<std::endl;
    std::cout<<"5.83095189485 vs "<<pt2.abs()<<std::endl;
    std::cout<<"7.28010988928 vs "<<pt2.distance(pt4)<<std::endl;

    // Testing sum and multiplication by a scalar
    std::cout<<"(-) Testing sum and multiplication by a scalar"<<std::endl;
    std::cout<<"(2.5, -5.0) vs "<<2.5*pt4<<std::endl;
    std::cout<<"(0.0, 2.0) vs "<<pt2+pt5<<std::endl;



    // [2] Testing "cell.h"
    std::cout<<std::endl<<"[2] Testing cell.h"<<std::endl;
    // Testing constructors
    std::cout<<"(-) Testing constructors"<<std::endl;
    Cell<real> cell1(pt2, 2.0, 1.0);
    std::cout<<"Center: (4.0, 5.5), dx = 2.0, dy = 1.0 vs "<<cell1<<std::endl;


    // Testing copy assignment operator
    std::cout<<"(-) Testing copy assignment operator"<<std::endl;
    Cell<real> cell2 = cell1;
    std::cout<<"Center: (4.0, 5.5), dx = 2.0, dy = 1.0 vs "<<cell2<<std::endl;

    // Testing getters
    std::cout<<"(-) Testing getters"<<std::endl;
    std::cout<<"1 vs "<<cell1.isLeaf()<<std::endl;
    std::cout<<"(4.0, 5.5) vs "<<cell1.getCenter()<<std::endl;
    std::cout<<"sqrt(5) vs "<<cell1.getDiagonal()<<std::endl;
    std::vector<Point<real>> tmp_vert = cell2.getVertices();
    std::cout<<"(3, 5)\n(5, 5)\n(5, 6)\n(3, 6) vs "<<std::endl;
    for (auto vt : tmp_vert)    {
        std::cout<<vt<<std::endl;
    }
    std::cout<<"2 vs "<<cell1.cellSurface()<<std::endl;

    // Testing splitting and and getting children
    std::cout<<"(-) Testing splitting and and getting children"<<std::endl;
    cell1.splitCell();
    std::vector<std::shared_ptr<Cell<real>>> tmp_child = cell1.getChildren();
    std::cout<<"Center: (3.5, 5.25), dx = 1, dy = 0.5\nCenter: (4.5, 5.25), dx = 1, dy = 0.5\nCenter: (3.5, 5.75), dx = 1, dy = 0.5\nCenter: (4.5, 5.75), dx = 1, dy = 0.5 vs"<<std::endl;
    for (auto ch : tmp_child)    {
        std::cout<<(*ch)<<std::endl;
    }
    std::cout<<"0 vs "<<cell1.isLeaf()<<std::endl;

    // Testing merging
    std::cout<<"(-) Testing merging"<<std::endl;
    cell1.mergeCell();
    std::cout<<"1 vs "<<cell1.isLeaf()<<std::endl;

    // [3] Testing "lipschitzfunction.h"
    std::cout<<std::endl<<"[3] Testing lipschitzfunction.h"<<std::endl;


    class MyLip : public LipschitzFunction<real>
    {
    public:
        MyLip() : LipschitzFunction<real>() {}
        MyLip(real lipschitz) : LipschitzFunction<real>(lipschitz) {}
        ~MyLip() = default;

        real operator()(Point<real> arg) const override
        {
            return arg.getX()+arg.getY();
        }
    };

    // Testing constructors
    std::cout<<"(-) Testing constructors"<<std::endl;
    MyLip fun1;
    MyLip fun2(1.5);

    // Testing setters
    std::cout<<"(-) Testing setters"<<std::endl;
    fun1.setLipschitzConstant(1.2);

    // Testing getters
    std::cout<<"(-) Testing getters"<<std::endl;
    std::cout<<"1.2 vs "<<fun1.getLipschitzConstant()<<std::endl;
    std::cout<<"1.5 vs "<<fun2.getLipschitzConstant()<<std::endl;

    // Testing () operator
    std::cout<<"(-) Testing operator ()"<<std::endl;
    std::cout<<"8.0 vs "<<fun1(pt2)<<std::endl;
    std::cout<<"8.0 vs "<<fun2(pt2)<<std::endl;


    // [4] Testing "refinementcriterion.h"
    std::cout<<std::endl<<"[4] Testing refinementcriterion.h"<<std::endl;

    class MyCrit : public RefinementCriterion<real>
    {
    public:
        MyCrit() : RefinementCriterion() {}
        ~MyCrit() = default;

    bool operator()(std::shared_ptr<Cell<real>> arg) const override
    {
        return (arg->getDiagonal() > 1.5 );
    }

    };

    // Testing constructor
    std::cout<<"(-) Testing constructor"<<std::endl;
    MyCrit crit1;

    // Testing operator()
    std::cout<<"(-) Testing operator()"<<std::endl;
    auto tmp_ptr = std::make_shared<Cell<real>>(cell1);
    std::cout<<"1 vs "<<crit1(tmp_ptr)<<std::endl;
    std::cout<<"0 vs "<<crit1(tmp_child[0])<<std::endl;



    // [5] Testing "quadtree.h"
    std::cout<<std::endl<<"[5] Testing quadtree.h"<<std::endl;

    // Testing constructor
    std::cout<<"(-) Testing constructor"<<std::endl;
    QuadTree<real> my_tree1(10.0, 20.0, 1.0, 7.0);
    QuadTree<real> my_tree2(10.0, 20.0, 4.0, 7.0);

    // Testing builduniform()
    std::cout<<"(-) Testing builduniform()"<<std::endl;
    my_tree1.buildUniform(5);
    my_tree2.buildUniform();



    // Testing clear
    std::cout<<"(-) Testing clear()"<<std::endl;
    my_tree2.clear();
    std::cout<<"1 vs "<<my_tree2.numberOfLeaves()<<std::endl;

    my_tree2.buildUniform(); //  We come back


    // Testing getters
    std::cout<<"(-) Testing getters"<<std::endl;
    std::cout<<"1 vs "<<my_tree1.getMinLevel()<<std::endl;
    std::cout<<"7 vs "<<my_tree1.getMaxLevel()<<std::endl;
    std::cout<<"4 vs "<<my_tree2.getMinLevel()<<std::endl;
    std::cout<<"256 vs "<<my_tree1.numberOfLeaves()<<std::endl;
    std::cout<<"1024 vs "<<my_tree2.numberOfLeaves()<<std::endl;

    std::cout<<"We will print a lot of points..."<<std::endl;
    std::vector<Point<real>> centers_1 = my_tree1.getCenters();
    std::vector<std::shared_ptr<Cell<real>>> leaves_1 = my_tree1.getLeaves();

    for (unsigned int i = 0; i < leaves_1.size(); i++)   {
        std::cout<<centers_1[i]<<std::endl;
        std::cout<<*leaves_1[i]<<std::endl;
    }

    // Testing refinement
    std::cout<<"(-) Testing refinement routines"<<std::endl;

    class SpaceBasedCriterion : public RefinementCriterion<real>
    {
    public:
        SpaceBasedCriterion() : RefinementCriterion<real>()  {}
        ~SpaceBasedCriterion() = default;

        bool operator()(std::shared_ptr<Cell<real>> arg) const override
    {
        real x = arg->getCenter().getX();
        real y = arg->getCenter().getY();

        return (y >= 2.0/5.0*pow(x,2.0)-4.0*x+20.0 && y <= 2.0*pow(x,2.0)-20.0*x+62.0) ||
            (y <= -40.0/3.0*x+20.0) || (x>=6.0 && y <= 3.0);

    }
    };

    SpaceBasedCriterion crit3;

    my_tree1.updateQuadTree(crit3);
    my_tree1.exportMeshTikz(std::string("simple_refinement.tex"),1.0);

    // Test with mandelbrot

    class MandelbrotCriterion : public RefinementCriterion<real>
    {
    public:
        MandelbrotCriterion() : RefinementCriterion<real>()  {}
        ~MandelbrotCriterion() = default;

        bool operator()(std::shared_ptr<Cell<real>> arg) const override
    {
        real x = arg->getCenter().getX()-1.0;
        real y = arg->getCenter().getY()-2.0;



        real max_abs = 1000.0;
        real curr_abs = 0.0;

        unsigned int it = 0;

        Point<real> p_n (0.0, 0.0);
        Point<real> c (x,y);

        while (it < 50 && curr_abs < 2.0*max_abs)
        {
            Point<real> tmp = Point<real>(pow(p_n.getX(),2.0)-pow(p_n.getY(),2.0),2*p_n.getX()*p_n.getY()) + c;   
            p_n = tmp;
            curr_abs = p_n.abs();
        }

        return (curr_abs < max_abs);

    }
    };

    QuadTree<real> my_tree_mandelbrot(3.0, 2.0, 1.0, 5.0);
    my_tree_mandelbrot.buildUniform(4.0);
    MandelbrotCriterion md_crit;


    my_tree_mandelbrot.updateQuadTree(md_crit);
    my_tree_mandelbrot.exportMeshTikz(std::string("mandelbrot_refinement.tex"),2.5);

    return 0;
}

