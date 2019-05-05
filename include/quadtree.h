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
#include "refinementcriteria.h"
#include "rgbcolor.h"

/*
This class implements the notion of 
QuadTree mesh based on the generic 
type T.
*/

template <typename T>
class QuadTree
{
protected:
    // We consider the minimum and the maximum level of the
    // QuadTree as immutable (one can change this, but it could
    // become complicated to adapt the whole structure).
    const T x_size; // Size along x
    const T y_size; // Size along y
    const unsigned char min_level;   // Minimum level of splitting
    const unsigned char max_level;   // Maximum level of splitting
    // It should not be const since it can be changed when resetting the whole tree
    // to an empty one.
    std::shared_ptr<Cell<T>> parent_cell;   // Top of the tree.

    // This is protected because though the pointers to the leaves
    // an external agent could modify the structure in an incoherent
    // way (for example violating the max and min levels).
    std::vector<std::shared_ptr<Cell<T>>> getLeaves() const
    {
        std::vector<std::shared_ptr<Cell<T>>> leaves;
        parent_cell->getLeaves(leaves);
        return leaves;
    }

public:
    QuadTree(Point<T> base_point, T xsize, T ysize, unsigned char minl, unsigned char maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new Cell<T>(base_point, xsize, ysize, 0)) {}

    virtual ~QuadTree() = default;

    unsigned int getMinLevel() const
    {
        // The conversion for the sake of printing, since we store the
        // level as unsigned char just to be consistent in the project.
        // This choice is important in the class Cell and Pixel, which
        // is allocated in large number, thus memomory consumption matters.
        // Here we do just to be consistent.
        return static_cast<unsigned int>(min_level);
    }

    unsigned int getMaxLevel() const
    {
        return static_cast<unsigned int>(max_level);
    }

    size_t numberOfLeaves() const
    {
        auto leaves = getLeaves();
        return leaves.size();
    }

    void buildUniform() 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        RefineAlwaysCriterion<T> criterion;
        updateQuadTree(criterion, min_level, max_level);
    }

    // I cannot resume the previous function and this in the second
    // with a default argument because min_level is not static.
    void buildUniform(const unsigned char lev)
    {
        if (lev > max_level || lev < min_level)   {
            throw std::invalid_argument("Trying to build the mesh outside the range [min_level, max_level].");
        }
        else    {
            clear();  
            RefineAlwaysCriterion<T> criterion;
            updateQuadTree(criterion, min_level, lev);
        }
    }

    void clear()
    {
        // We retrieve the base point in order to keep the same
        Point<T> base_pt = parent_cell->getBasePoint(); // Notice that retrieving the base point is especially useful to
                                                        // parallelize the tree.
        parent_cell = std::make_shared<Cell<T>>(Cell<T>(base_pt, x_size, y_size, 0));
    }

    // Updates the tree using the special criterion involing the level-set function.
    void updateWithLevelSet(const LipschitzFunction<T> & level_set)
    {
        LevelSetCriterion<T> level_set_criterion(level_set); 
        updateQuadTree(level_set_criterion);
    }

    // Updates the quadtree using a generic criterion.
    void updateQuadTree(const RefinementCriterion<T> & criterion)
    {
        updateQuadTree(criterion, min_level, max_level);
    }

    // Should be implemented because min_level and max_level
    // are not static thus cannot be used as default values for the
    // other function. As before
    void updateQuadTree(const RefinementCriterion<T> & criterion, const unsigned char min_lv, const unsigned char max_lv)
    {
        if (min_lv < min_level || max_lv > max_level)   {
            throw std::invalid_argument("Trying to update the mesh outside the range [min_level, max_level].");
        }
        else    {
            parent_cell->updateCell(criterion, min_lv, max_lv);
        }
    }

    // This can be obtained by the user since one cannot modify
    // the tree though this, since these are copies
    // of the centers of the leaves.
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

    // Used to export the plot of the mesh in a LaTeX file, made up of 
    // dots placed in the centers of the leaves.
    void exportCentersTikz(const std::string & filename) const
    {
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves(); 
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<beginTikzFigure();
        for (auto leave : leaves)   {
            output_f<<leave->tikzDot();
        }
        output_f<<endTikzFigure();
        output_f.close();
    }

    // The same but with the whole square indicating a Cell
    // and the possibility to color the interior of each leaf according to its level.
    void exportMeshTikz(const std::string & filename, bool color) const
    {
        // We recover a palette of color to differentiate between levels.
        std::vector<RGBColor> palette = rainbowOfColors(size_t(max_level-min_level+1));

        std::ofstream output_f;
        output_f.open (filename);
        output_f<<beginTikzFigure();

        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();
        for (std::shared_ptr<Cell<T>> leaf : leaves)    {
            if (color)  { // if we have to put colors
                RGBColor curr_color = palette[size_t(leaf->getLevel()-min_level)];
                output_f<<leaf->tikzSquare(curr_color, true);
            }
            else    {   // We simply put white inside the cells. Just the boundary will be drawn.
                output_f<<leaf->tikzSquare(RGBColor(255, 255, 255), true);
            }
        }

        output_f<<endTikzFigure();
        output_f.close();
    }

    // Integration relying on the zeroOrderIntegration of the class Cell
    T simpleIntegration(const std::function<T(Point<T>)> & f) const
    {
        T integral = 0.0;
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        for (auto leaf : leaves)    {
            integral += leaf->zeroOrderIntegration(f);
        }
        return integral;
    }

    // Integration relying on the thirdOrderGaussianIntegration of the class Cell
    T thirdOrderGaussianIntegration(const std::function<T(Point<T>)> & f) const
    {
        T integral = 0.0;
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        for (auto leaf : leaves)    {
            integral += leaf->thirdOrderGaussianIntegration(f);
        }
        return integral;
    }

    // This can integrate taking features of the cell into account.
    // to be integrated in the cell, for example, its level. It does not 
    // have a counterpart in the class Cell, for obvious reasons.  
    // It allows us to construct certain fun examples such as measuring the area
    // of the fractal known as Mandelbrot set.
    T simpleIntegration(const std::function<T(std::shared_ptr<Cell<T>>)> & f) const
    {
        T integral = 0.0;
        std::vector<std::shared_ptr<Cell<T>>> leaves = getLeaves();

        for (auto leaf : leaves)    {
            // It is indeed the same than for zeroOrderIntegration in the class Cell.
            integral += f(leaf)*leaf->cellSurface();
        }
        return integral;
    }

};

#endif
