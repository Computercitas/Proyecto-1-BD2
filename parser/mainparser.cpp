#include <iostream>
#include "tokensSQL.h"
#include "parserSQL.h"

using namespace std;

int main() {
    // Crear la instancia de AVLFile
    AVLFile<long> avlFile("C:/Users/Public/bd2/Proyecto-1-BD2/estructuras/avl/avl_tree.dat");

    while (true) {
        cout << "Ingrese una consulta SQL (o 'exit' para salir):" << endl;
        string input;
        getline(cin, input);

        // Salir si el usuario escribe 'exit'
        if (input == "exit") {
            cout << "Saliendo del programa..." << endl;
            break;
        }

        // Crear el escáner con la entrada del usuario
        Scanner scanner(input.c_str());

        // Crear el parser con el escáner y AVLFile
        Parser parser(&scanner, &avlFile);

        try {
            // Procesar la entrada
            parser.parse();
            cout << "Consulta procesada correctamente." << endl;
        } catch (const exception& e) {
            cerr << "Error al procesar la consulta: " << e.what() << endl;
        }
    }

    return 0;
}
