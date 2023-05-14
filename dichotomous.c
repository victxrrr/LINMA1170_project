#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "dichotomous.h"



void getDichotomousParam(double *param, double *intervalBound1, double *intervalBound2, int nParam) {
    for (int i=0; i<nParam; i++) {
        param[i] = (intervalBound1[i] + intervalBound2[i]) / 2;
    }
}


double *dichotomous(double(*myFunction)(double *), double *intervalBound1, double *intervalBound2, int nParam, int nMax, double tol) {

    // Getting the initial parameters.
    double *param = malloc(nParam*sizeof(double));
    getDichotomousParam(param, intervalBound1, intervalBound2, nParam);
    double currentEval = myFunction(param);

    // Finding the optimal parameters.
    int nIter = 0;
    while (nIter < nMax && fabs(currentEval) > tol) {
        // Updating the interval bounds.
        if (currentEval*myFunction(intervalBound1) > 0) {
            memcpy(intervalBound1, param, nParam*sizeof(double));
        } else {
            memcpy(intervalBound2, param, nParam*sizeof(double));
        }
        // Updating parameters.
        getDichotomousParam(param, intervalBound1, intervalBound2, nParam);
        currentEval = myFunction(param);
        nIter++;
    }

    //fprintf(stderr, "The number of iteration is : %d\n", nIter);

    return param;
}


void getSecantParam(double *exParam, double *currentParam, double *newParam, double exEval, double currentEval, int nParam) {
    for (int i=0; i<nParam; i++) {
        newParam[i] = fabs(currentParam[i] - currentEval * (currentParam[i]-exParam[i]) / (currentEval-exEval));
    }
}


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