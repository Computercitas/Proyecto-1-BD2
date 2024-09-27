#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "gramatica.hh"
#include "../Estructuras/SequentialFile.h"
#include <fstream>

class Executor {
private:
    SequentialFile<BX_Books>* sequentialFile; // Referencia al archivo secuencial
    
public:
    Executor(SequentialFile<BX_Books>* seqFile) : sequentialFile(seqFile) {}

    // Ejecuta la creación de la tabla desde un archivo CSV
    void execute(CreateTableStatement* stmt) {
        cout << "Executing CREATE TABLE: " << stmt->table_name << endl;  // Depuración
        cout << "Reading data from file: " << stmt->file_path << endl;   // 
        ifstream bx_books(stmt->file_path);  // Abrir el archivo CSV desde la consulta
        if (!bx_books) {
            throw runtime_error("No se pudo abrir el archivo CSV");
        }
        string line;
        BX_Books book;

        getline(bx_books, line); // saltar el header
        while (getline(bx_books, line)) {  // Insertar cada línea del CSV en `SequentialFile`
            parse_line(line, book);
            sequentialFile->add(book);
        }

        cout << "Tabla '" << stmt->table_name << "' creada con éxito desde el archivo " << stmt->file_path << endl;
    }

    // Ejecuta una inserción
    void execute(InsertStatement* stmt) {
        if (stmt->values.size() != 5) {
            throw runtime_error("Se esperaban 5 valores para la inserción.");
        }
        // Crear un registro `BX_Books` a partir de los valores en la consulta
        BX_Books book(stmt->values[0], stmt->values[1], stmt->values[2], stoi(stmt->values[3]), stmt->values[4]);
        sequentialFile->add(book);
        cout << "Registro insertado con éxito en la tabla." << endl;
    }

    // Ejecuta una búsqueda
    void execute(SelectStatement* stmt) {
        if (stmt->column != "ISBN") {
            throw runtime_error("Por ahora solo se soporta la búsqueda por ISBN.");
        }

        BX_Books result = sequentialFile->search(stoi(stmt->value));
        result.showData();
    }

    // Ejecuta una eliminación
    void execute(DeleteStatement* stmt) {
        if (stmt->column != "ISBN") {
            throw runtime_error("Por ahora solo se soporta la eliminación por ISBN.");
        }

        sequentialFile->remove(stmt->value);
        cout << "Registro con ISBN " << stmt->value << " eliminado con éxito." << endl;
    }
};

#endif // EXECUTOR_H
