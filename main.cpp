#include <fstream>
#include <iostream>
#include "GeneticTSP.h"

using namespace std::chrono;

enum Option {
    quit = 0,
    file = 1,
    set_stop = 2,
    set_population_size = 3,
    set_mutation_parameter = 4,
    set_crossing_parameter = 5,
    set_crossing_method = 6,
    solve = 7,
    tests = 8
};

void print_menu() {
    printf("----- Menu -----\n"
           "0. Quit.\n"
           "1. Load data from file and display it.\n"
           "2. Set the stop conditions.\n"
           "3. Set the initial population size.\n"
           "4. Set the mutation parameter.\n"
           "5. Set the crossing parameter.\n"
           "6. Choose the crossing method.\n"
           "7. Solve the TSP.\n"
           "8. Perform the tests.\n");
}

bool file_load(Graph *graph, const std::string &file_name) {
    std::fstream file;

    file.open(file_name);

    if (!file.good())
        return false;

    std::string buffer;

    do {
        file >> buffer;
    } while (buffer != "DIMENSION:");

    int nodes_count;
    file >> nodes_count;

    if (nodes_count <= 0)
        return false;

    graph->reset(nodes_count);

    SquareMatrix *matrix = graph->getAdjacencyMatrix();

    do {
        file >> buffer;
    } while (buffer != "EDGE_WEIGHT_SECTION");

    for (int i = 0; i < nodes_count; i++)
        for (int j = 0; j < nodes_count; j++)
            file >> (*matrix)[i][j];

    return true;
}

bool file_load_prompt(Graph *graph) {
    std::string file_name;

    std::cout << "Enter the file name: ";
    std::cin >> file_name;

    return file_load(graph, file_name);
}

void print_path(Path path) {
    int n = path.getSize();

    for (int i = 0; i < n - 1; i++)
        printf("%d  ->  ", path[i]);

    printf("%d\n", path[n - 1]);
}

void perform_test(Graph *graph, const std::string &file_name, unsigned population_size, unsigned duration,
                  float mutation_prob, float crossing_prob, int crossing_method) {
    file_load(graph, file_name);

    std::fstream file;

    // file name followed by population_size, mutation_prob and crossing_method
    std::string new_file_name = file_name + "_" + std::to_string(population_size) + "_" + std::to_string(crossing_method) + "_" +
                           std::to_string(mutation_prob);

    std::cout << new_file_name << std::endl;

    file.open(new_file_name, std::fstream::out);

    if (!file.good())
        printf("Error while opening the file.\n");

    for (int i = 0; i < 5; i++) {
        GeneticTSP solver(graph, population_size, duration, mutation_prob, crossing_prob,
                          (CrossingMethod) crossing_method);

        solver.solve();

        printf("%d. Best path cost: %d", i, solver.getBestPath().cost());
        file << i << ". Best path cost: " << solver.getBestPath().cost() << " Time until found: "
             << solver.getDurationUntilFound() << "\n";
    }

    printf("\n");
}

void operator_test() {
    Graph graph;
    Path p1(8, &graph), p2(8, &graph);
    //int a1[] = {3, 4, 8, 2, 7, 1, 6, 5};
    //int a2[] = {4, 2, 5, 1, 6, 8, 3, 7};
    int a1[] = {3, 5, 1, 2, 6, 7, 4, 8};
    int a2[] = {1, 7, 3, 5, 4, 8, 6, 2};
    for (int i = 0; i < p1.getSize(); i++) {
        p1[i] = a1[i];
        p2[i] = a2[i];
    }
    std::vector<Path> population;

    GeneticTSP::pmx(population, p1, p2, 2, 5);

    Path c1 = population.at(0), c2 = population.at(1);

    for (int i = 0; i < c1.getSize(); i++)
        printf("%d ", c1[i]);
    printf("\n");
    for (int i = 0; i < c2.getSize(); i++)
        printf("%d ", c2[i]);
    printf("\n");
}

