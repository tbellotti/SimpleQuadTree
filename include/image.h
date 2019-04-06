#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"
#include "refinementcriterioncolor.h"


template <typename T>
class Image
{
protected:
    T x_size;
    T y_size;
    unsigned int min_level;
    unsigned int max_level;
    std::shared_ptr<Pixel<T>> parent_cell; 

    bool simplifyImageDelete(const std::shared_ptr<Pixel<T>> cell, const CriterionVariance<T> & criterion)
    {
        std::vector<std::shared_ptr<Cell<T>>> children_vector = cell->getChildren();
        std::vector<std::shared_ptr<Pixel<T>>> children_vector_cast;

        for (auto ch : children_vector) {
            children_vector_cast.push_back(std::static_pointer_cast<Pixel<T>>(ch));
        }

        if (!cell->isLeaf())    {
            if (getLevel(cell) >= min_level
                && children_vector_cast[0]->isLeaf() && children_vector_cast[1]->isLeaf() 
                && children_vector_cast[2]->isLeaf() && children_vector_cast[3]->isLeaf() && !cell->getFlag())   {

                if (!criterion(cell))   {
                    cell->mergeCell();
                    return true;
                }
                else    {
                    cell->setFlag(true);
                    return false;
                }
            }
            else    {
                return simplifyImageDelete(children_vector_cast[0], criterion) ||
                   simplifyImageDelete(children_vector_cast[1], criterion) ||
                   simplifyImageDelete(children_vector_cast[2], criterion) ||
                   simplifyImageDelete(children_vector_cast[3], criterion);
            }
        }
        else    {
            return false;
        }
    }

// The level makes sense only in relation to the quadtree the cell belongs
    // to, hence it is a member.
    unsigned int getLevel(const std::shared_ptr<Pixel<T>> cell)    
    {
        return (unsigned)(log2(x_size / cell->getDx()));
    }


public:
    // These values are not impotant for what we have to do..
    Image() : x_size(1.0), y_size(1.0), min_level(1), max_level(10), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), 1.0, 1.0)) {}

    Image(T xsize, T ysize, unsigned int minl, unsigned int maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new Pixel<T>(Point<T>(0.0, 0.0), xsize, ysize)) {}
    ~Image() = default;


    void clear()
    {
        parent_cell = std::make_shared<Pixel<T>>(Pixel<T>(Point<T>(0.0, 0.0), x_size, y_size));
    }

    double foo()    
    {
        return 50.0;
    }



    void simplifyImage(double threshold)
    {

        const CriterionVariance<T> crt(threshold);      
        bool updated = true;

        while (updated) { 
            // Deleting unuseful cells.
            updated = simplifyImageDelete(parent_cell, crt);
        }        
    }

    void buildUniform(unsigned int m_l) 
    {
        clear();  // In this way, we do not have to assume to start from an empty grid
        splitHelp(parent_cell, m_l);
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
        max_level = log2(x_size);

        buildUniform(max_level);

        // Iterator for navigating the vector,
        // to be passed as reference in order to be modified.
        std::vector<RGBColor>::const_iterator it = std::get<2>(parsed_file).cbegin();

        putColorsInTreeHelp(parent_cell, it);
        //putColorsInTreeHelp(parent_cell);

    }



    void saveImage(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass{standalone}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{tikzpicture}[define rgb/.code={\\definecolor{mycolor}{RGB}{#1}},rgb color/.style={define rgb={#1},mycolor}]\n";

        //std::vector<std::shared_ptr<CellWithFather<T>>> leaves = getLeaves();
        std::vector<std::shared_ptr<Pixel<T>>> leaves = parent_cell->getLeavesCast();
        for (auto leaf : leaves)    {
            std::vector<Point<T>> vertices = leaf->getVertices();
            RGBColor fld = leaf->getField();
            //Point<T> ctr = leaf->getCenter();

            output_f<<"\\fill [rgb color={"<<fld.getRed()<<","<<fld.getGreen()<<","<<fld.getBlue()<<"}]"<<vertices[0]<<" rectangle "<<vertices[2]<<"; \n";
            /*std::vector<Point<T>> vertices = leaf->getVertices();
            output_f<<"\\draw "<<vertices[0]<<" -- "<<vertices[1]<<"; \n";
            output_f<<"\\draw "<<vertices[1]<<" -- "<<vertices[2]<<"; \n";
            output_f<<"\\draw "<<vertices[2]<<" -- "<<vertices[3]<<"; \n";
            output_f<<"\\draw "<<vertices[3]<<" -- "<<vertices[0]<<"; \n";*/
        }

        output_f<<"\\end{tikzpicture} \n \\end{document} \n";
        output_f.close();
    }
};


// An helper function to perform a recursive splitting
template <typename T>
void splitHelp(std::shared_ptr<Pixel<T>> father, unsigned int level_to_go) {
    if (level_to_go > 0)    {
        father->splitCell();

        std::vector<std::shared_ptr<Cell<T>>> children_vector = father->getChildren();
        std::vector<std::shared_ptr<Pixel<T>>> children_vector_cast;

        for (auto ch : children_vector) {
            children_vector_cast.push_back(std::static_pointer_cast<Pixel<T>>(ch));
        }

        splitHelp(children_vector_cast[0], level_to_go - 1);
        splitHelp(children_vector_cast[1], level_to_go - 1);
        splitHelp(children_vector_cast[2], level_to_go - 1);
        splitHelp(children_vector_cast[3], level_to_go - 1);
    }
}


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


