#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "../estructuras/funciones.h"
#include "../estructuras/extendiblehash/Extendible.h"

using namespace std;
using namespace std::chrono;

void loadCSVDataToMemory(const string &filePath, vector<Record> &records)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }

    string line;
    // Skip the header line
    if (!getline(file, line))
    {
        cerr << "Error: File is empty or header missing." << endl;
        return;
    }

    // Read the CSV file
    while (getline(file, line))
    {
        Record record;
        try
        {
            record.read(line);
            records.push_back(record);
        }
        catch (const exception &e)
        {
            cerr << "Error reading record: " << e.what() << endl;
        }
    }

    file.close();
}
int main()
{
    // Crear instancia de ExtendibleHashing
    ExtendibleHashing<long> hashTable("dni-person", 2);
    std::string csvFilePath = "datos/AdultDataset_16000.csv";

    // Cargar datos en memoria
    vector<Record> records;
    loadCSVDataToMemory(csvFilePath, records);

    // Medir tiempo de inserción
    cout << "Pruebas de Inserción: " << endl;
    auto start_insert = high_resolution_clock::now();
    for (const auto &record : records)
    {
        try
        {
            hashTable.insert(record.dni, record);
        }
        catch (const exception &e)
        {
            cerr << "Error inserting record: " << e.what() << endl;
        }
    }
    auto end_insert = high_resolution_clock::now();
    auto duration_insert = duration_cast<milliseconds>(end_insert - start_insert).count();
    cout << "Tiempo de inserción: " << duration_insert << " ms" << endl;

    // Medición del tiempo de búsqueda
    cout << "\nPruebas de Búsqueda: " << endl;
    long searchDNI = 26366568; // // 94164430, 95970592, 54225690, 57135975, 26366568

    auto start_search = high_resolution_clock::now();
    auto result = hashTable.search(searchDNI);
    auto end_search = high_resolution_clock::now();
    auto duration_search = duration_cast<milliseconds>(end_search - start_search).count();

    if (result.first)
    {
        cout << "Registro encontrado:" << endl;
        result.second.print();
    }
    else
    {
        cout << "No se encontró el registro con el DNI: " << searchDNI << endl;
    }
    cout << "Tiempo de búsqueda: " << duration_search << " ms" << endl;

    return 0;
}