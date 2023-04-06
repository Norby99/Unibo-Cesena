# Elaborato HPC

## Compilazione

### OpenMP

Si può utilizzare il comando:

```
make omp
```

### MPI

Si può utilizzare il comando:

```
make omp
```

---

## Esecuzione

### OpenMP

Si può utilizzare il comando `make run-omp`, oppure il comando:

```
./omp-sph N S
```

### MPI

Si può utilizzare il comando `make run-mpi`, oppure il comando:

```
mpirun ./mpi-sph N S
```

> **_NOTE:_** `N` è il numero di particelle e `S` è il numero di step.
