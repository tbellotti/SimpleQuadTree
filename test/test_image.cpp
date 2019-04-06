#include "../include/image.h"
#include <iostream>



int main()
{

    Image<double> my_tree(1.0, 1.0, 1, 10);
    std::cout<<"Tree created: "<<my_tree.foo()<<std::endl;
    my_tree.createFromFile(std::string("/home/thomas/SimpleQuadTree/media/images/sample6_ascii.ppm"));

    my_tree.saveImage(std::string("before.tex"));

    //my_tree.simplifyImage(0.007); // medium-low
    my_tree.simplifyImage(0.01); // low quality

    my_tree.saveImage(std::string("after.tex"));


    return 0;
}

