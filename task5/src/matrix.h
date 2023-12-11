#ifndef TASK5_MATRIX_H
#define TASK5_MATRIX_H
#include <cstdlib>
class matrix {
public:
    matrix(size_t size);
    ~matrix();
    void fill_matrix();
    void mul_matrix(const matrix& other) const;
private:
    float* data_;
    size_t size_;
};

#endif //TASK5_MATRIX_H