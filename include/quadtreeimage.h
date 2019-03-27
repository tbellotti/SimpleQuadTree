#ifndef QUADTREEIMAGE_H
#define QUADTREEIMAGE_H


#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "cellwithfather.h"

template <typename T>
class QuadTreeImage
{
protected:

    //const T x_size;
    //const T y_size;
    //const unsigned int min_level;
    //const unsigned int max_level;
    T x_size;
    T y_size;
    unsigned int min_level;
    unsigned int max_level;
    std::shared_ptr<CellWithFather<T>> parent_cell; 


    // TO be kept private since dangerous
    void setSize(T x_n, T y_n) 
    {
        x_size = x_n;
        y_size = y_n;
    }

    void setLevels(unsigned mn_l, unsigned mx_l)
    {
        min_level = mn_l;
        max_level = mx_l;
    }

public:
    QuadTreeImage(T xsize, T ysize, unsigned int minl, unsigned int maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new CellWithFather<T>(Point<T>(0.0, 0.0), xsize, ysize, nullptr)) {}


    void createFromImage(std::string filename)
    {


        std::tuple<unsigned, unsigned, std::vector<RGBColor>> parsed_file = parsePBM(filename);

        T x_size = T(std::get<0>(parsed_file)); 
        T y_size = T(std::get<1>(parsed_file)); 
        unsigned maxlv = log2(x_size);

        std::cout<<x_size<<std::endl;

        setSize(x_size, y_size);
        setLevels(1, maxlv);

        buildUniform(max_level);

        exportMeshTikz(std::string("fooTest.tex"));

        // Iterator for navigating the vector,
        // to be passed as reference in order to be modified.
        std::vector<RGBColor>::const_iterator it = std::get<2>(parsed_file).cbegin();

        std::cout<<"Here 1"<<std::endl;
        //putColorsInTreeHelp(parent_cell, it);
        putColorsInTreeHelp(parent_cell);

    }


    virtual ~QuadTreeImage() = default;


    void clear()
    {
        parent_cell = std::make_shared<CellWithFather<T>>(CellWithFather<T>(Point<T>(0.0, 0.0), x_size, y_size, nullptr));
    }


    void buildUniform() 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        splitHelp(parent_cell, min_level);
    }

    void buildUniform(const unsigned int lev)
    {
        if (lev >= min_level && lev <= max_level)   {
            clear();
            splitHelp(parent_cell, lev);
        }
    }

    std::vector<std::shared_ptr<CellWithFather<T>>> getLeaves() const
    {
        std::vector<std::shared_ptr<CellWithFather<T>>> to_return;
        getLeavesHelp(parent_cell, to_return);

        return to_return;
    }


    void exportMeshTikz(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass{standalone}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{tikzpicture}\n";

        std::vector<std::shared_ptr<CellWithFather<T>>> leaves = getLeaves();
        for (std::shared_ptr<CellWithFather<T>> leaf : leaves)    {
            std::vector<Point<T>> vertices = leaf->getVertices();
            output_f<<"\\draw "<<vertices[0]<<" -- "<<vertices[1]<<"; \n";
            output_f<<"\\draw "<<vertices[1]<<" -- "<<vertices[2]<<"; \n";
            output_f<<"\\draw "<<vertices[2]<<" -- "<<vertices[3]<<"; \n";
            output_f<<"\\draw "<<vertices[3]<<" -- "<<vertices[0]<<"; \n";
        }

        output_f<<"\\end{tikzpicture} \n \\end{document} \n";
        output_f.close();
    }

};



// An helper function to perform a recursive splitting
template <typename T>
void splitHelp(std::shared_ptr<CellWithFather<T>> father, unsigned int level_to_go) {
    if (level_to_go > 0)    {
        father->splitCell();

        std::vector<std::shared_ptr<CellWithFather<T>>> children_vector = father->getChildren();

        splitHelp(children_vector[0], level_to_go - 1);
        splitHelp(children_vector[1], level_to_go - 1);
        splitHelp(children_vector[2], level_to_go - 1);
        splitHelp(children_vector[3], level_to_go - 1);
    }
}



template <typename T>
void getLeavesHelp(std::shared_ptr<CellWithFather<T>> cell, std::vector<std::shared_ptr<CellWithFather<T>>> & to_fill) {

    if (cell->isLeaf()) {
        to_fill.push_back(cell);
    }
    else    {
        std::vector<std::shared_ptr<CellWithFather<T>>> children_vector = cell->getChildren();

        getLeavesHelp(children_vector[0], to_fill);
        getLeavesHelp(children_vector[1], to_fill);
        getLeavesHelp(children_vector[2], to_fill);
        getLeavesHelp(children_vector[3], to_fill);
    }

}

template <typename T>
void putColorsInTreeHelp(std::shared_ptr<CellWithFather<T>> father)//, std::vector<RGBColor>::const_iterator & it)
{

    std::cout<<"Here 2: "<<father->isLeaf()<<std::endl;
    if (father->isLeaf())   {
        std::cout<<"Here 3"<<std::endl;
        //father->setField(*it);
        //it++;
        std::cout<<"Here ??"<<std::endl;
        return;
        std::cout<<"Here MO"<<std::endl;

    }
    else    {
            std::cout<<"Here K"<<std::endl;
            std::vector<std::shared_ptr<CellWithFather<T>>> children_vector = father->getChildren();
            std::cout<<"Here 4"<<std::endl;

            /*
            putColorsInTreeHelp(children_vector[3], it);
            putColorsInTreeHelp(children_vector[4], it);
            putColorsInTreeHelp(children_vector[1], it);
            putColorsInTreeHelp(children_vector[2], it);
*/

            putColorsInTreeHelp(children_vector[3]);
            putColorsInTreeHelp(children_vector[4]);
            putColorsInTreeHelp(children_vector[1]);
            putColorsInTreeHelp(children_vector[2]);

}

}


#endif

