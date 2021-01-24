//
// Created by mikolaj on 19.12.2020.
//

#ifndef SIMULATED_ANNEALING_PATH_H
#define SIMULATED_ANNEALING_PATH_H

#include "Graph.h"

class Path {
private:
    int *ptr;
    int size;
    Graph* graph;
public:
    Path(int size, Graph* graph);
    Path( const Path& );
    ~Path();
    int cost() const;
    int getSize();
    int& operator [] (int index);
    Path& operator=(const Path& src);
    int* get_ptr() { return ptr; };
    bool contains(int n);
    bool containsInRange(int n, int start, int end);
    int find(int n);
};


#endif //SIMULATED_ANNEALING_PATH_H
