//
// Created by mikolaj on 09.01.2021.
//

#ifndef SIMULATED_ANNEALING_GENETICTSP_H
#define SIMULATED_ANNEALING_GENETICTSP_H

#include "Graph.h"
#include "Path.h"
#include "chrono"
#include <random>
#include <cstring>
#include <algorithm>

using namespace std::chrono;

enum CrossingMethod {
    PMX = 1,
    CX = 2
};

class GeneticTSP {
private:
    unsigned populationSize;
    unsigned duration;
    unsigned durationUntilFound;
public:
    unsigned int getDurationUntilFound() const;

private:
    float mutationProb;
    float crossingProb;
    CrossingMethod crossingMethod;
    Graph* graph;
    std::vector<Path> population;
    Path* bestPath;

public:
    GeneticTSP(Graph* graph, unsigned populationSize, unsigned duration, float mutationProb, float crossingProb, CrossingMethod crossingMethod);

    static void randomShuffle(Path &path, std::uniform_int_distribution<> &dist, std::mt19937 &gen);

    void populationInit();

    void solve();

    static void inversion(Path &path, int start, int end);

    static bool pathCompare(const Path& x, const Path& y);

    static void pmx(std::vector<Path> &population, Path &parent1, Path &parent2, int start, int end);

    static void cx(std::vector<Path> &population, Path &parent1, Path &parent2);

    static void randomRange(std::mt19937 gen, std::uniform_int_distribution<> nodes_dist, int &n1, int &n2);

    unsigned int getPopulationSize() const {
        return populationSize;
    }

    void setPopulationSize(unsigned int populationSize) {
        GeneticTSP::populationSize = populationSize;
    }

    unsigned int getDuration() const {
        return duration;
    }

    void setDuration(unsigned int duration) {
        GeneticTSP::duration = duration;
    }

    float getMutationProb() const {
        return mutationProb;
    }

    void setMutationProb(float mutationProb) {
        GeneticTSP::mutationProb = mutationProb;
    }

    float getCrossingProb() const {
        return crossingProb;
    }

    void setCrossingProb(float crossingProb) {
        GeneticTSP::crossingProb = crossingProb;
    }

    CrossingMethod getCrossingMethod() const {
        return crossingMethod;
    }

    void setCrossingMethod(CrossingMethod crossingMethod) {
        GeneticTSP::crossingMethod = crossingMethod;
    }

    Path getBestPath() const {
        return *bestPath;
    }
};

#endif //SIMULATED_ANNEALING_GENETICTSP_H
