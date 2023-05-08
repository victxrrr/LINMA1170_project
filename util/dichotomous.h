
// Implementation of the dichotomous method to solve a non-linear equation given two bounds.
void getDichotomousParam(double *param, double *intervalBound1, double *intervalBound2, int nParam);
double *dichotomous(double(*myFunction)(double *), double *intervalBound1, double *intervalBound2, int nParam, int nMax, double tol);

// Implementation of the secant method to solve a non-linear equation given the two initial parameters corresponding to the bound with 
// the same sign as the result of getDichotomousParam().
void getSecantParam(double *exParam, double *currentParam, double *newParam, double exEval, double currentEval, int nParam);
double *secant(double(*myFunction)(double *), double *param0, double *param1, double eval0, double eval1, int nParam, int nMax, double tol);