#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "immintrin.h"
#include <chrono>
#include "omp.h"

#define NREPET 1001

void printUsage(int argc, char **argv)
{
  printf("Usage: %s N\n", argv[0]);
  printf("Example: %s 1024\n", argv[0]);
}

inline void loadTile(__m256 tile[], float *addr, int N)
{
  tile[0] = _mm256_load_ps(addr);
  tile[1] = _mm256_load_ps(addr + N);
  tile[2] = _mm256_load_ps(addr + 2 * N);
  tile[3] = _mm256_load_ps(addr + 3 * N);
  tile[4] = _mm256_load_ps(addr + 4 * N);
  tile[5] = _mm256_load_ps(addr + 5 * N);
  tile[6] = _mm256_load_ps(addr + 6 * N);
  tile[7] = _mm256_load_ps(addr + 7 * N);
}

inline void storeTile(__m256 tile[], float *addr, int N)
{
  _mm256_store_ps(addr, tile[0]);
  _mm256_store_ps(addr + N, tile[1]);
  _mm256_store_ps(addr + 2 * N, tile[2]);
  _mm256_store_ps(addr + 3 * N, tile[3]);
  _mm256_store_ps(addr + 4 * N, tile[4]);
  _mm256_store_ps(addr + 5 * N, tile[5]);
  _mm256_store_ps(addr + 6 * N, tile[6]);
  _mm256_store_ps(addr + 7 * N, tile[7]);
}

void printMatrix(const float *mat, int N)
{
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%3.f ", mat[i * N + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void printTile(__m256 tile[8])
{
  float temp[64] __attribute__((aligned(32)));
  storeTile(tile, temp, 8);
  printMatrix(&temp[0], 8);
}

void verify(const float *mat, int N)
{
  int correct = 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (mat[i * N + j] != j - i) {
        printf("mat(%d, %d) = %.f is incorrect; mat(%d, %d) should be %.f\n", i, j, mat[i * N + j], i, j, (float)(j - i));
        correct = 0;
        break;
      }
    }
  }
  if (correct) {
    printf("The result is correct!\n\n");
  } else {
    printf("The result is not correct!\n\n");
  }
}


inline void transAVX8x8_ps(__m256 tile[8])
{
  __m256 tile2[8];
  __m256 tile3[8];

  // TODO / A FAIRE ...
  // Compute tile2 from tile using _mm256_unpacklo_ps and _mm256_unpackhi_ps
  // Calculer tile2 de tile en utilisant _mm256_unpacklo_ps et _mm256_unpackhi_ps
  
  // TODO / A FAIRE ...
  // Compute tile3 from tile2 using _mm256_shuffle_ps
  // Calculer tile3 de tile2 en utilisant _mm256_shuffle_ps

  // TODO / A FAIRE ...
  // Compute tile from tile3 using _mm256_permute2f128_ps
  // Calculer tile de tile3 en utilisant _mm256_permute2f128_ps
}

// Transposition of an 8x8 submatrix at address tA, and stores it in the 8x8 submatrix at address tB
// Transposition d'une sousmatrice 8x8 a l'adresse tA, stockee dans la sousmatrice 8x8 a l'adresse tB
inline void transAVX8x8(float *tA, float *tB, __m256 tile[8], int N)
{
  loadTile(tile, tA, N);
  transAVX8x8_ps(tile);
  storeTile(tile, tB, N);
}

// In-place transposition of two symmetric 8x8 submatrices at adresses tA and tA2
// Transposition en place des deux sousmatrices 8x8 aux adresses tA et tA2
inline void transAVX8x8InPlace(float *tA, float *tA2, __m256 tile[8], __m256 tile2[8], int N)
{
  // TODO / A FAIRE ...
}

int main(int argc, char **argv)
{
  // Get parameters
  // Recuperer les parametres
  if (argc != 2) {
    printUsage(argc, argv);
    return 0;
  }
  int N = std::atoi(argv[1]);
  __m256 tile[8], tile2[8];


  // Allocate and initialize the matrix A and B
  // Allouer et initialiser les matrices A et B
  float *A = (float *)_mm_malloc(N * N * sizeof(float), 32);
  float *B = (float *)_mm_malloc(N * N * sizeof(float), 32);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i * N + j] = i - j;
      B[i * N + j] = 0;
    }
  }


  // Transpose the matrix with a sequential and scalar (non-vectorized) code
  // Transposer la matrice avec un code sequentiel et scalaire (non-vectorise)
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repet = 0; repet < NREPET; repet++) {
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          B[i * N + j] = A[j * N + i];
        }
      }
    }
    std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Sequential transpose: " << time.count() / NREPET << "s\n";
    std::cout << "Performance: " << (long long) N * N * sizeof(float) / (1e9 * time.count() / NREPET) << "GB/s\n";
    verify(B, N);
  }


  // Transpose the matrix by 8x8 tiles using AVX transpose
  // Transposer la matrice en utilisant tuiles de taille 8x8 avec AVX
  {
    memset(B, 0, N * N * sizeof(float));
    auto start = std::chrono::high_resolution_clock::now();
    for (int repet = 0; repet < NREPET; repet++) {
      // TODO / A FAIRE ...

    }
    std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
    std::cout << "AVX transpose: " << time.count() / NREPET << "s\n";
    std::cout << "Performance: " << (long long) N * N * sizeof(float) / (1e9 * time.count() / NREPET) << "GB/s\n";
    verify(B, N);
  }


  // Transpose the matrix by 8x8 tiles using AVX transpose and in-place transpose
  // Transposer la matrice en utilisant tuiles de taille 8x8 avec AVX et transposition in-place
  {
    memcpy(B, A, N * N * sizeof(float));
    auto start = std::chrono::high_resolution_clock::now();
    for (int repet = 0; repet < NREPET; repet++) {
      // TODO / A FAIRE ...
    }
    std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
    std::cout << "AVX in-place transpose: " << time.count() << "s\n";
    std::cout << "Performance: " << (long long) N * N * sizeof(float) / (1e9 * time.count() / NREPET) << "GB/s\n";
    verify(A, N);
    memcpy(A, B, N * N * sizeof(float));
  }


  // Transpose the matrix by 8x8 tiles using AVX transpose and in-place transpose and OpenMP tasks
  // Transposer la matrice en utilisant tuiles de taille 8x8 avec AVX et transposition in-place, avec OpenMP tasks
  {
    memcpy(B, A, N * N * sizeof(float));
    auto start = std::chrono::high_resolution_clock::now();
    int B1 = 128; // Tile size for each task / Taille de tuile pour chaque tache
    for (int repet = 0; repet < NREPET; repet++) {
      // TODO / A FAIRE ...
    }
    std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - start;
    std::cout << "AVX in-place transpose with OpenMP tasks: " << time.count() << "s\n";
    std::cout << "Performance: " << (long long) N * N * sizeof(float) / (1e9 * time.count() / NREPET) << "GB/s\n";
    verify(A, N);
    memcpy(A, B, N * N * sizeof(float));
  }


  // Free matrices
  // Desallouer les matrices
  _mm_free(A);
  _mm_free(B);

  return 0;
}
