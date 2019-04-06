#include "../include/cellwithparent.h"
#include <iostream>


int main ()
{


    CellWithParent<double> cell1(Point<double>(0.0, 0.0), 1.0, 2.0, nullptr);
    CellWithParent<double> cell2(Point<double>(0.0, 0.0), 1.0, 2.0, &cell1);

    const CellWithParent<double> * ptr1 =  cell2.getFather();

    cell2.splitCell();

    

    std::cout<<ptr1<<std::endl;

    std::cout<<*ptr1<<std::endl;


    return 0;
}