int main() {
    Graph graph;

    unsigned duration = 300, population_size = 30;
    float mutation_prob = 0.1, crossing_prob = 0.9;
    int crossing_method = 1;

    int opt;

    do {
        print_menu();

        std::cin >> opt;

        switch (opt) {
            case file:
                if (file_load_prompt(&graph)) {
                    printf("The data has been loaded.\n");
                    std::cout << graph.toString();
                    printf("\n");
                } else
                    printf("Error: Could not open the file.\n");

                break;

            case set_stop:
                std::cout << "Enter max duration time in seconds:\n";
                std::cin >> duration;

                break;

            case set_population_size:
                std::cout << "Enter the population size:\n";
                std::cin >> population_size;
                break;

            case set_mutation_parameter:
                std::cout << "Enter the mutation probability: \n";
                std::cin >> mutation_prob;

                break;

            case set_crossing_parameter:
                std::cout << "Enter the crossing probability coefficient: \n";
                std::cin >> crossing_prob;

                break;

            case set_crossing_method:
                std::cout << "1. PMX (Partially mapped crossover) \n"
                             "2. CX (Cycle crossover) \n";
                std::cin >> crossing_method;

                break;

            case solve: {
                GeneticTSP solver(&graph, population_size, duration, mutation_prob, crossing_prob,
                                  (CrossingMethod) crossing_method);

                solver.solve();

                Path best_path = solver.getBestPath();
                printf("Best path cost: %d\n", best_path.cost());
                print_path(best_path);

                break;
            }
            case tests: {
                // c)
                perform_test(&graph, "ftv47.atsp", 800, 120, 0.02, 0.8, PMX);
                perform_test(&graph, "ftv47.atsp", 800, 120, 0.05, 0.8, PMX);
                perform_test(&graph, "ftv47.atsp", 800, 120, 0.1, 0.8, PMX);

                perform_test(&graph, "ftv170.atsp", 400, 120, 0.02, 0.8, PMX);
                perform_test(&graph, "ftv170.atsp", 400, 120, 0.05, 0.8, PMX);
                perform_test(&graph, "ftv170.atsp", 400, 120, 0.1, 0.8, PMX);

                perform_test(&graph, "rbg403.atsp", 100, 120, 0.02, 0.8, PMX);
                perform_test(&graph, "rbg403.atsp", 100, 120, 0.05, 0.8, PMX);
                perform_test(&graph, "rbg403.atsp", 100, 120, 0.1, 0.8, PMX);


                // b)
                /*
                perform_test(&graph, "ftv47.atsp", 600, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv47.atsp", 800, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv47.atsp", 1000, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv47.atsp", 600, 120, 0.01, 0.8, CX);
                perform_test(&graph, "ftv47.atsp", 800, 120, 0.01, 0.8, CX);
                perform_test(&graph, "ftv47.atsp", 1000, 120, 0.01, 0.8, CX);
                
                perform_test(&graph, "ftv170.atsp", 200, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv170.atsp", 400, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv170.atsp", 600, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "ftv170.atsp", 200, 120, 0.01, 0.8, CX);
                perform_test(&graph, "ftv170.atsp", 400, 120, 0.01, 0.8, CX);
                perform_test(&graph, "ftv170.atsp", 600, 120, 0.01, 0.8, CX);

                perform_test(&graph, "rbg403.atsp", 100, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "rbg403.atsp", 200, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "rbg403.atsp", 300, 120, 0.01, 0.8, PMX);
                perform_test(&graph, "rbg403.atsp", 100, 120, 0.01, 0.8, CX);
                perform_test(&graph, "rbg403.atsp", 200, 120, 0.01, 0.8, CX);
                perform_test(&graph, "rbg403.atsp", 300, 120, 0.01, 0.8, CX);
                */
                break;
            }

            default:
                break;
        }
    } while (opt != 0);
}