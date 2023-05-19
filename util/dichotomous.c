#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "dichotomous.h"

/**
 * @brief Multivariate dichotomous method to find the optimal parameters.
*/
double *polyDichotomous(double(*myFunction)(double *), double *a, double *b, int nParam, int nMax, double tol) {

    double *param = malloc(nParam*sizeof(double));
    getDichotomousParam(param, a, b, nParam);
    double f = myFunction(param);

    if (f*myFunction(a) > 0) {
        memcpy(a, param, nParam*sizeof(double));
    } else {
        memcpy(b, param, nParam*sizeof(double));
    }

    int nIter = 1;
    while (nIter < nMax && fabs(f) > tol) 
    {
        for (int i=0; i<nParam; i++) 
        {
            param[i] = (a[i] + b[i]) / 2;
            f = myFunction(param);
            if (fabs(f) <= tol) break;
            if (f*myFunction(a) > 0) {
                memcpy(a, param, nParam*sizeof(double));
            } else {
                memcpy(b, param, nParam*sizeof(double));
            }
        }
        nIter++;
    }
    fprintf(stderr, "The number of iteration is : %d\n", nIter*nParam);

    return param;
}

/**
 * @brief Updating the parameters for the dichotomous method.
*/
void getDichotomousParam(double *param, double *intervalBound1, double *intervalBound2, int nParam) 
{
    for (int i=0; i<nParam; i++) 
    {
        param[i] = (intervalBound1[i] + intervalBound2[i]) / 2;
    }
}

/**
 * @brief Dichotomous method to find the optimal parameters.
*/
double *dichotomous(double(*myFunction)(double *), double *intervalBound1, double *intervalBound2, int nParam, int nMax, double tol) 
{
    // Getting the initial parameters.
    double *param = malloc(nParam*sizeof(double));
    getDichotomousParam(param, intervalBound1, intervalBound2, nParam);
    double currentEval = myFunction(param);

    // Finding the optimal parameters.
    int nIter = 0;
    while (nIter < nMax && fabs(currentEval) > tol) 
    {
        // Updating the interval bounds.
        if (currentEval*myFunction(intervalBound1) > 0) 
        {
            memcpy(intervalBound1, param, nParam*sizeof(double));
        } else 
        {
            memcpy(intervalBound2, param, nParam*sizeof(double));
        }
        // Updating parameters.
        getDichotomousParam(param, intervalBound1, intervalBound2, nParam);
        currentEval = myFunction(param);
        nIter++;
    }

    fprintf(stderr, "The number of iteration is : %d\n", nIter);

    return param;
}

/**
 * @brief Updating the parameters for the secant method.
*/
void getSecantParam(double *exParam, double *currentParam, double *newParam, double exEval, double currentEval, int nParam) {
    for (int i=0; i<nParam; i++) {
        newParam[i] = currentParam[i] - currentEval * (currentParam[i]-exParam[i]) / (currentEval-exEval);
    }
}

/**
 * @brief Finding the initial parameters for the secant method from the dichotomous method.
*/
double *loadSecantInitParam(double *param0, double *param1, double(*myFunction)(double *), double *intervalBound1, double *intervalBound2, int nParam, int nMax) {
    
    // Initialize the array to return.
    double *evals = malloc(2*sizeof(double)); 

    // Getting the initial parameters.
    getDichotomousParam(param1, intervalBound1, intervalBound2, nParam);
    evals[1] = myFunction(param1);

    // Finding the optimal parameters.
    int nIter = 1;
    while (nIter < nMax) {
        // Updating the interval bounds.
        if (evals[1] * myFunction(intervalBound1) > 0) {
            memcpy(intervalBound1, param1, nParam*sizeof(double));
        } else {
            memcpy(intervalBound2, param1, nParam*sizeof(double));
        }
        // Updating parameters.
        memcpy(param0, param1, nParam*sizeof(double));
        evals[0] = evals[1];
        getDichotomousParam(param1, intervalBound1, intervalBound2, nParam);
        evals[1]  = myFunction(param1);
        nIter++;
    }

    fprintf(stderr, "The number of iteration is : %d\n", nIter);

    return evals;
}

/**
 * @brief Secant method to find the optimal parameters.
*/
double *secant(double(*myFunction)(double *), double *param0, double *param1, double eval0, double eval1, int nParam, int nMax, double tol) {

    // Setting the initial parameters.
    double exEval = eval0;
    double currentEval = eval1;
    double *exParam = param0;
    double *currentParam = param1;
    double *newParam = malloc(nParam*sizeof(double));
    getSecantParam(exParam, currentParam, newParam, exEval, currentEval, nParam);

    // Finding the optimal parameters.
    int nIter = 0;
    while (nIter < nMax && fabs(currentEval) > tol) {
        memcpy(exParam, currentParam, nParam*sizeof(double));
        memcpy(currentParam, newParam, nParam*sizeof(double));
        getSecantParam(exParam, currentParam, newParam, exEval, currentEval, nParam);
        exEval = currentEval;
        currentEval = myFunction(newParam);
        nIter++;
    }

    fprintf(stderr, "The number of iteration is : %d\n", nIter);
    
    return newParam;
}