#ifndef DESIGN_H // le header guard
#define DESIGN_H 

/**
 * @brief      A basic tuning fork design
*/
void designTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename);
/**
 * @brief      The half basic tuning fork design
*/
void designHalfTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename);
/**
 * @brief      The half basic tuning fork design with only one parameter to control
*/
void designHalfTuningFork1D(double l, double meshSizeFactor, char * filename);
/**
 * @brief      The half basic tuning fork design with two parameters to control
*/
void designHalfTuningFork2D(double r2, double l, double meshSizeFactor, char * filename);

/**
 * @brief      A beautiful tuning fork design
*/
void customTuningFork(double r1, double r2, double r3, double e, double l1, double l2, double meshSizeFactor, char * filename);
/**
 * @brief      The half beautiful tuning fork design with some parameters to control
*/
void customHalfTuningFork(double r2, double e, double l1, double l2, double meshSizeFactor, char * filename);
/**
 * @brief      The half beautiful tuning fork design with full parameters to control
*/
void customHalfTuningForkFull(double r1, double r2, double r3, double e, double l1, double l2, double meshSizeFactor, char * filename);

/**
 * @brief      A tuning fork on a tuning fork design
*/
void harmonic(double r1, double r2, double e, double l, double r11, double r22, double ee, double ll, double meshSizeFactor, char * filename);

#endif
