#include "point.h"
#include "cell.h"


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


}

