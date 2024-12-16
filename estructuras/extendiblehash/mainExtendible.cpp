#include <iostream>
#include <vector>
#include "../Record.h"
#include "Extendible.h"
#include "../funciones.cpp"

int main()
{
    vector<AdultRecord> adultRecords;
    vector<SuperstoreRecord> superstoreRecords;

    // Cargar los datos desde los archivos CSV
    cargarDatosAdultDataset("../../data/AdultDataset.csv", adultRecords);
    // cargarDatosSuperstore("../../data/superstore.csv", superstoreRecords);

    // Crear una tabla hash extendida para los registros de AdultDataset
    ExtendibleHashing<AdultRecord> hashTable;

    // Insertar registros en la tabla hash
    for (const auto &record : adultRecords)
    {
        hashTable.insert(record.dni, record);
    }

    // Buscar un registro
    int dniToSearch = 12345; // Cambiar con un DNI válido para búsqueda
    AdultRecord *foundRecord = hashTable.search(dniToSearch);
    if (foundRecord != nullptr)
    {
        cout << "Registro encontrado: " << foundRecord->nombre << endl;
    }
    else
    {
        cout << "Registro no encontrado." << endl;
    }

    // Eliminar un registro
    bool removed = hashTable.remove(dniToSearch);
    if (removed)
    {
        cout << "Registro eliminado con éxito." << endl;
    }
    else
    {
        cout << "No se pudo eliminar el registro." << endl;
    }

    return 0;
}
