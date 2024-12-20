#pragma once
#include <iostream>

class Graph
{
    int n;
    int *nodes;
    int **ms;

public:
    Graph(int nodeNum);
    ~Graph();

    bool connected(int, int) const;
    void ukloniAdresu(int, int);
    int *operator[](int);
    const int *operator[](int) const;
    int getN() const;
    void dodajAdresu(int, int, int);
    int stepenCvora(int) const;
    void ucitajCSV(const char *);
    void DFS();
    void DFS_visit(bool[], int);
};

std::ostream &operator<<(std::ostream &os, const Graph &);