#include <iostream>
#include <fstream>
#include <sstream>
#include "SequentialFile.cpp"

using namespace std;

int main()
{
    // Archivo CSV con los datos
    string csv_file = "../../data/SmallAdultDataset.csv";
    ifstream file(csv_file);

    if (!file.is_open())
    {
        cerr << "Error al abrir el archivo " << csv_file << endl;
        return 1;
    }

    // Crear el archivo secuencial
    SequentialFile sequentialFile("main.dat", "aux.dat");

    string line;
    bool isHeader = true;
    int insertedCount = 0;  // Contador de registros insertados
    // Leer línea por línea del archivo CSV
    while (getline(file, line))
    {
        if (isHeader)
        {
            isHeader = false; // Ignorar encabezado
            continue;
        }

        // Crear un Record a partir de la línea
        Record record;
        record.read(line);

        // Agregar el registro al archivo secuencial
        if (sequentialFile.add(record))
        {
            insertedCount++;  // Incrementar el contador si se agrega el registro
            cout << "Registro agregado: " << record.dni << endl;
        }
        else
        {
            cout << "Registro duplicado (no agregado): " << record.dni << endl;
        }
    }

    file.close();

    // Mostrar el número total de elementos insertados
    cout << "\nNumero de registros insertados: " << insertedCount << endl;

    // Prueba de búsqueda
    cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "\nPrueba de Busqueda: " << endl;
    long searchKey = 51118137; // DNI a buscar
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

    // Prueba de eliminación
    cout << "\nPrueba de Eliminacion: " << endl;
    long deleteKey = 96165; // DNI a eliminar
    try
    {
        sequentialFile.remove(deleteKey);
        cout << "Registro con DNI " << deleteKey << " eliminado con éxito." << endl;
    }
    catch (const exception &e)
    {
        cerr << "No se pudo eliminar el registro con DNI: " << deleteKey << endl;
    }

    // Pruebas de búsqueda por rango
    cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "\nPrueba de Busqueda por Rango:\n " << endl;
    long startSueldo = 3000, endSueldo = 4000; // Rango de ingreso_mensual
    auto results = sequentialFile.rangeSearch(startSueldo, endSueldo);
    for (const auto &record : results)
    {
        cout << "Registro encontrado en el rango: " << record.ingreso_mensual << endl;
        cout << "Datos del registro: " << endl;
        record.print();
    }

    return 0;
}
