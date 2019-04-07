#ifndef QUADTREEIMAGE_H
#define QUADTREEIMAGE_H


#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "cellwithfather.h"
#include "refinementcriterioncolor.h"

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



    bool simplifyImageDelete(const std::shared_ptr<CellWithFather<T>> cell, const RefinementCriterionColor<T> & criterion)
    {
        std::vector<std::shared_ptr<CellWithFather<T>>> children_vector = cell->getChildren();

        if (!cell->isLeaf())    {
            if (getLevel(cell) >= min_level
                && children_vector[0]->isLeaf() && children_vector[1]->isLeaf() 
                && children_vector[2]->isLeaf() && children_vector[3]->isLeaf()
                && !criterion(cell))   {

                cell->mergeCell();
                return true;
            }
            else    {
                return simplifyImageDelete(children_vector[0], criterion) ||
                   simplifyImageDelete(children_vector[1], criterion) ||
                   simplifyImageDelete(children_vector[2], criterion) ||
                   simplifyImageDelete(children_vector[3], criterion);
            }
        }
        else    {
            return false;
        }
    }


public:
    QuadTreeImage(T xsize, T ysize, unsigned int minl, unsigned int maxl) : 
            x_size(xsize), y_size(ysize), min_level(minl), max_level(maxl), 
            parent_cell(new CellWithFather<T>(Point<T>(0.0, 0.0), xsize, ysize, nullptr)) {}



    // The level makes sense only in relation to the quadtree the cell belongs
    // to, hence it is a member.
    unsigned int getLevel(const std::shared_ptr<CellWithFather<T>> cell)    
    {
        return (unsigned)(log2(x_size / cell->getDx()));
    }

    void simplifyImage()//const RefinementCriterionColor<T> & criterion)
    {

        const RefinementCriterionColor<T> crt(0.10);      
        bool updated = true;

        while (updated) { 
            // Deleting unuseful cells.
            updated = simplifyImageDelete(parent_cell, crt);
        }        
    }

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
        putColorsInTreeHelp(parent_cell, it);
        //putColorsInTreeHelp(parent_cell);

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


    void exportToFile(const std::string & filename) const
    {
        std::ofstream output_f;
        output_f.open (filename);
        output_f<<"\\documentclass{standalone}\n";
        output_f<<"\\usepackage{tikz} \n";
        output_f<<"\\begin{document}\n";
        output_f<<"\\begin{tikzpicture}[define rgb/.code={\\definecolor{mycolor}{RGB}{#1}},rgb color/.style={define rgb={#1},mycolor}]\n";

        std::vector<std::shared_ptr<CellWithFather<T>>> leaves = getLeaves();
        for (std::shared_ptr<CellWithFather<T>> leaf : leaves)    {
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
void putColorsInTreeHelp(std::shared_ptr<CellWithFather<T>> father, std::vector<RGBColor>::const_iterator & it)
{


    
    //std::cout<<"IsLeaf: "<<father->isLeaf()<<std::endl;

    if (father->isLeaf())   {
        //std::cout<<"Elaborating leaf"<<std::endl;
        father->setField(*it);
        //std::cout<<"Field set = "<<*it<<std::endl;
        it++;
        //std::cout<<"Iterator incremented"<<std::endl;

        return;

    }
    else    {
            std::cout<<"Before getting sons"<<std::endl;
            std::vector<std::shared_ptr<CellWithFather<T>>> children_vector = father->getChildren();
            std::cout<<"Sons gotten"<<std::endl;

            /*
            putColorsInTreeHelp(children_vector[2], it);
            putColorsInTreeHelp(children_vector[3], it);
            putColorsInTreeHelp(children_vector[0], it);
            putColorsInTreeHelp(children_vector[1], it);*/

            
            putColorsInTreeHelp(children_vector[0], it);
            putColorsInTreeHelp(children_vector[1], it);
            putColorsInTreeHelp(children_vector[3], it);
            putColorsInTreeHelp(children_vector[2], it);

/*
            if (children_vector[2] != nullptr)  {
                std::cout<<"Descending 2"<<std::endl;
                putColorsInTreeHelp(children_vector[2]);
            }
            //std::cout<<"AA : "<<children_vector[4]<<std::endl;
            if (children_vector[3] != nullptr)  {
                std::cout<<"Descending 3"<<std::endl;
                putColorsInTreeHelp(children_vector[3]);
            }
            if (children_vector[0] != nullptr)  {
                std::cout<<"Descending 0"<<std::endl;
                putColorsInTreeHelp(children_vector[0]);
            }
            if (children_vector[1] != nullptr)  {
                std::cout<<"Descending 1"<<std::endl;
                putColorsInTreeHelp(children_vector[1]);
            }
*/
}

}


#endif

