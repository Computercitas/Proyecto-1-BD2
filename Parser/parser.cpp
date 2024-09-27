#include "parser.hh"

const char* Token::token_names[29] = { "LPAREN", "RPAREN", "ID", "SEMICOLON", "ASSIGN", "ERR", "END", 
        "COMILLAS", "COLON", "CREATE", "INSERT", "DELETE", "SELECT", 
        "FROM", "USING", "TABLE_ID", "VALUES", "WHERE", "BETWEEN", 
        "TABLE", "FILE", "INTO", "AND", "ID", "INDEX", "SEQUENTIAL", "SLASH", "POINT", "ALL"};

Token::Token(Type type) : type(type) {
    lexema = "";
}

Token::Token(Type type, const string source) : type(type) {
    lexema = source;
}

std::ostream& operator<<(std::ostream& outs, const Token& tok) {
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator<<(std::ostream& outs, const Token* tok) {
    return outs << *tok;
}

Scanner::Scanner(const char* s) : input(s), first(0), current(0) {
    reserved["create"] = Token::CREATE;
    reserved["insert"] = Token::INSERT;
    reserved["delete"] = Token::DELETE;
    reserved["select"] = Token::SELECT;
    reserved["from"] = Token::FROM;
    reserved["using"] = Token::USING;
    reserved["table"] = Token::TABLE;
    reserved["file"] = Token::FILE;
    reserved["between"] = Token::BETWEEN;
    reserved["where"] = Token::WHERE;
    reserved["table"] = Token::TABLE;
    reserved["index"] = Token::INDEX;
    reserved["sequential"] = Token::SEQUENTIAL;
}

Token* Scanner::nextToken() {
    Token* token;
    char c;

    // consume whitespaces
    c = nextChar();
    while (c == ' ') c = nextChar();
    if (c == '\0') return new Token(Token::END);
    startLexema();
    if (isalpha(c)) {
        c = nextChar();
        while (isalpha(c) || isdigit(c) || c == '_') c = nextChar();
        rollBack();
        string lex = getLexema();
        Token::Type ttype = checkReserved(lex);
        if (ttype != Token::ERR)
            token = new Token(ttype);
        else
            token = new Token(Token::ID, getLexema());
    }
    else if (strchr("()/*\";=.", c)) {
        switch (c) {
        case '(': token = new Token(Token::LPAREN); break;
        case ')': token = new Token(Token::RPAREN); break;
        case '*': token = new Token(Token::ALL); break; 
        case ';': token = new Token(Token::SEMICOLON); break;
        case '=': token = new Token(Token::ASSIGN); break;
        case '\"': token = new Token(Token::COMILLAS); break;
        case '/': token = new Token(Token::SLASH);break;
        case '.': token = new Token(Token::POINT);break;
        default: cout << "No debería llegar acá" << endl;
        }
    }
    else {
        token = new Token(Token::ERR, getLexema());
    }
    return token;
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
    first = current - 1;
    return;
}

string Scanner::getLexema() {
    return input.substr(first, current - first);
}

Token::Type Scanner::checkReserved(string lexema) {
    std::unordered_map<std::string, Token::Type>::const_iterator it = reserved.find(lexema);
    if (it == reserved.end())
        return Token::ERR;
    else
        return it->second;
}

/* ***************** Parser ********************* */

// match and consume next token
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
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Parse error, unrecognized character: " << current->lexema << endl;
            exit(0);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = current = nullptr;
}

Program* Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - carácter inválido" << endl;
        exit(0);
    }
    Program* p = parseProgram();
    if (current->type != Token::END) {
        cout << "Esperaba fin-de-input, se encontró " << current << endl;
        delete p; // Liberar memoria si hay un error
        p = nullptr;
    }

    if (current) delete current; // Liberar memoria del token actual
    return p; // Retornar el programa parseado
}

Program* Parser::parseProgram() {
    Program* p = new Program(); // Usar new para crear el programa
    p->add(parseStatement());
    while (match(Token::SEMICOLON)) {
        p->add(parseStatement());
    }
    return p;
}

Stm* Parser::parseStatement() {
    Stm* s = nullptr;

    if (match(Token::CREATE)) {
        if (match(Token::TABLE)) {
            if (match(Token::ID)) {
                string table_id = previous->lexema;
                if (match(Token::FROM)) {
                    if (match(Token::FILE)) {
                        if (match(Token::COMILLAS)) {
                            string file_path;
                            while (!match(Token::COMILLAS)) {
                                file_path += previous->lexema;
                                advance();
                            }
                            if (match(Token::USING)) {
                                if (match(Token::INDEX)) {
                                    if (match(Token::SEQUENTIAL)) {
                                        if (match(Token::LPAREN)) {
                                            if (match(Token::ID)) {
                                                string key = previous->lexema;  // Debería ser "ISBN"
                                                if (match(Token::RPAREN)) {
                                                    return new CreateTableStatement(table_id, file_path, key);
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
    } else if (match(Token::SELECT)) {
        if (match(Token::ALL)) { // Reconocer * en SELECT
            if (match(Token::FROM)) {
                if (match(Token::ID)) {
                    string table_name = previous->lexema;
                    if (match(Token::WHERE)) {
                        if (match(Token::ID)) {
                            string column = previous->lexema; // Debería ser "ISBN"
                            if (match(Token::ASSIGN)) {
                                if (match(Token::ID)) {
                                    string value = previous->lexema; // Valor esperado para ISBN
                                    return new SelectStatement(table_name, column, value);
                                }
                            } else if (match(Token::BETWEEN)) { // Búsqueda por rango con BETWEEN
                                if (match(Token::ID)) {
                                    string value1 = previous->lexema;
                                    if (match(Token::AND)) {
                                        if (match(Token::ID)) {
                                            string value2 = previous->lexema;
                                            return new SelectStatement(table_name, column, value1, true, value2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (match(Token::INSERT)) {
        if (match(Token::INTO)) {
            if (match(Token::ID)) {
                string table_name = previous->lexema;
                if (match(Token::VALUES)) {
                    if (match(Token::LPAREN)) {
                        vector<string> values;
                        while (!match(Token::RPAREN)) {
                            values.push_back(previous->lexema);
                            advance();
                        }
                        return new InsertStatement(table_name, values);
                    }
                }
            }
        }
    } else if (match(Token::DELETE)) {
        if (match(Token::FROM)) {
            if (match(Token::ID)) {
                string table_name = previous->lexema;
                if (match(Token::WHERE)) {
                    if (match(Token::ID)) {
                        string column = previous->lexema; // Debería ser "ISBN"
                        if (match(Token::ASSIGN)) {
                            if (match(Token::ID)) {
                                string value = previous->lexema;
                                return new DeleteStatement(table_name, column, value);
                            }
                        }
                    }
                }
            }
        }
    }

    return nullptr; // Si no reconoce el comando
}

/*
Exp* Parser::parseTerm() {
    Exp* e, *rhs;
    e = parseFExp();
    return e;
}

Exp* Parser::parseFExp() {
    Exp* lhs, *rhs;
    lhs = parseFactor();
    return lhs;
}

Exp* Parser::parseFactor() {
    if (match(Token::ID)) {
        return new IdExp(previous->lexema);
    }
    if (match(Token::LPAREN)) {
        Exp* e = parseExpression();
        if (!match(Token::RPAREN)) {
            cout << "Expecting right parenthesis" << endl;
            exit(0);
        }
        return new ParenthExp(e);
    }
    cout << "Couldn't find match for token: " << current << endl;
    exit(0);
}
*/