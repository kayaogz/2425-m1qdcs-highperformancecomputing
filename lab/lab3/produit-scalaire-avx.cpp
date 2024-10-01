/**
  * Produit scalaire de deux tableaux avec les intrinseques AVX et FMA.
  * A compiler avec les drapeaux -O2 -mavx2 -mfma
  */

#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>

#define NREPET 1024

int main(int argc, char **argv)
{
  if (argc < 2) {
    std::cout << "Utilisation: \n  " << argv[0] << " [taille-de-tableau]\n";
    return 1;
  }
  int dim = std::atoi(argv[1]);
  if (dim % 8) {
    std::cout << "La taille de tableau doit etre un multiple de 8.\n";
    return 1;
  }

  // Allouer les tableaux tab0 et tab1 de flottants de taille dim alignes par 32 octets, puis initialiser tab0[i]=i
  float* tab0;
  float* tab1;
  // A FAIRE ...

  // Faire le produit scalaire non-vectorise. On repete le calcul NREPET fois pour mieux mesurer le temps d'execution
  auto start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> tempsSeq = end-start;
  std::cout << std::scientific << "Produit scalaire sans AVX: " << tempsSeq.count() / NREPET << "s" << std::endl;

  // Faire le produit scalaire vectorise AVX. On repete le calcul NREPET fois pour mieux mesurer le temps d'execution
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> tempsAVX = end-start;
  std::cout << std::scientific << "Produit scalaire avec AVX: " << tempsAVX.count() / NREPET << "s" << std::endl;

  // Produit scalaire vectorise AVX FMA. On repete le calcul NREPET fois pour mieux mesurer le temps d'execution
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> tempsParAVXFMA = end-start;
  std::cout << std::scientific << "Produit scalaire avec AVX FMA: " << tempsParAVXFMA.count() / NREPET << "s" <<
    std::endl;

  // Produit scalaire vectorise AVX FMA et deroulement. On repete le calcul NREPET fois pour mieux mesurer le temps
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    // A FAIRE ...
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> tempsParAVXFMADeroule = end-start;
  std::cout << std::scientific << "Produit scalaire avec AVX FMA deroulement: " << tempsParAVXFMADeroule.count() /
    NREPET << "s" << std::endl;

  // Desallouer les tableaux tab0 et tab1
  // A FAIRE ...

  return 0;
}
