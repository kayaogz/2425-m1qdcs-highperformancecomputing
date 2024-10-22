// Installation: Install openblas-dev in linux, Accelerate framework in Mac to get BLAS and LAPACK functions
// Compilation: g++ -O2 cholesky.cpp -o cholesky -fopenmp -lblas -llapack
// Execution: OPENBLAS_NUM_THREADS=1 VECLIB_MAXIMUM_THREADS=1 ./cholesky
// For more info regarding BLAS/LAPACK functions, c.f. Intel MKL documentation
// Pour plus d'information concernant les fonctions dans BLAS/LAPACK, c.f. documentation d'Intel MKL
//   https://www.intel.com/content/www/us/en/docs/onemkl/developer-reference-fortran/2024-2
// Information on blocked Cholesky algorithm
// Information sur l'algorithme de Cholesky par blocs
//   https://www.netlib.org/utk/papers/factor/node9.html
// Complete list of BLAS routines
// Liste complete des routines BLAS
//   https://www.netlib.org/blas/blasqr.pdf

#include <iostream>
#include <vector>

// 2-norm of a vector (BLAS1)
// 2-norme pour un vecteur (BLAS1)
extern "C" double dnrm2_(
  int *n,
  double *x,
  int *incx);

// Compute y = y + a x, where x and y are vectors and a is a scalar (BLAS1)
// Calculer y = y + a x, x et y etant des vecteurs et a etant un scalaire (BLAS1)
extern "C" void daxpy_(
  int *n,
  double *a,
  double *x,
  int *incx,
  double *y,
  int *incy);

// Matrix-vector multiplication where alpha and beta are scalars (BLAS2)
// Produit matrice-vecteur, alpha et beta etant des scalaires (BLAS2)
// y = alpha A x + beta y
extern "C" void dgemv_(
  char *trans,
  int *m,
  int *n,
  double *alpha,
  double *a,
  int *lda,
  double *x,
  int *incx,
  double *beta,
  double *y,
  int *incy);

// Lower/upper triangular system solve for a single column vector. x is modified in-place (overwritten by x_new)
// Resoudre un systeme triangulaire superieure/inferieure pour un vecteur de colonne. x est modifie en place (surecrit
// par x_new)
// L x_new = x, U x_new = x (BLAS2)
extern "C" void dtrsv_(
  char *uplo,
  char *trans,
  char *diag,
  int *n,
  double *A,
  int *lda,
  double *x,
  int *incx);

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

// Symmetric matrix-matrix multiplication. A itself does not have to be symmetric
// Produit matrice-matrice symmetrique. A elle-meme n'a pas a etre symmetrique
// C = alpha A A^T + beta C, C = alpha A^T A + beta C
extern "C" void dsyrk_(
  char *uplo,
  char *trans,
  int *n,
  int *k,
  double *alpha,
  double *A,
  int *lda,
  double *beta,
  double *C,
  int *ldc);

// Lower/upper triangular system solve for a block of column vectors
// Resolution d'un systeme triangulaire inferieure/superieure pour un bloc de vecteur de colonne 
// L B_new = B, U B_new = B (BLAS3)
extern "C" void dtrsm_(
  char *side,
  char *uplo,
  char *transA,
  char *diag,
  int *m,
  int *n,
  double *alpha,
  double *A,
  int *lda,
  double *B,
  int *ldb);

// Cholesky factorization L L^T = A or U UˆT = A (LAPACK). A is overwritten by L or L^T
// Once computed, you can solve A x = b as follows:
// Factorisation Cholesky L L^T = A or U UˆT = A (LAPACK). A est surecrit par L ou L^T
// Une fois calculee, on peut resoudre A x = B comme suit:
// A x = b 
// L L^T x = b         -> dpotrf
// L L^T x = L y = b   -> dtrsv
// L^T x = y           -> dtrsv
extern "C" void dpotrf_(
  char *uplo,
  int *n,
  double *A,
  int *lda,
  int *info);

// Cholesky factorization of a small block of matrix; use only when n is small
// Factorisation Cholesky pour un petit bloc de matrice; a utiliser seulement si n est petit
extern "C" void dpotf2_(
    char *uplo,
    int *n,
    double *A,
    int *lda,
    int *info);

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
  // Dimension of matrices / Dimensions des matrices
  int N = 8; 
  // Block size for the task-parallel blocked potrf code / Taille de bloc pour potrf parallele par bloc a base de taches
  int BS = 4;  
  // Matrices
  std::vector<double> L(N * N), A(N * N), B(N * N);
  // Vectors
  std::vector<double> x(N), b(N), b2(N);

  // Generate a lower-triangular N x N matrix L with random values between 0.0 and 1.0
	// Optional: Use LAPACK random matrix generator dlatmr
  // Generer une matrice triangulaire inferieure L de taille N x N avec valeurs aleatoires entre 0.0 et 1.0
  // Optionnel: Utiliser la fonction generatrice de matrice aleatoire dlatmr dans LAPACK
  // TODO / A FAIRE

  // Generate a symmetric positive definite matrix A = L * L^T using the dsyrk function (BLAS3)
  // Generer une matrice symmetrique positive definite A = L * L^T avec la fonction dsyrk (BLAS3)
  // TODO / A FAIRE

  // printMatrix(&A[0], N, N);

  // Perform a Cholesky factorization on the matrix A, A = L LˆT using the potrf function (LAPACK)
  // Effecture une factorisation Cholesky sur la matrice A, A = L L^T avec la fonction potrf (LAPACK)
  // TODO / A FAIRE

  // Solve the linear system A x = L L^T x = b by first solving L y = b, then solving LˆT x = y, with two successive
  // calls to dtrsv
  // Resoudre le systeme lineaire A x = L L^T x = b, d'abort en resolvant L y = b, ensuite LˆT x = y avec deux appels
  // successifs au dtrsv.
  // TODO / A FAIRE

  // Verify the solution x by computing b2 = A x using dgemv, then compare it to the initial right hand side vector by
  // computing (b - b2) using daxpy, and computing the norm of this vector~(which is the error) by dnrm2
  // Verifier la solution x en calculant b2 = A x avec dgemv, puis en le comparant au second membre initial en calculant
  // (b - b2) avec daxpy d'abord, en suite en calculant la norme de ce vecteur~(ce qui est l'erreur) avec dnrm2.
  // TODO / A FAIRE

  // Now implement a blocked version of the potrf yourself using dpotf2, dtrsm, dsyrk, and dgemm routines, using block
  // size BS.
  // Use OpenMP Tasks to parallelize the computation, specifying dependencies between operations.
  // For simplicity, you can assume that N is divisible by BS.
  // Maintenant, implanter une version tuilee de potrf avec fonctions dpotf2, dtrsm, dsyrk, et dgemm, en utilisant une
  // taille de tuile BS.
  // Utiliser OpenMP Tasks afin de paralleliser le calcul, en precisant les dependences entre les operations.
  // Pour simplicite, Supposer que N est divisible par BS.
  // TODO / A FAIRE

  return 0;
}
