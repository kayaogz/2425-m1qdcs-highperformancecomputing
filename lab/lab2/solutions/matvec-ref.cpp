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

  // Calculer b = A * x NREPET fois en parallele en sequentiel
  auto start = std::chrono::high_resolution_clock::now();
  // Calculer b = A * x NREPET fois en parallele
  for (int repet = 0; repet < NREPET; repet++) {
    for (int i = 0; i < dim; i++) {
      b[i] = 0;
      for (int j = 0; j < dim; j++) {
        b[i] += A[i * dim + j] * x[j];
      }
    }
  }
  std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution sequentiel: " << tempsSeq.count() / NREPET << "s" << std::endl;

  // Calculer b = A * x NREPET fois en parallele
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
#pragma omp parallel for if (dim >= 64)
    for (int i = 0; i < dim; i++) {
      b[i] = 0;
      for (int j = 0; j < dim; j++) {
        b[i] += A[i * dim + j] * x[j];
      }
    }
  }
  std::chrono::duration<double> tempsPar = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution parallele avec omp for: " << tempsPar.count() / NREPET << "s" <<
    std::endl;

  // Calculer b = A * x NREPET fois en parallele avec omp task
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
#pragma omp parallel
    {
      // Les taches sont generees par un thread dans une region parallele mais executees par tous les threads
#pragma omp single
      for (int i = 0; i < dim; i++) {
#pragma omp task firstprivate(i) // Pour produit scalaire b[i] on genere une tache.
        { 
          double res = 0.0; // Il faut travailler sur une variable temporaire pour eviter le faux partage
          for (int j = 0; j < dim; j++) {
            res += A[i * dim + j] * x[j];
          }
          b[i] = res; // Une seule ecriture qui evite le faux partage
        }
      }
    }
  }  
  std::chrono::duration<double> tempsParTasks = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::scientific << "Temps d'execution parallele avec omp tasks: " << tempsParTasks.count() / NREPET <<
    "s" << std::endl;


  // Calculer et afficher l'acceleration et l'efficacite de la parallelisation avec omp for
  int numThreads;
#pragma omp parallel
#pragma omp single
    numThreads = omp_get_num_threads(); // Attention, dans un region sequentiel omp_get_num_threads() rend 1
  std::cout << "Acceleration: " << tempsSeq.count() / tempsPar.count() << std::endl;
  std::cout << "Efficacite: " << tempsSeq.count() / (numThreads * tempsPar.count()) << std::endl;

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
