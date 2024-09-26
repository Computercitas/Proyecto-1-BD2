#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>

#include <unordered_map>

using namespace std;

class Token {
public:
  enum Type { SELECT, ALL, 
                FROM, TABLE, 
                WHERE, CREATE, 
                INSERT, DELETE, 
                VALUES, INTO, 
                FILE, USING, 
                INDEX, BPLUSTREE,
                AVL, SEQUENTIAL,
                BETWEEN, AND,
                STRING, NUMBER, 
                EQUAL, ID,
                LPAREN, RPAREN, VALUEPAREN,
                END, ERR };
  static const char* token_names[25];
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
};


class Parser {
private:
  Scanner* scanner;
  Token *current, *previous;
  bool match(Token::Type ttype);
  bool check(Token::Type ttype);
  bool advance();
  bool isAtEnd();


public:
    Parser(Scanner* scanner);
    bool parse();
};