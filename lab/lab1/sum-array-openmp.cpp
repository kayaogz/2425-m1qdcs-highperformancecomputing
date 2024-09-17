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

  // TODO / A FAIRE ...

  std::cout << "Sum is " << sum << std::endl;
  std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Execution time: " << time.count() << "s\n";

  return 0;
}
