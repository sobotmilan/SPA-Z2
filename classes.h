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

class Taxi
{
    int id;
    int currLocation;
    int timeToPassenger;
    bool free;

public:
    Taxi(int id, int currentLocation);

    int getId() const;
    int getCurrLocation() const;
    int getTimeToPassenger() const;
    bool isFree() const;

    void setCurrLocation(int);
    void setTimeToPassenger(int);
    void setFree(bool);

    void calcTime(int, const Graph &);
};

std::ostream &operator<<(std::ostream &os, const Taxi &);

class TaxiSys
{
    int t;
    Taxi *arr;
    Graph &city;

public:
    TaxiSys(Graph &city, int n = 0);
    ~TaxiSys();
    Taxi *assignTaxi(int);
};

std::ostream &operator<<(std::ostream &os, const TaxiSys &);
