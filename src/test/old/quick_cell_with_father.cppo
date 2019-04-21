#include "../include/cellwithfather.h"
#include <iostream>


int main ()
{


    CellWithFather<double> cell1(Point<double>(0.0, 0.0), 1.0, 2.0, nullptr);
    CellWithFather<double> cell2(Point<double>(0.0, 0.0), 1.0, 2.0, std::make_shared<CellWithFather<double>>(cell1));

    std::shared_ptr<CellWithFather<double>> ptr1 =  cell2.getFather();

    std::cout<<ptr1<<std::endl;

    std::cout<<*ptr1<<std::endl;


    return 0;
}
