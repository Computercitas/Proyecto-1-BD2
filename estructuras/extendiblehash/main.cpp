#include <iostream>
#include <fstream>
#include <string>
#include "../funciones.h"
#include "Extendible.h"

using namespace std;

int main()
{
    ifstream file("../../data/SmallAdultDataset.csv");
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        return 1;
    }

    string line;
    vector<Record> records;

    // Skip the header line
    getline(file, line);

    // Read the CSV file
    while (getline(file, line))
    {
        cout << "Reading line: " << line << endl; // Debugging line
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

    // Create an ExtendibleHashing instance
    ExtendibleHashing<long> hashTable("dni-person", 2);

    // Insert records into the hash table
    for (const auto &record : records)
    {
        hashTable.insert(record.dni, record);
    }

    // Search for a specific record
    auto result = hashTable.search(records[0].dni);
    if (result.first)
    {
        cout << "Record found:" << endl;
        result.second.print();
    }
    else
    {
        cout << "Record not found" << endl;
    }

    // Range search
    // Realizar búsqueda por rango en ingreso mensual
    /*
    long incomeMin = 3000;
    long incomeMax = 5000;

    cout << "Performing range search by income (" << incomeMin << " to " << incomeMax << "):" << endl;
    auto incomeRangeResults = hashTable.rangeSearchByIncome(incomeMin, incomeMax);

    for (const auto &record : incomeRangeResults)
    {
        record.print();
    }
    */

    // Remove a specific record
    cout << "Removing record with bookID: " << records[1].dni << endl;
    hashTable.remove(records[1].dni);

    // Verify removal
    result = hashTable.search(records[1].dni);
    if (result.first)
    {
        cout << "Record found (should have been removed):" << endl;
        result.second.print();
    }
    else
    {
        cout << "Record successfully removed" << endl;
    }

    // Insert a new record
    Record newRecord;
    newRecord.dni = 99999999;
    strcpy(newRecord.nombre, "Nuevo Nombre");
    strcpy(newRecord.ocupacion, "Nueva Ocupacion");
    strcpy(newRecord.estado_civil, "Soltero");
    newRecord.ingreso_mensual = 5000;
    newRecord.edad = 30;
    strcpy(newRecord.sexo, "M");
    hashTable.insert(newRecord.dni, newRecord);

    // Verify insertion
    result = hashTable.search(newRecord.dni);
    if (result.first)
    {
        cout << "New record inserted successfully:" << endl;
        result.second.print();
    }
    else
    {
        cout << "Failed to insert new record" << endl;
    }

    return 0;
}