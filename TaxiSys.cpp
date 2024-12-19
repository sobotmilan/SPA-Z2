#include "Taxisys.h"
#define _HAS_STD_BYTE 0
#include <windows.h>
TaxiSys::TaxiSys(const Graph &city, int num, const char *filename, TaxiBST &tree) : city(city), t(num), vehicles(new Taxi[num]), tree(tree)
{
    FILE *vehicleList = fopen(filename, "r");
    if (vehicleList == nullptr)
    {
        t = 0;
        delete[] vehicles;
        return;
    }
    for (int i = 0; i < num; i++)
    {
        char buffer[256];

        while (fgets(buffer, sizeof(buffer), vehicleList) != nullptr)
        {
            int j = 0;
            char tempID[8];
            while (buffer[j] != ',' && buffer[j] != '\0')
            {
                tempID[j] = buffer[j];
                j++;
            }
            vehicles[i].id = new char[strlen(tempID) + 1];
            strcpy(vehicles[i].id, tempID);
            char tempAddr[5];
            int k = 0;
            if (buffer[j] = ',')
                j++;
            while (buffer[j] != '\0')
            {
                tempAddr[k] = buffer[j];
                j++;
                k++;
            }
            vehicles[i].currAddr = atoi(tempAddr);
        }

        tree.insert(vehicles[i]);
    }
}
TaxiSys::~TaxiSys()
{
    delete[] this->arr;
    this->t = 0;
    delete[] this->vehicles;
}

void TaxiSys::executeQuery(const char *filename)
{
    FILE *queries = fopen(filename, "r");
    if (queries == nullptr)
    {
        t = 0;
        delete[] vehicles;
        std::cout << "Neuspjesno otvaranje ulazne datoteke.";
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), queries) != nullptr)
    {
        int location;
        int destination;
        char tempAddr[5];
        int i = 0;
        while (buffer[i] != '\0' && buffer[i] != ',')
        {
            tempAddr[i] = buffer[i];
            i++;
        }
        location = atoi(tempAddr);
        int j = 0;
        strcpy(tempAddr, "");
        while (buffer[i] != '\0')
        {
            tempAddr[j] = buffer[i];
            j++;
            i++;
        }
        destination = atoi(tempAddr);
        strcpy(tempAddr, "");

        this->executeRide(location, destination);
    }
}

void TaxiSys::executeRide(int start, int destination)
{
    Taxi *assignedVehicle = this->tree.findMin();

    while (assignedVehicle == nullptr)
    {
        std::cout << "Nema slobodnih vozila. Pricekajte dok se ne oslobodi jedno vozilo!" << std::endl;
        wait(10);
        assignedVehicle = this->tree.findMin();
    }

    int *distances = new int[this->city.getN()];
    int *previous = new int[this->city.getN()];
    dijkstra(city, assignedVehicle->currAddr, distances, previous);

    int pickupDistance = distances[start];
    if (pickupDistance == INT_MAX)
    {
        std::cout << "Korisnik je nedostizan od strane bilo kog vozila!" << std::endl;
        delete[] distances;
        delete[] previous;
        assignedVehicle->free = true;
        return;
    }

    assignedVehicle->free = false;

    dijkstra(city, start, distances, previous);
    int travelDistance = distances[destination];
    if (travelDistance == INT_MAX)
    {
        std::cout << "Ne postoji putanja do destinacije za pozivaoca." << std::endl;
        delete[] distances;
        delete[] previous;
        assignedVehicle->free = true;
        return;
    }

    int totalTravelTime = pickupDistance + travelDistance;

    assignedVehicle->currAddr = destination;

    std::cout << "Vozilo " << assignedVehicle->id << " je dodijeljeno pozivaocu." << std::endl;
    std::cout << "Procijenjeno vrijeme dolaska vozila do korisnika: " << pickupDistance << " units." << std::endl;
    std::cout << "Procijenjeno vrijeme putovanja do destinacije: " << travelDistance << std::endl;

    std::cout << "Put do lokacije preuzimanja pozivaoca: ";
    path(assignedVehicle->currAddr, start, previous);
    std::cout << std::endl;
    std::cout << "Put do destinacije: ";
    path(start, destination, previous);
    std::cout << std::endl;

    putovanje(assignedVehicle, totalTravelTime);

    assignedVehicle->free = true;

    delete[] distances;
    delete[] previous;
}

void TaxiSys::wait(int elapsedTime)
{
    std::cout << "Simuliranje prolaska vremena..." << std::endl;

    for (int i = 0; i < this->t; ++i)
    {
        Sleep(1000);
        if (!vehicles[i].free && vehicles[i].arrivalTime <= elapsedTime)
        {
            vehicles[i].free = true;
            std::cout << "Vozilo " << vehicles[i].id << " je sada slobodno" << std::endl;
            break;
        }
    }

    std::cout << "Vremenska simulacija ispunjena." << std::endl;
}

void TaxiSys::putovanje(Taxi *assignedVehicle, int totalTravelTime)
{
    if (assignedVehicle == nullptr)
    {
        std::cout << "Nema dodijeljenog vozila za ovaj put!!!" << std::endl;
        return;
    }

    std::cout << "Simuliranje putovanja za vozilo sa identifikatorom " << assignedVehicle->id << "." << std::endl;

    assignedVehicle->arrivalTime += totalTravelTime;
    assignedVehicle->free = false;

    std::cout << "Vozilo " << assignedVehicle->id << " trenutno putuje, bice slobodno za " << totalTravelTime << " minuta." << std::endl;

    for (int i = 1; i <= totalTravelTime; ++i)
    {
        Sleep(1000);
        std::cout << "Napredak putovanja: " << i << "/" << totalTravelTime << " minuta." << std::endl;
    }

    tree.remove(assignedVehicle->id);
    tree.insert(*assignedVehicle);

    std::cout << "Simulacija putovanja kompletirana. Vozilo se sada nalazi na adresi " << assignedVehicle->currAddr << "." << std::endl;
}

TaxiBST::TaxiBST() : root(nullptr) {};

Node *TaxiBST::insert(Node *root, Taxi vehicle)
{
    if (root == nullptr)
        return new Node(vehicle);
    if (vehicle.arrivalTime < root->vehicle.arrivalTime)
        root->left = insert(root->left, vehicle);
    else
        root->right = insert(root->right, vehicle);
    return root;
}
Node *TaxiBST::findMin(Node *root)
{
    while (root && root->left)
        root = root->left;
    return root;
}
Node *TaxiBST::deleteNode(Node *root, const char *taxiId)
{
    if (root == nullptr)
        return nullptr;
    if (strcmp(root->vehicle.id, taxiId) < 0)
        root->left = deleteNode(root->left, taxiId);
    else if (strcmp(taxiId, root->vehicle.id) > 0)
        root->right = deleteNode(root->right, taxiId);
    else
    {
        if (root->left == nullptr)
        {
            Node *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr)
        {
            Node *temp = root->left;
            delete root;
            return temp;
        }
        Node *temp = findMin(root->right);
        root->vehicle = temp->vehicle;
        root->right = deleteNode(root->right, temp->vehicle.id);
    }
    return root;
}
void TaxiBST::insert(Taxi &vehicle)
{
    root = insert(root, vehicle);
}
Taxi *TaxiBST::findMin()
{
    Node *res = findMin(root);
    if (res != nullptr)
        return &res->vehicle;
    else
        return nullptr;
}
void TaxiBST::remove(const char *targetID)
{
    root = deleteNode(root, targetID);
}
bool TaxiBST::isEmpty()
{
    return root == nullptr;
}
