#include "classes.h"
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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

        assignVehicle();
    }
}