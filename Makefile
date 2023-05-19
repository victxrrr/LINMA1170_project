CC := gcc
CFLAGS := -Wall

optimize: util/matrix.c util/lu.c util/elasticity.c util/eigen.c geometries/design.c util/dichotomous.c util/nelderMead.c main/optimize.c
	$(CC) $(CFLAGS) -o $@ $^ -lm ../gmsh-sdk/lib/libgmsh.so -Wl,-rpath,../gmsh-sdk/lib -llapacke
	./$@ files/paramsOutput.txt
	rm -f $@

mode: util/matrix.c util/lu.c util/elasticity.c util/eigen.c geometries/design.c main/mode.c
	$(CC) $(CFLAGS) -o $@ $^ -lm -Wno-unused-variable -Wno-unused-function ../gmsh-sdk/lib/libgmsh.so -Wl,-rpath,../gmsh-sdk/lib
	./$@ 2 files/paramsHarmonic.txt
	rm -f $@


clean:
	rm -f *.o
	rm -f all
