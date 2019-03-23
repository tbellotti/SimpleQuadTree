#ifndef FUNCTIONONQUADTREE_H
#define FUNCTIONONQUADTREE_H

#include <vector>
#include <functional>
#include "quadtree.h"

//          points      values of the function
template <typename T, typename V>
class FunctionOnQuadTree
{
protected:
    std::vector<V> data;

public:
    FunctionOnQuadTree(const QuadTree<T> & tree) : data() { data.reserve(tree.numberOfLeaves()); }
    FunctionOnQuadTree(const QuadTree<T> & tree, std::function<V(T)> func) : FunctionOnQuadTree(tree)
    { 
        std::vector<Point<T>> centers = tree.getCenters();
        for (auto it = centers.cbegin(); it != centers.cend(); it++)    {
            data.push_back(func(*it));
        }
    }

    std::vector<V> getData() const
    {
        return data;
    }

};


#endif
