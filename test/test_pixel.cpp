#include "../include/pixel.h"
#include <iostream>

int main()
{

    Pixel<double> px1(Point<double>(0.0, 0.0), 1.0, 2.0, RGBColor(120, 50, 21));
    px1.splitCell();
    std::cout<<px1<<"||"<<px1.getField()<<std::endl;
    std::cout<<"mean = "<<px1.meanField()<<" | standard deviation = "<<px1.stdDevField()<<std::endl;


    px1.mergeCell();

    std::cout<<px1<<"||"<<px1.getField()<<std::endl;

    return 0;
}
