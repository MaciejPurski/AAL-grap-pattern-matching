//
// Created by maciej on 12.01.18.
//

#ifndef AAL_PROJEKT_BITMATRIX_H
#define AAL_PROJEKT_BITMATRIX_H

#include "BitVector.h"
#include <vector>

class BitMatrix {
private:
    std::vector<BitVector> matrix;

public:
    BitMatrix(int rows, int columns) {
        for (int i = 0; i < rows; i++)
            matrix.emplace_back(BitVector(columns));
    }

    BitMatrix(const BitMatrix &arg) {
        for (int i = 0; i < arg.size(); i++) {
            matrix.emplace_back(arg[i]);
        }
    }

    BitVector &operator[](int index) {
        return matrix[index];
    }

    const BitVector &operator[](int index) const {
        return matrix[index];
    }

    int size() const {
        return matrix.size();
    }

    BitMatrix transpose() const {
        BitMatrix result(matrix[0].size(), size());

        for (int i = 0; i < result.size(); i++) {
            for (int j = 0; j < result[0].size(); j++) {
                if (matrix[j].test(i))
                    result[i].set(j);
            }
        }

        return result;
    }

    BitMatrix operator*(const BitMatrix &arg) const {
        BitMatrix result(size(), arg[0].size());

        for(int i = 0; i < result.size(); ++i)
            for (int j = 0; j < result[0].size(); ++j)
                for (int k = 0; k < matrix[0].size(); ++k)
                {
                    if (matrix[i].test(k) && arg.matrix[k].test(j)) {
                        result[i].set(j);
                        break;
                    }
                }


        return result;
    }

    bool operator==(const BitMatrix &arg) const {
        if (arg.size() != size() || arg.matrix[0].size() != matrix[0].size())
            return false;

        for (int i = 0; i < arg.size(); i++) {
            if (!arg[i].equals(matrix[i]))
                return false;
        }

        return true;
    }
};

#endif //AAL_PROJEKT_BITMATRIX_H
