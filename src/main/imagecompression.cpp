#include "../../include/image.h"


int main(int argc, char * argv[])
{
    if (argc < 4)  {
        std::cerr<<"Please specify the full path of the input .ppm file and the full path of the output .tex file, along with the compression factor"<<std::endl;
        return -1;
    }

    std::string input_name(argv[1]);
    std::string output_name(argv[2]);
    double compression_factor = atof(argv[3]);

    Image<double> my_image(1.0, 1.0, 1, 10);

    my_image.createFromFile(input_name);
    my_image.simplifyImage(compression_factor); 
    my_image.saveImage(output_name);

    return 0;
}
