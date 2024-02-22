#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void fill_arr(int *arr, size_t N) {
  for (size_t i = 0; i < N; ++i) {
    arr[i] = (N - i);
  }
}

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void bubble_sort(int *arr, size_t N) {
  for (size_t i = 0; i < N - 1; ++i) {
    int swapped = 0;
    for (size_t j = 0; j < N - i - 1; ++j) {
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
        swapped = 1;
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Wrong number of arguments\n");
    return -1;
  }
  size_t N = atoll(argv[1]);
  int arr[N];
  fill_arr(arr, N);
  bubble_sort(arr, N);
  return 0;
}
