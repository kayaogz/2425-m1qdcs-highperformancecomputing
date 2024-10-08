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
  printf("Usage: ./inversion-tableau-avx [taille-du-tableau]\n");
}

int main(int argc, char **argv)
{
  if (argc < 2) { 
    afficherUsage();
    return 1;
  }
  int dim = std::atoi(argv[1]);
  
  // Allouer et initialiser un tableau de taille dim aligne par 32 octets
  float *tab = (float *)_mm_malloc(dim * sizeof(float), 32);
  for (int i = 0; i < dim; i++) { tab[i] = i; }
  
  // Inverser le tableau en place~(c'est a dire sans utiliser un deuxieme tableau auxiliaire)
  // On repete NREPET fois pour mieux mesurer le temps d'execution
  auto start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    for (int i = 0; i < dim / 2; i++) { 
      float temp = tab[i];
      tab[i] = tab[dim - i];
      tab[dim - i] = temp;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diffSeq = end-start;
  std::cout << std::scientific << "Inversion sans AVX: " << diffSeq.count() / NREPET << "s" << std::endl;

  // Inverser le tableau en place avec AVX~(c'est a dire sans utiliser un deuxieme tableau auxiliaire)
  // On repete NREPET fois pour mieux mesurer le temps d'execution
  start = std::chrono::high_resolution_clock::now();
  for (int repet = 0; repet < NREPET; repet++) {
    __m256i permIdx = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    int i = 0; // L'indice qui parcourt le cote gauche
    int j = dim - 8; // L'indice qui parcourt le cote droit
    for (; i < j; i += 8, j -= 8) {
      __m256 gauche = _mm256_permutevar8x32_ps(_mm256_load_ps(tab + i), permIdx);
      __m256 droite = _mm256_permutevar8x32_ps(_mm256_load_ps(tab + j), permIdx);
      _mm256_store_ps(tab + j, gauche);
      _mm256_store_ps(tab + i, droite);
    }
    if (i == j) { // Si la taille du tableau n'est pas un multiple de 16, renverser le milieu en place
      _mm256_store_ps(tab + i, _mm256_permutevar8x32_ps(_mm256_load_ps(tab + i), permIdx));
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diffPar = end-start;
  std::cout << std::scientific << "Inversion avec AVX: " << diffPar.count() / NREPET << "s" << std::endl;
  // Afficher l'acceleration et l'efficacite
  double acceleration = diffSeq.count() / diffPar.count();
  double efficacite = acceleration / 8;
  std::cout << std::fixed << std::setprecision(2) << "Acceleration: " << acceleration << std::endl;
  std::cout << "Efficacite: " << 100 * efficacite << "%" << std::endl;

  // Desallouer le tableau tab
  _mm_free(tab);

  return 0;
}
