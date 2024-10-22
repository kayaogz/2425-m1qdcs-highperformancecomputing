// Installation: Install libopenblas-dev in linux, Accelerate framework in Mac to get BLAS and LAPACK functions
// Compilation: g++ -O2 gemm-example.cpp -o gemm-example -fopenmp -lblas -llapack
// Execution: ./gemm-example
#include <iostream>
#include <vector>

// Matrix-matrix multiplication, alpha and beta are scalars and op(.) is an optional matrix transposition operator.
// Produit matrice-vecteur, alpha et beta sont des scalaires et op(.) est une transposition matricielle optionnelle.
// C = alpha opA(A) op(B) + beta op(C) (BLAS3)
extern "C" void dgemm_(
  char *transA,
  char *transB,
  int *m,
  int *n,
  int *k,
  double *alpha,
  double *A,
  int *lda,
  double *B,
  int *ldb,
  double *beta,
  double *C,
  int *ldc);

void printMatrix(
  double *A,
  int m,
  int n)
{
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) { printf("%5.1lf", A[i + j * n]); }
    printf("\n");
  }
  printf("\n");
}

int main()
{
  // Allocate matrices A, B, C of size N x N
  // Allouer les matrices A, B, C de taille N x N
  int N = 4;
  std::vector<double> A(N * N), B(N * N), C(N * N);

  // Initialize A(i, j) = i + j, B(i, j) = 1, C(i, j) = 0. Matrices must be column-major in BLAS/LAPACK.
  // Initializer A(i, j) = i + j, B(i, j) = 1, C(i, j) = 0. Les matrices doivent etre colonne-majeur en BLAS/LAPACK.
  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      A[i + j * N] = i + j;
      B[i + j * N] = 1.0; // Column-major / Colonne-majeur
      C[i + j * N] = 2.0; // Column-major / Colonne-majeur
    }
  }
  char transA = 'N', transB = 'N';
  int m = N, n = N, k = N;
	int lda = N, ldb = N, ldc = N;
  double alpha = 1.0;
  double beta = 0.0;

  // Compute C = 1.0 * A * B + 0.0 * C
  // Calculer C = 1.0 * A * B + 0.0 * C
  dgemm_(&transA, &transB, &m, &n, &k, &alpha, &A[0], &lda, &B[0], &ldb, &beta, &C[0], &ldc);

  // Print matrices
  // Afficher les matrices
  printMatrix(&A[0], N, N);
  printMatrix(&B[0], N, N);
  printMatrix(&C[0], N, N);

  return 0;
}
