#ifndef RGBCOLOR_H
#define RGBCOLOR_H


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <cmath>

#include "matrix.h"

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

    double distanceEuclidian(const RGBColor &) const;
    double distanceEuclidianCorrected(const RGBColor &) const;

    RGBColor & operator+=(const RGBColor &);

    double abs() const;


};

std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePBM(const std::string &);

std::ostream & operator<<(std::ostream &, const RGBColor &);

void indicesVectorBuilderHelp(const Matrix<size_t> &, std::vector<size_t> &, size_t, size_t, size_t, size_t);

RGBColor operator+ (const RGBColor &, const RGBColor &);
RGBColor operator- (const RGBColor &, const RGBColor &);
RGBColor operator* (double, const RGBColor &);

std::vector<RGBColor> rainbowOfColors(size_t number_of_colors);

#endif

