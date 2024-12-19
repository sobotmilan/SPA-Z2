#include "Graph.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

static int ERRORADDR = INT_MIN;

Graph::Graph(int nodeNum) : n(nodeNum), nodes(new char[n])
{
    ms = new int *[n];
    for (int i = 0; i < n; i++)
    {
        ms[i] = new int[n];
        for (int j = 0; j < n; ++j)
        {
            ms[i][j] = 0;
        }
    }
}
Graph::~Graph()
{
    delete[] this->nodes;
    for (int i = 0; i < this->n; i++)
        delete[] this->ms[i];
    delete[] this->ms;
}
int Graph::getN() const
{
    return this->n;
}
bool Graph::connected(int i, int j) const
{
    return this->ms[i][j] > 0;
}
int *Graph::operator[](int index)
{
    if (index >= 0 && index < this->n)
        return this->ms[index];
    else
        return &ERRORADDR;
}
int Graph::stepenCvora(int i) const
{
    int rez = 0;
    for (int j = 0; j < this->n; j++)
        if (ms[i][j] > 0)
            rez++;
    return rez;
}
const int *Graph::operator[](int index) const
{
    if (index >= 0 && index < this->n)
        return this->ms[index];
    else
        return &ERRORADDR;
}
void Graph::dodajAdresu(int i, int j, int tezina)
{
    if (i >= 0 && i < n && j >= 0 && j < n)
    {
        ms[i][j] = tezina;
        ms[j][i] = tezina;
    }
    else
        std::cout << "Nepravilno proslijedjena adresa..." << std::endl;
}
void Graph::ukloniAdresu(int i, int j)
{
    if (i >= 0 && i < this->n && j >= 0 && j < this->n)
    {
        ms[i][j] = 0;
        ms[j][i] = 0;
    }
}
std::ostream &operator<<(std::ostream &os, const Graph &other)
{
    for (int i = 0; i < other.getN(); ++i, os << std::endl)
        for (int j = 0; j < other.getN(); ++j)
            os << other[i][j] << " ";
    return os;
}
void Graph::ucitajCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == nullptr)
    {
        std::cout << "Neuspjesno otvaranje ulazne datoteke.";
        return;
    }

    for (int i = 0; i < this->n; i++)
    {
        for (int j = 0; j < this->n; j++)
            if (fscanf(file, "%d", &ms[i][j]) != 1)
            {
                std::cout << "Greska pri citanju vrijednosti iz datoteke!";
                fclose(file);
                return;
            }
    }
    fclose(file);
    std::cout << "Citanje uspjesno." << std::endl;
}