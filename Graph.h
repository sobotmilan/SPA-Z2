#pragma once
#include <iostream>

class Graph
{
    int n;
    char *nodes;
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
};

std::ostream &operator<<(std::ostream &os, const Graph &);