#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "math.h"
#include "../util/lu.h"
#include "../util/eigen.h"
#include "../util/matrix.h"
#include "../util/elasticity.h"
#include "../geometries/design.h"

#ifdef __linux__
  #include "../../gmsh-sdk/include/gmshc.h"
#else
  #include "../gmsh-sdk/include/gmshc.h"
#endif

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define N_PARAMS 4
#define MESH_SIZE_FACTOR .15

int main (int argc, char *argv[]) {
  clock_t start = clock();

  if (argc < 2){
    printf("Usage: \n"
			"./project <k> <out>\n" 
			"---------------------------- \n\n"
			"- k is the number of frequencies to compute. \n "
			"- out is the output file to write the frequencies. \n "
      "\n");
		return -1;
  } 

  // Define physical constants
  double E = 0.7e11;  // Young's modulus for Aluminum
  double nu = 0.3;    // Poisson coefficient
  double rho = 3000;  // Density of Aluminum

  // read parameters from file
  FILE *fp;
  fp = fopen(argv[2], "r");
  if (fp == NULL) {
      printf("Erreur lors de l'ouverture du fichier.\n");
      return 1;
  }

  double params[N_PARAMS];
  char line[100];
  int i = 0;

  while (fgets(line, sizeof(line), fp) != NULL && i < N_PARAMS) {
      double value;
      sscanf(line, "opt. param%d : %lf", &i, &value);
      params[i] = value;
  }

  fclose(fp);

	// Initialize Gmsh and create geometry
  int ierr;
  gmshInitialize(argc, argv, 0, 0, &ierr);

  // designHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);
  customHalfTuningFork(params[0], params[1], params[2], params[3], MESH_SIZE_FACTOR, NULL);

  // Number of vibration modes to find
  int k = atoi(argv[1]);

  // Assemble the 2 matrices of the linear elasticity problem: 
  // M is the mass matrix
  // K is the stiffness matrix
  Matrix *K, *M;
  size_t* boundary_nodes;
  size_t n_boundary_nodes;
  size_t* symmetry_nodes;
  size_t n_symmetry_nodes;
  double * coord;

  assemble_system(&K, &M, &coord, &boundary_nodes, &n_boundary_nodes, &symmetry_nodes, &n_symmetry_nodes, E, nu, rho);

  // Remove lines from matrix that are boundary
  Matrix *K_new;
  Matrix *M_new;
  remove_bnd_lines(K, M, boundary_nodes, n_boundary_nodes, symmetry_nodes, n_symmetry_nodes, &K_new, &M_new, NULL);

  // Compute M := K^{-1} M 
  int n = K_new->n;
  lu(K_new);
  double Mj[n]; // column j
  for (int j=0; j<n; j++){ // column by column
    for (int i=0; i<n; i++) Mj[i] = M_new->a[i][j];
    solve(K_new, Mj);
    for (int i=0; i<n; i++){
      M_new->a[i][j] = Mj[i];
    }
  } // -> Now, in M_new we have stored K^-1M

  // Power iteration + deflation to find k largest eigenvalues
  Matrix * A = M_new; // alias for conciseness
  double * v = malloc(A->m * sizeof(double));
  double lambda, freq;

  for(int ki = 0; ki < k; ki++) {
    lambda = power_iteration(A, v);
    freq = 1./(2*M_PI*sqrt(lambda));

    printf("lambda = %.9e, f = %.3lf\n", lambda, freq);
    // Deflate matrix
    for(int i = 0; i < A->m; i++)
      for(int j = 0; j < A->n; j++)
        A->a[i][j] -= lambda * v[i] * v[j];

    // Put appropriate BC and plot
    double * vall = calloc(K->m, sizeof(double));
    // double vall[K->m];
    int iv = 0, i_bnd = 0, i_sym = 1; 
    for(int i = 0; i < K->m/2; i++) {
      if(i_bnd < n_boundary_nodes && i == boundary_nodes[i_bnd]) {
        i_bnd++;
        continue;
      }
      if(i_sym < n_symmetry_nodes && i == symmetry_nodes[i_sym]) {
        i_sym++;
        vall[2*(i)] = v[iv++];
        continue;
      }
      vall[2*(i)]   = v[iv++];
      vall[2*(i)+1] = v[iv++];
    }
    visualize_in_gmsh(vall, K->m/2);
    free(vall);
  }

  //fclose(file);

  gmshFltkRun(&ierr);

  free_matrix (K);
  free_matrix (M);
  free_matrix (K_new);
  free_matrix (M_new);
  free(boundary_nodes);
  if (n_symmetry_nodes != 0) free(symmetry_nodes);
  free(v);

  return 0;
}
