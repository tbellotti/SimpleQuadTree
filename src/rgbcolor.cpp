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


double RGBColor::distanceEuclidian(const RGBColor & rhs) const
{
    RGBColor tmp = *this - rhs;
    return tmp.abs();
    //return sqrt(pow(red - rhs.red, 2.0) + pow(green - rhs.green, 2.0) + pow(blue - rhs.blue, 2.0));
}
double RGBColor::distanceEuclidianCorrected(const RGBColor & rhs) const 
{
    return sqrt(2.0*pow(red - rhs.red, 2.0) + 4.0*pow(green - rhs.green, 2.0) + 3.0*pow(blue - rhs.blue, 2.0));
}

RGBColor & RGBColor::operator+=(const RGBColor & rhs)
{
    red   += rhs.red;
    green += rhs.green;
    blue  += rhs.blue;

    return *this;
}

double RGBColor::abs() const
{
    return sqrt(pow(red, 2.0) + pow(green, 2.0) + pow(blue, 2.0));
}

std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePBM(const std::string & filename)
    {

        std::vector<RGBColor> vector_buffer;    
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
        std::getline(file_stream, line);


        unsigned int curr_line = 0;

        unsigned short int red = 0, green = 0, blue = 0;

        while (std::getline(file_stream, line))
        {

            unsigned int reminder = curr_line % 3;

            switch (reminder)   {
                case 0 : { red = std::stoi(line); break; }
                case 1 : { green = std::stoi(line); break; }
                case 2 : { blue = std::stoi(line); vector_buffer.push_back(RGBColor(red, green, blue)); break;}
            }

            curr_line++;
        }

        //file_stream.fclose();

        // We have parsed in the ppm format
        // Now we need something to keep the tree surfing 
        // as simple as possible in a recurrent fashion

        std::vector<size_t> indices(vector_buffer.size());

        for (size_t idx = 0; idx < indices.size(); idx++)   {
            indices[idx] = idx;
        }


        const Matrix<size_t> mtx(dim_x, indices);
        std::vector<size_t> indices_ordered;


        indicesVectorBuilderHelp(mtx, indices_ordered, 0, dim_y-1, 0, dim_x-1);

        std::vector<RGBColor> vector_to_return(indices_ordered.size());  

    
        for (size_t idx = 0; idx < indices_ordered.size(); idx++)   {
            vector_to_return[idx] = vector_buffer[indices_ordered[idx]];
        }
        // returning 

        return std::make_tuple(dim_x, dim_y, vector_to_return);
    } 


std::ostream & operator<<(std::ostream & os, const RGBColor & color)
{
    os<<"Red = "<<color.getRed()<<", Green = "<<color.getGreen()<<", Blue = "<<color.getBlue();
    return os;
}



void indicesVectorBuilderHelp(const Matrix<size_t> & mt, std::vector<size_t> & to_fill,
                         size_t i_start, size_t i_end, size_t j_start, size_t j_end)
{

    if (i_start == i_end)   {
        to_fill.push_back(mt.getEntry(i_start, j_start));
    }
    else    {
        indicesVectorBuilderHelp(mt, to_fill, i_start + (i_end-i_start) / 2 + 1, i_end, j_start, j_start + (j_end-j_start) / 2);
        indicesVectorBuilderHelp(mt, to_fill, i_start + (i_end-i_start) / 2 + 1, i_end, j_start + (j_end-j_start) / 2 + 1, j_end);
        indicesVectorBuilderHelp(mt, to_fill, i_start, i_start + (i_end-i_start) / 2, j_start + (j_end-j_start) / 2 + 1, j_end);
        indicesVectorBuilderHelp(mt, to_fill, i_start, i_start + (i_end-i_start) / 2, j_start, j_start + (j_end-j_start) / 2);
    }
}




RGBColor operator+ (const RGBColor & lhs, const RGBColor & rhs)
{
    // We should not limit the components of the returned 
    // color between 0 and 255 because the operation is almost 
    // used to compute means, hence the result will be divided 
    // in order to fit the range.

    return RGBColor(lhs.getRed()   + rhs.getRed(),
                    lhs.getGreen() + rhs.getGreen(),
                    lhs.getBlue()  + rhs.getBlue());

}

RGBColor operator- (const RGBColor & lhs, const RGBColor & rhs)
{
    return RGBColor(lhs.getRed()   - rhs.getRed(),
                    lhs.getGreen() - rhs.getGreen(),
                    lhs.getBlue()  - rhs.getBlue());

}

RGBColor operator* (double factor, const RGBColor & rhs)
{
    return RGBColor((unsigned short int)(factor * rhs.getRed()),
                    (unsigned short int)(factor * rhs.getGreen()),
                    (unsigned short int)(factor * rhs.getBlue()));
}





