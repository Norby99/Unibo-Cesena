# Introduction

In this lab, we look at how collective aggregation works in swarm robotics. The goal is to build and test a probabilistic control model to see how these grouping behaviors actually play out in a simulated environment.

# Excercise 1

For this first part, the idea was to have robots stop at random, with the catch that their chances of stopping increase if there are already other stationary robots nearby. Conversely, the odds of a stopped robot starting to move again drop depending on the number of stopped neighbors.

Naturally, this leads to robots forming clusters. The more crowded a zone gets, the longer the cluster sticks around. Going into this, I assumed that if we let the simulation run long enough, we’d eventually just get one massive clump containing all the robots.

However, because the robots also run obstacle avoidance routines, they tend to turn and move away as they get too close to an existing cluster. On top of that, robots on the outer edges of a group have fewer neighbors, meaning they’re much more likely to wander off. Ultimately, instead of one giant blob, the simulation settles into one main cluster surrounded by several smaller ones.

# Excercise 2

Next, we tweaked the probabilities so that robots are more likely to hit the brakes when they drive over a dark spot in the arena.

The outcome was pretty similar to the first exercise, but with a predictable twist: the largest cluster now naturally gravitates toward and forms over that dark spot.

# Excercise 3

Building on the previous logic, we added a second dark spot to the arena to see how the swarm would react.

At first, they just form scattered, random clusters everywhere. But as the iterations go on, the swarm usually ends up splitting pretty evenly, with half on one spot and half on the other. Occasionally, you'll see a strong preference for just one of the spots, but that really only happens if a huge group randomly decides to stop on it right at the beginning of the run.

# Food for thought

## Is the behaviour robust wrt parameter values?

The system is somewhat robust—thanks to the underlying probabilities, you can tweak the parameters a bit without breaking everything. That said, it's incredibly sensitive if you mess up the balance of the feedback loops. For example, if the exploration variables ($W$ or $\beta$) are set way too high compared to the exploitation ones ($S$ or $\alpha$), negative feedback takes over. When that happens, clusters break apart almost instantly, and stable aggregation becomes impossible. So, the system's robustness really depends on maintaining a tight balance between the forces pulling the robots together and the ones pushing them apart.

## What are the advantages and disadvantages of a collective choice?

The biggest perk of collective choice is that the swarm can reach a global consensus strictly through local interactions. There's no need for a central leader, which helps keep the group cohesive. The downside, though, is that the swarm can easily get stuck in deadlocks or make sub-optimal decisions. For instance, if they split perfectly between two identical spots, it can take forever to break that symmetry. Plus, once the positive feedback loop kicks in, it's very hard for the swarm to back out of a premature or poor choice.

## How would you bias the aggregation on a specific area of the arena?

You can bias the aggregation using an environmental cue, exactly like the black spot in Exercise 2. From a math perspective, you just adjust the probability rules based on what the floor sensors read. By dropping in an environmental bias (like $D_s$ and $D_w$), you artificially boost the chances of a robot stopping—or drop its chances of leaving—when it detects the target area. Essentially, it safely "traps" the robots there, firing up the positive feedback loop of aggregation right where you want it.

## What are pros and cons of distributed solutions wrt centralised ones?

Scalability is the real winner for distributed solutions. Since individuals only care about their immediate neighbors, the control algorithm handles a massive swarm just as efficiently as a small one. It's also highly fault-tolerant since there's no single point of failure, and it keeps the hardware and processing needs for each individual robot incredibly simple.

On the flip side, you lose a lot of predictability and direct control. It's really hard to mathematically guarantee when (or if) the swarm will converge on a target. It's also fairly time-inefficient; a centralized system would be much faster and more direct compared to sitting around waiting for the desired behavior to naturally emerge from random walks.

### Note

For the implementation of the different exercises, three distinct ARGOS configuration files were created.
They all share the exact same controller and underlying logic. The only variation among them is the additional probability of stopping on the dark spot. This modification does not affect Exercise 1 in any way, as these additional probabilities are simply evaluated as zero when no black spots are present in the arena, thereby preserving the integrity of the baseline aggregation behavior. 
