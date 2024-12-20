#include "Structs.h"

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
        std::cout << target;
        return;
    }
    if (previous[target] == -1)
    {
        std::cout << "Ne postoji direktna putanja izmedju zadatih cvorova.";
        return;
    }

    path(start, previous[target], previous);
    std::cout << " -> " << target;
}

int readNumberOfVehicles(const char *filename)
{
    FILE *opener = fopen(filename, "r");
    if (opener == nullptr)
    {
        std::cout << "Nemoguce otvaranje ulazne datoteke." << std::endl;
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
        std::cout << "Nemoguce otvaranje ulazne datoteke." << std::endl;
        return -1;
    }
    char buffer[1024];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), opener) != nullptr)
        count++;
    fclose(opener);
    return count;
}