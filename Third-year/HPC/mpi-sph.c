/****************************************************************************
 *
 * sph.c -- Smoothed Particle Hydrodynamics
 *
 * https://github.com/cerrno/mueller-sph
 *
 * Copyright (C) 2016 Lucas V. Schuermann
 * Copyright (C) 2022 Moreno Marzolla
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <mpi.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* "Particle-Based Fluid Simulation for Interactive Applications" by
   Müller et al. solver parameters */

const float Gx = 0.0, Gy = -10.0;   // external (gravitational) forces
const float REST_DENS = 300;    // rest density
const float GAS_CONST = 2000;   // const for equation of state
const float H = 16;             // kernel radius
const float EPS = 16;           // equal to H
const float MASS = 2.5;         // assume all particles have the same mass
const float VISC = 200;         // viscosity constant
const float DT = 0.0007;        // integration timestep
const float BOUND_DAMPING = -0.5;

// rendering projection parameters
// (the following ought to be "const float", but then the compiler
// would give an error because VIEW_WIDTH and VIEW_HEIGHT are
// initialized with non-literal expressions)
const int MAX_PARTICLES = 20000;
// Larger window size to accommodate more particles
#define WINDOW_WIDTH 3000
#define WINDOW_HEIGHT 2000

const int DAM_PARTICLES = 500;

const float VIEW_WIDTH = 1.5 * WINDOW_WIDTH;
const float VIEW_HEIGHT = 1.5 * WINDOW_HEIGHT;

/* Particle data structure; stores position, velocity, and force for
   integration stores density (rho) and pressure values for SPH.

   You may choose a different layout of the particles[] data structure
   to suit your needs. */
typedef struct {
    float x, y;         // position
    float vx, vy;       // velocity
    float fx, fy;       // force
    float rho, p;       // density, pressure
} particle_t;

particle_t *particles;
int n_particles = 0;    // number of currently active particles

/**
 * Return a random value in [a, b]
 */
float randab(float a, float b) {
    return a + (b-a)*rand() / (float)(RAND_MAX);
}

/**
 * Set initial position of particle `*p` to (x, y); initialize all
 * other attributes to default values (zeros).
 */
void init_particle( particle_t *p, float x, float y ) {
    p->x = x;
    p->y = y;
    p->vx = p->vy = 0.0;
    p->fx = p->fy = 0.0;
    p->rho = 0.0;
    p->p = 0.0;
}

/**
 * Return nonzero iff (x, y) is within the frame
 */
int is_in_domain( float x, float y ) {
    return ((x < VIEW_WIDTH - EPS) &&
            (x > EPS) &&
            (y < VIEW_HEIGHT - EPS) &&
            (y > EPS));
}

/**
 * Initialize the SPH model with `n` particles. The caller is
 * responsible for allocating the `particles[]` array of size
 * `MAX_PARTICLES`.
 *
 * DO NOT parallelize this function, since it calls rand() which is
 * not thread-safe.
 *
 * For MPI and OpenMP: only the master must initialize the domain;
 *
 * For CUDA: the CPU must initialize the domain.
 */
void init_sph( int n ) {
    n_particles = 0;
    printf("Initializing with %d particles\n", n);

    for (float y = EPS; y < VIEW_HEIGHT - EPS; y += H) {
        for (float x = EPS; x <= VIEW_WIDTH * 0.8f; x += H) {
            if (n_particles < n) {
                float jitter = rand() / (float)RAND_MAX;
                init_particle(particles + n_particles, x+jitter, y);
                n_particles++;
            } else {
                return;
            }
        }
    }
    assert(n_particles == n);
}

/**
 ** You may parallelize the following four functions
 **/

