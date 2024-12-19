#include "classes.h"
#include "classes.cpp"
#include <cstdlib>
#include <iostream>
#include <ctime>

int readNumberOfVehicles(const char *filename)
{
    FILE *opener = fopen(filename, "r");
    if (opener == nullptr)
    {
        cout << "Nemoguce otvaranje ulazne datoteke." << endl;
        return -1;
    }
    char buffer[256];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), opener) != nullptr)
        count++;
    fclose(opener);
    return count;
}

int readNumberOfNodes(const char *filename)
{
    FILE *opener = fopen(filename, "r");
    if (opener == nullptr)
    {
        cout << "Nemoguce otvaranje ulazne datoteke." << endl;
        return -1;
    }
    char buffer[1024];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), opener) != nullptr)
        count++;
    fclose(opener);
    return count;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    const char *filenameGraf = argc > 1 ? argv[1] : "matricaGrada.csv";
    const char *filenameUpiti = argc > 2 ? argv[2] : "upitiKorisnika.csv";
    const char *filenameTaxi = argc > 3 ? argv[3] : "vozila.txt";

    int t = readNumberOfVehicles(filenameTaxi);
    if (t == -1)
    {
        std::cerr << "Error reading the number of vehicles from file.\n";
        return EXIT_FAILURE;
    }

    int numNodes = readNumberOfNodes(filenameGraf);
    if (numNodes == -1)
    {
        std::cerr << "Error reading the number of nodes from graph file.\n";
        return EXIT_FAILURE;
    }

    Graph grad(numNodes);
    grad.ucitajCSV(filenameGraf);

    TaxiBST tree;
    TaxiSys sistem(grad, t, filenameTaxi, tree);

    sistem.executeQuery(filenameUpiti);

    std::cout << "Simulation completed successfully.\n";
    return EXIT_SUCCESS;
}
