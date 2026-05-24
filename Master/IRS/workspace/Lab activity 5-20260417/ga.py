import random
import math
import statistics
import os
import subprocess

###
#5 Question: what is the role of n eval? What happens if you set it to 1?
# The number of times a genome is evaluated to compute its fitness. If set to 1, the fitness will be more noisy and less reliable, since it will depend on the specific run of the simulation. By evaluating multiple times and taking the average (or min), we can get a more robust estimate of the fitness of a genome.

#6 What happens if you set n eval to 3 and use the min instead of the mean of the fitness values?
# Dipendo meno da valori anomali, ma non è comunque il modello migliore nella media.

#7 Consider the following parameter configuration:
#GENOME_LENGTH = 50
# POP_SIZE = 10
# GENERATIONS = 5
# MUTATION_RATE = 0.1 # prob of mutating each gene
# CX_RATE = 0.5 # prob of applying crossover
# MUTATION_INTENSITY = 1 # stddev of a Gaussian distribution with mean 0
# n_eval = 3

# test and compare on 5 runs the cases with CX RATE∈ {0, 0.5, 1} using median and means.
# Fa schifo perche gira solo per 5 generazioni, inoltre con il fatto che la crossover viene applicata meno spesso la popolazione non evolve molto.
# anche se il crossover è 0, i risultati dello stesso genoma possono variare, perche i robottini cambiano di posto quando vengono inizializzati.
# forse l'ultima cosa che ho scritto è sbagliata

# 9 Run with the following parameter setting:
# GENOME_LENGTH = 50
# POP_SIZE = 10
# ELITE_SIZE = 5
# GENERATIONS = 5
# MUTATION_RATE = 0.1 # prob of mutating each gene
# CX_RATE = 0 # prob of applying crossover
# MUTATION_INTENSITY = 1 # stddev of a Gaussian distribution with mean 0
# n_eval = 3

# and compare replacement with elitism (5 replicas each and compare medians and means).

# Problema simile al punto 7, la popolazione evolve poco a causa di CX rate a 0, ma ho notato che usando la fitness mediana tende a convergere più velocemten.
# Inoltre per gioco ho notato che usando una fitness di tipo min ottenevo risultato eccellenti, penso sia dovuto al fatto che cosi facendo si favoriscono i genomi che sono più stabili e meno soggetti a colpi di fortuna, magari dovuti al rumore.

# 11 Compare tournament selection and roulette wheel selection operators—using the previous parameter and experimental setting.
# Comparing the two operators on just 5 generations showed minor empirical differences, heavily masked by the high elitism rate (50% of the population). However, theoretically and observing the logs, Tournament selection exhibits a more consistent selection pressure when fitness variance is low (early generations), whereas Roulette Wheel selection risks premature convergence if an artificially high-fitness 'super-individual' emerges early on due to initial noise.

#? Idee
# - fare crossover solo quando si ha una fitness bassa (es. sotto 0.5)

# per la avoidance: bisogna raddioppiare il nuemro di parametri da 24 a 48, mentre i bias rimangono gli stessi.
# inoltre bisogna aggiugnere i sensori ai parametri e dargli una penalità se collidono (dire che è difficile capire se c'è collisione
# perche non posso fare un check ogni step.). Comunque è difficile da far fuznionare perche i robot tendono a girare su se stessi.
# una soluzione funzionava, ma non benissimo.

#? TL:DR:
# C'è stato un piccolo bug in ga.py con CX_RATE=0, risolto da gemini.
# In pratica crossover restituiva semplicemente una copia del genitore, e poi mutate modificava 
# in-place il figlio, alterando involontariamente anche il genitore a causa del passaggio per
# riferimento delle liste in Python. Questo faceva sì che la popolazione non evolvesse e i robot
# girassero su se stessi senza migliorare.

###

GENOME_LENGTH = 50
POP_SIZE = 10
ELITE_SIZE = 5
GENERATIONS = 5
MUTATION_RATE = 0.1 # prob of mutating each gene
CX_RATE = 0.0 # prob of applying crossover
MUTATION_INTENSITY = 1 # stddev of a Gaussian distribution with mean 0
N_EVAL = 3  #! quante volte viene valutato un genoma per calcolare la fitness (media)