void compute_density_pressure( void ) {
    const float HSQ = H * H;    // radius^2 for optimization

    /* Smoothing kernels defined in Muller and their gradients adapted
       to 2D per "SPH Based Shallow Water Simulation" by Solenthaler
       et al. */
    const float POLY6 = 4.0 / (M_PI * pow(H, 8));

    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Distribuire l'array di puntatori delle particelle tra i processi
    particle_t **local_particles = (particle_t**)malloc(n_particles * sizeof(particle_t*));
    MPI_Scatter(particles, n_particles, MPI_PTR, local_particles, n_particles, MPI_PTR, 0, MPI_COMM_WORLD);
    
    // Calcolare la densità e la pressione per le particelle assegnate a ogni processo
    for (int i=rank; i<n_particles; i+=size) {
        particle_t *pi = local_particles[i];
        pi->rho = 0.0;
        for (int j=0; j<n_particles; j++) {
            const particle_t *pj = particles[j];
            const float dx = pj->x - pi->x;
            const float dy = pj->y - pi->y;
            const float d2 = dx*dx + dy*dy;
            if (d2 < HSQ) {
                pi->rho += MASS * POLY6 * pow(HSQ - d2, 3.0);
            }
        }
        pi->p = GAS_CONST * (pi->rho - REST_DENS);
    }

    // Raccogliere i risultati parziali dai processi e aggiornare i dati delle particelle
    MPI_Gather(local_particles, n_particles, MPI_PTR, particles, n_particles, MPI_PTR, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();
    free(local_particles);

}

void compute_forces( void ) {
    /* Smoothing kernels defined in Muller and their gradients adapted
       to 2D per "SPH Based Shallow Water Simulation" by Solenthaler
       et al. */
    const float SPIKY_GRAD = -10.0 / (M_PI * pow(H, 5));
    const float VISC_LAP = 40.0 / (M_PI * pow(H, 5));
    const float EPS = 1e-6;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n_particles_per_proc = (n_particles + size - 1) / size;
    int start = rank * n_particles_per_proc;
    int end = (rank + 1) * n_particles_per_proc;
    if (end > n_particles) {
        end = n_particles;
    }

    for (int i=start; i<end; i++) {
        particle_t *pi = &particles[i];
        float fpress_x = 0.0, fpress_y = 0.0;
        float fvisc_x = 0.0, fvisc_y = 0.0;

        for (int j=0; j<n_particles; j++) {
            const particle_t *pj = &particles[j];

            if (pi == pj)
                continue;

            const float dx = pj->x - pi->x;
            const float dy = pj->y - pi->y;
            const float dist = hypotf(dx, dy) + EPS; // avoids division by zero later on

            if (dist < H) {
                const float norm_dx = dx / dist;
                const float norm_dy = dy / dist;
                // compute pressure force contribution
                fpress_x += -norm_dx * MASS * (pi->p + pj->p) / (2 * pj->rho) * SPIKY_GRAD * pow(H - dist, 3);
                fpress_y += -norm_dy * MASS * (pi->p + pj->p) / (2 * pj->rho) * SPIKY_GRAD * pow(H - dist, 3);
                // compute viscosity force contribution
                fvisc_x += VISC * MASS * (pj->vx - pi->vx) / pj->rho * VISC_LAP * (H - dist);
                fvisc_y += VISC * MASS * (pj->vy - pi->vy) / pj->rho * VISC_LAP * (H - dist);
            }
        }
        const float fgrav_x = Gx * MASS / pi->rho;
        const float fgrav_y = Gy * MASS / pi->rho;
        pi->fx = fpress_x + fvisc_x + fgrav_x;
        pi->fy = fpress_y + fvisc_y + fgrav_y;
    }

    // collect the updated particle data from all processes
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, particles, n_particles, PARTICLE_MPI_TYPE, MPI_COMM_WORLD);
}

void integrate( void ) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n_particles_local = n_particles / size;
    int start_idx = rank * n_particles_local;
    int end_idx = (rank + 1) * n_particles_local;
    if (rank == size - 1) {
        end_idx = n_particles;
    }

    for (int i = start_idx; i < end_idx; i++) {
        particle_t *p = &particles[i];
        // forward Euler integration
        p->vx += DT * p->fx / p->rho;
        p->vy += DT * p->fy / p->rho;
        p->x += DT * p->vx;
        p->y += DT * p->vy;

        // enforce boundary conditions
        if (p->x - EPS < 0.0) {
            p->vx *= BOUND_DAMPING;
            p->x = EPS;
        }
        if (p->x + EPS > VIEW_WIDTH) {
            p->vx *= BOUND_DAMPING;
            p->x = VIEW_WIDTH - EPS;
        }
        if (p->y - EPS < 0.0) {
            p->vy *= BOUND_DAMPING;
            p->y = EPS;
        }
        if (p->y + EPS > VIEW_HEIGHT) {
            p->vy *= BOUND_DAMPING;
            p->y = VIEW_HEIGHT - EPS;
        }
    }

    // Allgather particles array to ensure each process has the updated positions and velocities
    MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, particles, n_particles_local, particle_type, MPI_COMM_WORLD);
}

float avg_velocities( void ) {
    double local_result = 0.0;
    double result = 0.0;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = n_particles / size;
    int chunk_start = rank * chunk_size;
    int chunk_end = (rank == size-1) ? n_particles : chunk_start + chunk_size;

    for (int i = chunk_start; i < chunk_end; i++) {
        local_result += hypot(particles[i].vx, particles[i].vy) / chunk_size;
    }

    MPI_Reduce(&local_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        result /= n_particles;
    }

    return result;
}

void update( void ) {
    compute_density_pressure();
    compute_forces();
    integrate();
}

int main(int argc, char **argv) {
    int radius, my_rank, comm_sz;
    int n = DAM_PARTICLES;
    int nsteps = 50;

    double t_start;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // initial setup
    if (my_rank == 0) {
        srand(1234);

        particles = (particle_t*)malloc(MAX_PARTICLES * sizeof(*particles));
        assert( particles != NULL );

        if (argc > 3) {
            fprintf(stderr, "Usage: %s [nparticles [nsteps]]\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        if (argc > 1) {
            n = atoi(argv[1]);
        }

        if (argc > 2) {
            nsteps = atoi(argv[2]);
        }

        if (n > MAX_PARTICLES) {
            fprintf(stderr, "FATAL: the maximum number of particles is %d\n", MAX_PARTICLES);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        init_sph(n);
    }

    if (my_rank == 0) {
        double start = MPI_Wtime();
    }
    
    for (int s=0; s<nsteps; s++) {
        update();
        /* the average velocities MUST be computed at each step, even
           if it is not shown (to ensure constant workload per
           iteration) */
        const float avg = avg_velocities();
        if (my_rank == 0 && s % 10 == 0)
            printf("step %5d, avgV=%f\n", s, avg);
    }
    if (my_rank == 0) {
        printf("elapsed time: %f seconds", MPI_Wtime() - start);
    }

    free(particles);
    return EXIT_SUCCESS;
}
