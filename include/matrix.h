#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

// This class implements the concepts of matrix as well as some simple operations 
// on them. It is not widely used in the project. Indeed, just once to convert
// the pixel ordering from the .ppm format (row-by-row) to that of a quadtree.

template <typename T>
class Matrix
{
protected:
    std::vector<std::vector<T>> matrix;

public:
    Matrix(size_t i_size, size_t j_size) : matrix(i_size, std::vector<T>(j_size)) {}

    // Construct a matrix scanning a vector with a row-by-row logic.
    Matrix(size_t n_cols, const std::vector<T> & line) : Matrix(line.size() / n_cols, n_cols) 
    {
        for (size_t idx_line = 0; idx_line < line.size(); idx_line++)   {
            matrix[idx_line / n_cols][idx_line % n_cols] = line[idx_line];
        }
    }

    virtual ~Matrix() = default;

    // Transforms a matrix to a vector reading it row-by-row
    std::vector<T> flatten() const
    {
        size_t n_cols = matrix[0].size();
        std::vector<T> to_return(matrix.size() * n_cols);

        for (size_t idx = 0; idx < to_return.size(); idx++) {
            to_return[idx] = matrix[idx / n_cols][idx % n_cols];
        }

        return to_return;
    }

    void setEntry(size_t i, size_t j, T elem)
    {
        matrix[i][j] = elem;
    }

    T getEntry(size_t i, size_t j) const
    {
        return matrix[i][j];
    }
};

#endif
