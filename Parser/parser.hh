#ifndef PARSER_H
#define PARSER_H
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bits/stdc++.h> 
#include "gramatica.hh"

using namespace std;

class Token {
public:
    enum Type { 
        LPAREN = 0, RPAREN, KEY, SEMICOLON, ASSIGN, ERR, END, 
        COMILLAS, COLON, CREATE, INSERT, DELETE, SELECT, 
        FROM, USING, TABLE_ID, VALUES, WHERE, BETWEEN, 
        TABLE, FILE, INTO, AND, ID, INDEX, SEQUENTIAL,SLASH,POINT, ALL, COMA, NUM
    };
    static const char* token_names[31];
    Type type;
    string lexema;

    Token(Type);
    Token(Type, const string source);
};

class Scanner {
public:
    Scanner(const char* in_s);
    Token* nextToken();
    ~Scanner();  
private:
    string input;
    int first, current;
    unordered_map<string, Token::Type> reserved;
    char nextChar();
    void rollBack();
    void startLexema();
    string getLexema();
    Token::Type checkReserved(string);
    char peek();
};

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    
    Program* parseProgram();
    Stm* parseStatement();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFExp();
    Exp* parseFactor();

public:
    Parser(Scanner* scanner);
    Program* parse(); // Cambiado de std::unique_ptr<Program> a Program*
};


#endif // PARSER_H
