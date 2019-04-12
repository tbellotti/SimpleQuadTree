#include "../include/image.h"
#include <iostream>



int main()
{

    Image<double> my_tree(1.0, 1.0, 1, 10);
    std::cout<<"Tree created: "<<my_tree.foo()<<std::endl;
    //my_tree.createFromFile(std::string("/home/thomas/SimpleQuadTree/media/images/debug1.ppm"));
    my_tree.createFromFile(std::string("/home/thomas/SimpleQuadTree/media/images/sample9_ascii.ppm"));
    //my_tree.createFromFile(std::string("/home/thomas/SimpleQuadTree/media/images/me.ppm"));

    //my_tree.createFromFile(std::string("/home/thomas/SimpleQuadTree/media/images/large_ex1.ppm"));

    std::cout<<my_tree.getMinLevel()<<" ! "<<my_tree.getMaxLevel()<<std::endl;

    my_tree.saveImage(std::string("before.tex"));

    //my_tree.simplifyImage(0.007); // medium-low
    //my_tree.simplifyImage(0.010); // low quality

    my_tree.simplifyImageNew(0.010); // low-medium quality
    //my_tree.simplifyImageNew(0.008); // low-medium quality

    my_tree.saveImage(std::string("after.tex"));


    return 0;
}

