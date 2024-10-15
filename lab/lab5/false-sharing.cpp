#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>
#include "omp.h"

#define NREPEAT 128
#define NTHREADMAX 8
#define STRIDE 1

int main()
{
  int N = 10000000;
  float sum[NTHREADMAX * STRIDE] __attribute__((aligned(64))) = {0};
  std::vector<float> vec(N);
  vec[0] = 0;
  for (int i = 1; i < N; i++) { 
    vec[i] = 1;
  }

  auto start = std::chrono::high_resolution_clock::now();
  for (int repeat = 0; repeat < NREPEAT; repeat++) {
    for (int i = 0; i < NTHREADMAX; i++) { sum[i * STRIDE] = 0.0; }
#pragma omp parallel
    {
      int thid = omp_get_thread_num();
#pragma omp for
      for (int i = 0; i < N; i++) {
        sum[thid * STRIDE] += vec[i];
      }
    }
  }
  std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Time: " << time.count() << "s\n";

  float sumFinal = 0.0;
  for (int i = 0; i < NTHREADMAX; i++) { sumFinal += sum[i]; }
  printf("sum = %f", sumFinal);

  return 0;
}
