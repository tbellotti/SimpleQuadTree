#ifndef MATRIX_H
#define MATRIX_H

#include <vector>


template <typename T>
class Matrix
{
protected:
    std::vector<std::vector<T>> matrix;

public:
    Matrix(size_t i_size, size_t j_size) : matrix(i_size, std::vector<T>(j_size)) {}
    Matrix(size_t n_cols, std::vector<T> & line) : Matrix(line.size() / n_cols, n_cols) 
    {
        for (size_t idx_line = 0; idx_line < line.size(); idx_line++)   {
            matrix[idx_line / n_cols][idx_line % n_cols] = line[idx_line];
        }
    }

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

    std::vector<T> & operator[](size_t i) const
    {
        return matrix[i];
    }

};


#endif

