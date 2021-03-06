#include "../../include/image.h"
#include <iostream>

int main()
{

    Image<double> my_image(1.0, 1.0, 1, 10);
    /*
    double high = 0.005;
    double medium = 0.008;
    double low = 0.013;
    */

    double high = 0.012;
    double medium = 0.024;
    double low = 0.048;

    std::string project_path("/home/thomas/SimpleQuadTree");

    std::vector<std::string> filenames;
    filenames.push_back(std::string("s1"));
    filenames.push_back(std::string("s2"));
    filenames.push_back(std::string("s3"));
    filenames.push_back(std::string("s4"));
    filenames.push_back(std::string("s5"));
    filenames.push_back(std::string("s6"));
    filenames.push_back(std::string("s7"));
    filenames.push_back(std::string("s8"));
    filenames.push_back(std::string("s9"));


    for (auto name : filenames) {
        std::string filepath (project_path+"/media/images/"+name+".ppm");
        size_t initial_size;
        size_t current_size;

        std::cout<<"[Image] - Testing with "<<name<<std::endl;
        my_image.createFromFile(std::string(filepath));

        initial_size = my_image.numberOfPixels();
        std::cout<<"Image generated with #  pixels: "<<initial_size<<std::endl;

        my_image.simplifyImage(high); // high quality
        current_size = my_image.numberOfPixels();
        my_image.saveImage(std::string(project_path+"/media/output/test/image_compression/"+name+"_high.tex"));

        std::cout<<"Compressed 1 with #  pixels: "<<current_size<<" - Compression ratio = "<<1.0/(double(current_size)/double(initial_size))<<std::endl;


        my_image.simplifyImage(medium); // medium quality
        current_size = my_image.numberOfPixels();
        my_image.saveImage(std::string(project_path+"/media/output/test/image_compression/"+name+"_medium.tex"));

        std::cout<<"Compressed 2 with #  pixels: "<<current_size<<" - Compression ratio = "<<1.0/(double(current_size)/double(initial_size))<<std::endl;


        my_image.simplifyImage(low); // low quality
        current_size = my_image.numberOfPixels();
        my_image.saveImage(std::string(project_path+"/media/output/test/image_compression/"+name+"_low.tex"));

        std::cout<<"Compressed 3 with #  pixels: "<<current_size<<" - Compression ratio = "<<1.0/(double(current_size)/double(initial_size))<<std::endl;
    }



    return 0;
}

