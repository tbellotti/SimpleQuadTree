#ifndef RGBCOLOR_H
#define RGBCOLOR_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

class RGBColor
{

protected:
    unsigned short int red = 0;
    unsigned short int green = 0;
    unsigned short int blue = 0;

public:
    RGBColor() = default;
    RGBColor(unsigned short int, unsigned short int, unsigned short int);
    explicit RGBColor(unsigned short int);

    void setRed(unsigned short int); 
    void setGreen(unsigned short int);
    void setBlue(unsigned short int);


    unsigned short int getRed() const { return red; } 
    unsigned short int getGreen() const { return green; } 
    unsigned short int getBlue() const { return blue; }

    // Add other stuff when needed.

};

std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePBM(const std::string &);

std::ostream & operator<<(std::ostream &, const RGBColor &);

#endif