USE_ELITISM = True # whether to use elitism or standard replacement

# Create random individual
def create_individual():
    return [random.uniform(-1,1) for _ in range(GENOME_LENGTH)]

# Compute fitness by evaluating the robot (N_EVAL times)
def fitness(individual):
    os.environ["GENOME"] = ",".join(map(str,individual))
    fvalue = []
    for _ in range(N_EVAL):
        proc = subprocess.Popen(
            ["argos3", "-c", "evaluate-controller-nn.argos"],
            stdout=subprocess.PIPE,
            text=True
        )
        for line in proc.stdout:
            if "FITNESS:" in line:
                fitness = str(line.strip().split(":")[1])
                fitness = float(fitness.replace(",", "."))
        fvalue.append(fitness)
    #fitness = statistics.median(fvalue)
    #fitness = statistics.mean(fvalue)
    fitness = min(fvalue)
    return fitness

# Selection: tournament
# Randomly pick 2 individuals and return the one with the highest fitness
def select_tournament(population,fitness_values):
    tournament_size = 3
    tournament = random.sample(list(enumerate(population)), tournament_size)
    winner = max(tournament, key=lambda x: fitness_values[x[0]])
    return winner[1]

# Selection: roulette wheel (proportional)
def select_proportional(population,fitness_values):
    total_fitness = sum(fitness_values)
    r = random.random() * total_fitness
    if r > total_fitness:
        r = total_fitness
    i = 0
    mysum = fitness_values[i]
    while mysum <= r:
        i += 1
        mysum += fitness_values[i]
    return population[i]

# Crossover: linear combination
def crossover(parent1, parent2):
    if random.random() <= CX_RATE:
        alpha = random.random()
        return [
            alpha * x + (1 - alpha) * y
            for x,y in zip(parent1,parent2)
        ]
    else:
        return list(parent1) if random.random() <= 0.5 else list(parent2)

# Mutation: Gaussian noise
# each gene has a probability MUTATION_RATE to be changed
# random.random() : rnd number uniformly distributed in [0,1]
# random.gauss(0,MUTATION_INTENSITY) : rnd number in normal distrib with mean=0 and stddev=MUTATION_INTENSITY
def mutate(individual):
    mutated = list(individual)
    for i in range(GENOME_LENGTH):
        if random.random() <= MUTATION_RATE:
            mutated[i] += random.gauss(0, MUTATION_INTENSITY)
    return mutated

def print_stats(pop,fv):
    idx = max(range(POP_SIZE), key=lambda i: fv[i])
    best = pop[idx]
    best_fit = fv[idx]

    print("Gen", gen, ": Best fitness =", round(best_fit,4))
    print("Best solution:", best)
    print("\n")
    return [round(best_fit,4), best]
    

# Main GA loop
population = [create_individual() for _ in range(POP_SIZE)]

#print("Random solution:", population[0])

best_model = {"fitness": 0, "genome": None}
for gen in range(GENERATIONS):
    fitness_values = [fitness(i) for i in population]
    res_best_fit, res_best_sol = print_stats(population,fitness_values)
    
    if USE_ELITISM:
        # Elitism: retain the top ELITE_SIZE individuals
        elite_indices = sorted(range(POP_SIZE), key=lambda i: fitness_values[i], reverse=True)[:ELITE_SIZE]
        new_population = [list(population[i]) for i in elite_indices]
        offspring_count = POP_SIZE - ELITE_SIZE
    else:
        # Replacement: create a completely new population from the current one
        new_population = []
        offspring_count = POP_SIZE
    
    for _ in range(offspring_count):
        selection_alg = select_proportional
        #selection_alg = select_tournament

        parent1 = selection_alg(population,fitness_values)
        parent2 = selection_alg(population,fitness_values)
        child = crossover(parent1, parent2)
        child = mutate(child)
        new_population.append(child)

    population = new_population

    if res_best_fit > best_model["fitness"]:
        best_model["fitness"] = res_best_fit
        best_model["genome"] = res_best_sol

print("Best model found:", best_model)
