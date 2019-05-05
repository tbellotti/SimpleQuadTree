#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"
#include "refinementcriteria.h"

/*
This class implements the notion of 
Image based on a quadtree parametrized by on the generic 
type T.
*/

template <typename T>
class Image
{
protected:
    // These are not const since we have to modify them
    // when we load an image from a file, which is the way we create it
    // This is completely different from what happens for a quadtree mesh.
    T x_size;   // Size on x
    T y_size;   // Size on y
    unsigned char min_level;    // Minimum level allowed  
    unsigned char max_level;    // Maximum level allowed
    
    std::shared_ptr<Pixel<T>> parent_cell; // Top of the tree (can change when we reset it)


public:
    Image() : x_size(1.0), y_size(1.0), min_level(1), max_level(10), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), 1.0, 1.0, 0)) {}

    Image(T xsize, T ysize, unsigned char minl, unsigned char maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), xsize, ysize, 0)) {}

    ~Image() = default;

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

    size_t numberOfPixels() const
    {
        std::vector<std::shared_ptr<Cell<T>>> leaves_tmp; // We do not care about casting them, 
        parent_cell->getLeaves(leaves_tmp);               // to count the number of element, it does not matter.

        return leaves_tmp.size();
    }

    void clear()
    {
        parent_cell = std::make_shared<Pixel<T>>(Pixel<T>(Point<T>(0.0, 0.0), x_size, y_size, 0));
    }

    // We can only simplify (compress) an image. We do not allow
    // eriching it, since we do not want to create information from nowhere.
    void simplifyImage(double threshold)
    {
        const CriterionVariance<T> crt(threshold);
        parent_cell->simplifyCell(crt, min_level);
    }

    void buildUniform(unsigned char m_l) 
    {
        clear();  // In this way, we do not have to assume to start from an empty tree
        RefineAlwaysCriterion<T> criterion;
        parent_cell->updateCell(criterion, min_level, m_l);
    }

    // Import an image from a .ppm ascii file (see https://fr.wikipedia.org/wiki/Portable_pixmap)
    void createFromFile(std::string filename)
    {
        // We support only square images with pixels on a side which are
        // powers of 2:
        // 2 4 8 16 32 64 128 256 ...., just for the sake of simplicity

        // We need to reorder the vector of pixel because the way of storing them
        // in a .ppm, row-by-row, is not at all that for a quadtree. The parser does it automatically
        std::tuple<unsigned, unsigned, std::vector<RGBColor>> parsed_file = parsePPM(filename);

        x_size = T(std::get<0>(parsed_file)); 
        y_size = T(std::get<1>(parsed_file)); 
        min_level = 1;
        max_level = static_cast<unsigned char>(log2(x_size)); // Level needed in order to have one pixel for one cell.

        buildUniform(max_level);

        // Iterator for navigating the vector, to be passed as reference in order to be modified.
        std::vector<RGBColor>::const_iterator it = std::get<2>(parsed_file).cbegin();
        putColorsInTreeHelp(parent_cell, it);
    }

    void saveImage(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<beginTikzFigure();

        // We might factorize it but it does not simplify things a lot,
        // since it is just used twice.
        std::vector<std::shared_ptr<Cell<T>>> leaves_tmp; 
        parent_cell->getLeaves(leaves_tmp);
        std::vector<std::shared_ptr<Pixel<T>>> leaves;
        castVectorOfCellToVectorOfPixel(leaves_tmp, leaves); // Since Pixel inherits from Cell.

        for (auto leaf : leaves)    {
            std::vector<Point<T>> vertices = leaf->getVertices();
            output_f<<leaf->tikzSquare((leaf->getField()), false); // Printing the square with the relative color.
        }

        output_f<<endTikzFigure();
        output_f.close();
    }
};

// This is a very important helper function which puts the colors in the leaves of the tree.
template <typename T>
void putColorsInTreeHelp(std::shared_ptr<Pixel<T>> father, std::vector<RGBColor>::const_iterator & it)
{
    if (father->isLeaf())   { //  We are at a generalized pixel of the figure.
        father->setField(*it);
        it++;
        return;
    }
    else    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = father->getChildren();
        std::vector<std::shared_ptr<Pixel<T>>> children_vector_cast;
        castVectorOfCellToVectorOfPixel(children_vector, children_vector_cast); // Since Pixel inherits from Cell.

        putColorsInTreeHelp(children_vector_cast[0], it);
        putColorsInTreeHelp(children_vector_cast[1], it);
        putColorsInTreeHelp(children_vector_cast[3], it);
        putColorsInTreeHelp(children_vector_cast[2], it);
    }
}

#endif
