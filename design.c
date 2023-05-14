
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
  #include "../gmsh-sdk/include/gmshc.h"
#else
  #include "gmshc.h"
#endif

void designTuningFork(double e, double l, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius (half-distance between prongs)
   * r2 = outer radius (half-width of fork)
   * e  = length of handle
   * l  = length of prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h, r1, r2; // width of prongs
  h = 7e-3;
  r1 = 7e-3;
  r2 = r1 + h;
  double meshSize = 4 * h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,16,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,16,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddCircleArc(9,15,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  gmshModelOccAddLine(11,12,11,&ierr);
  gmshModelOccAddLine(12,13,12,&ierr);
  gmshModelOccAddCircleArc(13,15,14,13,&ierr);
  gmshModelOccAddLine(14,1,14,&ierr);

  // Add wire (closed curve)
  int curveTags[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  gmshModelOccAddWire(curveTags, 14, 1, 1, &ierr);

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}

void designTuningFork4(double r1, double r2, double e, double l, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius (half-distance between prongs)
   * r2 = outer radius (half-width of fork)
   * e  = length of handle
   * l  = length of prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h = r2 - r1; // width of prongs
  double meshSize = h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,16,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,16,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddCircleArc(9,15,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  gmshModelOccAddLine(11,12,11,&ierr);
  gmshModelOccAddLine(12,13,12,&ierr);
  gmshModelOccAddCircleArc(13,15,14,13,&ierr);
  gmshModelOccAddLine(14,1,14,&ierr);

  // Add wire (closed curve)
  int curveTags[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  gmshModelOccAddWire(curveTags, 14, 1, 1, &ierr);

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}

void designTuningFork3(double r1, double r2, double e, double l, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius (half-distance between prongs)
   * r2 = outer radius (half-width of fork)
   * e  = length of handle
   * l  = length of prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h = r1 - r2; // width of prongs
  double meshSize = 4 * h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,16,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,16,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddCircleArc(9,15,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  gmshModelOccAddLine(11,12,11,&ierr);
  gmshModelOccAddLine(12,13,12,&ierr);
  gmshModelOccAddCircleArc(13,15,14,13,&ierr);
  gmshModelOccAddLine(14,1,14,&ierr);

  // Add wire (closed curve)
  int curveTags[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  gmshModelOccAddWire(curveTags, 14, 1, 1, &ierr);

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}


void designHalfTuningFork(double r1, double r2, double e, double l, double meshSizeFactor, char * filename) {
  int ierr;
  gmshClear(&ierr);
  double h = r2 - r1; // width of prongs
  double meshSize = h * meshSizeFactor;
  double x = h/2, y = 0, z = 0;

  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h/2;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y -= l;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  x -= h/2;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x += h/2;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);

  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,10,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);
  gmshModelOccAddLine(5,6,5,&ierr);
  gmshModelOccAddLine(6,7,6,&ierr);
  gmshModelOccAddCircleArc(7,10,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddLine(9,1,9,&ierr);

  int curveTags[9] = {1,2,3,4,5,6,7,8,9};
  gmshModelOccAddWire(curveTags, 9, 1, 1, &ierr);

  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  gmshModelOccSynchronize(&ierr);

  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  int clampedCurveTags[3] = {1, 2, 9};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}

void chandelier(double r1, double r2, double r3, double e, double l1, double l2, double meshSizeFactor, char * filename) {
  /**
   * r1 = inner radius of lower circle (half-distance between prongs)
   * r2 = outer radius of lower circle (half-width of fork)
   * r3 = inner radius of upper circle 
   * e  = length of handle
   * l1  = length of inner branch prongs
   * l2 = length of outer branch prongs
   * meshSizeFactor = meshSize / width of prongs
   * if `filename` is not NULL, save to file
  */
  
  int ierr;

  gmshClear(&ierr);

  double h = r2 - r1; // width of prongs
  double r4 = r3 + h; // outer radius of upper circle
  double meshSize = h * meshSizeFactor;

  // Add points
  double x = 0;
  double y = 0;
  double z = 0;
  gmshModelOccAddPoint(x,y,z,meshSize,1,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,2,&ierr);
  y += e;
  gmshModelOccAddPoint(x,y,z,meshSize,3,&ierr);
  x += r2;
  y += r2;
  gmshModelOccAddPoint(x,y,z,meshSize,4,&ierr);
  y += l1;
  gmshModelOccAddPoint(x,y,z,meshSize,5,&ierr);

  // ------------------------------------------------
  x += r3;
  gmshModelOccAddPoint(x,y,z,meshSize,6,&ierr);
  y += r3;
  gmshModelOccAddPoint(x,y,z,meshSize,7,&ierr);
  x += r3;
  y -= r3;
  gmshModelOccAddPoint(x,y,z,meshSize,8,&ierr);
  y -= l2;
  gmshModelOccAddPoint(x,y,z,meshSize,9,&ierr);
  x += h;                                               // Modified.
  gmshModelOccAddPoint(x,y,z,meshSize,10,&ierr);   
  y += l2;
  gmshModelOccAddPoint(x,y,z,meshSize,11,&ierr);
  x -= r4;
  y += r4;
  gmshModelOccAddPoint(x,y,z,meshSize,12,&ierr);
  x -= r4;
  y -= r4;
  gmshModelOccAddPoint(x,y,z,meshSize,13,&ierr);
  // ------------------------------------------------

  y -= l1;
  gmshModelOccAddPoint(x,y,z,meshSize,14,&ierr);
  x -= r1;
  y -= r1;
  gmshModelOccAddPoint(x,y,z,meshSize,15,&ierr);
  x -= h;
  gmshModelOccAddPoint(x,y,z,meshSize,16,&ierr);
  x -= r1;
  y += r1;
  gmshModelOccAddPoint(x,y,z,meshSize,17,&ierr);
  y += l1;
  gmshModelOccAddPoint(x,y,z,meshSize,18,&ierr);
  
  // ------------------------------------------------
  x -= r4;
  y += r4;
  gmshModelOccAddPoint(x,y,z,meshSize,19,&ierr);
  x -= r4;
  y -= r4;
  gmshModelOccAddPoint(x,y,z,meshSize,20,&ierr);
  y -= l2;
  gmshModelOccAddPoint(x,y,z,meshSize,21,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,22,&ierr);
  y += l2;                                              // Modified.
  gmshModelOccAddPoint(x,y,z,meshSize,23,&ierr);  
  x += r3;
  gmshModelOccAddPoint(x,y,z,meshSize,24,&ierr);
  y += r3;
  gmshModelOccAddPoint(x,y,z,meshSize,25,&ierr);
  x += r3;
  y -= r3;
  gmshModelOccAddPoint(x,y,z,meshSize,26,&ierr);
  // ------------------------------------------------

  y -= l1;
  gmshModelOccAddPoint(x,y,z,meshSize,27,&ierr);
  x += r2;
  y -= r2;
  gmshModelOccAddPoint(x,y,z,meshSize,28,&ierr);
  y += (h+r1);
  gmshModelOccAddPoint(x,y,z,meshSize,29,&ierr);
  x += h;
  gmshModelOccAddPoint(x,y,z,meshSize,30,&ierr);
  
  // Add curves
  gmshModelOccAddLine(1,2,1,&ierr);
  gmshModelOccAddLine(2,3,2,&ierr);
  gmshModelOccAddCircleArc(3,30,4,3,&ierr);
  gmshModelOccAddLine(4,5,4,&ierr);

  // ------------------------------------------------
  gmshModelOccAddCircleArc(5,6,7,5,&ierr);
  gmshModelOccAddCircleArc(7,6,8,6,&ierr);
  gmshModelOccAddLine(7,8,7,&ierr);
  gmshModelOccAddLine(8,9,8,&ierr);
  gmshModelOccAddLine(9,10,9,&ierr);
  gmshModelOccAddLine(10,11,10,&ierr);
  gmshModelOccAddCircleArc(11,6,12,11,&ierr);
  gmshModelOccAddCircleArc(12,6,13,12,&ierr);
  // ------------------------------------------------

  gmshModelOccAddLine(13,14,13,&ierr);
  gmshModelOccAddCircleArc(14,30,15,14,&ierr);
  gmshModelOccAddLine(15,16,15,&ierr);
  gmshModelOccAddCircleArc(16,29,17,16,&ierr);
  gmshModelOccAddLine(17,18,17,&ierr);

  // ------------------------------------------------
  gmshModelOccAddCircleArc(18,24,19,18,&ierr);
  gmshModelOccAddCircleArc(19,24,20,19,&ierr);
  gmshModelOccAddLine(20,21,20,&ierr);
  gmshModelOccAddLine(21,22,21,&ierr);
  gmshModelOccAddLine(22,23,22,&ierr);
  gmshModelOccAddCircleArc(23,24,25,23,&ierr);
  gmshModelOccAddCircleArc(25,24,26,24,&ierr);
  // ------------------------------------------------

  gmshModelOccAddLine(26,27,25,&ierr);
  gmshModelOccAddCircleArc(27,29,28,26,&ierr);
  gmshModelOccAddLine(28,1,27,&ierr);

  // Add wire (closed curve)
  int curveTags[27] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};
  gmshModelOccAddWire(curveTags, 27, 1, 1, &ierr);

  fprintf(stderr, "\nAAA\n\n");

  // Add surface
  int wireTags[1] = {1};
  gmshModelOccAddPlaneSurface(wireTags, 1, 100, &ierr);

  // Sync
  gmshModelOccSynchronize(&ierr);

  // Create physical group for surface
  int surfaceTags[1] = {100};
  gmshModelAddPhysicalGroup(2, surfaceTags, 1, -1, "bulk", &ierr);

  // Create physical group for clamped curves
  int clampedCurveTags[3] = {1, 2, 14};
  gmshModelAddPhysicalGroup(1, clampedCurveTags, 3, -1, "clamped", &ierr);

  gmshModelMeshGenerate(2, &ierr);

  if(filename != NULL) gmshWrite(filename, &ierr);
}
