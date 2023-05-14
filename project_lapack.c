#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
  #include "../gmsh-sdk/include/gmshc.h"
#else
  #include "gmshc.h"
#endif
#include "matrix.h"
#include "elasticity.h"
#include "math.h"
#include "lu.h"
#include "design.h"
#include "eigen.h"
#include "dichotomous.h"
#include <lapacke.h>
#include <time.h>

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define freq(w) \
  ({ __typeof__ (w) _w = (w); \
    1./(2*M_PI*sqrt(_w)); \
  })

#define MESH_SIZE_FACTOR 0.09 // mesh size factor for the design
#define E 0.7e11              // Young's modulus for Aluminum
#define NU 0.3                // Poisson coefficient
#define RHO 3000              // Density of Aluminum


void ErrorScan(int info, int n) {
    if (info < 0) {
        printf("The %d-th argument had an illegal value", -info);
        exit(1);
    }
    else if (info > 0) {
        if (info <= n) {
            printf("DLASCL  : %d off-diagonal elements of an intermediate form did not converge to zero\n", info);
            //exit(1);
        } else {
            printf("DLASCL  : the leading minor of order %d is not positive definite; the factorization of B could not be completed\n", info-n);
            //exit(1);
        }
    }
}

void DisplayValues(double *w, int n, int k) {
    printf("\nthe %d eigenvalues are : \n", k);
    for (int i = 0; i < k; i++) {
        printf("    lambda = %.9e, f = %.3lf Hz\n", w[n - i - 1], freq(w[n - i - 1]));
    }
}

double f(double *params) {

    printf("\n=============================\n");
    printf("r1 = %.3lf mm\n", params[0] * 1e3);
    printf("r2 = %.3lf mm\n", params[1] * 1e3);
    printf("e = %.3lf mm\n", params[2] * 1e3);
    printf("l = %.3lf mm\n", params[3] * 1e3);

    designTuningFork4(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);

    Matrix *K, *M;
    int* boundary_nodes;
    int n_boundary_nodes;
    double * coord;
    assemble_system(&K, &M, &coord, &boundary_nodes, &n_boundary_nodes, E, NU, RHO);

    Matrix *K_new;
    Matrix *M_new;
    remove_bnd_lines(K, M, boundary_nodes, n_boundary_nodes, &K_new, &M_new, NULL);

    int n = K_new->n;
    double w[n]; 
    w[n-2] = 1123.6;
    
    clock_t start = clock();
    ErrorScan(LAPACKE_dsygv(LAPACK_ROW_MAJOR, 1, 'N', 'L', n, M_new->data, n, K_new->data, n, w), n);
    printf("computation time     : %.3lf s\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    free_matrix (K);
    free_matrix (M);
    free_matrix (K_new);
    free_matrix (M_new);
    free(boundary_nodes);

    printf("f = %.3lf Hz\n", freq(w[n - 2]));
    printf("=============================\n\n");

    return freq(w[n - 2]) - 1760.0;
}


int main (int argc, char *argv[]) {

    clock_t start = clock(), end;

    // if (argc < 2){
    //     printf("Usage: \n"
    //       "./project <k> <out>\n" 
    //       "---------------------------- \n\n"
    //       "- k is the number of frequencies to compute. \n "
    //       "- out is the output file to write the frequencies. \n "
    //       "\n");
    //     return -1;
    // } 


    // Initialize Gmsh and create geometry
    int ierr;
    gmshInitialize(argc, argv, 0, 0, &ierr);

    // Disable terminal output
    gmshOptionSetNumber("General.Terminal", 1, &ierr);
    
    // Number of vibration modes to find
    int k = 2; // atoi(argv[1]);

    double lowerBounds[4] = {5e-3, 10e-3, 1e-2, 5e-2}; // 5e-2
    double upperBounds[4] = {5e-2, 10e-2, 3e-2, 10e-2};

    // // First value of the secant method
    // double *param1 = malloc(4 * sizeof(double));
    // getDichotomousParam(param1, lowerBounds, upperBounds, 4);
    // double eval1 = f(param1);

    // // Initial value of the secant method
    // double eval0 = f(lowerBounds);
    // double *param0 = lowerBounds;
    // if (eval0 * eval1 < 0) {
    //     eval0 = f(upperBounds);
    //     param0 = upperBounds;
    // }

    double *params = dichotomous(f, lowerBounds, upperBounds, 4, 20, 0.1);
    // double *params = secant(f, param0, param1, eval0, eval1, 4, 30, 0.1);

    // write to file
    FILE *fp;
    fp = fopen(argv[2], "w");
    for (int i = 0; i < 3; i++) {
        fprintf(fp, "%.9e, ", params[i]);
    }
    fprintf(fp, "%.9e\n", params[3]);
    fclose(fp);
    free(params);

    // execution time 
    end = clock();
    printf("execution time       : %.3lf s\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    gmshFinalize(&ierr);

    return 0;
}
