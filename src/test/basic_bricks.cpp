#include "../../include/point.h"
#include "../../include/matrix.h"
#include "../../include/rgbcolor.h"

#include <string>
#include <cmath>

/*
Test for some basics classes of the projects, namely
point.h / matrix.h and rgbcolor.h
*/

typedef double real;

int main()
{
    // [1] Testing "point.h"
    std::cout<<"[1] Testing point.h"<<std::endl;
    // Testing constructors
    std::cout<<"(-) Testing constructors"<<std::endl;
    
    Point<real> pt1; // (0.0, 0.0) 
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
    std::cout<<"(2.5, -5.0) vs "<<(real(2.5))*pt4<<std::endl;
    std::cout<<"(0.0, 2.0) vs "<<pt2+pt5<<std::endl;


    // [1] Testing "rgbcolor.h"
    std::cout<<"[2] Testing rgbcolor.h"<<std::endl;
    // Testing constructors
    std::cout<<"(-) Testing constructors"<<std::endl;
    
    RGBColor cl1; // (0, 0, 0) 
    RGBColor cl2(23, 46, 11); // (23, 46, 11) 
    RGBColor cl3(221); // (221, 221, 221) 
    std::cout<<"(0, 0, 0) vs "<<cl1<<std::endl;
    std::cout<<"(23, 46, 11) vs "<<cl2<<std::endl;
    std::cout<<"(221, 221, 221) vs "<<cl3<<std::endl;


    // Testing copy assignment operator
    std::cout<<"(-) Testing copy assignment operator"<<std::endl;
    RGBColor cl4 = cl2;
    std::cout<<"(23, 46, 11) vs "<<cl4<<std::endl;
    
    // Testing getters
    std::cout<<"(-) Testing getters"<<std::endl;
    std::cout<<"0 vs "<<int(cl1.getRed())<<std::endl;
    std::cout<<"46 vs "<<int(cl2.getGreen())<<std::endl;
    std::cout<<"221 vs "<<int(cl3.getBlue())<<std::endl;

    // Testing setters
    std::cout<<"(-) Testing setters"<<std::endl;
    cl1.setGreen(102);
    cl3.setBlue(23);
    cl3.setRed(1);

    std::cout<<"(0, 102, 0) vs "<<cl1<<std::endl;
    std::cout<<"(1, 221, 23) vs "<<cl3<<std::endl;
    
    // Testing distances
    std::cout<<"(-) Testing distances and abs (without reference values...no time to do them)"<<std::endl;
    std::cout<<cl1.distanceEuclidian(cl2)<<std::endl;
    std::cout<<cl1.distanceEuclidianCorrected(cl2)<<std::endl;
    std::cout<<cl3.abs()<<std::endl;

    // Converting to black and white
    std::cout<<"(-) Testing conversion to BW"<<std::endl;
    std::cout<<"(72, 72, 72) vs "<<cl1.bw()<<std::endl;

    // TO be completed accordingly
    
    return 0;
}