#ifndef QUADTREE_H
#define QUADTREE_H

#include <iostream>
#include <memory>
#include <fstream>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include "cell.h"
#include "lipschitzfunction.h"
#include "refinementcriterion.h"

/*
This class implements the notion of 
QuadTree mesh based on the generic 
type T.
*/


template <typename T>
class QuadTree
{
protected:

    const T x_size;
    const T y_size;

    // We consider the minimum and the maximum level of the
    // QuadTree as immutable (one can change this)
    const unsigned int min_level;
    const unsigned int max_level;

    std::shared_ptr<Cell<T>> parent_cell; 

    // The level makes sense only in relation to the quadtree the cell belongs
    // to, hence it is a member.
    unsigned int getLevel(std::shared_ptr<Cell<T>> cell)    
    {
        return (unsigned)(log2(x_size / cell->getDx()));
    }


    bool updateQuadTreeDelete(std::shared_ptr<Cell<T>> cell, const RefinementCriterion<T> & criterion)
    {

        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        if (!cell->isLeaf())    {
            if (getLevel(cell) >= min_level
                && children_vector[0]->isLeaf() && children_vector[1]->isLeaf() 
                && children_vector[2]->isLeaf() && children_vector[3]->isLeaf()
                && !criterion(cell))   {

                cell->mergeCell();
                return true;
            }
            else    {
                return updateQuadTreeDelete(children_vector[0], criterion) ||
                   updateQuadTreeDelete(children_vector[1], criterion) ||
                   updateQuadTreeDelete(children_vector[2], criterion) ||
                   updateQuadTreeDelete(children_vector[3], criterion);
            }
        }
        else    {
            return false;
        }
    }

    void updateQuadTreeCreate(std::shared_ptr<Cell<T>> cell, const RefinementCriterion<T> & criterion)   
    {   
        bool i_splitted = false;
        if (cell->isLeaf() && getLevel(cell) < max_level)  {
            if (criterion(cell))   {
                cell->splitCell();
                i_splitted = true;
            }

        }   
        if(i_splitted || !cell->isLeaf())    {

            std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

            updateQuadTreeCreate(children_vector[0], criterion);
            updateQuadTreeCreate(children_vector[1], criterion);
            updateQuadTreeCreate(children_vector[2], criterion);
            updateQuadTreeCreate(children_vector[3], criterion);
        } 
    }

public:
    QuadTree(T xsize, T ysize, unsigned int minl, unsigned int maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new Cell<T>(Point<T>(0.0, 0.0), xsize, ysize)) {}

    ~QuadTree() = default;

    // Simple sequential integration taking a function 
    // as being constant on each cell with the value at the cell
    // center
    T simpleIntegration(std::function<T(Point<T>)> & f) const
    {
        T integral = 0.0;
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        for (auto leaf : leaves)    {
            integral += f(leaf->getCenter()) * leaf->cellSurface();
        }

        return integral;
    }

    unsigned int getMinLevel() const
    {
        return min_level;
    }

    unsigned int getMaxLevel() const
    {
        return max_level;
    }


    unsigned int numberOfLeaves() const
    {
        return numberOfLeavesHelp(parent_cell);
    }


