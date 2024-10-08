#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "immintrin.h"
#include <chrono>
#include "omp.h"

#define NREPEAT 10

void printUsage(int argc, char **argv)
{
  printf("Usage: %s N\n", argv[0]);
  printf("Example: %s 1024\n", argv[0]);
}

void verify(const float *A, const float *B, const float *C, int N)
{
  int correct = 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (C[i * N + j] != N) {
        printf("C(%d, %d) = %f is incorrect; C(%d, %d) should be %f\n", i, j, C[i * N + j], i, j, N);
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

inline void loadTile(__m256 tile[8], float *addr, int N)
{
  // Load the 8x8 subtile from memory whose upper left element is addr[0] in an NxN matrix
  // TODO / A FAIRE ...
}

inline void storeTile(__m256 tile[8], float *addr, int N)
{
  // Store the 8x8 subtile into memory whose upper left element is addr[0], in an NxN matrix
  // TODO / A FAIRE ...
}

inline void multiplyTile(float *tA, float *tB, float *tC, __m256 atile[8], __m256 btile[8], __m256 ctile[8], int N)
{
  loadTile(btile, tB, N);
  loadTile(ctile, tC, N);
  for (int i = 0; i < 8; i++) {
    // Perform the multiplication Ctile(i, :) += Atile(i, k) * Btile(k, :) for all values k = 0...7 in an unrolled way
    // Effectuer la multiplication Ctile(i, :) += Atile(i, k) * Btile(k, :) pour toutes les valeurs de k = 0...7 de
    // manière déroulé sur les valeurs de k
    // TODO / A FAIRE ...
  }
  storeTile(ctile, tC, N);
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    printUsage(argc, argv);
    return 0;
  }
  int N = std::atoi(argv[1]);
  const int B1 = 32;
  const int B2 = 256;

  // Allocate and initialize the matrix A and vectors x, b
  // Allouer et initialiser la matrice A et matrices x, b
  float *A = (float *)_mm_malloc(N * N * sizeof(float), 32);
  float *B = (float *)_mm_malloc(N * N * sizeof(float), 32);
  float *C = (float *)_mm_malloc(N * N * sizeof(float), 32);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i * N + j] = 1.0f;
      B[i * N + j] = 1.0f;
      C[i * N + j] = 0.0f;
    }
  }

  // Sequential and scalar matrix-matrix multiplication code with loop order i->j->k
  // Code sequentiel et scalaire produit matrice-matrice avec l'ordre de boucles i->j->k
  {
    auto start = std::chrono::high_resolution_clock::now();
    // TODO / A FAIRE ...

    std::chrono::duration<double> timeDiff = std::chrono::high_resolution_clock::now() - start;
    std::cout << std::scientific << "Sequential scalar matmat i->j->k took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }

  // Sequential and scalar matrix-matrix multiplication code with loop order i->k->j
  // Code sequentiel et scalaire produit matrice-matrice avec l'ordre de boucles i->k->j
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < NREPEAT; repeat++) {
      memset(&C[0], 0, N * N * sizeof(float));
      // TODO / A FAIRE ...

    }
    std::chrono::duration<double> timeDiff = (std::chrono::high_resolution_clock::now() - start) / NREPEAT;
    std::cout << std::scientific << "Sequential scalar matmat i->k->j took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }

  // Sequential and scalar matrix-matrix multiplication code with loop order i->k->j and single level tiling
  // Code sequentiel et scalaire produit matrice-matrice avec l'ordre de boucles i->k->j et tuilage d'un niveau
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < NREPEAT; repeat++) {
      memset(&C[0], 0, N * N * sizeof(float));
      // TODO / A FAIRE ...
      for (int i = 0; i < N; i += B1) {
        for (int k = 0; k < N; k += B1) {
          for (int j = 0; j < N; j += B1) {
            float *tA = &A[i * N + k];
            float *tB = &B[k * N + j];
            float *tC = &C[i * N + j];
            for (int i2 = 0; i2 < B1; i2++) {
              for (int k2 = 0; k2 < B1; k2++) {
                for (int j2 = 0; j2 < B1; j2++) {
                  tC[i2 * N + j2] += tA[i2 * N + k2] * tB[k2 * N + j2];
                }
              }
            }
          }
        }
      }
    }
    std::chrono::duration<double> timeDiff = (std::chrono::high_resolution_clock::now() - start) / NREPEAT;
    std::cout << std::scientific << "Single tile scalar matmat i->k->j took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }


  // Sequential and scalar matrix-matrix multiplication code with loop order i->k->j and two level tiling
  // Code sequentiel et scalaire produit matrice-matrice avec l'ordre de boucles i->k->j et tuilage de deux niveaux
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < NREPEAT; repeat++) {
      memset(&C[0], 0, N * N * sizeof(float));
      // TODO / A FAIRE ...
    }
    std::chrono::duration<double> timeDiff = (std::chrono::high_resolution_clock::now() - start) / NREPEAT;
    std::cout << std::scientific << "Double tile scalar matmat i->k->j took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }


  // Vectorized matrix-matrix multiplication code with loop order i->k->j and two level tiling + AVX
  // Produit matrice-matrice vectorise avec l'ordre de boucles i->k->j et tuilage de deux niveaux + AVX
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < NREPEAT; repeat++) {
      memset(&C[0], 0, N * N * sizeof(float));
      __m256 atile[8], btile[8], ctile[8];
      // TODO / A FAIRE ...
    }
    std::chrono::duration<double> timeDiff = (std::chrono::high_resolution_clock::now() - start) / NREPEAT;
    std::cout << std::scientific << "Double tile AVX matmat i->k->j took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }


  // Task-parallel and vectorized matrix-matrix multiplication code with loop order i->k->j and two level tiling + AVX
  // Produit matrice-matrice vectorise et parallelise par taches avec l'ordre de boucles i->k->j et tuilage de deux
  // niveaux+AVX
  {
    auto start = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < NREPEAT; repeat++) {
      memset(&C[0], 0, N * N * sizeof(float));
      // TODO / A FAIRE ...
    }
    std::chrono::duration<double> timeDiff = (std::chrono::high_resolution_clock::now() - start) / NREPEAT;
    std::cout << std::scientific << "Task parallel double tile AVX matmat i->k->j took " << timeDiff.count() << "s." << std::endl;
    std::cout << std::fixed << std::setprecision(2) << "Performance: " << 2.0*N*N*(N-1) / ((1e6) * timeDiff.count()) <<
      "Mflops/s" << std::endl;
    verify(A, B, C, N);
  }

  // Free matrices
  // Desallouer les matrices
  _mm_free(A);
  _mm_free(B);
  _mm_free(C);

  return 0;
}
