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

