/**
 * @file nelderMead.c
 * @brief Implementation of the Nelder-Mead algorithm
 *
 * A full description of the algorithm can be found here:
 * https://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method
 *
 * @author Victor Lepère
 * @date Thu 18 May 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALPHA 1.0
#define GAMMA 2.0
#define RHO 0.5
#define SIGMA 0.5

#define N_PARAMS 4

typedef struct {
    double *params;
    double value;
} Simplex;

void updateBounds(double *params, const double *lower_bounds, const double *upper_bounds) 
{
    for (int i = 0; i < N_PARAMS; i++) {
        if (params[i] < lower_bounds[i])
            params[i] = lower_bounds[i];
        else if (params[i] > upper_bounds[i])
            params[i] = upper_bounds[i];
    }
}

void replaceSimplex(Simplex *simplexes, int pos, double *params, double value)
{
    for (int i = 0; i < N_PARAMS; i++)
    {
        simplexes[pos].params[i] = params[i];
    }
    simplexes[pos].value = value;
}

int compareSimplex(const void * a, const void * b) 
{
    Simplex* sA = (Simplex*)a;
    Simplex* sB = (Simplex*)b;
    if (sA->value < sB->value) return -1;
    if (sA->value > sB->value) return 1;
    return 0;
}

double *nelderMead(double(*f)(double *), const double *initialParams, const double *lBounds, const double *uBounds, int nmax, double tol) 
{
    Simplex simplex[N_PARAMS + 1];

    // initialization
    for (int i = 0; i <= N_PARAMS; i++) {
        simplex[i].params = malloc(N_PARAMS * sizeof(double));
        for (int j = 0; j < N_PARAMS; j++) {
            simplex[i].params[j] = initialParams[2 * i + j];
        }
        simplex[i].value = f(simplex[i].params);
    }

    double reflection[N_PARAMS], expansion[N_PARAMS], contraction[N_PARAMS], centroid[N_PARAMS];
    double reflectionValue, expansionValue, contractionValue;

    int n = 0;
    while (n < nmax) {
        n++;

        // f(x1) <= f(x2) <= ... <= f(xn)
        qsort(simplex, N_PARAMS + 1, sizeof(Simplex), compareSimplex);

        if (fabs(simplex[0].value) < tol) {
            break;
        }

        // mean of the N_PARAMS best points
        for (int j = 0; j < N_PARAMS; j++) {
            centroid[j] = 0.0;
            for (int i = 0; i < N_PARAMS; i++) {
                centroid[j] += simplex[i].params[j];
            }
            centroid[j] /= N_PARAMS;
        }

        // reflexion
        for (int j = 0; j < N_PARAMS; j++) {
            reflection[j] = centroid[j] + ALPHA * (centroid[j] - simplex[N_PARAMS].params[j]);
        }
        updateBounds(reflection, lBounds, uBounds);
        reflectionValue = f(reflection);
        if (simplex[0].value <= reflectionValue && reflectionValue < simplex[N_PARAMS - 1].value) 
        {
            replaceSimplex(simplex, N_PARAMS, reflection, reflectionValue);
            continue;
        }

        // expansion
        if (reflectionValue < simplex[0].value) {
            for (int j = 0; j < N_PARAMS; j++) {
                expansion[j] = centroid[j] + GAMMA * (reflection[j] - centroid[j]);
            }
            updateBounds(expansion, lBounds, uBounds);
            expansionValue = f(expansion);
            if (expansionValue < reflectionValue) {
                replaceSimplex(simplex, N_PARAMS, expansion, expansionValue);
            }
            else {
                replaceSimplex(simplex, N_PARAMS, reflection, reflectionValue);
            }
            continue;
        }

        // contraction
        for (int j = 0; j < N_PARAMS; j++) 
        {
            contraction[j] = centroid[j] + RHO * (simplex[N_PARAMS].params[j] - centroid[j]);
        }
        updateBounds(contraction, lBounds, uBounds);
        contractionValue  = f(contraction);
        if (contractionValue < simplex[N_PARAMS].value) 
        {
            replaceSimplex(simplex, N_PARAMS, contraction, contractionValue);
            continue;
        }

        // homothetie
        for (int i = 1; i <= N_PARAMS; i++) {
            for (int j = 0; j < N_PARAMS; j++) {
                simplex[i].params[j] = simplex[0].params[j] + SIGMA * (simplex[i].params[j] - simplex[0].params[j]);
            }
            simplex[i].value = f(simplex[i].params);
        }
    }

    printf("\n=============================\n");
    printf("Nelder-Mead Algorithm Results\n");
    for (int j = 0; j < N_PARAMS; j++) {
        printf("    param %d      : %.6lf\n", j, simplex[0].params[j]);
    }
    printf("    value        : %.6lf\n", simplex[0].value);
    printf("    # iterations : %d\n", n);
    printf("=============================\n");

    double *result = malloc(N_PARAMS * sizeof(double));
    memcpy(result, simplex[0].params, N_PARAMS * sizeof(double));

    for (int i = 0; i <= N_PARAMS; i++) {
        free(simplex[i].params);
    }

    return result;
}
