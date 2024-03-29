/****************************************************************************
 *
 * Gabos Norbert - 0000970451
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
#include <string.h>
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

// create a new MPI data type for the particle_t struct to be used in communication
MPI_Datatype MPI_PARTICLE;

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

int my_rank, comm_sz;

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
 * Sincronize the particles between the processes
 * @param start the start index of the particles to be sent
 * @param end the end index of the particles to be sent
 * @param chunk_size the size of the chunk of particles to be sent
 */
void sync_particles(int start, int end, int chunk_size) {
    int remainder = n_particles % comm_sz;

    int *recvcounts = malloc(comm_sz * sizeof(int));
    int *displs = malloc(comm_sz * sizeof(int));
    int displacement = 0;
    for (int i = 0; i < comm_sz; i++) {
        recvcounts[i] = chunk_size + (i < remainder);
        displs[i] = displacement;
        displacement += recvcounts[i];
    }

    MPI_Allgatherv(
                    &particles[start],  // sendbuf
                    end - start,        // sendcnts
                    MPI_PARTICLE,       // sendtype
                    particles,          // recvbuf
                    recvcounts,         // recvcnts
                    displs,             // displs
                    MPI_PARTICLE,       // recvtype
                    MPI_COMM_WORLD      // comm
    );

    free(recvcounts);
    free(displs);
}

/**
 * Compute the density and pressure of each particle
*/
void compute_density_pressure(int start, int end, int chunk_size) {
    const float HSQ = H * H;    // radius^2 for optimization

    /* Smoothing kernels defined in Muller and their gradients adapted
       to 2D per "SPH Based Shallow Water Simulation" by Solenthaler
       et al. */
    const float POLY6 = 4.0 / (M_PI * pow(H, 8));

    for (int i = start; i < end; i++) {
        particle_t *pi = &particles[i];
        pi->rho = 0.0;
        for (int j = 0; j < n_particles; j++) {
            const particle_t *pj = &particles[j];

            const float dx = pj->x - pi->x;
            const float dy = pj->y - pi->y;
            const float d2 = dx*dx + dy*dy;

            if (d2 < HSQ) {
                pi->rho += MASS * POLY6 * pow(HSQ - d2, 3.0);
            }
        }
        pi->p = GAS_CONST * (pi->rho - REST_DENS);
    }

    sync_particles(start, end, chunk_size);
}

/**
 * Compute the forces on each particle
*/
void compute_forces(int start, int end, int chunk_size) {
    /* Smoothing kernels defined in Muller and their gradients adapted
       to 2D per "SPH Based Shallow Water Simulation" by Solenthaler
       et al. */
    const float SPIKY_GRAD = -10.0 / (M_PI * pow(H, 5));
    const float VISC_LAP = 40.0 / (M_PI * pow(H, 5));
    const float EPS = 1e-6;

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

    sync_particles(start, end, chunk_size);
}

/**
 * Integrate the particles forward in time
*/
void integrate(int start, int end, int chunk_size) {
    for (int i = start; i < end; i++) {
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

    sync_particles(start, end, chunk_size);
}

/**
 * Compute the average velocity of all particles
*/
float avg_velocities(int start, int end) {
    double local_result = 0.0;
    double result = 0.0;

    for (int i = start; i < end; i++) {
        local_result += hypot(particles[i].vx, particles[i].vy) / n_particles;
    }

    // reduce the local results to the root process
    MPI_Allreduce(
                    &local_result,              // sendbuf
                    &result,                    // recvbuf
                    1,                          // count
                    MPI_DOUBLE,                 // type
                    MPI_SUM, MPI_COMM_WORLD     // comm
    );

    return result;
}

void update(int start, int end, int chunk_size) {
    compute_density_pressure(start, end, chunk_size);
    compute_forces(start, end, chunk_size);
    integrate(start, end, chunk_size);
}

int main(int argc, char **argv) {
    int n = DAM_PARTICLES;
    int nsteps = 50;

    double t_start;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    particles = (particle_t*)malloc(MAX_PARTICLES * sizeof(*particles));
    assert( particles != NULL );

    // initial setup
    if (my_rank == 0) {
        srand(1234);

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

    // initialize MPI particle type
    MPI_Type_contiguous(8, MPI_FLOAT, &MPI_PARTICLE);
    MPI_Type_commit(&MPI_PARTICLE);

    MPI_Bcast(
                &n_particles,   // sendbuf
                1,              // count
                MPI_INT,        // type
                0,              // root
                MPI_COMM_WORLD  // comm
    );
    MPI_Bcast(
                &nsteps,        // sendbuf
                1,              // count
                MPI_INT,        // type
                0,              // root
                MPI_COMM_WORLD  // comm
    );
    MPI_Bcast(
                particles,      // sendbuf
                n_particles,    // count
                MPI_PARTICLE,   // type
                0,              // root
                MPI_COMM_WORLD  // comm
    );

    int chunk_size = n_particles / comm_sz;
    int remainder = n_particles % comm_sz;
    int start = my_rank * chunk_size + (my_rank < remainder ? my_rank : remainder);
    int end = start + chunk_size + (my_rank < remainder);

    if (my_rank == 0) {
        t_start = MPI_Wtime();
    }
    
    for (int s=0; s<nsteps; s++) {
        update(start, end, chunk_size);
        
        const float avg = avg_velocities(start, end);
        if (my_rank == 0 && s % 10 == 0) {
            printf("step %5d, avgV=%f\n", s, avg);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (my_rank == 0) {
        printf("elapsed time: %f seconds", MPI_Wtime() - t_start);
    }

    free(particles);
    MPI_Type_free(&MPI_PARTICLE);
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}
