#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>
#ifndef CLASSES_H
#define CLASSES_H
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

    Node(Taxi v);
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
    void executeRide(int, int);
    void wait(int);
    void putovanje(Taxi *, int);
};

void dijkstra(const Graph &, int, int *, int *);
void path(int, int, const int *);

#endif