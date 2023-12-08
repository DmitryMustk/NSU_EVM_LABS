#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cblas.h>

struct matrix {
    float* data;
    size_t size;
};

matrix* create_matrix(size_t size) {
    matrix* mx = new matrix;
    mx->data = new float[size * size];
    mx->size = size;
    return mx;
}

void fill_matrix(matrix* mx) {
    for (size_t i = 0; i < mx->size; ++i) {
        for (size_t j = 0; j < mx->size; ++j) {
            mx->data[i * mx->size + j] = j;
        }
    }
}

void destruct_matrix(matrix* mx) {
    delete[] mx->data;
    delete mx;
}

void print_matrix(const matrix* mx) {
    std::cout << "\n";
    for (size_t i = 0; i < mx->size; ++i) {
        for (size_t j = 0; j < mx->size; ++j) {
            std::cout << mx->data[i * mx->size + j] << " ";
        }
        std::cout << "\n";
    }
}

matrix* identity_matrix(size_t size) {
    matrix* res = create_matrix(size);
    std::fill(res->data, res->data + size * size, 0.0f);
    for (size_t i = 0; i < size; ++i) {
        res->data[i * size + i] = 1.0f;
    }
    return res;
}

matrix* matrix_transposition(const matrix* mx) {
    matrix* res = create_matrix(mx->size);

    for (size_t i = 0; i < mx->size; ++i) {
        for (size_t j = 0; j < mx->size; ++j) {
            res->data[i * mx->size + j] = mx->data[j * mx->size + i];
        }
    }
    return res;
}

matrix* sub_matrix(const matrix* mx1, const matrix* mx2) {
    if (mx1->size != mx2->size)
        throw std::runtime_error("Different matrix sizes in sub_matrix func\n");
    matrix* res = create_matrix(mx1->size);
    cblas_scopy(mx1->size * mx1->size, mx1->data, 1, res->data, 1);
    cblas_saxpy(mx1->size * mx1->size, -1.0f, mx2->data, 1, res->data, 1); //ax + y
    return res;
}

matrix* sum_matrix(const matrix* mx1, const matrix* mx2) {
    if (mx1->size != mx2->size)
        throw std::runtime_error("Different matrix sizes in sum_matrix func\n");
    matrix* res = create_matrix(mx1->size);
    cblas_scopy(mx1->size * mx1->size, mx1->data, 1, res->data, 1);
    cblas_saxpy(mx1->size * mx1->size, 1.0f, mx2->data, 1, res->data, 1);
    return res;
}


matrix* mul_matrix(const matrix* mx1, const matrix* mx2) {
    if (mx1->size != mx2->size)
        throw std::runtime_error("Different matrix sizes in mul_matrix func\n");
    matrix* res = create_matrix(mx1->size);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, mx1->size, mx1->size, mx1->size, 1.0f, mx1->data, mx1->size,
                mx2->data, mx2->size, 0.0f, res->data, mx1->size);
    return res;
}

void scalar_mul_matrix(matrix* mx, const float scalar) {
    cblas_sscal(mx->size * mx->size, scalar, mx->data, 1);
}

unsigned int max_col(const matrix* mx) {
    unsigned int max = 0;
    for (size_t j = 0; j < mx->size; ++j) {
        unsigned int cur = 0;
        for (size_t i = 0; i < mx->size; ++i) {
            cur += std::abs(mx->data[i + j * mx->size]);
        }
        if (cur > max) {
            max = cur;
        }
    }
    return max;
}

unsigned int max_row(const matrix* mx) {
    unsigned int max = 0;
    for (size_t i = 0; i < mx->size; ++i) {
        unsigned int cur = 0;
        for (size_t j = 0; j < mx->size; ++j) {
            cur += std::abs(mx->data[i * mx->size + j]);
        }
        if (cur > max) {
            max = cur;
        }
    }
    return max;
}

matrix* inverse_matrix(const matrix* mx, size_t iterations) {
    matrix* res = identity_matrix(mx->size);
    matrix* mx_b = matrix_transposition(mx);
    scalar_mul_matrix(mx_b, 1.0f / (max_col(mx) * max_row(mx)));
    matrix* mx_r = sub_matrix(res, mul_matrix(mx_b, mx));
    matrix* mx_r_pow = sub_matrix(res, mul_matrix(mx_b, mx));

    for (size_t i = 0; i < iterations; ++i) {
        res = sum_matrix(res, mx_r_pow);
        mx_r_pow = mul_matrix(mx_r_pow, mx_r);
    }
    res = mul_matrix(res, mx_b);

    destruct_matrix(mx_r_pow);
    destruct_matrix(mx_r);
    destruct_matrix(mx_b);

    return res;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        throw std::runtime_error("Wrong number of args.\nUsage: $./task4 N M\n");
    }

    srand(time(0));
    size_t N = atoll(argv[1]);
    size_t M = atoll(argv[2]);

    matrix* mx = create_matrix(N);
    fill_matrix(mx);

    //print_matrix(mx);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    matrix* res = inverse_matrix(mx, M);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    std::cout << "Time taken: " << end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec) << " secs."
              << std::endl;
    //print_matrix(res);

    destruct_matrix(res);
    destruct_matrix(mx);

    return 0;
}
