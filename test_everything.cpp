#include "point.h"


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



}

