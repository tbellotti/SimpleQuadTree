#include "../include/rgbcolor.h"

RGBColor::RGBColor(unsigned char r, unsigned char g, unsigned char b) :
        red(r), green(g), blue(b) {}

RGBColor::RGBColor(unsigned char col) : RGBColor(col, col, col) {}

void RGBColor::setRed(unsigned char new_red)
{
    red = new_red;
}

void RGBColor::setGreen(unsigned char new_green)
{
    green = new_green;
}

void RGBColor::setBlue(unsigned char new_blue)
{
    blue = new_blue;
}

double RGBColor::distanceEuclidian(const RGBColor & rhs) const
{
    return sqrt(pow(red - rhs.red, 2.0) + pow(green - rhs.green, 2.0) + pow(blue - rhs.blue, 2.0));
}
double RGBColor::distanceEuclidianCorrected(const RGBColor & rhs) const 
{
    return sqrt(2.0*pow(red - rhs.red, 2.0) + 4.0*pow(green - rhs.green, 2.0) + 3.0*pow(blue - rhs.blue, 2.0));
}

RGBColor RGBColor::bw() const
{
    double luminance = 0.2126*red + 0.7152*green + 0.0722*blue;
    return RGBColor(static_cast<unsigned char>(luminance));
}

// This would or can result in an overflow, thus it cannot be used.
/*
RGBColor & RGBColor::operator+=(const RGBColor & rhs)
{
    red   += rhs.red;
    green += rhs.green;
    blue  += rhs.blue;

    return *this;
}
*/

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

        unsigned char red = 0, green = 0, blue = 0;

        while (std::getline(file_stream, line))
        {

            unsigned int reminder = curr_line % 3;

            switch (reminder)   {
                case 0 : { red = static_cast<unsigned char>(std::stoi(line)); break; }
                case 1 : { green = static_cast<unsigned char>(std::stoi(line)); break; }
                case 2 : { blue = static_cast<unsigned char>(std::stoi(line)); vector_buffer.push_back(RGBColor(red, green, blue)); break;}
                default : {}
            }

            curr_line++;
        }


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
    os<<int(color.getRed())<<", "<<int(color.getGreen())<<", "<<int(color.getBlue());
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


RGBColor meanColor(const std::vector<RGBColor> & colors)
{
    double prefactor = 1.0/double(colors.size());
    auto it = colors.cbegin();

    double totred   = it->getRed();
    double totgreen = it->getGreen();
    double totblue  = it->getBlue();

    it++;

    for ( ; it < colors.cend(); it++)   {
        totred   += it->getRed();
        totgreen += it->getGreen();
        totblue  += it->getBlue();
    }

    return RGBColor(static_cast<unsigned char>(prefactor*totred),
                    static_cast<unsigned char>(prefactor*totgreen),
                    static_cast<unsigned char>(prefactor*totblue));        
    
}


// Really dangerous for the same reasons we have seen before.
/*
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
*/

std::vector<RGBColor> rainbowOfColors(size_t number_of_colors)
{
    std::vector<RGBColor> rainbow;
    unsigned int current = 0;
    unsigned int step = static_cast<unsigned int>(floor(double(255*4)/double(number_of_colors)));

    while (number_of_colors > 0)    {
        if (current < 256)  {
            rainbow.push_back(RGBColor(255, 0, static_cast<unsigned char>(current)));
        }
        if (current >= 256 && current < 256*2)  {
            rainbow.push_back(RGBColor(static_cast<unsigned char>(255-current%255), 0, 255));
        }
        if (current >= 256*2 && current < 256*3)  {
            rainbow.push_back(RGBColor(0, static_cast<unsigned char>(current%255), 255));
        }
        if (current >= 256*3 && current < 256*4)  {
            rainbow.push_back(RGBColor(0, 255, static_cast<unsigned char>(255-current%255)));
        }
        number_of_colors--;
        current += step;
    }

    return rainbow;
}




