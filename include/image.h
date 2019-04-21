#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"
#include "refinealwayscriterion.h"
#include "refinementcriterioncolor.h"

template <typename T>
class Image
{
protected:
    // This are not const since we have to modify them
    // when we load an image from a file. This is completely
    // different from what happens for a quadtree mesh.
    T x_size;
    T y_size;
    unsigned char min_level;
    unsigned char max_level;
    std::shared_ptr<Pixel<T>> parent_cell; 


public:
    // These values are not impotant for what we have to do..
    Image() : x_size(1.0), y_size(1.0), min_level(1), max_level(10), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), 1.0, 1.0, 0)) {}

    Image(T xsize, T ysize, unsigned char minl, unsigned char maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), xsize, ysize, 0)) {}
    ~Image() = default;

    unsigned int getMinLevel() const
    {
        // Conversion for the sake of printing...
        return static_cast<unsigned int>(min_level);
    }

    unsigned int getMaxLevel() const
    {
        return static_cast<unsigned int>(max_level);
    }

    size_t numberOfPixels() const
    {
        // We do not care about casting them, it does not matter.
        std::vector<std::shared_ptr<Cell<T>>> leaves_tmp; 
        parent_cell->getLeaves(leaves_tmp);

        return leaves_tmp.size();
    }

    void clear()
    {
        parent_cell = std::make_shared<Pixel<T>>(Pixel<T>(Point<T>(0.0, 0.0), x_size, y_size, 0));
    }

    void simplifyImage(double threshold)
    {
        const CriterionVariance<T> crt(threshold);
        parent_cell->simplifyCell(crt, min_level);
    }

    void buildUniform(unsigned char m_l) 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        RefineAlwaysCriterion<T> criterion;
        parent_cell->updateCell(criterion, min_level, m_l);

    }

    void createFromFile(std::string filename)
    {
        // We support only square images with pixels on a side which are
        // powers of 2:
        // 2 4 8 16 32 64 128 256 ....
        std::tuple<unsigned, unsigned, std::vector<RGBColor>> parsed_file = parsePBM(filename);

        x_size = T(std::get<0>(parsed_file)); 
        y_size = T(std::get<1>(parsed_file)); 
        min_level = 1;
        max_level = static_cast<unsigned char>(log2(x_size));

        buildUniform(max_level);

        // Iterator for navigating the vector,
        // to be passed as reference in order to be modified.
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
        castVectorOfCellToVectorOfPixel(leaves_tmp, leaves);

        for (auto leaf : leaves)    {
            std::vector<Point<T>> vertices = leaf->getVertices();
            output_f<<leaf->tikzSquare((leaf->getField()), false);        
        }

        output_f<<endTikzFigure();
        output_f.close();
    }


    void saveImageContours(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<beginTikzFigure();

        std::vector<std::shared_ptr<Cell<T>>> leaves_tmp; 
        parent_cell->getLeaves(leaves_tmp);
        std::vector<std::shared_ptr<Pixel<T>>> leaves;
        castVectorOfCellToVectorOfPixel(leaves_tmp, leaves);

        for (auto leaf : leaves)    {
            if (leaf->getLevel() == max_level)  {    
                output_f<<leaf->tikzSquare(RGBColor(0, 0, 0), false);
            }
        }

        output_f<<endTikzFigure();
        output_f.close();
    }

};

template <typename T>
void putColorsInTreeHelp(std::shared_ptr<Pixel<T>> father, std::vector<RGBColor>::const_iterator & it)
{

    if (father->isLeaf())   {
        father->setField(*it);
        it++;
        return;
    }
    else    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = father->getChildren();
        std::vector<std::shared_ptr<Pixel<T>>> children_vector_cast;

        for (auto ch : children_vector) {
            children_vector_cast.push_back(std::static_pointer_cast<Pixel<T>>(ch));
        }
            
        putColorsInTreeHelp(children_vector_cast[0], it);
        putColorsInTreeHelp(children_vector_cast[1], it);
        putColorsInTreeHelp(children_vector_cast[3], it);
        putColorsInTreeHelp(children_vector_cast[2], it);
    }
}


#endif

