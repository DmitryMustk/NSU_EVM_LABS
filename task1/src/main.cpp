#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

vector<int> fill_vec(const long long int N) {
    vector<int> vec;
    for (int i = 0; i < N; ++i) {
        vec.push_back(rand() % 1000);
    }
    return vec;
}

void print_vec(const vector<int>& vec) {
    long long int N = vec.size();
    for (long long int i = 0; i < N; ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

bool is_sorted(const vector<int>& vec) {
    long long int N = vec.size();
    for (long long int i = 0; i < N - 1; ++i) {
        if (vec[i] > vec[i + 1])
            return false;
    }
    return true;
}

void bubble_sort(vector<int>& vec) {
    bool swapped;
    long long int N = vec.size();
    for (long long int i = 0; i < N - 1; ++i) {
        swapped = false;
        for (int j = 0; j < N - i - 1; ++j) {
            if (vec[j] > vec[j + 1]) {
                swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            return;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Wrong number of args" << '\n';
        return 0;
    }
    long long int N = atoll(argv[1]);
    srand(0);
    vector<int> vec = fill_vec(N);
    //    print_vec(vec);
    struct timespec start {
    }, end{};
    //    cout << "Before sort: " << is_sorted(vec) << '\n';
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    bubble_sort(vec);
    //    cout << "After sort: " << is_sorted(vec) << "\n";
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    cout << "Time taken: " << end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec) << endl;
    //    print_vec(vec);
    return 0;
}

