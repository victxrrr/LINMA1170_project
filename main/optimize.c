#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lapacke.h>
#include <time.h>
#include "math.h"
#include "../util/lu.h"
#include "../util/eigen.h"
#include "../util/matrix.h"
#include "../util/elasticity.h"
#include "../util/dichotomous.h"
#include "../util/nelderMead.h"
#include "../geometries/design.h"

// CHANGE THIS TO THE CORRECT PATH
#ifdef __linux__
  #include "../../gmsh-sdk/include/gmshc.h"
#else
  #include "gmshc.h"
#endif

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define freq(w) \
  ({ __typeof__ (w) _w = (w); \
    1./(2*M_PI*sqrt(_w)); \
  })

#define N_PARAMS 4
#define MESH_SIZE_FACTOR .3   // mesh size factor for the design
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
        } else {
            printf("DLASCL  : the leading minor of order %d is not positive definite; the factorization of B could not be completed\n", info-n);
        }
    }
}

double last_val = 0;

double f(double *params) {

    //designHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);    // A basic tuning fork design :-(
    customHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);      // A beautiful tuning fork design :-)

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
    
    int result = LAPACKE_dsygv(LAPACK_ROW_MAJOR, 1, 'N', 'L', n, M_new->data, n, K_new->data, n, w);
    ErrorScan(result, n);

    free_matrix (K);
    free_matrix (M);
    free_matrix (K_new);
    free_matrix (M_new);
    if (n_boundary_nodes) free(boundary_nodes);
    if (n_symmetry_nodes) free(symmetry_nodes);

    last_val = w[n - 1];

    // return freq(w[n - 1]) - 1760.0;
    return fabs(freq(w[n - 1]) - 1760.0) + fabs(freq(w[n - 2]) - 2640.0); // calculation of the fifth
}


int main (int argc, char *argv[]) {

    clock_t start = clock(), end;

    // Initialize Gmsh and create geometry
    int ierr;
    gmshInitialize(argc, argv, 0, 0, &ierr);

    // Disable terminal output 0/1
    gmshOptionSetNumber("General.Terminal", 0, &ierr);
    
    // double lowerBounds[N_PARAMS] = {2e-3, 7e-3, 1e-2, 3e-2};   // You can use these bounds for the dichotomous method !
    // double upperBounds[N_PARAMS] = {8e-3, 15e-3, 3e-2, 10e-2};
    double lowerBounds[N_PARAMS] = {5e-3, 2e-2, 0.5e-2, 1e-2};    // You can use these bounds for the Nelder-Mead method !
    double upperBounds[N_PARAMS] = {10e-3, 7e-2, 3e-2, 6e-2};

    // Initialisation of a simplex  
    double init[(N_PARAMS + 1) * N_PARAMS] = {7e-3, 2e-2, 2e-2, 4e-2,
                                             10e-3, 3e-2, 2e-2, 3e-2,
                                              8e-3, 4e-2, 3e-2, 35e-3,
                                              9e-3, 5e-2, 2e-2, 2e-2};

    // double *params = polyDichotomous(f, lowerBounds, upperBounds, N_PARAMS, 10000, 0.15);
    double *params = nelderMead(f, init, lowerBounds, upperBounds, 200, 1.0);

    // Write optimal parameters to file
    FILE *fp;
    fp = fopen(argv[1], "w");
    for (int i = 0; i < N_PARAMS; i++) {
        fprintf(fp, "opt. param%d : %.9e\n", i, params[i]);
    }
    fclose(fp);

    // Execution time 
    end = clock();
    printf("\nfondamental frequency : %.3lf Hz\n", freq(last_val));
    printf("execution time        : %.3lf s\n\n", (double)(end - start) / CLOCKS_PER_SEC);

    gmshFinalize(&ierr);

    free(params);

    return 0;
}
