//
// Created by mikolaj on 09.01.2021.
//

#include "GeneticTSP.h"

GeneticTSP::GeneticTSP(Graph *graph, unsigned populationSize, unsigned duration, float mutationProb, float crossingProb,
                       CrossingMethod crossingMethod) {
    this->populationSize = populationSize;
    this->duration = duration;
    this->mutationProb = mutationProb;
    this->crossingProb = crossingProb;
    this->crossingMethod = crossingMethod;
    this->graph = graph;
    this->durationUntilFound = 0;

    bestPath = new Path(graph->getNodesCount(), graph);
    for (int i = 0; i < graph->getNodesCount(); i++) {
        (*bestPath)[i] = i;
    }

    populationInit();
}

void GeneticTSP::populationInit() {
    Path path = *bestPath;
    int min = path.cost();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> nodes_dist(0, graph->getNodesCount() - 1);

    for (int i = 1; i <= populationSize; i++) {
        population.push_back(path);

        int c = path.cost();
        if (c < min) {
            *bestPath = path;
            min = c;
        }

        randomShuffle(path, nodes_dist, gen);
    }
}

void GeneticTSP::solve() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> nodes_dist(0, graph->getNodesCount() - 1);
    std::uniform_int_distribution<> population_dist(0, populationSize - 1);
    std::uniform_real_distribution<> probability_dist(0.0, 1.0);

    high_resolution_clock::time_point start;
    high_resolution_clock::time_point stop;

    start = high_resolution_clock::now();

    do {
        for(int parent1 = 0; parent1 < populationSize; parent1++)
        {
            for(int parent2 = parent1 + 1; parent2 < populationSize; parent2++)
            {
                if (probability_dist(gen) < crossingProb) {

                    switch (crossingMethod) {
                        case PMX: {
                            int a, b;
                            randomRange(gen, nodes_dist, a, b);

                            pmx(population, population.at(parent1), population.at(parent2), a, b);
                            break;
                        }
                        case CX:
                            cx(population, population.at(parent1), population.at(parent2));
                            break;
                    }
                }
            }
        }

        for(int i = 0; i < population.size(); i++)
        {
            if(probability_dist(gen) < mutationProb) {
                int a, b;
                randomRange(gen, nodes_dist, a, b);

                inversion(population.at(i), a, b);
            }
        }

        std::sort(population.begin(), population.end(), pathCompare);

        printf("best: %d\n", population.at(0).cost());


        while (population.size() > getPopulationSize()) {
            population.pop_back();
        }

        if((&population.at(0))->cost() < bestPath->cost()) {
            durationUntilFound = duration_cast<seconds>(high_resolution_clock::now() - start).count();
            *bestPath = population.at(0);
        }


        stop = high_resolution_clock::now();
    } while (duration_cast<seconds>(stop - start).count() <= duration);

}


bool GeneticTSP::pathCompare(const Path &x, const Path &y) {
    return x.cost() < y.cost();
}


void GeneticTSP::randomShuffle(Path &path, std::uniform_int_distribution<> &dist, std::mt19937 &gen) {
    bool *node_used = new bool[path.getSize()];
    memset(node_used, false, sizeof(*node_used) * path.getSize());

    int v;
    for (int i = 0; i < path.getSize(); i++) {
        do {
            v = dist(gen);
        } while (node_used[v]);

        path[i] = v;
        node_used[v] = true;
    }

    delete[] node_used;
}

void GeneticTSP::randomRange(std::mt19937 gen, std::uniform_int_distribution<> nodes_dist, int &n1, int &n2) {
    n1 = nodes_dist(gen);

    do {
        n2 = nodes_dist(gen);
    } while (n2 == n1);

    if (n1 > n2)
        std::swap(n1, n2);
}

void GeneticTSP::inversion(Path &path, int start, int end) {
    for (int i = 0; i < (end - start) / 2; i++) {
        std::swap(path[start + i], path[end - i]);
    }
}

void GeneticTSP::pmx(std::vector<Path> &population, Path &parent1, Path &parent2, int start, int end) {
    Path child1 = parent1;
    Path child2 = parent2;

    for(int i = 0; i < parent1.getSize(); i++)
    {
        if(start <= i && i <= end) {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        } else {
            child1[i] = -1;
            child2[i] = -1;
        }
    }

    for(int i = 0; i < parent1.getSize(); i++)
    {
        if(i == start)
        {
            i = end;
            continue;
        }

        int t = parent1[i];
        while (child1.contains(t)) {
            t = child2[child1.find(t)];
        }

        child1[i] = t;
    }

    for(int i = 0; i < parent2.getSize(); i++)
    {
        if(i == start)
        {
            i = end;
            continue;
        }

        int t = parent2[i];
        while (child2.contains(t)) {
            t = child1[child2.find(t)];
        }

        child2[i] = t;
    }

    population.push_back(child1);
    population.push_back(child2);
}

void GeneticTSP::cx(std::vector<Path> &population, Path &parent1, Path &parent2)
{
    Path child1 = parent1;
    Path child2 = parent2;

    child1[0] = parent1[0];
    child2[0] = parent2[0];

    for(int i = 1; i < parent1.getSize(); i++)
    {
        child1[i] = -1;
        child2[i] = -1;
    }

    int i = 0, j;

    // If contains, we found a cycle.
    while(!child1.contains(parent2[i])) {
        j = parent1.find(parent2[i]);
        child1[j] = parent1[j];
        child2[j] = parent2[j];
        i = j;
    }

    for(int i = 0; i < child1.getSize(); i++)
    {
        if(child1[i] == -1) {
            child1[i] = parent2[i];
            child2[i] = parent1[i];
        }
    }

    population.push_back(child1);
    population.push_back(child2);
}

unsigned int GeneticTSP::getDurationUntilFound() const {
    return durationUntilFound;
}
