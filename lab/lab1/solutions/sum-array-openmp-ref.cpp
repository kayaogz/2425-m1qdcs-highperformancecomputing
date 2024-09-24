#include <chrono>
#include <iostream>
#include <vector>
#include "omp.h"

int main()
{
  int i;
  int N = 10000000;
  std::vector<double> A(N);
  double sum = 0.0;

  auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel default(none) shared(A, sum, i, N)
  {
#pragma omp for
    for (i = 0; i < N; i++) {
      A[i] = (double)i;
    } // Implicit barrier / Barrier implicite
    
    // Sum with omp for / Somme avec omp for
//#pragma omp for reduction(+:sum)
//    for (i = 0; i < N; i++) {
//      sum += A[i];
//    }

    // Sum with four sections / Somme avec quatre sections
#pragma omp sections
    {
#pragma omp section
      {
        double localSum = 0;
        for (int i = 0; i < N / 4; i++) { localSum += A[i]; }
        // We can put critical in all sections as well but atomic is faster for a simple add
        // On peut mettre critical dans toutes les sections également, mais atomic est plus rapide pour une simple add
//#pragma omp critical
#pragma omp atomic      
        sum += localSum;
      }
#pragma omp section
      {
        double localSum = 0;
        for (int i = N / 4; i < (2 * N) / 4; i++) { localSum += A[i]; }
//#pragma omp critical
#pragma omp atomic      
        sum += localSum;
      }
#pragma omp section
      {
        double localSum = 0;
        for (int i = (2 * N) / 4; i < (3 * N) / 4; i++) { localSum += A[i]; }
//#pragma omp critical
#pragma omp atomic
        sum += localSum;
      }
#pragma omp section
      {
        double localSum = 0;
        for (int i = (3 * N) / 4; i < N; i++) { localSum += A[i]; }
//#pragma omp critical
#pragma omp atomic
        sum += localSum;
      }
    }
  }
  std::cout << "Sum is " << sum << std::endl;
  std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Execution time: " << time.count() << "s\n";

  return 0;
}
