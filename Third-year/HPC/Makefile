# Makefile for the High Performance Computing programming project,compute_dens
# Academic Year 2022/2023.
#
# Available targets:
#
# - sph: builds the non-GUI version (default)
#
# - all: builds both the GUI and non-GUI versions
#
# - clean: clean up
#

EXE:=sph
CFLAGS+=-std=c99 -Wall -Wpedantic
LDLIBS=-lm
VARS=1000 200

.PHONY: clean

sph: sph.c

all: $(EXE)

omp: omp-sph.c
	$(CC) $(CFLAGS) -fopenmp -o omp-sph $< $(LDLIBS)

mpi: mpi-sph.c
	mpicc $(CFLAGS) -o mpi-sph $< $(LDLIBS)

run-omp: omp
	./omp-sph $(VARS)

run-mpi: mpi
	mpirun ./mpi-sph $(VARS)

clear:
	\rm -f $(EXE) omp-sph *.o *~
