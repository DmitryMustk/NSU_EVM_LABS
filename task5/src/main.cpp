#include "matrix.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <emmintrin.h>
#include <immintrin.h>
#include <iostream>
#include <random>
#include <set>

// #pragma intrinsic(__rdtsc)

#define L1_BYTE_SIZE 32 * 1024
#define L3_BYTE_SIZE 4 * 1024 * 1024

#define MATRIX_SIZE 100
#define ARR_SIZE_MULT 1.2
#define ATTEMPTS_NUMBER 10

void direct_fill(int32_t* arr, const size_t size){
    arr[size - 1] = 0;
    for(size_t i = 0; i < size - 1; ++i){
        arr[i] = i + 1;
    }
}

void reverse_fill(int32_t* arr, const size_t size){
    arr[0] = size - 1;
    for(size_t i = 1; i < size; ++i){
        arr[i] = i - 1;
    }
}

void random_fill(int32_t* arr, const size_t size){
    for(size_t i = 0; i < size; ++i){
        arr[i] = i;
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr, arr + size, g);
}
void clock(uint64_t& time) {
  uint64_t s;
  asm("rdtsc\n" : "=a"(s));
  time = s;
  //  asm("rdtsc\n" : "=a"(time));
}

void assign(const int32_t* arr, const size_t size, uint64_t& start, uint64_t& end, const size_t k = 1){
    clock(start);
    size_t l = 0;
    for(size_t i  = 0; i < size * k; ++i){
        l = arr[l];
    }
    clock(end);
    if(l == 1488)
        std::cout << "0/";
}

void preheat_processor(){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    while(end.tv_sec - start.tv_sec < 1){
        matrix mx1(MATRIX_SIZE);
        matrix mx2(MATRIX_SIZE);
        mx1.fill_matrix();
        mx2.fill_matrix();
        mx1.mul_matrix(mx2);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    }
}

void preheat_cache(const std::string mode, const size_t size, const size_t k=1){
    auto arr = new int32_t[size];
    if(mode == "direct")
            direct_fill(arr, size);
    else if(mode == "reverse")
            reverse_fill(arr, size);
    else if(mode == "random")
            random_fill(arr, size);
    size_t l = 0;
    for(size_t i = 0; i < size * k; ++i){
        l = arr[l];
    }
    if(l == 1488)
        std::cout << "0/";
    delete[] arr;
}

void calculate_time(const std::string mode, const size_t N_min, const size_t N_max, const size_t k){
    std::ofstream output("../resourses/" + mode + ".txt");
    size_t N = N_min;
    while(N < N_max) {
        auto* arr = new int32_t[N];
        if(mode == "direct")
            direct_fill(arr, N);
        else if(mode == "reverse")
            reverse_fill(arr, N);
        else if(mode == "random")
            random_fill(arr, N);
        else
            throw std::runtime_error("ERROR! Rerun with -h to get help");
        uint64_t min_time = __LONG_MAX__;
        preheat_processor();
        preheat_cache(mode, N);
        
        for(size_t i = 0; i < ATTEMPTS_NUMBER; ++i){
            uint64_t start, end;
            assign(arr, N, start, end, k);
            min_time = std::min(end - start, min_time);
        }
        output << N << " " << min_time / N / k  << "\n";
        delete[] arr;
        N *= ARR_SIZE_MULT;
    }
}

int main(int argc, char** argv) {
    const size_t N_min = L1_BYTE_SIZE / sizeof(uint32_t) / 32;
    const size_t N_max  = L3_BYTE_SIZE / sizeof(uint32_t) * 8;
    if(argc < 2)
        throw std::runtime_error("Use ./task5 MODE");

    srand(time(0));

    calculate_time(std::string(argv[1]), N_min, N_max, 5);
    return 0;
}