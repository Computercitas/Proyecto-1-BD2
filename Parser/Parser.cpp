
//#include "indexs.cpp"
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Parser.h"
using namespace std;

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
        while (getline(file, line)) {
            vector<string> row = parseCSVLine(line);
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
            result.push_back(column);
        }
        return result;
    }
};


const char* Token::token_names[25] = { "select", "*", 
                                        "from", "table", 
                                        "where", "create", 
                                        "insert", "delete", 
                                        "values", "into", 
                                        "file", "using", 
                                        "index", "hash",
                                        "avl", "sequential",
                                        "between", "and",
                                        "string", "number", 
                                        "=","ID",
                                        "(", ")","ValueParen" };

Token::Token(Type type):type(type) { lexema = ""; }

Token::Token(Type type, const string source):type(type) {
  lexema = source;
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
  if (tok.lexema.empty())
    return outs << Token::token_names[tok.type];
  else
    return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
  return outs << *tok;
}

Scanner::Scanner(const char* s):input(s),first(0),current(0) {
    reserved["select"] = Token::SELECT;
    reserved["from"] = Token::FROM;
    reserved["table"] = Token::TABLE;
    reserved["where"] = Token::WHERE;
    reserved["create"] = Token::CREATE;
    reserved["insert"] = Token::INSERT;
    reserved["delete"] = Token::DELETE;
    reserved["values"] = Token::VALUES;
    reserved["into"] = Token::INTO;
    reserved["file"] = Token::FILE;
    reserved["using"] = Token::USING;
    reserved["index"] = Token::INDEX;
    reserved["bplustree"] = Token::BPLUSTREE;
    reserved["avl"] = Token::AVL;
    reserved["sequential"] = Token::SEQUENTIAL;
    reserved["between"] = Token::BETWEEN;
    reserved["and"] = Token::AND;
    reserved["structure"] = Token::STRUCTURE;
}

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
                                if (match(Token::STRUCTURE)) {
                                    if (match(Token::LPAREN)) {
                                        if (match(Token::ID)) {
                                            string indexField = previous->lexema;// Guarda el campo del índice
                                            if (match(Token::RPAREN)) {
                                                if (match(Token::SEQUENTIAL) || match(Token::AVL)) {
                                                    string indexStructure = (previous->type == Token::SEQUENTIAL) ? "Sequential File" : "AVL Tree";
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
    }
    cout << "Error en la creación de la tabla." << endl;
    exit(0);
}

bool Parser::parse() {
  advance();
  if (current->type == Token::CREATE) {
    create();
    return true;
  }
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
  }
  cout << "No ha introducido un comando valido"<<endl;
  exit(0);
}
