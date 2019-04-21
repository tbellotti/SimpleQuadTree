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
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

public:
    RGBColor() = default;
    RGBColor(unsigned char, unsigned char, unsigned char);
//    explicit RGBColor(unsigned char);
    // I do not want it to be explicit, we can use it for cast...makes sense
    RGBColor(unsigned char);

    void setRed(unsigned char); 
    void setGreen(unsigned char);
    void setBlue(unsigned char);

    unsigned char getRed() const { return red; } 
    unsigned char getGreen() const { return green; } 
    unsigned char getBlue() const { return blue; }

    // Add other stuff when needed.
    double distanceEuclidian(const RGBColor &) const;
    double distanceEuclidianCorrected(const RGBColor &) const;

    //  RGBColor & operator+=(const RGBColor &);
    double abs() const;
    // Gets the black-white equivalent
    RGBColor bw() const;
};


RGBColor meanColor(const std::vector<RGBColor> &);

std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePBM(const std::string &);

std::ostream & operator<<(std::ostream &, const RGBColor &);

void indicesVectorBuilderHelp(const Matrix<size_t> &, std::vector<size_t> &, size_t, size_t, size_t, size_t);

/*
RGBColor operator+ (const RGBColor &, const RGBColor &);
RGBColor operator- (const RGBColor &, const RGBColor &);
RGBColor operator* (double, const RGBColor &);
*/

std::vector<RGBColor> rainbowOfColors(size_t number_of_colors);

#endif

