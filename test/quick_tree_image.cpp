#include "../include/quadtreeimage.h"
#include <iostream>



int main()
{
    QuadTreeImage<double> my_tree(1.0, 1.0, 3, 6);
    std::cout<<"Tree created"<<std::endl;

    my_tree.buildUniform();
    std::cout<<"Uniform made"<<std::endl;


    my_tree.exportMeshTikz(std::string("test.tex"));

    std::tuple<unsigned, unsigned, std::vector<RGBColor>> test_parser = parsePBM(std::string("/home/thomas/SimpleQuadTree/media/images/sample1_ascii.ppm"));




    for (auto cl : std::get<2>(test_parser)) {
        std::cout<<cl<<std::endl;
    }

    std::cout<<std::get<0>(test_parser)<<"  |  "<<std::get<1>(test_parser)<<std::endl;

    my_tree.createFromImage(std::string("/home/thomas/SimpleQuadTree/media/images/sample1_ascii.ppm"));




    return 0;
}

