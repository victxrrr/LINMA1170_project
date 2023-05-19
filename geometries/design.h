#ifndef DESIGN_H // le header guard
#define DESIGN_H 

void designTuningForkCustom(double l, double meshSizeFactor, char * filename);
void designTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename);
void designHalfTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename);
void designHalfTuningFork1D(double l, double meshSizeFactor, char * filename);
void designHalfTuningFork2D(double r2, double l, double meshSizeFactor, char * filename);

void harmonic(double r1, double r2, double e, double l, double r11, double r22, double ee, double ll, double meshSizeFactor, char * filename);


void chandelier(double r1, double r2, double r3, double e, double l1, double l2, double meshSizeFactor, char * filename);
void customHalfTuningFork(double r1, double r2, double r3, double e, double l1, double l2, double meshSizeFactor, char * filename);
void customHalfTuningFork2(double r2, double e, double l1, double l2, double meshSizeFactor, char * filename);


#endif
