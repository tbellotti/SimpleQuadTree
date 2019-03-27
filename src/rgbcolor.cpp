#include "../include/rgbcolor.h"

RGBColor::RGBColor(unsigned short int r, unsigned short int g, unsigned short int b) :
        red(r), green(g), blue(b) {}

RGBColor::RGBColor(unsigned short int col) : RGBColor(col, col, col) {}

void RGBColor::setRed(unsigned short int new_red)
{
    red = new_red;
}

void RGBColor::setGreen(unsigned short int new_green)
{
    green = new_green;
}

void RGBColor::setBlue(unsigned short int new_blue)
{
    blue = new_blue;
}



std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePBM(const std::string & filename)
    {

        std::vector<RGBColor> vector_to_return;    
        unsigned int dim_x = 0;
        unsigned int dim_y = 0;

        std::ifstream file_stream(filename);

        std::string line;

        // Skipping the first three lines
        std::getline(file_stream, line);
        std::getline(file_stream, line);
        std::getline(file_stream, line);
        std::istringstream iss(line);
        iss >> dim_x >> dim_y;

        unsigned int curr_line = 0;

        unsigned short int red = 0, green = 0, blue = 0;

        while (std::getline(file_stream, line))
        {

            unsigned int reminder = curr_line % 3;

            switch (reminder)   {
                case 0 : { red = std::stoi(line); break; }
                case 1 : { green = std::stoi(line); break; }
                case 2 : { blue = std::stoi(line); vector_to_return.push_back(RGBColor(red, green, blue)); break;}
            }

            curr_line++;
        }

        //file_stream.fclose();

        return std::make_tuple(dim_x, dim_y, vector_to_return);
    } 


std::ostream & operator<<(std::ostream & os, const RGBColor & color)
{
    os<<"Red = "<<color.getRed()<<", Green = "<<color.getGreen()<<", Blue = "<<color.getBlue();
    return os;
}




