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

#ifndef NELDER_MEAD_H
#define NELDER_MEAD_H

double *nelderMead(double(*f)(double *), const double *initialParams, const double *lBounds, const double *uBounds, int nmax, double tol);

#endif