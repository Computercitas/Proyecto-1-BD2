#include "indexs.cpp"


void leer_csv(string table_name, string filename, Token::Type index_type, string key_table){
    ifstream file_(filename);
    string line;
    getline(file_, line);

    if(index_type == Token::SEQUENTIAL){
        create_sequential(key_table, table_name, file_); // create_Sequential desarrolloado en indexs.cpp
    }else if(index_type == Token::AVL){
        
    }
    // else if(index_type == Token::AVL){
    //     cout<<"Creando indice AVL con key "<<key_table<<endl;
    //     cout<<"Ruta: "<<filename<<endl;}
    // else if(index_type == Token::HASH)
    //     cout<<"Creando indice hash con key "<<key_table<<endl;
    else
        cout<<"Error"<<endl;


}

//Estructura
class Parser{
    Scanner* scanner;
    Token* current, *previous;
    bool match(Token::Type type);
    bool check(Token::Type type);
    bool advance();
    bool isAtEnd();
public:
    Parser(Scanner* scanner):scanner(scanner){
        previous = current = nullptr;
    }
    bool parse();
    bool parse_create_table();
    bool parse_select();
    bool parse_insert();
    bool parse_delete();
};
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

bool Parser::parse_create_table(){
    if(!match(Token::CREATE)) return false;
    if(!match(Token::TABLE)) return false;
    if(!match(Token::ID)) return false;
    string table_name = previous->lexema;
    if(!match(Token::FROM)) return false;
    if(!match(Token::FILE)) return false;
    if(!match(Token::STRING)) return false;
    string filename = previous->lexema;
    if(!match(Token::USING)) return false;
    if(!match(Token::INDEX)) return false;
    if(!match(Token::BPLUS) && !match(Token::AVL) && !match(Token::SEQUENTIAL)) return false;
    Token::Type index_type = previous->type;
    if(!match(Token::LPAREN)) return false;
    if(!match(Token::VALUEPAREN)) return false; // match(Token::ID) && match(Token::STRING)
    string key_table = previous->lexema;
    if(!match(Token::RPAREN)) return false;
    leer_csv(table_name, filename, index_type, key_table);
    return true;
}
bool Parser::parse_select(){
    if(!match(Token::SELECT)) return false;
    if(!match(Token::ALL)) return false;
    if(!match(Token::FROM)) return false;
    if(!match(Token::ID)) return false;
    string table_name = previous->lexema;
    if(!match(Token::WHERE)) return false;
    if(!match(Token::ID)) return false;
    string key_table = previous->lexema;
    if(!match(Token::EQUAL) && !match(Token::BETWEEN)) return false;
    if(previous->type == Token::EQUAL){
        if(!match(Token::ID) && !match(Token::NUMBER)) return false;
        Token value1 = *previous;
        search_sequential(key_table, table_name, value1.lexema);

    }else{
        if(!match(Token::NUMBER) && !match(Token::ID)) return false;
        Token value1 = *previous;
        if(!match(Token::AND)) return false;
        if(value1.type!=current->type) return false;
        search_sequential_range(key_table, table_name, value1.lexema, current->lexema);
    }
    return true;
}
bool Parser::parse_insert(){
    if(!match(Token::INSERT)) return false;
    if(!match(Token::INTO)) return false;
    if(!match(Token::ID)) return false;
    string table_name = previous->lexema;
    if(!match(Token::VALUES)) return false;
    if(!match(Token::LPAREN)) return false;
    if(!match(Token::VALUEPAREN)) return false;
    string values = previous->lexema;
    if(!match(Token::RPAREN)) return false;
    return true;
}
bool Parser::parse_delete(){
    if(!match(Token::DELETE)) return false;
    if(!match(Token::FROM)) return false;
    if(!match(Token::ID)) return false;
    string table_name = previous->lexema;
    if(!match(Token::WHERE)) return false;
    if(!match(Token::ID)) return false;
    string key_table = previous->lexema;
    if(!match(Token::EQUAL)) return false;
    if(!match(Token::ID) && !match(Token::NUMBER)) return false;
    Token value = *previous;
    if(value.type == Token::NUMBER){
        int number = stoi(value.lexema);
        cout<<"Eliminando en la tabla donde "<<key_table<<" = "<<number<<endl;
    }else{
        string cadena = value.lexema;
        cout<<"Eliminando en la tabla donde "<<key_table<<" = "<<cadena<<endl;
    }
    return true;
}
bool Parser::parse(){
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    if(check(Token::CREATE)){
        return parse_create_table();
    }else if(check(Token::SELECT)){
        return parse_select();
    }else if (check(Token::INSERT)) {
        return parse_insert();
    }else if (check(Token::DELETE)) {
        return parse_delete();
    }else{
        return false;
    }
}

