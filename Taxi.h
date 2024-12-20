#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>

extern int totalUserTravelTime;
extern int totalUserWaitTime;
extern int totalVehicleMovementTime;
extern int totalNumberOfRides;

struct Taxi
{
    char *id;
    int arrivalTime;
    int currAddr;
    int distanceToUser;
    bool free;
    Taxi();
    Taxi(const Taxi &);
    Taxi(Taxi &&);
    ~Taxi();

    Taxi &operator=(const Taxi &);
    Taxi &operator=(Taxi &&);
};

std::ostream &operator<<(std::ostream &os, const Taxi &);