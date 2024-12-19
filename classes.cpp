#include "classes.h"
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static int ERRORADDR = INT_MIN;

Graph::Graph(int nodeNum) : n(nodeNum), nodes(new char[n])
{
    ms = new int *[n];
    for (int i = 0; i < n; i++)
    {
        ms[i] = new int[n];
        for (int j = 0; j < n; ++j)
        {
            ms[i][j] = 0;
        }
    }
}
Graph::~Graph()
{
    delete[] this->nodes;
    for (int i = 0; i < this->n; i++)
        delete[] this->ms[i];
    delete[] this->ms;
}
int Graph::getN() const
{
    return this->n;
}
bool Graph::connected(int i, int j) const
{
    return this->ms[i][j] > 0;
}
int *Graph::operator[](int index)
{
    if (index >= 0 && index < this->n)
        return this->ms[index];
    else
        return &ERRORADDR;
}
int Graph::stepenCvora(int i) const
{
    int rez = 0;
    for (int j = 0; j < this->n; j++)
        if (ms[i][j] > 0)
            rez++;
    return rez;
}
const int *Graph::operator[](int index) const
{
    if (index >= 0 && index < this->n)
        return this->ms[index];
    else
        return &ERRORADDR;
}
void Graph::dodajAdresu(int i, int j, int tezina)
{
    if (i >= 0 && i < n && j >= 0 && j < n)
    {
        ms[i][j] = tezina;
        ms[j][i] = tezina;
    }
    else
        cout << "Nepravilno proslijedjena adresa..." << endl;
}
void Graph::ukloniAdresu(int i, int j)
{
    if (i >= 0 && i < this->n && j >= 0 && j < this->n)
    {
        ms[i][j] = 0;
        ms[j][i] = 0;
    }
}
std::ostream &operator<<(std::ostream &os, const Graph &other)
{
    for (int i = 0; i < other.getN(); ++i, os << endl)
        for (int j = 0; j < other.getN(); ++j)
            os << other[i][j] << " ";
    return os;
}
void Graph::ucitajCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == nullptr)
    {
        cout << "Neuspjesno otvaranje ulazne datoteke.";
        return;
    }

    for (int i = 0; i < this->n; i++)
    {
        for (int j = 0; j < this->n; j++)
            if (fscanf(file, "%d", &ms[i][j]) != 1)
            {
                cout << "Greska pri citanju vrijednosti iz datoteke!";
                fclose(file);
                return;
            }
    }
    fclose(file);
    cout << "Citanje uspjesno." << endl;
}

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
        cout << "Neuspjesno otvaranje ulazne datoteke.";
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
        cout << "Nema slobodnih vozila. Pricekajte dok se ne oslobodi jedno vozilo!" << endl;
        wait(10);
        assignedVehicle = this->tree.findMin();
    }

    int *distances = new int[this->city.getN()];
    int *previous = new int[this->city.getN()];
    dijkstra(city, assignedVehicle->currAddr, distances, previous);

    int pickupDistance = distances[start];
    if (pickupDistance == INT_MAX)
    {
        cout << "Korisnik je nedostizan od strane bilo kog vozila!" << endl;
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
        cout << "Ne postoji putanja do destinacije za pozivaoca." << endl;
        delete[] distances;
        delete[] previous;
        assignedVehicle->free = true;
        return;
    }

    int totalTravelTime = pickupDistance + travelDistance;

    assignedVehicle->currAddr = destination;

    cout << "Vozilo " << assignedVehicle->id << " je dodijeljeno pozivaocu." << endl;
    cout << "Procijenjeno vrijeme dolaska vozila do korisnika: " << pickupDistance << " units." << endl;
    cout << "Procijenjeno vrijeme putovanja do destinacije: " << travelDistance << endl;

    cout << "Put do lokacije preuzimanja pozivaoca: ";
    path(assignedVehicle->currAddr, start, previous);
    cout << endl;
    cout << "Put do destinacije: ";
    path(start, destination, previous);
    cout << endl;

    putovanje(assignedVehicle, totalTravelTime);

    assignedVehicle->free = true;

    delete[] distances;
    delete[] previous;
}

void TaxiSys::wait(int elapsedTime)
{
    cout << "Simuliranje prolaska vremena..." << endl;

    for (int i = 0; i < this->t; ++i)
    {
        Sleep(1000);
        if (!vehicles[i].free && vehicles[i].arrivalTime <= elapsedTime)
        {
            vehicles[i].free = true;
            cout << "Vozilo " << vehicles[i].id << " je sada slobodno" << endl;
            break;
        }
    }

    cout << "Vremenska simulacija ispunjena." << endl;
}

void TaxiSys::putovanje(Taxi *assignedVehicle, int totalTravelTime)
{
    if (assignedVehicle == nullptr)
    {
        cout << "Nema dodijeljenog vozila za ovaj put!!!" << endl;
        return;
    }

    cout << "Simuliranje putovanja za vozilo sa identifikatorom " << assignedVehicle->id << "." << endl;

    assignedVehicle->arrivalTime += totalTravelTime;
    assignedVehicle->free = false;

    cout << "Vozilo " << assignedVehicle->id << " trenutno putuje, bice slobodno za " << totalTravelTime << " minuta." << endl;

    for (int i = 1; i <= totalTravelTime; ++i)
    {
        Sleep(1000);
        cout << "Napredak putovanja: " << i << "/" << totalTravelTime << " minuta." << endl;
    }

    tree.remove(assignedVehicle->id);
    tree.insert(*assignedVehicle);

    cout << "Simulacija putovanja kompletirana. Vozilo se sada nalazi na adresi " << assignedVehicle->currAddr << "." << endl;
}

void dijkstra(const Graph &grad, int start, int *distances, int *previous)
{
    const int n = grad.getN();
    bool visited[MAX] = {false};
    for (int i = 0; i < n; i++)
    {
        distances[i] = INT_MAX;
        previous[i] = -1;
    }
    distances[start] = 0;

    for (int i = 0; i < n - 1; ++i)
    {
        int minDist = INT_MAX, minIndex = -1;
        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && distances[j] < minDist)
            {
                minDist = distances[j];
                minIndex = j;
            }
        }

        if (minIndex == -1)
            break;

        visited[minIndex] = true;

        for (int j = 0; j < n; j++)
        {
            int tezina = grad[minIndex][j];
            if (!visited[j] && tezina > 0 && distances[minIndex] != INT_MAX &&
                distances[minIndex] + tezina < distances[j])
            {
                distances[j] = distances[minIndex] + tezina;
                previous[j] = minIndex;
            }
        }
    }
}

void path(int start, int target, const int *previous)
{
    if (start == target)
    {
        cout << target;
        return;
    }

    if (previous[target] == -1)
    {
        cout << "Ne postoji putanja izmedju zadatih cvorova.";
        return;
    }

    path(start, previous[target], previous);
    std::cout << " -> " << target;
}

Node::Node(Taxi v) : vehicle(v), left(nullptr), right(nullptr) {};