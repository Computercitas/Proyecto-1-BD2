
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

bool Parser::parse() {
  if (match(Token::CREATE)) {
    create();
    return true;
  }
  if (match(Token::SELECT)) {
    select();
    return true;
  }
  if (match(Token::INSERT)) {
    insert();
    return true;
  }
  if (match(Token::DELETE)) {
    del();
    return true;
  }
  cout << "No ha introducido un comando valido"<<endl;
  exit(0);
}
