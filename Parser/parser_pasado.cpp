
//#include "indexs.cpp"
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Parser.h"
using namespace std;


const char* Token::token_names[32] = {
  "create", "table", "file", "using", "index", "bplus", "avl", "sequential", "select", "*",
  "from", "where", "=", "ID", "string", "number", "between", "and", "insert", "into",
  "values", "(", ")", "delete", "end", "err", "semicolon", "colon", "filename", "ValueParen",
  "true","false"};


std::ostream& operator << ( std::ostream& outs, const Token & tok ){
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}
ostream& operator << (ostream& out, const Token* tok){
    return out << *tok;
}


//---------------------------Table:
class Table {
public:
    string name;        // Nombre de la tabla
    string filePath;    // Ruta del archivo
    string indexField;  // Campo del índice
    string indexStructure; // Estructura de índice (sequential o AVL)
    vector<vector<string>> data;

    Table(const string& name, const string& filePath, const string& indexField, const string& indexStructure) : name(name), filePath(filePath), indexField(indexField), indexStructure(indexStructure) {}

    void display() {
        cout << "Tabla: " << name << endl;
        cout << "Archivo: " << filePath << endl;
        cout << "Campo de índice: " << indexField << endl;
        cout << "Estructura de índice: " << indexStructure << endl;
    }
    void loadCSV() {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo: " << filePath << endl;
            return;
        }

        string line;
        size_t lineCount = 0;
        while (getline(file, line)) {
            lineCount++;
            if (line.empty()) {
                cerr << "Línea vacía en el archivo, línea " << lineCount << endl;
                continue; // Ignorar líneas vacías
            }
            vector<string> row = parseCSVLine(line);
            if (row.empty()) {
                cerr << "Error al procesar la línea " << lineCount << " en el archivo." << endl;
                continue;
            }
            data.push_back(row);
        }

        file.close();
        cout << "Datos del archivo CSV cargados exitosamente." << endl;
    }


private:
    vector<string> parseCSVLine(const string& line) {
        vector<string> result;
        stringstream s_stream(line);
        string column;
        while (getline(s_stream, column, ',')) {
            if (!column.empty()) { // Evitar añadir columnas vacías innecesariamente
                result.push_back(column);
            }
        }
        return result;
    }

};



Scanner::Scanner(const char* s):input(s),first(0),current(0) {
    reserved["create"] = Token::CREATE;
    reserved["table"] = Token::TABLE;
    reserved["file"] = Token::FILE;
    reserved["using"] = Token::USING;
    reserved["index"] = Token::INDEX;
    reserved["bplus"] = Token::BPLUS;
    reserved["avl"] = Token::AVL;
    reserved["sequential"] = Token::SEQUENTIAL;
    reserved["select"] = Token::SELECT;
    reserved["from"] = Token::FROM;
    reserved["where"] = Token::WHERE;
    reserved["insert"] = Token::INSERT;
    reserved["into"] = Token::INTO;
    reserved["values"] = Token::VALUES;
    reserved["delete"] = Token::DELETE;
    reserved["between"] = Token::BETWEEN;
    reserved["and"] = Token::AND;
    reserved["true"] = Token::TRUE;
    reserved["false"] = Token::FALSE;
}



