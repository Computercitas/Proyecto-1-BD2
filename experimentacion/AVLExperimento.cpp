#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../estructuras/avl/AVLFile.h"
#include "../estructuras/avl/Nodo.h"

using namespace std;
using namespace std::chrono;

void loadCSVDataToMemory(const string &filePath, vector<Record> &records)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error al abrir el archivo: " << filePath << endl;
        return;
    }

    string line;
    bool isHeader = true;

    while (getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false;
            continue;
        }

        Record record;
        try
        {
            record.read(line);
            records.push_back(record);
        }
        catch (const exception &e)
        {
            cerr << "Error al leer un registro: " << e.what() << endl;
        }
    }

    file.close();
}

int main()
{
    // Crear instancia de AVLFile
    AVLFile<long> avlTree("avl_tree.dat");
    string csvFilePath = "datos/AdultDataset_8000.csv";

    // Cargar datos en memoria
    vector<Record> records;
    loadCSVDataToMemory(csvFilePath, records);

    // Medir tiempo de inserción
    cout << "Pruebas de Inserción: " << endl;
    auto start_insert = high_resolution_clock::now();
    for (const auto &record : records)
    {
        Nodo<long> node(record);
        avlTree.add(node);
    }
    auto end_insert = high_resolution_clock::now();
    auto duration_insert = duration_cast<milliseconds>(end_insert - start_insert).count();
    cout << "Tiempo de inserción: " << duration_insert << " ms" << endl;

    // Medir tiempo de búsqueda
    cout << "\nPruebas de Búsqueda: " << endl;
    long searchKey = 57135975; // // 94164430, 95970592, 54225690, 57135975, 26366568

    auto start_search = high_resolution_clock::now();
    auto foundNodePtr = avlTree.search(searchKey);
    auto end_search = high_resolution_clock::now();
    auto duration_search = duration_cast<milliseconds>(end_search - start_search).count();

    if (!foundNodePtr)
    {
        cerr << "No se encontró el nodo con la clave: " << searchKey << endl;
    }
    else
    {
        foundNodePtr->showData();
    }
    cout << "Tiempo de búsqueda: " << duration_search << " ms" << endl;

    return 0;
}
