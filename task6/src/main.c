#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define L3_CACHE_BYTE_SIZE 12 * 1024 * 1024

uint64_t rdtsc() {
	uint64_t lo, hi;
	__asm__ __volatile__("rdtsc" : "=a" (lo), "=d" (hi));
	return ((uint64_t)hi << 32) | lo;
}

void calc_time(int fragments_number){ 
	int ofset = L3_CACHE_BYTE_SIZE; 
	int size = ofset * fragments_number;
	int fragment_size = ofset / fragments_number;

	int* arr = (int*)malloc(size * sizeof(int));
	for (int n = 0; n < fragments_number - 1; n++){
		for (int i = 0; i < fragment_size; i++)
			arr[ofset * n + i] = ofset * (n + 1) + i;
	}

	for (int i = 0; i < fragment_size; i++)
		arr[(fragments_number - 1) * ofset + i] = (i + 1) % fragment_size;

	int k = 0;
	for (int i = 0; i < size; i++)
		k = arr[k];
	k = 0;

	int min = INT_MAX;
	for (int i = 0; i < 500; i++){
		uint64_t start = rdtsc();
		for (int j = 0; j < 10000; j++)
			k = arr[k];
		uint64_t end = rdtsc();

		if (end - start < min)
			min = end - start;
	}
	if (k == 1234567)
		printf("!!WOW!!");
	double avg = (double)(min / 10000);
	printf("%d %lf\n", fragments_number, avg);
	free(arr);
}

int main(void){
	for (int fragments_number = 1; fragments_number <= 32; fragments_number++)
		calc_time(fragments_number);
	return 0;
}