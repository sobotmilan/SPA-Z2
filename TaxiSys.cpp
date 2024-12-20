#include "TaxiSys.h"
#define _HAS_STD_BYTE 0
#include <windows.h>

TaxiSys::TaxiSys(Graph &city, int num, const char *filename, TaxiBST &giventree) : city(city), t(num), vehicles(new Taxi[num]()), tree(giventree)
{
    FILE *vehicleList = fopen(filename, "r");
    if (vehicleList == nullptr)
    {
        t = 0;
        delete[] vehicles;
        return;
    }
    char buffer[256];
    int i = 0;
    while ((fgets(buffer, sizeof(buffer), vehicleList) != nullptr) && i < num)
    {

        int j = 0;
        char tempID[10];
        while (isSymbol(buffer[j]))
        {
            tempID[j] = buffer[j];
            j++;
        }
        vehicles[i].id = new char[strlen(tempID) + 1];
        strcpy(vehicles[i].id, tempID);
        char tempAddr[5] = "";
        int k = 0;
        if (buffer[j] = ',')
            j++;
        while (isSymbol(buffer[j]))
        {
            tempAddr[k] = buffer[j];
            j++;
            k++;
        }
        std::cout << tempAddr << std::endl;
        vehicles[i].currAddr = atoi(tempAddr);
        vehicles[i].arrivalTime = 0;
        vehicles[i].distanceToUser = -1;
        vehicles[i].free = true;
        std::cout << "kreirano vozilo sa identifikatorom " << vehicles[i].id << " curraddr = " << vehicles[i].currAddr << std::endl;

        this->tree.insert(vehicles[i]);
        i++;
    }
    this->tree.preorder(this->tree.getRoot());
}
TaxiSys::~TaxiSys()
{
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
        totalNumberOfRides++;
        int location;
        int destination;
        char tempAddr[5] = "";
        int i = 0;
        while (isSymbol(buffer[i]))
        {
            tempAddr[i] = buffer[i];
            i++;
        }
        location = atoi(tempAddr);
        int j = 0;
        char tempDest[5] = "";
        if (buffer[i] == ',')
            i++;
        while (isSymbol(buffer[i]))
        {
            tempDest[j] = buffer[i];
            j++;
            i++;
        }
        destination = atoi(tempDest);

        this->calculateTimeToUser(location);
        this->rebuildBST();

        this->executeRide(location, destination);
    }
}

void TaxiSys::executeRide(int start, int destination)
{
    this->calculateTimeToUser(start);
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

    totalVehicleMovementTime += pickupDistance;
    totalUserWaitTime += pickupDistance;

    assignedVehicle->free = false;

    dijkstra(city, start, distances, previous);
    int travelDistance = distances[destination];

    totalVehicleMovementTime += travelDistance;
    totalUserTravelTime += travelDistance;

    if (travelDistance == INT_MAX)
    {
        std::cout << "Ne postoji putanja do destinacije za pozivaoca." << std::endl;
        delete[] distances;
        delete[] previous;
        assignedVehicle->free = true;
        return;
    }

    int totalTravelTime = pickupDistance + travelDistance;

    std::cout << "Vozilo " << assignedVehicle->id << " je dodijeljeno pozivaocu." << std::endl;
    std::cout << "Procijenjeno vrijeme dolaska vozila do korisnika: " << pickupDistance << " minuta." << std::endl;
    std::cout << "Procijenjeno vrijeme putovanja do destinacije: " << travelDistance << " minuta." << std::endl;

    std::cout << "Put do lokacije preuzimanja pozivaoca: ";
    path(assignedVehicle->currAddr, start, previous);
    std::cout << std::endl;
    std::cout << "Put do destinacije: ";
    path(start, destination, previous);
    std::cout << std::endl;

    putovanje(assignedVehicle, totalTravelTime);

    assignedVehicle->free = true;
    assignedVehicle->currAddr = destination;

    std::cout << "Vozilo se sada nalazi na adresi " << assignedVehicle->currAddr << "." << std::endl;

    for (int i = 0; i < this->t; i++)
        if (!strcmp(vehicles[i].id, assignedVehicle->id))
            vehicles[i] = *assignedVehicle;

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
        std::cout << "Napredak putovanja: " << i << "/" << totalTravelTime << " minuta." << std::endl;
    }
    std::cout << "Simulacija putovanja kompletirana.";
    assignedVehicle->free = true;
}

TaxiBST::TaxiBST() : root(nullptr) {};

Node::Node(Taxi &v) : vehicle(v), left(nullptr), right(nullptr) {};

Node *TaxiBST::insertNode(Node *root, Taxi &vehicle)
{
    if (root == nullptr)
        return new Node(vehicle);
    if (vehicle.distanceToUser < root->vehicle.distanceToUser)
        root->left = insertNode(root->left, vehicle);
    else
        root->right = insertNode(root->right, vehicle);
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
    this->root = insertNode(this->root, vehicle);
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

bool isSymbol(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

void TaxiBST::preorder(Node *root)
{
    if (root != nullptr)
    {
        std::cout << "id = " << root->vehicle.id << ", addr = " << root->vehicle.currAddr << std::endl;
        preorder(root->left);
        preorder(root->right);
    }
}
Node *TaxiBST::getRoot()
{
    return this->root;
}

void TaxiBST::destroyTree(Node *node)
{
    if (node == nullptr)
        return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}
void TaxiSys::rebuildBST()
{
    this->tree.destroyTree(this->tree.getRoot());
    this->tree.setRoot(nullptr);

    for (int i = 0; i < this->t; i++)
    {
        this->tree.insert(this->vehicles[i]);
    }
}

void TaxiSys::calculateTimeToUser(int userLocation)
{
    int *distances = new int[this->city.getN()];
    int *previous = new int[this->city.getN()];

    dijkstra(this->city, userLocation, distances, previous);

    for (int i = 0; i < this->t; i++)
    {
        bool flag = false;
        for (int j = 0; j < this->city.getN(); j++)
        {
            if (vehicles[i].currAddr == j)
            {
                flag = true;
                vehicles[i].distanceToUser = distances[j];
            }
            if (flag == true)
                break;
        }
        if (flag == false)
            return;
    }
    std::cout << "CONTROL PRINT:" << std::endl;
    std::cout << "id,distance,curraddr" << std::endl;
    for (int i = 0; i < this->t; i++)
        std::cout << this->vehicles[i].id << "," << this->vehicles[i].distanceToUser << "," << this->vehicles[i].currAddr << std::endl;
}

void TaxiBST::setRoot(Node *node)
{
    this->root = node;
}