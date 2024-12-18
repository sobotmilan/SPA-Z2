#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;
#define MAX 1000

class Graph
{
    int n;
    char *nodes;
    int **ms;

public:
    Graph(int nodeNum = MAX);
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

struct Taxi
{
    char *id;
    int arrivalTime;
    int currAddr;
    bool free;
};

std::ostream &operator<<(std::ostream &os, const Taxi &);

struct Node
{
    Taxi vehicle;
    struct Node *left, *right;

    Node(Taxi v) : vehicle(v), left(nullptr), right(nullptr) {};
};

class TaxiBST
{
    Node *root;
    Node *insert(Node *root, Taxi vehicle);
    Node *findMin(Node *root);
    Node *deleteNode(Node *root, const char *taxiId);

public:
    TaxiBST() : root(nullptr) {};
    void insert(Taxi &);
    Taxi *findMin();
    void remove(const char *);
    bool isEmpty();
};

struct Journey
{
    int location;
    int destination;
};

class TaxiSys
{
    int t;
    Journey *arr;
    Taxi *vehicles;
    Graph city;
    TaxiBST tree;

public:
    TaxiSys(const Graph &, int num, const char *filename, TaxiBST &tree);
    ~TaxiSys();

    void executeQuery(const char *filename = nullptr);
};

void dijkstra(const Graph &grad, int start, int *distances, int *previous)
{
    const int n = grad.getN();
    bool visited[MAX] = {false};
    for (int i = 0; i < n; i++)
    {
        distances[i] = INT_MAX;
        previous[i] = -1;
    }
    distances[start] = 0;

    for (int i = 0; i < n - 1; ++i)
    {
        int minDist = INT_MAX, minIndex = -1;
        for (int j = 0; j < n; i++)
        {
            if (!visited[j] && distances[j] < minDist)
            {
                minDist = distances[j];
                minIndex = j;
            }
        }

        if (minIndex == -1)
            break;

        visited[minIndex] = true;

        for (int j = 0; j < n; j++)
        {
            int tezina = grad[minIndex][j];
            if (!visited[j] && tezina > 0 && distances[minIndex] != INT_MAX &&
                distances[minIndex] + tezina < distances[j])
            {
                distances[j] = distances[minIndex] + tezina;
                previous[j] = minIndex;
            }
        }
    }
}

void path(int start, int target, const int *previous)
{
    if (start == target)
    {
        cout << target;
        return;
    }

    if (previous[target] == -1)
    {
        cout << "Ne postoji putanja izmedju zadatih cvorova.";
        return;
    }

    path(start, previous[target], previous);
    std::cout << " -> " << target;
}
