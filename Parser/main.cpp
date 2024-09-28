#include <iostream>
#include "parser.hh"
#include "executor.hh"
#include "../Estructuras/SequentialFile.h" // Tu archivo con la clase SequentialFile

using namespace std;

int main() {
    // Inicializa el archivo secuencial
    SequentialFile<BX_Books> *seqFile = new SequentialFile<BX_Books>();

    // Inicializa el executor con el archivo secuencial
    Executor executor(seqFile);

    // Parser para leer una consulta
    const char* input = "create table Books from file \"data.csv\" using index sequential(ISBN);";
    const char* input2 = "insert into Books values (\"9783161484100\", \"The Catcher in the Rye\", \"J.D. Salinger\", 1951, \"Little, Brown and Company\");";
    const char* input3 = "select * from Books where ISBN = 9783161484100;";
    const char* input4 = "delete from Books where ISBN = 9783161484100;";
    const char* input5 = "select * from Books where ISBN between 11 and 50;";

    // Crea un scanner con la consulta
    Scanner* scanner = new Scanner(input2); // Cambiar `input1` a `input2`, `input3` o `input4` para otras consultas
    Parser* parser = new Parser(scanner);
    
    // Parsear la consulta
    Program* program = parser->parse();

    // Si la consulta es CREATE TABLE
    if (CreateTableStatement* stmt = dynamic_cast<CreateTableStatement*>(program)) {
        executor.execute(stmt);  // Ejecuta la creación de la tabla
        cout << "CREATE TABLE ejecutado correctamente." << endl;  // Depuración
    }
    // Si la consulta es INSERT
    else if (InsertStatement* stmt = dynamic_cast<InsertStatement*>(program)) {
        executor.execute(stmt);  // Ejecuta la inserción
        cout << "INSERT ejecutado correctamente." << endl;  // Depuración
    }
    // Si la consulta es SELECT
    else if (SelectStatement* stmt = dynamic_cast<SelectStatement*>(program)) {
        executor.execute(stmt);  // Ejecuta la búsqueda
        cout << "SELECT ejecutado correctamente." << endl;  // Depuración
    }
    // Si la consulta es DELETE
    else if (DeleteStatement* stmt = dynamic_cast<DeleteStatement*>(program)) {
        executor.execute(stmt);  // Ejecuta la eliminación
        cout << "DELETE ejecutado correctamente." << endl;  // Depuración
    }


    // Limpiar la memoria
    delete seqFile;
    delete program;
    delete parser;
    delete scanner;

    return 0;
}
