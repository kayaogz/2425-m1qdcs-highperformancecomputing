/**
  * Copie d'un tableau dans un autre avec les intrinseques AVX.
  * A compiler avec les drapeaux -O2 -mavx2.
  */

#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>

#define NREPET 1024

void afficherUsage()
{ 
  printf("Usage: ./copier-vecteur-avx [taille-du-tableau]\n");
}

int main(int argc, char **argv)
{
  if (argc < 2) { 
    afficherUsage();
    return 1;
  }
  int dim = std::atoi(argv[1]);

  // Allouer et initialiser deux tableaux de flottants de taille dim alignes par 32 octets
  float* tab0;
  float* tab1;
  // A FAIRE ...
  
  // Copier tab0 dans tab1 de manière scalaire~(code non-vectorise).
  // On repete NREPET fois pour mieux mesurer le temps d'execution
  auto start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diffSeq = end-start;
  std::cout << std::scientific << "Copier sans AVX: " << diffSeq.count() / NREPET << "s" << std::endl;

  // Copier tab0 dans tab1 de maniere vectorisee avec AVX
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diffPar = end-start;
  std::cout << std::scientific << "Copier avec AVX: " << diffPar.count() / NREPET << "s" << std::endl;
  // Afficher l'acceleration et l'efficacite
  // A FAIRE ...

  // Copier tab0 dans tab1 de maniere vectorisee avec AVX et deroulement de facteur 4
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diffParDeroule = end-start;
  std::cout << std::scientific << "Copier avec AVX et deroulement: " << diffParDeroule.count() / NREPET << "s" << std::endl;
  // Afficher l'acceleration et l'efficacite
  // A FAIRE ...

  // Desallouer les tableaux tab0 et tab1
  // A FAIRE ...

  return 0;
}
