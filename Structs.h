#pragma once
#ifndef CLASSES_H
#define CLASSES_H
#include "Graph.h"
#include "Taxi.h"
#include "TaxiSys.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cstring>
#define MAX 1000

void dijkstra(const Graph &, int, int *, int *);
void path(int, int, const int *);

int readNumberOfVehicles(const char *filename);
int readNumberOfNodes(const char *filename);

#endif