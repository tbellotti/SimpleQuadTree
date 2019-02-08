#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <memory>
#include <fstream>
#include <cmath>
#include <functional>
#include "cell.h"


template <typename T>
class QuadTree
{
protected:

    T x_size;
    T y_size;

    unsigned int min_level;
    unsigned int max_level;
    std::shared_ptr<Cell<T>> parent_cell; 


public:
    //explicit QuadTree(unsigned int minl) : min_level(minl), max_level(minl) {}
    QuadTree(T xsize, T ysize, unsigned int minl, unsigned int maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), parent_cell(new Cell<T>(Point<T>(0.0, 0.0), xsize, ysize)) 
            {
                std::cout<<"Created QuadTree with base cell."<<std::endl;
            }


    void buildUniform() {
        splitHelp(parent_cell, min_level);
        std::cout<<"Uniform mesh created"<<std::endl;
    }

    void refineWithCriterion(std::function<bool(Point<T>)> criterion)   
    {
        std::cout<<"Level difference : "<<max_level - min_level<<std::endl;
        refineWithCriterionHelp(criterion, parent_cell, max_level - min_level);
    }

    void refineWithLevelSet(std::function<T(Point<T>)> level_set)
    {

        std::cout<<"Level difference : "<<max_level - min_level<<std::endl;
        refineWithLevelSetHelp(level_set, parent_cell, max_level - min_level);
    }


    std::vector<Point<T>> getCenters() const
    {
        std::vector<Point<T>> to_return;
        getCentersHelp(parent_cell, to_return);

        return to_return;

    }

    void exportCentersTikz(std::string filename) const
    {
        double scale_factor = 8.0;
        std::vector<Point<T>> centers = getCenters();

        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass[a4paper,11pt, final]{article}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{center} \n \\begin{tikzpicture}\n";

        for (auto ctr : centers)    {
            output_f<<"\\draw [fill=blue, line width = 0.1pt]"<<(scale_factor*ctr).print()<<" circle[radius= 0.5pt]; \n";

        }
        output_f<<"\\end{tikzpicture}\n \\end{center} \n \\end{document} \n";


        output_f.close();

    }

    void exportMeshTikz(std::string filename) const
    {
        double scale_factor = 8.0;

        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass[a4paper,11pt, final]{article}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{center} \n \\begin{tikzpicture}\n";


        exportMeshTikzHelp(parent_cell, output_f, scale_factor);

        output_f<<"\\end{tikzpicture}\n \\end{center} \n \\end{document} \n";


        output_f.close();

    }

};

// An helper function to perform a recursive splitting
template <typename T>
void splitHelp(std::shared_ptr<Cell<T>> cell, unsigned int level_to_go) {
    if (level_to_go > 0)    {
        cell->splitCell();
        //std::cout<<"Splitting at level = "<<level_to_go<<std::endl;

        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        splitHelp(children_vector[0], level_to_go - 1);
        splitHelp(children_vector[1], level_to_go - 1);
        splitHelp(children_vector[2], level_to_go - 1);
        splitHelp(children_vector[3], level_to_go - 1);
    }
}

template <typename T>
void getCentersHelp(std::shared_ptr<Cell<T>> cell, std::vector<Point<T>> & to_fill) {

    if (cell->isLeaf()) {
        to_fill.push_back(cell->getCenter());
    }
    else    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        getCentersHelp(children_vector[0], to_fill);
        getCentersHelp(children_vector[1], to_fill);
        getCentersHelp(children_vector[2], to_fill);
        getCentersHelp(children_vector[3], to_fill);
    }

}

template <typename T>
void refineWithCriterionHelp(std::function<bool(Point<T>)> criterion, std::shared_ptr<Cell<T>> cell, unsigned int level_to_go)   
{   
    bool i_splitted = false;
    if (cell->isLeaf() && level_to_go > 0)  {
        if (criterion(cell->getCenter()))   {
            cell->splitCell();
            i_splitted = true;
            std::cout<<"Cell split"<<std::endl;
        }

    }   
    if(i_splitted || !cell->isLeaf())    {
        // It has to be factorized using callable objects
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        refineWithCriterionHelp(criterion, children_vector[0], level_to_go - 1);
        refineWithCriterionHelp(criterion, children_vector[1], level_to_go - 1);
        refineWithCriterionHelp(criterion, children_vector[2], level_to_go - 1);
        refineWithCriterionHelp(criterion, children_vector[3], level_to_go - 1);
    } 
}

template <typename T>
void refineWithLevelSetHelp(std::function<T(Point<T>)> level_set, std::shared_ptr<Cell<T>> cell, unsigned int level_to_go)   
{   
    bool i_splitted = false;
    if (cell->isLeaf() && level_to_go > 0)  {
        if (std::abs(level_set(cell->getCenter())) <= 1.0*cell->getDiagonal())   {
            cell->splitCell();
            i_splitted = true;
            std::cout<<"Cell split"<<std::endl;
        }

    }   
    if(i_splitted || !cell->isLeaf())    {
        // It has to be factorized using callable objects
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        refineWithLevelSetHelp(level_set, children_vector[0], level_to_go - 1);
        refineWithLevelSetHelp(level_set, children_vector[1], level_to_go - 1);
        refineWithLevelSetHelp(level_set, children_vector[2], level_to_go - 1);
        refineWithLevelSetHelp(level_set, children_vector[3], level_to_go - 1);
    } 
}
   
template <typename T>
void exportMeshTikzHelp(std::shared_ptr<Cell<T>> cell, std::ofstream & output_f, double scale_factor)
{
    if (cell->isLeaf()) {
        std::vector<Point<T>> vertices = cell->getVertices();
        output_f<<"\\draw "<<(scale_factor*vertices[0]).print()<<" -- "<<(scale_factor*vertices[1]).print()<<"; \n";
        output_f<<"\\draw "<<(scale_factor*vertices[1]).print()<<" -- "<<(scale_factor*vertices[2]).print()<<"; \n";
        output_f<<"\\draw "<<(scale_factor*vertices[2]).print()<<" -- "<<(scale_factor*vertices[3]).print()<<"; \n";
        output_f<<"\\draw "<<(scale_factor*vertices[3]).print()<<" -- "<<(scale_factor*vertices[0]).print()<<"; \n";


        Point<T> ctr = cell->getCenter();
        output_f<<"\\draw [red] "<<(scale_factor*ctr).print()<<" circle[radius= 0.01pt]; \n";

    }
    else    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        exportMeshTikzHelp(children_vector[0], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[1], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[2], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[3], output_f, scale_factor);
    }

}


#endif

