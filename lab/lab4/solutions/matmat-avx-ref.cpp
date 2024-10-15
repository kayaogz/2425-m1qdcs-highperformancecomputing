#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "immintrin.h"
#include <chrono>
#include "omp.h"

#define NREPEAT 1

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

inline void loadTile(__m256 tile[], float *addr, int N)
{
  // Load the 8x8 subtile from memory whose upper left element is addr[0] in an NxN matrix
  // TODO / A FAIRE ...
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
  // Store the 8x8 subtile into memory whose upper left element is addr[0], in an NxN matrix
  // TODO / A FAIRE ...
  _mm256_store_ps(addr, tile[0]);
  _mm256_store_ps(addr + N, tile[1]);
  _mm256_store_ps(addr + 2 * N, tile[2]);
  _mm256_store_ps(addr + 3 * N, tile[3]);
  _mm256_store_ps(addr + 4 * N, tile[4]);
  _mm256_store_ps(addr + 5 * N, tile[5]);
  _mm256_store_ps(addr + 6 * N, tile[6]);
  _mm256_store_ps(addr + 7 * N, tile[7]);
}

inline void multiplyTile(float *tA, float *tB, float *tC, __m256 atile[], __m256 btile[], __m256 ctile[], int N)
{
  loadTile(btile, tB, N);
  loadTile(ctile, tC, N);
  for (int i = 0; i < 8; i++) {
    // Perform the multiplication Ctile(i, :) += Atile(i, k) * Btile(k, :) for all values k = 0...7 in an unrolled way
    // Effectuer la multiplication Ctile(i, :) += Atile(i, k) * Btile(k, :) pour toutes les valeurs de k = 0...7 de
    // manière déroulé sur les valeurs de k
    // TODO / A FAIRE ...
    float *aline = &tA[i * N];
    atile[0] = _mm256_broadcast_ss(&aline[0]);
    atile[1] = _mm256_broadcast_ss(&aline[1]);
    atile[2] = _mm256_broadcast_ss(&aline[2]);
    atile[3] = _mm256_broadcast_ss(&aline[3]);
    atile[4] = _mm256_broadcast_ss(&aline[4]);
    atile[5] = _mm256_broadcast_ss(&aline[5]);
    atile[6] = _mm256_broadcast_ss(&aline[6]);
    atile[7] = _mm256_broadcast_ss(&aline[7]);
    ctile[0] = _mm256_fmadd_ps(atile[0], btile[0], ctile[0]);
    ctile[1] = _mm256_fmadd_ps(atile[1], btile[1], ctile[1]);
    ctile[2] = _mm256_fmadd_ps(atile[2], btile[2], ctile[2]);
    ctile[3] = _mm256_fmadd_ps(atile[3], btile[3], ctile[3]);
    ctile[4] = _mm256_fmadd_ps(atile[4], btile[4], ctile[4]);
    ctile[5] = _mm256_fmadd_ps(atile[5], btile[5], ctile[5]);
    ctile[6] = _mm256_fmadd_ps(atile[6], btile[6], ctile[6]);
    ctile[7] = _mm256_fmadd_ps(atile[7], btile[7], ctile[7]);
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
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
          C[i * N + j] += A[i * N + k] * B[k * N + j]; // C(i, j) = C(i, j) + A(i, k) * B(k, j)
        }
      }
    }
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
      for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
          for (int j = 0; j < N; j++) {
            C[i * N + j] += A[i * N + k] * B[k * N + j]; // C(i, j) = C(i, j) + A(i, k) * B(k, j)
          }
        }
      }
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
      for (int i = 0; i < N; i += B2) {
        for (int k = 0; k < N; k += B2) {
          for (int j = 0; j < N; j += B2) {
            for (int i2 = i; i2 < i + B2; i2 += B1) {
              for (int k2 = k; k2 < k + B2; k2 += B1) {
                for (int j2 = j; j2 < j + B2; j2 += B1) {
                  float *tA = &A[i2 * N + k2];
                  float *tB = &B[k2 * N + j2];
                  float *tC = &C[i2 * N + j2];
                  for (int i1 = 0; i1 < B1; i1++) {
                    for (int k1 = 0; k1 < B1; k1++) {
                      for (int j1 = 0; j1 < B1; j1++) {
                        tC[i1 * N + j1] += tA[i1 * N + k1] * tB[k1 * N + j1];
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
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
      for (int i = 0; i < N; i += B2) {
        for (int k = 0; k < N; k += B2) {
          for (int j = 0; j < N; j += B2) {
            for (int i2 = i; i2 < i + B2; i2 += B1) {
              for (int k2 = k; k2 < k + B2; k2 += B1) {
                for (int j2 = j; j2 < j + B2; j2 += B1) {
                  for (int i1 = i2; i1 < i2 + B1; i1 += 8) {
                    for (int k1 = k2; k1 < k2 + B1; k1 += 8) {
                      for (int j1 = j2; j1 < j2 + B1; j1 += 8) {
                        multiplyTile(&A[i1 * N + k1], &B[k1 * N + j1], &C[i1 * N + j1], atile, btile, ctile, N);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
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
#pragma omp parallel default(none) shared(A, B, C, B1, B2, N)
      {
#pragma omp single
        for (int i = 0; i < N; i += B2) {
          for (int k = 0; k < N; k += B2) {
            for (int j = 0; j < N; j += B2) {
#pragma omp task default(none) shared(A, B, C, N, B1, B2) firstprivate(i, j, k) depend(in:A[i*N+k]) \
              depend(in:B[k*N+j]) depend(in:C[i*N+j]) depend(out:C[i*N+j])
              {
                __m256 atile[8], btile[8], ctile[8];
                float *tA, *tB, *tC;
                for (int i2 = i; i2 < i + B2; i2 += B1) {
                  for (int k2 = k; k2 < k + B2; k2 += B1) {
                    for (int j2 = j; j2 < j + B2; j2 += B1) {
                      for (int i1 = i2; i1 < i2 + B1; i1 += 8) {
                        for (int k1 = k2; k1 < k2 + B1; k1 += 8) {
                          for (int j1 = j2; j1 < j2 + B1; j1 += 8) {
                            multiplyTile(&A[i1 * N + k1], &B[k1 * N + j1], &C[i1 * N + j1], atile, btile, ctile, N);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
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