Token* Scanner::nextToken() {
    // Ignorar espacios en blanco y tabulaciones
    while (current < input.size() && (isspace(input[current]) || input[current] == '\t')) {
        current++;
    }

    if (current >= input.size()) {
        return new Token(Token::END);  // Si llegamos al final del input
    }

    startLexema(); // Comenzar a registrar el lexema desde el primer carácter relevante
    char c = nextChar();

    // Manejar identificadores y palabras clave (empiezan con una letra)
    if (isalpha(c)) {
        while (isalnum(input[current])) {
            nextChar();
        }
        string lexema = getLexema();
        Token::Type tokenType = checkReserved(lexema);
        return new Token(tokenType != Token::ERR ? tokenType : Token::ID, lexema);
    }

    // Manejar números
    if (isdigit(c)) {
        while (isdigit(input[current])) {
            nextChar();
        }
        return new Token(Token::NUMBER, getLexema());
    }

    // Manejar cadenas entre comillas dobles
    if (c == '\"') {
        while (current < input.size() && input[current] != '\"') {
            nextChar();
        }
        nextChar(); // Consumir la comilla de cierre
        string lexema = input.substr(first + 1, current - first - 2); // Remover comillas
        return new Token(Token::STRING, lexema);
    }

    // Manejar paréntesis, operadores y símbolos específicos
    switch (c) {
        case '(': return new Token(Token::LPARENT);
        case ')': return new Token(Token::RPARENT);
        case '=': return new Token(Token::EQUAL);
        case ',': return new Token(Token::COLON);
        case ';': return new Token(Token::SEMICOLON);
        default: break;
    }

    // Si llega un carácter no reconocido, se devuelve un error
    return new Token(Token::ERR, getLexema());
}


//-------------------Scanner:

Scanner::~Scanner() { }

char Scanner::nextChar() {
  int c = input[current];
  current++;
  return c;
}

void Scanner::rollBack() { // retract
  current--;
}

void Scanner::startLexema() {
  first = current-1;
  return;
}

string Scanner::getLexema() {
  return input.substr(first,current-first);
}

Token::Type Scanner::checkReserved(string lexema) {
  std::unordered_map<std::string,Token::Type>::const_iterator it = reserved.find (lexema);
  if (it == reserved.end())
    return Token::ERR;
 else
   return it->second;
}


/* ***************** Parser ********************* */
bool Parser::match(Token::Type ttype) {
  if (check(ttype)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::check(Token::Type ttype) {
  if (isAtEnd()) return false;
  return current->type == ttype;
}


bool Parser::advance() {
  if (!isAtEnd()) {
    Token* temp =current;
    if (previous) delete previous;
    current = scanner->nextToken();
    previous = temp;
    if (check(Token::ERR)) {
      cout << "Parse error, unrecognised character: " << current->lexema << endl;
      exit(0);
    }
    return true;
  }
  return false;
} 

bool Parser::isAtEnd() {
  return (current->type == Token::END);
} 


Parser::Parser(Scanner* sc):scanner(sc) {
  previous = current = NULL;
  return;
};

void Parser::create() {
    if (match(Token::TABLE)) {
        if (match(Token::ID)) {
            string tableName = previous->lexema;// Guarda el nombre de la tabla
            if (match(Token::FROM)) {
                if (match(Token::FILE)) {
                    if (match(Token::STRING)) {
                        string filePath = previous->lexema;// Guarda el archivo
                        if (match(Token::USING)) {
                            if (match(Token::INDEX)) {
                                if (match(Token::SEQUENTIAL) || match(Token::AVL)) {
                                    string indexStructure = (previous->type == Token::SEQUENTIAL) ? "Sequential File" : "AVL Tree";
                                    if (match(Token::LPARENT)) {
                                        if (match(Token::ID)) {
                                            string indexField = previous->lexema;// Guarda el campo del índice
                                            if (match(Token::RPARENT)) {
                                                    Table newTable(tableName, filePath, indexField, indexStructure);
                                                    newTable.display();
                                                    newTable.loadCSV();
                                                    return;
                                                
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "Error en la creación de la tabla." << endl;
    exit(0);
}

bool Parser::parse() {
  advance();
  if (current->type == Token::CREATE) {
    create();
    return true;
  }/*
  if (current->type == Token::SELECT) {
    select();
    return true;
  }
  if (current->type == Token::INSERT) {
    insert();
    return true;
  }
  if (current->type == Token::DELETE) {
    del();
    return true;
  }*/
  cout << "No ha introducido un comando valido"<<endl;
  exit(0);
}
