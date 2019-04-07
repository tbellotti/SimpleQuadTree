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
#include "levelsetcriterion.h"

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


    bool updateQuadTreeDelete(const std::shared_ptr<Cell<T>> cell, const RefinementCriterion<T> & criterion)
    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();

        if (!cell->isLeaf())    {
            if (getLevel(cell) >= min_level
                && children_vector[0]->isLeaf() && children_vector[1]->isLeaf() 
                && children_vector[2]->isLeaf() && children_vector[3]->isLeaf()
                && !criterion(cell))   {
                // We compute the value of the criterion even when
                // the cell has already been scanned and declared not to
                // erase because the function is very fast to evaluate
                // that adding a flag in the Cell class to check this
                // gives a slight slow down.
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

    void updateQuadTreeCreate(const std::shared_ptr<Cell<T>> cell, const RefinementCriterion<T> & criterion)   
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

    virtual ~QuadTree() = default;

    // The level makes sense only in relation to the quadtree the cell belongs
    // to, hence it is a member.
    unsigned int getLevel(const std::shared_ptr<Cell<T>> cell)    
    {
        return (unsigned)(log2(x_size / cell->getDx()));
    }

    unsigned int getMinLevel() const
    {
        return min_level;
    }

    unsigned int getMaxLevel() const
    {
        return max_level;
    }

    size_t numberOfLeaves() const
    {
        //return numberOfLeavesHelp(parent_cell);
        auto leaves = getLeaves();
        return leaves.size();
    }

    void buildUniform() 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        splitHelp(parent_cell, min_level);
    }

    // I cannot resume the previous function and this in the second
    // with a default argument because min_level is not static.
    void buildUniform(const unsigned int lev)
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

    void updateWithLevelSet(const LipschitzFunction<T> & level_set)
    {
        const LipschitzFunction<T> * tmp_lv = &level_set;
        LevelSetCriterion<T> level_set_criterion(tmp_lv); 
        updateQuadTree(level_set_criterion);
    }

    void updateQuadTree(const RefinementCriterion<T> & criterion)
    {
        // Refinement 
        updateQuadTreeCreate(parent_cell, criterion);

        bool updated = true;
        int i = 0;
        while (updated) { 
            
            std::cout<<"# "<<i<<std::endl;
            i++;

            // Deleting unuseful cells.
            updated = updateQuadTreeDelete(parent_cell, criterion);
        }        
    }

    std::vector<Point<T>> getCenters() const
    {
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
        /*
        std::vector<std::shared_ptr<Cell<T>>> to_return;
        getLeavesHelp(parent_cell, to_return);
        */
        /*
        // Super ugly notation but works.
        using ::getLeaves;
        return getLeaves(parent_cell);
        */
        return parent_cell->getLeaves();
        //return to_return;
    }

    // Used to export the plot of the mesh in a LaTeX file.
    void exportCentersTikz(const std::string & filename) const
    {
        std::vector<Point<T>> centers = getCenters();

        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass{standalone}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{tikzpicture}\n";

        for (auto ctr : centers)    {
            output_f<<"\\draw [fill=blue, line width = 0.1pt]"<<ctr<<" circle[radius= 0.5pt]; \n";

        }
        output_f<<"\\end{tikzpicture}\n \\end{document} \n";


        output_f.close();

    }

    void exportMeshTikz(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass{standalone}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{tikzpicture}\n";

        std::vector<std::shared_ptr<Cell<T>>> leaves = parent_cell->getLeaves();
        for (std::shared_ptr<Cell<T>> leaf : leaves)    {
            std::vector<Point<T>> vertices = leaf->getVertices();
            output_f<<"\\draw [ultra thin]"<<vertices[0]<<" -- "<<vertices[1]<<"; \n";
            output_f<<"\\draw [ultra thin]"<<vertices[1]<<" -- "<<vertices[2]<<"; \n";
            output_f<<"\\draw [ultra thin]"<<vertices[2]<<" -- "<<vertices[3]<<"; \n";
            output_f<<"\\draw [ultra thin]"<<vertices[3]<<" -- "<<vertices[0]<<"; \n";
        }

        output_f<<"\\end{tikzpicture} \n \\end{document} \n";
        output_f.close();
    }


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

    // This can integrate taking features of the cell into account.
    T simpleIntegration(std::function<T(std::shared_ptr<Cell<T>>)> & f) const
    {
        T integral = 0.0;
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        for (auto leaf : leaves)    {
            integral += f(leaf)*leaf->cellSurface();
        }
        
        return integral;
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




#endif

