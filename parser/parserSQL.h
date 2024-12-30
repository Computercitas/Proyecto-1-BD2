#ifndef PARSERSQL_H
#define PARSERSQL_H

#include <iostream>
#include <vector>
#include <string>
#include "tokensSQL.h"
#include "../estructuras/avl/AVLFile.h"
//#include "ISAM.h"
//#include "Extendible.h"

using namespace std;

struct Table {
    string name;
    string file;
    string index;
};

struct Condition {
    string field;
    string op;
    string value1;
    string value2;
};

class Parser {
public:
    Parser(Scanner* scanner, AVLFile<long>* avlFile) : scanner(scanner), avlFile(avlFile) {
        currentToken = scanner->nextToken();
    }

    void parse() {
        while (currentToken->type != Token::END) {
            parseStatement();
            if (currentToken->type == Token::SEMICOLON) {
                currentToken = scanner->nextToken();
            } else if (currentToken->type != Token::END) {
                error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
            }
        }
    }

private:
    Scanner* scanner;
    Token* currentToken;
    vector<Table> tables;
    AVLFile<long>* avlFile;
    //ISAMFile* isamFile;
    //Extendible* extendibleFile;

    void parseStatement() {
        if (currentToken->type == Token::CREATE) {
            parseCreateTable();
        } else if (currentToken->type == Token::SELECT) {
            parseSelect();
        } else if (currentToken->type == Token::INSERT) {
            parseInsert();
        } else if (currentToken->type == Token::DELETE) {
            parseDelete();
        } else {
            error("Unexpected token: " + string(Token::token_names[currentToken->type]));
        }
    }

    void parseCreateTable() {
        expect(Token::CREATE);
        expect(Token::TABLE);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::FROM);
        expect(Token::FILE);
        string fileName = expect(Token::VALUE)->lexema;
        expect(Token::USING);
        expect(Token::INDEX);

        Token* indexType = expectOneOf({Token::AVL});
        expect(Token::LPARENT);
        string indexField = expect(Token::VALUE)->lexema;
        expect(Token::RPARENT);

        Table table = {tableName, fileName, Token::token_names[indexType->type]};
        tables.push_back(table);

        if (indexType->type == Token::AVL) {
            avlFile->loadData(fileName);
        }

        vector<Nodo<long>> sortedData = avlFile->seekAll();
        for (const Nodo<long>& rec : sortedData) {
            rec.record.print();
        }

        cout << "---- Created table " << tableName << " from file " << fileName
             << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
    }

void parseSelect() {
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (condition.op == "=") {
        auto result = avlFile->search(stoi(condition.value1));
        if (!result) { // Verificar si el resultado es nulo
            cerr << "Error: Nodo no encontrado en " << tableName << " para la condición: "
                 << condition.field << " = " << condition.value1 << endl;
        } else {
            cout << "---- Selecting from " << tableName << " where " << condition.field
                 << " = " << condition.value1 << ": " << endl;
            result->showData();
        }
    } else if (condition.op == "between") {
        vector<Nodo<long>> results = avlFile->rangeSearch(stoi(condition.value1), stoi(condition.value2));
        cout << "----- Selecting from " << tableName << " where " << condition.field
             << " between " << condition.value1 << " and " << condition.value2 << ":" << endl;
        for (const Nodo<long>& rec : results) {
            rec.record.print();
        }
    }
}

    void parseInsert() {
        expect(Token::INSERT);
        expect(Token::INTO);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::VALUES);
        expect(Token::LPARENT);
        vector<string> values = parseValues();
        expect(Token::RPARENT);

        Record record;
        record.dni = stoi(values[0]);
        strncpy(record.nombre, values[1].c_str(), sizeof(record.nombre) - 1);
        strncpy(record.ocupacion, values[2].c_str(), sizeof(record.ocupacion) - 1);
        strncpy(record.estado_civil, values[3].c_str(), sizeof(record.estado_civil) - 1);
        record.ingreso_mensual = stoi(values[4]);
        record.edad = stoi(values[5]);
        strncpy(record.sexo, values[6].c_str(), sizeof(record.sexo) - 1);

        Nodo<long> nodo;
        nodo.key = record.dni;
        nodo.record = record;

        avlFile->add(nodo);

        cout << "---- Inserted into " << tableName << " values: ";
        for (const string& value : values) {
            cout << value << " ";
        }
        cout << endl;

        vector<Nodo<long>> sortedData = avlFile->seekAll();
        for (const Nodo<long>& rec : sortedData) {
            rec.record.print();
        }
    }

    void parseDelete() {
        expect(Token::DELETE);
        expect(Token::FROM);
        string tableName = expect(Token::ID)->lexema;
        expect(Token::WHERE);
        Condition condition = parseCondition();

        avlFile->remove(stoi(condition.value1));
        cout << "---- Deleted from " << tableName << " where " << condition.field
                << " = " << condition.value1 << endl;


        vector<Nodo<long>> sortedData = avlFile->seekAll();
        // for (const Nodo<long>& rec : sortedData) {
        //     rec.record.print();
        // }
    }

    Condition parseCondition() {
        string field = expect(Token::ID)->lexema;
        Token* opToken = currentToken;
        if (opToken->type == Token::EQUAL) {
            expect(Token::EQUAL);
            string value = expect(Token::VALUE)->lexema;
            return {field, "=", value, ""};
        } else if (opToken->type == Token::BETWEEN) {
            expect(Token::BETWEEN);
            string value1 = expect(Token::VALUE)->lexema;
            expect(Token::AND);
            string value2 = expect(Token::VALUE)->lexema;
            return {field, "between", value1, value2};
        } else {
            error("Expected = or between");
            return {};
        }
    }

    vector<string> parseValues() {
        vector<string> values;
        values.push_back(expect(Token::VALUE)->lexema);
        while (currentToken->type == Token::COLON) {
            expect(Token::COLON);
            values.push_back(expect(Token::VALUE)->lexema);
        }
        return values;
    }

    Token* expect(Token::Type type) {
        if (currentToken->type == type) {
            Token* token = currentToken;
            currentToken = scanner->nextToken();
            return token;
        } else {
            error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
            return nullptr;
        }
    }

    Token* expectOneOf(const std::initializer_list<Token::Type>& types) {
        for (Token::Type type : types) {
            if (currentToken->type == type) {
                Token* token = currentToken;
                currentToken = scanner->nextToken();
                return token;
            }
        }
        string expected = "";
        for (Token::Type type : types) {
            if (!expected.empty()) expected += ", ";
            expected += Token::token_names[type];
        }
        error("Expected one of: " + expected + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }

    void error(const string& message) {
        cerr << "Error: " << message << endl;
        exit(1);
    }
};

#endif //PARSERSQL_H
