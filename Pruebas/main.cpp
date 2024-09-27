#include <iostream>
#include "imp_parser.hh"

using namespace std;

int main() {
    // Aquí se puede especificar la entrada de texto para el escáner
    const char* input = "create table libros from file \"Parser/BX_Books_l.csv\" using index sequential(ISBN);";
    
    Scanner* scanner = new Scanner(input);
    Parser* parser = new Parser(scanner);

    Program* program = parser->parse();

    delete program;
    delete parser;
    delete scanner;

    return 0;
}
