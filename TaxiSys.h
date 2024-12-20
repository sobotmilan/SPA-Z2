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

    Node(Taxi &v);
};

class TaxiBST
{
    Node *root;
    Node *insertNode(Node *root, Taxi &vehicle);
    Node *findMin(Node *root);
    Node *deleteNode(Node *root, const char *taxiId);

public:
    TaxiBST();
    void insert(Taxi &);
    Taxi *findMin();
    void remove(const char *);
    bool isEmpty();
    void preorder(Node *);
    Node *getRoot();
    void setRoot(Node *);
    void destroyTree(Node *);
};

struct Journey
{
    int location;
    int destination;
};

class TaxiSys
{
    int t;
    Taxi *vehicles;
    Graph &city;
    TaxiBST tree;

public:
    TaxiSys(Graph &, int num, const char *filename, TaxiBST &giventree);
    ~TaxiSys();

    void executeQuery(const char *filename = nullptr);
    void executeRide(int, int);
    void wait(int);
    void putovanje(Taxi *, int);
    void calculateTimeToUser(int);
    void rebuildBST();
};

bool isSymbol(char);