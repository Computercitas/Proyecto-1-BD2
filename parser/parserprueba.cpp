#include <iostream>
#include "tokensSQL.h"
#include "parserSQL.h"

using namespace std;

int main() {
    const char* input =
            "CREATE TABLE Adult FROM FILE 'C:/Users/Public/bd2/Proyecto-1-BD2/data/SmallAdultDataset.csv' USING INDEX AVL('DNI');"
            "INSERT INTO Adult VALUES ('12345678', 'Mariel Tovar Tolentino', 'CS', 'Soltero/a', '5000', '21', 'F');"
            "SELECT * FROM Adult WHERE DNI = '35060539';"
            "SELECT * FROM Adult WHERE SUELDO BETWEEN '4000' AND '5000';"
            "DELETE FROM Adult WHERE DNI='12345678';";

    // Crear el escáner
    Scanner scanner(input);
    // Crear la instancia de AVLFile
    AVLFile<long> avlFile("C:/Users/Public/bd2/Proyecto-1-BD2/estructuras/avl/avl_tree.dat");

    // Crear el parser con ambos argumentos
    Parser parser(&scanner, &avlFile);
    parser.parse();

    return 0;
}