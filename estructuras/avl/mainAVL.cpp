#include <iostream>
#include <fstream>
#include <sstream>
#include "AVLFile.h"
#include "Nodo.h"
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

    // Crear el AVL Tree
    AVLFile<long> avlTree("avl_tree.dat");

    string line;
    bool isHeader = true;

    // Leer línea por línea
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

        // Crear un nodo AVL a partir del Record
        Nodo<long> node(record);

        // Insertar en el árbol
        avlTree.add(node);
    }

    file.close();

    // Pruebas de inserción
    cout << "Pruebas de Inserción: " << endl;
    for (const auto &node : avlTree.seekAll())
    {
        node.showData();
    }

    // Prueba de búsqueda
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "\nPrueba de Búsqueda: " << endl;
    long searchKey = 51118137; // dni a buscar
    try
    {
        Nodo<long> foundNode = avlTree.search(searchKey);
        foundNode.showData();
    }
    catch (const exception &e)
    {
        cerr << "No se encontró el nodo con la clave: " << searchKey << endl;
    }

    // Prueba de eliminación
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "\nPrueba de Eliminación: " << endl;
    long deleteKey = 96165; // dni a eliminar
    if (avlTree.remove(deleteKey))
    {
        cout << "Nodo eliminado con éxito." << endl;
    }
    else
    {
        cerr << "No se pudo eliminar el nodo con clave: " << deleteKey << endl;
    }

    // Pruebas de búsqueda por rango
    cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "\nPrueba de Búsqueda por Rango:\n " << endl;
    auto results = avlTree.rangeSearch(3000, 4000); // Rango de sueldos entre 5000 y 10000
    for (const auto &nodo : results)
    {
        nodo.showData();
    }

    return 0;
}
