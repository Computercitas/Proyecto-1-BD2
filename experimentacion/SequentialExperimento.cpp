#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../estructuras/sequentialfile/SequentialFile.h"

using namespace std;
using namespace std::chrono;

int main()
{
    SequentialFile sequentialFile("main.dat", "aux.dat");
    std::string csvFilePath = "datos/AdultDataset_1000.csv";

    // Medición del tiempo de inserción
    cout << "Pruebas de Inserción: " << endl;
    auto start_insert = high_resolution_clock::now();
    loadCSVDataSeq(csvFilePath, sequentialFile); // Carga los datos desde el CSV
    auto end_insert = high_resolution_clock::now();
    auto duration_insert = duration_cast<milliseconds>(end_insert - start_insert).count();
    cout << "Tiempo de inserción: " << duration_insert << " ms" << endl;

    // Prueba de búsqueda
    cout << "\nPruebas de Búsqueda: " << endl;
    long searchKey = 94164430; // // 94164430, 95970592, 54225690, 57135975, 26366568

    auto start_search = high_resolution_clock::now();
    try
    {
        Record foundRecord = sequentialFile.search(searchKey);
        cout << "Registro encontrado: " << foundRecord.dni << endl;
        cout << "Datos del registro: " << endl;
        foundRecord.print();
    }
    catch (const exception &e)
    {
        cerr << "No se encontró el registro con el DNI: " << searchKey << endl;
    }
    auto end_search = high_resolution_clock::now();
    auto duration_search = duration_cast<milliseconds>(end_search - start_search).count();
    cout << "Tiempo de búsqueda: " << duration_search << " ms" << endl;

    return 0;
}
