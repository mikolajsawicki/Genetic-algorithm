//
// Created by mikolaj on 19.12.2020.
//

#include "Path.h"

Path::Path(int size, Graph *graph) {
    this->size = size;
    ptr = new int[size];
    this->graph = graph;
}

Path::Path(const Path &src) {
    this->size = src.size;
    ptr = new int[size];
    graph = src.graph;
    std::copy(src.ptr, src.ptr + size, ptr);
}

Path::~Path() {
    delete[] ptr;
}

int Path::cost() const {
    int cost = 0;

    for (int i = 0; i < size - 1; i++) {
        cost += graph->getWeight(ptr[i], ptr[i + 1]);
    }

    cost += graph->getWeight(ptr[size - 1], ptr[0]);

    return cost;
}

int &Path::operator[](int index) {
    return ptr[index];
}

Path &Path::operator=(const Path &src) {
    if (src.size == size)
        std::copy(src.ptr, src.ptr + size, ptr);
    else {
        delete[] ptr;
        size = src.size;
        ptr = new int[size];
        std::copy(src.ptr, src.ptr + size, ptr);
    }

    graph = src.graph;

    return *this;
}

int Path::getSize() {
    return size;
}

bool Path::contains(int n) {
    for (int i = 0; i < size; i++)
        if (ptr[i] == n)
            return true;

    return false;
}

bool Path::containsInRange(int n, int start, int end) {
    for (int i = start; i <= end; i++)
        if (ptr[i] == n)
            return true;

    return false;
}

int Path::find(int n) {
    for (int i = 0; i < size; i++)
        if (ptr[i] == n)
            return i;

    return -1;
}