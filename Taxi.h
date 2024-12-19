#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>

struct Taxi
{
    char *id;
    int arrivalTime;
    int currAddr;
    bool free;
};

std::ostream &operator<<(std::ostream &os, const Taxi &);