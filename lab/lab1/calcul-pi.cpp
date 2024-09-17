#include <chrono>
#include <iostream>
#include "omp.h"

inline double f(double x)
{
  return (4 / (1 + x * x));
}

int main()
{
  int i;
  const int N = 100000000;
  double pi = 0.0;

  // Compute pi sequentially
  // Calculer le pi en sequentiel
  auto start = std::chrono::high_resolution_clock::now();
  // TODO / A FAIRE ...
  std::cout << "pi = " << pi << std::endl;
  std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps sequentiel: " << tempsSeq.count() << "s\n";

  // Compute pi with omp for and reduction
  // Calculer le pi avec omp for et reduction
  start = std::chrono::high_resolution_clock::now();
  // TODO / A FAIRE ...
  std::cout << "pi = " << pi << std::endl;
  std::chrono::duration<double> tempsOmpFor = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps parallel omp for: " << tempsOmpFor.count() << "s\n";

  // Compute pi with a loop parallalized "by hand"
  // Calculer le pi avec boucle parallele faite a la main
  pi = 0.0;
  start = std::chrono::high_resolution_clock::now();
  // TODO / A FAIRE ...
  std::cout << "pi = " << pi << std::endl;
  std::chrono::duration<double> tempsForMain = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps parallel for a la main: " << tempsForMain.count() << "s\n";

  return 0;
}
