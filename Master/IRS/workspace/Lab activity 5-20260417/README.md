## Introduction

The objective of this lab is to experiment with the genome evolution techniques presented in class by applying them to the phototaxis problem.

## Observations

The `n_eval` parameter plays a crucial role in individual evaluation. Setting it to 1 makes the fitness score highly susceptible to simulation noise and the robot's initial conditions. Evaluating the genome multiple times yields far more reliable estimates. Furthermore, aggregating these evaluations using the minimum value instead of the mean penalizes anomalous positive fluctuations. This approach prevents the genetic algorithm from rewarding lucky runs, instead favoring solutions that are intrinsically stable and robust against environmental variations. This intuition was empirically validated, demonstrating that a minimum-based fitness function yields excellent results regarding behavioral reliability.

When testing different crossover rates (CX_RATE) over just 5 generations, the evolutionary process is severely bottlenecked by the short timeframe. With low or zero crossover rates, population exploration is minimal. It is worth noting that even with a crossover rate of zero, the fitness results of the exact same genome can vary across evaluations due to the stochastic positioning of the foot-bots upon initialization and the simulator's inherent noise.

Implementing elitism ensures that the top-performing genomes are preserved, but a high elite size (50% of the population) combined with a zero crossover rate causes genetic diversity to stagnate. While utilizing the median fitness showed a tendency to converge more rapidly toward sub-optimal solutions, the robustness guaranteed by the minimum fitness function proved superior.

Finally, comparing the selection operators `Tournament selection` and `Roulette Wheel selection` revealed minor empirical differences over just 5 generations. This outcome was heavily masked by the aggressive elitism rate. Theoretically and through log observation, however, Tournament selection demonstrates a more consistent selection pressure when fitness variance is low in early generations. It effectively mitigates the risk of premature convergence inherent to Roulette Wheel selection, which is prone to over-selecting a single 'super-individual' that happens to score artificially high due to initial simulation noise.

## Data evaluation

### Point 4
| Gaussian eval=3 mean |
| :---: |
| ![Point 4](/images/point4.png) |

### Point 6
| Gaussian eval=3 min |
| :---: |
| ![Point 6](/images/point6.png) |

### Point 7
| CX_RATE=1 - mean | CX_RATE=1 - median | CX_RATE=0.5 - mean | CX_RATE=0.5 - median | CX_RATE=0 - mean | CX_RATE=0 - median |
| :---: | :---: | :---: | :---: | :---: | :---: |
| ![Point 7 - CX_RATE=1 - mean](/images/point7-1-mean.png) | ![Point 7 - CX_RATE=1 - median](/images/point7-1-median.png) | ![Point 7 - CX_RATE=0.5 - mean](/images/point7-05-mean.png) | ![Point 7 - CX_RATE=0.5 - median](/images/point7-05-median.png) | ![Point 7 - CX_RATE=0 - mean](/images/point7-0-mean.png) | ![Point 7 - CX_RATE=0 - median](/images/point7-0-median.png) |

### Point 9
| replacement - mean | replacement - median | elitism - mean | elitism - median |
| :---: | :---: | :---: | :---: |
| ![Point 9 - replacement - mean](/images/point7-0-mean.png) | ![Point 9 - replacement - median](/images/point7-0-median.png) |![Point 9 - elitism - mean](/images/point9-e-mean.png) | ![Point 9 - elitism - median](/images/point9-e-median.png)|

### Point 11
| tournament-selection - mean | tournament-selection - median | roulette_wheel-selection - mean | roulette_wheel-selection - median |
| :---: | :---: | :---: | :---: |
| ![Point 11 - tournament-selection - mean](/images/point7-0-mean.png) | ![Point 11 - tournament-selection - median](/images/point7-0-median.png) |![Point 11 - roulette_wheel-selection - mean](/images/point11-r-mean.png) | ![Point 11 - roulette_wheel-selection - median](/images/point11-r-median.png)|



## Food for though

The naive comparisons conducted during this short experimentation highlight that small population sizes and limited generations make the evolutionary process highly susceptible to stochastic noise, underscoring the necessity of more replicas and statistical testing for any principled analysis. To further improve the algorithm, future setups should scale up the population size and generation count, alongside introducing adaptive mutation rates or rank-based selection. Additionally, convergence could be accelerated by applying crossover conditionally—for instance, only triggering the operator when a parent's fitness falls below a specific threshold (e.g., 0.5)—thereby dynamically shifting the balance toward exploration only when the current solutions are underperforming.

Transitioning the target behavior to pure collision avoidance requires replacing the 24 light sensors with 24 proximity sensors as the network inputs, while keeping the biases constant. The fitness function must also be modified to introduce a penalty for collisions. However, this behavior is notoriously difficult to evolve successfully in practice, as the genetic algorithm often gets trapped in local optima where the robots simply spin in circles to avoid moving forward into obstacles.

To extend the behavior to combined phototaxis with collision avoidance, the architecture must be expanded to 48 input nodes to accommodate both the 24 light sensors and the 24 proximity sensors simultaneously, keeping the biases constant. The primary complexity here lies in designing a multi-objective fitness function that balances two potentially conflicting goals. This could be implemented via a weighted sum that rewards proximity to the light source while penalizing closeness to obstacles. Tuning these weights is highly sensitive, beacouse incorrect calibration often drives the genetic algorithm toward conservative local optima, causing the robots to stop or spin in place to avoid collisions, thereby completely ignoring the primary task of phototaxis.
