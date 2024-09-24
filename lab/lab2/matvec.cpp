#include <iostream>
#include <chrono>
#include <vector>
#include "omp.h"

#define NREPET 1024

int main(int argc, char **argv)
{
  std::cout << "Produit matrice-vecteur avec OpenMP\n";
  if (argc < 2) {
    std::cout << "Utilisation: " << argv[0] << " [num-lignes/colonnes]\n";
    std::cout << "  Example: " << argv[0] << " 1024\n";
    return 1;
  }
  int dim = std::atoi(argv[1]); 

  std::vector<double> A(dim * dim);
  std::vector<double> x(dim);
  std::vector<double> b(dim);

  // Initialiser A et x tel que A(i, j) = i + j et b(j) = 1.
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      A[i * dim + j] = i + j;
    }
    x[i] = 1;
  }

  // Calculer b = A * x NREPET fois en sequentiel
  auto start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution sequentiel: " << tempsSeq.count() / NREPET << "s" << std::endl;

  // Calculer b = A * x NREPET fois en parallele avec omp for
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  std::chrono::duration<double> tempsPar = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution parallele avec omp for: " << tempsPar.count() / NREPET << "s" <<
    std::endl;

  // Calculer b = A * x NREPET fois en parallele avec omp task
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  std::chrono::duration<double> tempsParTasks = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution parallele avec omp tasks: " << tempsParTasks.count() / NREPET <<
    "s" << std::endl;

  // Calculer et afficher l'acceleration et l'efficacite de la parallelisation avec omp for
  double acceleration = 0.0;
  double efficacite = 0.0;
  // A FAIRE ...
  std::cout << "Acceleration: " << acceleration << std::endl;
  std::cout << "Efficacite: " << acceleration << std::endl;

  // Verifier le resultat. b(i) est cense etre (dim - 1) * dim / 2 + i * dim
  for (int i = 0; i < dim; i++) {
    double val = (dim - 1) * (double)dim / 2.0 + (double)i * dim;
    if (b[i] != val) {
      std::cout << "valeur incorrecte: b[" << i << "] = " << b[i] << " != " << val << std::endl;
      break;
    }
    if (i == dim - 1) {
      std::cout << "Produit matrice-vecteur est effectue avec succes!\n";
    }
  }

  return 0;
}
