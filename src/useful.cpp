#include "../include/useful.h"

std::string beginTikzFigure()
{
    std::string to_return("\\RequirePackage{luatex85} \n \\documentclass{standalone}\n \\usepackage{tikz} \n \\begin{document}\n \\begin{tikzpicture}[define rgb/.code={\\definecolor{mycolor}{RGB}{#1}},rgb color/.style={define rgb={#1},mycolor}]\n");
    return to_return;
}

std::string endTikzFigure()
{
    std::string to_return("\\end{tikzpicture} \n \\end{document} \n");
    return to_return;
}

bool isPowerOf(long long int n, int base) 
{ 
    double i = log(double (n)) / log(base); 
    // check if i is an integer or not 
    return (i - trunc(i) < 0.000001);
} 
