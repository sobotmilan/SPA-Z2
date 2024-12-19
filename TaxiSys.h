#pragma once
#include "Taxi.h"
#include "Structs.h"
#include "Graph.h"
#include <iostream>
#include <strings.h>
#include <cstdio>
#include <cstdlib>

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
    TaxiBST();
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