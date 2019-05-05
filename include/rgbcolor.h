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

/*
This is a minimal implementation of the concept of RGB color,
which is nothing but a three component vector whose components
range from 0 to 255.
Here we do not check neither for the validity of the data (even if it automatically
enforced by the type "unsigned char"), nor for the possible overflows taking place.
This is the reason why we proscribed certain operators (previously used) such
as the operator +, -,etc ...
*/

class RGBColor
{
protected:
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

public:
    RGBColor() = default;
    RGBColor(unsigned char, unsigned char, unsigned char);
    //  explicit RGBColor(unsigned char);
    // I do not want it to be explicit, we can use it for cast...makes sense
    RGBColor(unsigned char);

    void setRed(unsigned char); 
    void setGreen(unsigned char);
    void setBlue(unsigned char);

    unsigned char getRed() const { return red; } 
    unsigned char getGreen() const { return green; } 
    unsigned char getBlue() const { return blue; }

    double distanceEuclidian(const RGBColor &) const; // Euclidian distance
    double distanceEuclidianCorrected(const RGBColor &) const;  // Corrected Euclidian distance following the 
                                                                // discussion from https://en.wikipedia.org/wiki/Color_difference

    double abs() const;
    RGBColor bw() const; // Return the black-white correspoding color.
};

RGBColor meanColor(const std::vector<RGBColor> &);

// Used to parse a ppm file (see https://en.wikipedia.org/wiki/Color_difference) into something
// which can be simply put in a quadtree structure. This is highly necessary because the way of
// ordering pixels is way different.
std::tuple<unsigned int, unsigned int, std::vector<RGBColor>> parsePPM(const std::string &);

std::ostream & operator<<(std::ostream &, const RGBColor &);

// This is a helper function needed to change the ordering of the pixels from the way they are read
// from the ppm file to the way we can use to insert them into a quadtree.
void indicesVectorBuilderHelp(const Matrix<size_t> &, std::vector<size_t> &, size_t, size_t, size_t, size_t);

// To generate a color palette to be used to produce nice plots of 
// quadtrees.
std::vector<RGBColor> rainbowOfColors(size_t number_of_colors);

#endif
