#include "Structs.h"
#include "Taxi.h"
#include "Graph.h"
#include "TaxiSys.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    const char *filenameGraf = argc > 1 ? argv[1] : "matricaGrada.csv";
    const char *filenameUpiti = argc > 2 ? argv[2] : "upitiKorisnika.csv";
    const char *filenameTaxi = argc > 3 ? argv[3] : "vozila.txt";

    int t = readNumberOfVehicles(filenameTaxi);
    if (t == -1)
    {
        std::cerr << "Greska u citanju broja vozila.\n";
        return EXIT_FAILURE;
    }

    int numNodes = readNumberOfNodes(filenameGraf);
    if (numNodes == -1)
    {
        std::cerr << "Greska u citanju broja cvorova.\n";
        return EXIT_FAILURE;
    }

    Graph grad(numNodes);
    grad.ucitajCSV(filenameGraf);

    TaxiBST tree;
    TaxiSys sistem(grad, t, filenameTaxi, tree);

    sistem.executeQuery(filenameUpiti);

    std::cout << "Simulacija kompletirana.\n";
    return 0;
}
