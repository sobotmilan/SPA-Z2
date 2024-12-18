#include "classes.h"
#include "classes.cpp"
#include <cstdlib>
#include <iostream>

int readNumberOfVehicles(const char *filename)
{
    FILE *opener = fopen(filename, "r");
    if (opener == nullptr)
        return -1;
    char counter[256];
    int res = 0;
    while (fgets(counter, sizeof(counter), opener) != nullptr)
        res++;
    fclose(opener);
    return res;
}

int main()
{
    srand(time(NULL));
    const char *filenameGraf = "matricaGrada.csv";
    const char *filenameUpiti = "upitiKorisnika.csv";
    const char *filenameTaxi = "vozila.txt";
    int t = readNumberOfVehicles(filenameTaxi);
    if (t == -1)
        return -1;
    Graph grad;
    grad.ucitajCSV(filenameGraf);
    TaxiBST tree;
    TaxiSys sistem(grad, t, filenameTaxi, tree);
    sistem.executeQuery(filenameUpiti);

    return 0;
}