    void buildUniform() 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        splitHelp(parent_cell, min_level);
    }

    // I cannot resume the previous function and this in the second
    // with a default argument because min_level is not static.
    void buildUniform(unsigned int lev)
    {
        if (lev >= min_level && lev <= max_level)   {
            clear();
            splitHelp(parent_cell, lev);
        }
        else    {
            throw std::invalid_argument("[buildUniform] - Trying to build a uniform mesh outside the range [min_level, max_level].");
        }
    }

    void clear()
    {
        parent_cell = std::make_shared<Cell<T>>(Cell<T>(Point<T>(0.0, 0.0), x_size, y_size));
    }


    void refineWithLevelSet(const LipschitzFunction<T> & level_set)
    {
        // !!! THink about passing a constant argument !!!
        // It seems the good idea because the tree does not have
        // the necessity and the right to modify the level set
        // function autonomously
        const LipschitzFunction<T> * tmp_lv = &level_set;
        LevelSetCriterion<T> level_set_criterion(tmp_lv); 
        //level_set_criterion.setLevelSet(level_set);
        updateQuadTree(level_set_criterion);

    }


    void updateQuadTree(const RefinementCriterion<T> & criterion)
    {
        // Refinement 
        updateQuadTreeCreate(parent_cell, criterion);

        bool updated = true;

        while (updated) { // && i < 20000) {
            // Deleting unuseful cells.
            updated = updateQuadTreeDelete(parent_cell, criterion);
            // i++;
        }

        
    }


    std::vector<Point<T>> getCenters() const
    {
        /*std::vector<Point<T>> to_return;
        getCentersHelp(parent_cell, to_return);

        return to_return;
        */
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        std::vector<Point<T>> to_return;

        // Maybe we can do it with a std::for_each, but it finally
        // does the same.

        for (auto leave : leaves)   {
            to_return.push_back(leave->getCenter());
        }


        return to_return;

    }

    std::vector<std::shared_ptr<Cell<T>>> getLeaves() const
    {
        std::vector<std::shared_ptr<Cell<T>>> to_return;
        getLeavesHelp(parent_cell, to_return);

        return to_return;
    }

    void exportCentersTikz(const std::string & filename, const T scale_factor) const
    {
        std::vector<Point<T>> centers = getCenters();

        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass[a4paper,11pt, final]{article}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{center} \n \\begin{tikzpicture}\n";

        for (auto ctr : centers)    {
            output_f<<"\\draw [fill=blue, line width = 0.1pt]"<<(scale_factor*ctr)<<" circle[radius= 0.5pt]; \n";

        }
        output_f<<"\\end{tikzpicture}\n \\end{center} \n \\end{document} \n";


        output_f.close();

    }

    void exportMeshTikz(const std::string & filename, const T scale_factor) const
    {
        //double scale_factor = 8.0;

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

        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        splitHelp(children_vector[0], level_to_go - 1);
        splitHelp(children_vector[1], level_to_go - 1);
        splitHelp(children_vector[2], level_to_go - 1);
        splitHelp(children_vector[3], level_to_go - 1);
    }
}


template <typename T>
void getLeavesHelp(std::shared_ptr<Cell<T>> cell, std::vector<std::shared_ptr<Cell<T>>> & to_fill) {

    if (cell->isLeaf()) {
        to_fill.push_back(cell);
    }
    else    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        getLeavesHelp(children_vector[0], to_fill);
        getLeavesHelp(children_vector[1], to_fill);
        getLeavesHelp(children_vector[2], to_fill);
        getLeavesHelp(children_vector[3], to_fill);
    }

}


template <typename T>
void refineWithLevelSetHelp(std::shared_ptr<Cell<T>> cell, const LipschitzFunction<T> & level_set, unsigned int level_to_go)   
{   
    bool i_splitted = false;
    if (cell->isLeaf() && level_to_go > 0)  {
        if (std::abs(level_set(cell->getCenter())) <= level_set.getLipschitzConstant() * cell->getDiagonal())   {
            cell->splitCell();
            i_splitted = true;
            std::cout<<"Cell split"<<std::endl;
        }

    }   
    if(i_splitted || !cell->isLeaf())    {

        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        refineWithLevelSetHelp(children_vector[0], level_set, level_to_go - 1);
        refineWithLevelSetHelp(children_vector[1], level_set, level_to_go - 1);
        refineWithLevelSetHelp(children_vector[2], level_set, level_to_go - 1);
        refineWithLevelSetHelp(children_vector[3], level_set, level_to_go - 1);
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
        output_f<<"\\draw [red] "<<(scale_factor*ctr)<<" circle[radius= 0.01pt]; \n";

    }
    else    {
        
        
        
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        exportMeshTikzHelp(children_vector[0], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[1], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[2], output_f, scale_factor);
        exportMeshTikzHelp(children_vector[3], output_f, scale_factor);
        
        
        /*
        recursiveFunctionCallOnChildren(cell, [] (std::shared_ptr<Cell<T>> cl, std::ofstream & o_f, double s_f)
            { exportMeshTikzHelp(cl, o_f, s_f); }, output_f, scale_factor);
        */
    }

}

template<typename T>
void stupidTestHelp(std::shared_ptr<Cell<T>> cell, T value)
{
    if (!cell->isLeaf())    {
        recursiveFunctionCallOnChildren(cell, [] (std::shared_ptr<Cell<T>> cl, T vl) { stupidTestHelp(cl, vl); }, value);
    }
}

template<typename T>
unsigned int numberOfLeavesHelp(const std::shared_ptr<Cell<T>> & cell)
{
    if (cell->isLeaf())
        return 1;
    else
    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        return numberOfLeavesHelp(children_vector[0])
             + numberOfLeavesHelp(children_vector[1])
             + numberOfLeavesHelp(children_vector[2])
             + numberOfLeavesHelp(children_vector[3]);
   
    }
    
}

#endif

