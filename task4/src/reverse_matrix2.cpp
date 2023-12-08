#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>
#include <time.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <unistd.h>


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
    for(size_t i = 0; i < mx->size; ++i){
        for(size_t j = 0; j < mx->size; ++j){
            mx->data[i * mx->size + j] = rand() % 3;
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
    memset(res->data, 0, size * size * sizeof(float));
    for (size_t i = 0; i < size; ++i) {
        res->data[i * size + i] = 1;
    }
    return res;
}

matrix* matrix_transposition(const matrix* mx) {
    matrix* res = create_matrix(mx->size);

    for (size_t i = 0; i < mx->size; ++i) {
        for (size_t j = 0; j < mx->size; j += 4) {
            __m128 data = _mm_loadu_ps(&mx->data[j * mx->size + i]);
            _mm_storeu_ps(&res->data[i * mx->size + j], data);
        }
    }

    return res;
}

matrix* sub_matrix(const matrix* mx1, const matrix* mx2) {
    if (mx1->size != mx2->size)
        throw std::runtime_error("Different matrix sizes in sub_matrix func\n");
    matrix* res = create_matrix(mx1->size);

    for (size_t i = 0; i < mx1->size; ++i) {
        for (size_t j = 0; j < mx1->size; j += 4) {
            __m128 a = _mm_loadu_ps(&mx1->data[i * mx1->size + j]);
            __m128 b = _mm_loadu_ps(&mx2->data[i * mx1->size + j]);
            __m128 result = _mm_sub_ps(a, b);
            _mm_storeu_ps(&res->data[i * mx1->size + j], result);
        }
    }

    return res;
}

matrix* sum_matrix(const matrix* mx1, const matrix* mx2) {
    if (mx1->size != mx2->size)
        throw std::runtime_error("Different matrix sizes in sum_matrix func\n");
    matrix* res = create_matrix(mx1->size);

    for (size_t i = 0; i < mx1->size; ++i) {
        for (size_t j = 0; j < mx1->size; j += 4) {
            __m128 a = _mm_loadu_ps(&mx1->data[i * mx1->size + j]);
            __m128 b = _mm_loadu_ps(&mx2->data[i * mx1->size + j]);
            __m128 result = _mm_add_ps(a, b);
            _mm_storeu_ps(&res->data[i * mx1->size + j], result);
        }
    }

    return res;
}

matrix* transpose(const matrix* mx) {
    auto *m_a = (__m128 *) mx->data;
    auto tmp_m = m_a;
    matrix* res = create_matrix(mx->size);
    for (size_t i = 0; i < mx->size; ++i) {
        for (int j = 0; j < mx->size / 4; ++j) {
            __m128 tmp = _mm_load_ps((float *) tmp_m);
            float four[4];
            _mm_store_ps(four, tmp);
            res->data[(j * 4 + 0) * mx->size + i] = four[0];
            res->data[(j * 4 + 1) * mx->size + i] = four[1];
            res->data[(j * 4 + 2) * mx->size + i] = four[2];
            res->data[(j * 4 + 3) * mx->size + i] = four[3];
            tmp_m++;
        }
    }
    return res;
}

matrix* mul_matrix(const matrix* mx1, const matrix* mx2) {
    matrix* mx2_tr = transpose(mx2);
    matrix* res = create_matrix(mx1->size);
    for (size_t i = 0; i < mx1->size; ++i) {
        for (int j = 0; j < mx1->size; ++j) {
            __m128 m_sum = _mm_setzero_ps(); 
            for (int r = 0; r < mx1->size / 4; r++) {
                __m128 temp1 = _mm_load_ps(&mx1->data[i * mx1->size + r * 4]); 
                __m128 temp2 = _mm_load_ps(&mx2_tr->data[j * mx1->size + r * 4]);
                m_sum = _mm_add_ps(m_sum, _mm_mul_ps(temp1, temp2));
            }
            float four[4];
            _mm_store_ps(four, m_sum); 
            float sum = four[0] + four[1] + four[2] + four[3];
            res->data[i * mx1->size + j] = sum;
        }
    }
    destruct_matrix(mx2_tr);
    return res;
}

void scalar_mul_matrix(matrix* mx, const float scalar) {
    for (size_t i = 0; i < mx->size; ++i) {
        for (size_t j = 0; j < mx->size; j += 4) {
            __m128 data = _mm_loadu_ps(&mx->data[i * mx->size + j]);
            __m128 result = _mm_mul_ps(data, _mm_set1_ps(scalar));
            _mm_storeu_ps(&mx->data[i * mx->size + j], result);
        }
    }
}

unsigned int max_col(const matrix* mx) {
    unsigned int max = 0;
    for (size_t j = 0; j < mx->size; ++j) {
        unsigned int cur = 0;
        for (size_t i = 0; i < mx->size; ++i) {
            cur += abs(mx->data[i + j * mx->size]);
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
            cur += abs(mx->data[i * mx->size + j]);
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
    if(N % 4 != 0) {
        throw std::runtime_error("N should be % 4 == 0");
    }
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
