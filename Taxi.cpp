#include "Taxi.h"
#include <cstring>

int totalUserTravelTime = 0;
int totalUserWaitTime = 0;
int totalVehicleMovementTime = 0;
int totalNumberOfRides = 0;

Taxi &Taxi::operator=(const Taxi &other)
{
    if (this != &other)
    {
        delete[] this->id;

        this->arrivalTime = other.arrivalTime;
        this->currAddr = other.currAddr;
        this->distanceToUser = other.distanceToUser;
        this->free = other.free;

        this->id = new char[strlen(other.id) + 1];
        strcpy(this->id, other.id);
    }
    return *this;
}
Taxi &Taxi::operator=(Taxi &&other)
{
    if (this != &other)
    {
        delete[] this->id;

        this->arrivalTime = other.arrivalTime;
        this->currAddr = other.currAddr;
        this->distanceToUser = other.distanceToUser;
        this->free = other.free;

        this->id = other.id;
        other.id = nullptr;
    }
    return *this;
}

Taxi::Taxi(const Taxi &other) : arrivalTime(other.arrivalTime), distanceToUser(other.distanceToUser), currAddr(other.currAddr), id(new char[strlen(other.id)])
{
    strcpy(this->id, other.id);
}
Taxi::Taxi(Taxi &&other) : arrivalTime(other.arrivalTime), distanceToUser(other.distanceToUser), currAddr(other.currAddr), id(other.id)
{
    other.id = nullptr;
}
Taxi::Taxi() : arrivalTime(-1), distanceToUser(-1), currAddr(-1), id(nullptr) {}
Taxi::~Taxi()
{
}
