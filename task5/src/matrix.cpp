#include "matrix.h"

#include <stdexcept>

matrix::matrix(size_t size){
    data_ = new float[size * size];
    size_ = size;
}

matrix::~matrix(){
    delete[] data_;
}

void matrix::fill_matrix(){
    for(size_t i = 0; i < size_ * size_; ++i){
        data_[i] = rand() % 1000;
    }
}

void matrix::mul_matrix(const matrix& other) const {
    matrix res(size_);
    for(size_t i = 0; i < size_; ++i){
        for(size_t j = 0; j < size_; ++j){
            for(size_t k = 0; k < size_; ++k){
                res.data_[i * size_ + k] += data_[i * size_ + j] * other.data_[j * size_ + k];
            }
        }
    }
}