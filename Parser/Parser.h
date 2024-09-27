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
  enum Type { CREATE, TABLE, FILE, USING, INDEX, BPLUS, AVL, SEQUENTIAL, 
              SELECT, ALL, FROM, WHERE, EQUAL, ID, STRING, NUMBER, 
              BETWEEN, AND, INSERT, INTO, VALUES, LPAREN, RPAREN, 
              DELETE, END, ERR, SEMICOLON, COLON, FILENAME, VALUEPAREN, 
              TRUE, FALSE };

  static const char* token_names[32]; 
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
  void select();
  void insert();
  void create();
  void del();


public:
    Parser(Scanner* scanner);
    bool parse();
};
