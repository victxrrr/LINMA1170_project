#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
  #include "../../gmsh-sdk/include/gmshc.h"
#else
  #include "gmshc.h"
#endif
#include "../util/matrix.h"
#include "../util/elasticity.h"
#include "math.h"
#include "../util/lu.h"
#include "../geometries/design.h"
#include "../util/eigen.h"
#include "../util/dichotomous.h"
#include "../util/nelderMead.h"
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

#define N_PARAMS 4
#define MESH_SIZE_FACTOR .15   // mesh size factor for the design
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
    for (int i = 0; i < N_PARAMS; i++) {
        printf("params[%d] = %.3lf mm\n", i, params[i] * 1e3);
    }

    //designHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);
    //designHalfTuningFork2D(params[0], params[1], MESH_SIZE_FACTOR, NULL);
    customHalfTuningFork2(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);
    //designHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);
    // harmonic(params[0], params[1], params[2], params[3], params[0]/2, params[1]/2, params[2]/2, params[3]/2, MESH_SIZE_FACTOR, NULL);

    Matrix *K, *M;
    size_t* boundary_nodes;
    size_t n_boundary_nodes;
    size_t* symmetry_nodes;
    size_t n_symmetry_nodes;
    double * coord;
    assemble_system(&K, &M, &coord, &boundary_nodes, &n_boundary_nodes, &symmetry_nodes, &n_symmetry_nodes, E, NU, RHO);

    Matrix *K_new;
    Matrix *M_new;
    remove_bnd_lines(K, M, boundary_nodes, n_boundary_nodes, symmetry_nodes, n_symmetry_nodes, &K_new, &M_new, NULL);

    int n = K_new->n;
    double w[n]; 
    
    clock_t start = clock();
    int result = LAPACKE_dsygv(LAPACK_ROW_MAJOR, 1, 'N', 'L', n, M_new->data, n, K_new->data, n, w);
    ErrorScan(result, n);
    printf("computation time     : %.3lf s\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    free_matrix (K);
    free_matrix (M);
    free_matrix (K_new);
    free_matrix (M_new);
    if (n_boundary_nodes) free(boundary_nodes);
    if (n_symmetry_nodes) free(symmetry_nodes);

    printf("f = %.3lf Hz\n", freq(w[n - 1]));
    printf("f2 = %.3lf Hz\n", freq(w[n - 2]));
    printf("=============================\n\n");

    FILE *fp;
    fp = fopen("frequencies.txt", "w");
    for (int i = 1; i < 3; i++) {
        fprintf(fp, "%.9e\n", freq(w[n - i]));
    }
    fclose(fp);

    return fabs(freq(w[n - 1]) - 1760.0) + fabs(freq(w[n - 2]) - 2640.0); //2112Hz
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
    gmshOptionSetNumber("General.Terminal", 0, &ierr);
    
    // Number of vibration modes to find
    int k = 2; // atoi(argv[1])
    // double lowerBounds[4] = {2e-3, 7e-3, 1e-2, 3e-2}; // 5e-2
    // double upperBounds[4] = {8e-3, 15e-3, 3e-2, 10e-2};
    // double lowerBounds[1] = {5e-2};
    // double upperBounds[1] = {20e-2};
    double lowerBounds[N_PARAMS] = {5e-3, 2e-2, 0.5e-2, 1e-2};
    double upperBounds[N_PARAMS] = {10e-3, 7e-2, 3e-2, 6e-2};

    // double lowerBounds[2] = {7e-3, 5e-2};
    // double upperBounds[2] = {20e-3, 15e-2};

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
    // double params[N_PARAMS] = {atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6])};
    // double value = f(params);
    double init[(N_PARAMS + 1) * N_PARAMS] = {7e-3, 2e-2, 2e-2, 4e-2,
                                             10e-3, 3e-2, 2e-2, 3e-2,
                                              8e-3, 4e-2, 3e-2, 35e-3,
                                              9e-3, 5e-2, 2e-2, 2e-2};

    double *params = nelderMead(f, init, lowerBounds, upperBounds, 200, 1.0);

    // double *params = polyDichotomous(f, lowerBounds, upperBounds, N_PARAMS, 10000, 0.15);
    // double *params = secant(f, param0, param1, eval0, eval1, 4, 30, 0.1);

    //double test[4] = {6e-3, 11e-3, 38e-3, 82e-3};

    // double test[6] = {5e-3, 12e-3, 5e-3, 2e-2, 4e-2, 5e-2};
    // f(test);
    // f(test);

    // write to file
    FILE *fp;
    fp = fopen(argv[2], "w");
    for (int i = 0; i < N_PARAMS; i++) {
        fprintf(fp, "opt. param%d : %.9e\n", i, params[i]);
    }
    fclose(fp);


    // execution time 
    end = clock();
    printf("execution time       : %.3lf s\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    gmshFinalize(&ierr);

    free(params);

    return 0;
}
