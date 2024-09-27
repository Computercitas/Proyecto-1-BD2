#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#define min_size_main 4 //cantidad minima de registros en el main para poder insertar en aux

struct BX_Books {
    char isbn[13] = {0};  // Zero-initialize the char array
    char book_title[256];
    char book_author[143];
    unsigned short year_of_publication;
    char publisher[134];

    BX_Books(){}
    BX_Books(string isbn, string book_title, string book_author, int year_of_publication, string publisher){
        strncpy(this->isbn, isbn.c_str(), 13);
        strncpy(this->book_title, book_title.c_str(), 256);
        strncpy(this->book_author, book_author.c_str(), 143);
        this->year_of_publication = year_of_publication;
        strncpy(this->publisher, publisher.c_str(), 134);
    }

    void showData(){
        cout << "ISBN: " << isbn << endl;
        cout << "Book Title:" << book_title << endl;
        cout << "Book Author: " << book_author << endl;
        cout << "Year of Publication: " << year_of_publication << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "------------------------------------" << endl;
    }
    void clearReg(){
        string empty = "";
        strncpy(this->isbn, empty.c_str(), 13);
        strncpy(this->book_title, empty.c_str(), 256);
        strncpy(this->book_author, empty.c_str(), 143);
        this->year_of_publication = 0;
        strncpy(this->publisher, empty.c_str(), 134);
    }
};

template <class T>
class SequentialFile {
private:
    string mainFilename; //nombre del archivo main
    string auxFilename; //nombre del archivo auxiliar

public:
    SequentialFile() {
        this->mainFilename = "sequential_datos.dat";
        this->auxFilename = "sequential_aux.dat";

        //crear archivos
        fstream fileMain(mainFilename, ios::out | ios::binary);
        if (fileMain.fail()) {cout << "No se pudo crear archivo main" << endl;}
        fstream fileAux(auxFilename, ios::out | ios::binary);
        if (fileAux.fail()) {cout << "No se pudo crear archivo auxiliar" << endl;}

        fileMain.close();
        fileAux.close();
    }

    ~SequentialFile(){
        mainFilename = "";
        auxFilename = "";
    }

private:

    // Calcula la cantidad de registros en un archivo
    int fileSize(string filename){
        ifstream file(filename, ios::in | ios::binary);
        if (!file) {throw runtime_error("No se pudo abrir el archivo");}
        file.seekg(0, ios::end);
        int size = file.tellg() / sizeof(T);
        file.close();
        return size;
    }

    // Inserta un registro en el archivo main
    bool add_main(T &registro) {
        fstream file(mainFilename, ios::in | ios::out | ios::binary); // fstream para leer y escribir
        if (!file) {
            cerr << "Error al abrir el archivo " << mainFilename << endl;
            return false;
        }
        file.seekp(0, ios::beg);

        // Si el main está vacío, simplemente insertamos el primer registro
        if (fileSize(mainFilename) == 0) {
            file.write((char*)&registro, sizeof(T));
            file.close();
            return true;
        }

        //si ya hay registros en main tengo que insertar ordenadamente
        fstream tempFile("temp.dat", ios::out | ios::binary); // Creo archivo temporal

        T temp_reg;
        bool inserted = false;

        file.read((char*)&temp_reg, sizeof(T));

        while (!file.eof()) {
            // Verificar si es el lugar adecuado para insertar
            if (!inserted && stoll(registro.isbn) < stoll(temp_reg.isbn)) { //convierto char a long
                tempFile.write((char*)&registro, sizeof(T)); // Inserto el nuevo registro
                inserted = true;
            }
            tempFile.write((char*)&temp_reg, sizeof(T)); // Copiar el registro actual
            file.read((char*)&temp_reg, sizeof(T)); // Leer el siguiente registro
        }
        // Si aún no se ha insertado, agrego al final
        if (!inserted) {
            tempFile.write((char*)&registro, sizeof(T));
        }
        file.close();
        tempFile.close();
        std::remove(mainFilename.c_str());  // Elimina el archivo original
        std::rename("temp.dat", mainFilename.c_str());  // Renombra el archivo temporal
        return true;
    }

    vector<T> loadToVec(string filename){ //Carga los elementos en espacio auxiliar a un vector
        ifstream file(filename, ios::in | ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        vector<T> aux;
        file.seekg(0,ios::beg);
        T reg;
        while (file.read((char*)&reg, sizeof(T))) { //while(!file.eof()) estaba insertando doble el registro final
            aux.push_back(reg);
        }
        file.close();
        return aux;
    }

    // merge parametros: mainFile, auxFile
    void merge() {
        //Paso los datos de aux.dat a un vector para poder ordenarlos
        vector<T> aux = loadToVec("sequential_aux.dat");
        sort(aux.begin(), aux.end(), [](const T& a, const T& b) {return stoll(a.isbn) < stoll(b.isbn);});

        // Empiezo el merge entre main antiguo y vector aux ordenado
        ifstream file_main_old("sequential_datos.dat", ios::in | ios::binary); //para leer main
        fstream merge_file("merge_file.dat", ios::out | ios::binary); //creo un nuevo archivo para insertar mergeados

        //insertar en orden creciente
        auto it = aux.begin();
        T reg_main;
        file_main_old.seekg(0,ios::beg);
        file_main_old.read((char*) &reg_main, sizeof(T));
        while(!file_main_old.eof() && it != aux.end()){
            if (stoll(reg_main.isbn) < stoll((*it).isbn)){
                if (stoll(reg_main.isbn) != -1){
                    merge_file.write((char*) &reg_main, sizeof(T));
                    file_main_old.read((char*) &reg_main, sizeof(T));
                }else{//ignoro los registros borrados (tag -1)
                    file_main_old.read((char*) &reg_main, sizeof(T));
                }
            }else if (stoll(reg_main.isbn) >= stoll((*it).isbn)){
                if (stoll(reg_main.isbn) != -1){
                    merge_file.write((char*) &(*it), sizeof(T));
                    ++it;
                }else{ //ignoro los registros borrados (tag -1)
                    ++it;
                }
            }
        }
        //terminar de insertar elementos faltantes del main antiguo
        while(!file_main_old.eof()){
            if (stoll(reg_main.isbn) != -1){ 
                merge_file.write((char*) &reg_main, sizeof(T));
                file_main_old.read((char*) &reg_main, sizeof(T));
            }else{ //ignoro los registros borrados (tag -1)
                file_main_old.read((char*) &reg_main, sizeof(T));
            }
        }
        //terminar de insertar elementos faltandes del aux ordenado
        while(it != aux.end()){
            if (stoll((*it).isbn) != -1){ 
                merge_file.write((char*) &(*it), sizeof(T));
                ++it;
            }else{ //ignoro los registros borrados (tag -1)
                ++it;
            }
        }

        file_main_old.close();//cierro main antiguo
        std::remove("sequential_datos.dat"); //quito el main antiguo

        merge_file.close(); //cierro para poder cambiar el nombre
        std::rename("merge_file.dat", "sequential_datos.dat"); //hago que el merge_file sea el nuevo main file

        //borro el aux antiguo y creo uno nuevo
        std::remove("sequential_aux.dat");
        fstream new_aux_file("sequential_aux.dat", ios::out | ios::binary);
        new_aux_file.close();
    }

public:
    bool add(T registro) { //insertar al final del archivo auxiliar
        ofstream file(auxFilename, ios::out | ios::app | ios::binary);
        if (!file) {throw runtime_error("No se pudo abrir el archivo");}
        file.seekp(0, ios::end);

        int sizeAux = fileSize(auxFilename);
        int sizeMain = fileSize(mainFilename);

        if(sizeMain < min_size_main){ //si el main no tiene la cantidad minima de elementos inserto en main
            add_main(registro);
            return true; //terminar la función add
        }

        if (sizeAux < log(sizeMain)){ //verifico si aux no está lleno
            file.write((char*) &registro, sizeof(T)); //escribo en el aux
        }else{
            merge();
            file.write((char*) &registro, sizeof(T)); //escribo en el aux
        }
        file.close();
        return true;
    }

    void print_file(string filename){
        ifstream file(filename, ios::in | ios::binary);
        if (!file) {throw runtime_error("Error al abrir archivo");}
        file.seekg(0,ios::beg);
        T reg;
        cout << filename << " contents:" << endl;
        while(file.peek() != EOF){
            file.read((char*) &reg, sizeof(T));
            reg.showData();
        }
        file.close();
    }

    T search(int key) {
        // Abro ambos archivos, main y aux
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }
        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo auxiliar");
        }
        bool found = false;
        T registro;

        // Busqueda en el main
        mainFile.seekg(0, ios::end);
        int sizeMain = (int)mainFile.tellg() / sizeof(T);
        mainFile.seekg(0, ios::beg);

        for (int i = 0; i < sizeMain; i++) {
            mainFile.read((char*)&registro, sizeof(T));
            if (stoll(registro.isbn) == key) {
                found = true;
                return registro;
            }
        }

        // Busqueda en el aux
        auxFile.seekg(0, ios::end);
        int size = (int)auxFile.tellg() / sizeof(T);
        auxFile.seekg(0, ios::beg);

        for (int i = 0; i < size; i++) {
            auxFile.read((char*)&registro, sizeof(T));
            if (stoll(registro.isbn) == key) {
                found = true;
                return registro;
            }
        }

        mainFile.close();
        auxFile.close();
        //solo para imprimir en blanco caso no encuentra
        if (!found){
            cout << "No se encontro el key " << key <<endl;
            registro.clearReg();
        } 
            
        return registro;
    }

    vector<T> rangeSearch(int begin_id, int end_id) {
        vector<T> registros;
        // Abro ambos archivos, main y aux
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }
        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo auxiliar");
        }
        T registro;

        // Busqueda en el main
        mainFile.seekg(0, ios::end);
        int sizeMain = (int)mainFile.tellg() / sizeof(T);
        mainFile.seekg(0, ios::beg);

        for (int i = 0; i < sizeMain; i++) {
            mainFile.read((char*)&registro, sizeof(T));
            if (stoll(registro.isbn) >= begin_id && stoll(registro.isbn) <= end_id) {
                registros.push_back(registro);
            }
        }

        // Busqueda en el aux
        auxFile.seekg(0, ios::end);
        int size = (int)auxFile.tellg() / sizeof(T);
        auxFile.seekg(0, ios::beg);

        for (int i = 0; i < size; i++) {
            auxFile.read((char*)&registro, sizeof(T));
            if (stoll(registro.isbn) >= begin_id && stoll(registro.isbn) <= end_id) {
                registros.push_back(registro);
            }
        }

        mainFile.close();
        auxFile.close();

        return registros;
    }

    void remove(string isbn) { //borrar todos los registros con un mismo isbn
        remove(isbn, mainFilename);
        remove(isbn, auxFilename);
    }
    void remove(string isbn, string filename) { //borrar todos los registros con un mismo isbn
        fstream file(filename, ios::in | ios::out | ios::binary);  
        T reg; 
        while (file.read((char*)&reg, sizeof(T))) {
            if (stoll(reg.isbn) == stoll(isbn)) {
                int position = file.tellg() - sizeof(T); 
                file.seekp(position, ios::beg); 
                string deleted = "-1";
                strncpy(reg.isbn, deleted.c_str(), 13);
                file.write((char*)&reg, sizeof(T));  
            }
        }
        file.close();
    }


};
void custom_getline(stringstream& ss, string& token) {
    token.clear(); // Clear the token
    char ch;
    bool in_quotes = false;

    while (ss.get(ch)) {
        if (ch == '\"') {
            in_quotes = !in_quotes; // Toggle the in_quotes flag
        } else if (ch == ';' && !in_quotes) {
            // If we encounter a semicolon and we're not inside quotes, break
            break;
        }
        token += ch; // Add character to token
    }
    
    // Trim any trailing spaces that might have been added
    while (!token.empty() && isspace(token.back())) {
        token.pop_back();
    }
}
void parse_line(const string& line, BX_Books& book) {
    stringstream ss(line);
    string token;

    // Parse ISBN
    custom_getline(ss, token);
    strncpy(book.isbn, token.c_str(), sizeof(book.isbn) - 1);
    book.isbn[sizeof(book.isbn) - 1] = '\0'; // Ensure null-termination

    // Parse Book Title
    custom_getline(ss, token);
    strncpy(book.book_title, token.c_str(), sizeof(book.book_title) - 1);
    book.book_title[sizeof(book.book_title) - 1] = '\0'; // Ensure null-termination

    // Parse Book Author
    custom_getline(ss, token);
    strncpy(book.book_author, token.c_str(), sizeof(book.book_author) - 1);
    book.book_author[sizeof(book.book_author) - 1] = '\0'; // Ensure null-termination

    // Parse Year of Publication
    custom_getline(ss, token);
    if (token.empty() || !all_of(token.begin(), token.end(), ::isdigit)) {
        throw runtime_error("Invalid year of publication: " + token);
    }
    book.year_of_publication = static_cast<unsigned short>(stoi(token));

    // Parse Publisher
    custom_getline(ss, token);
    strncpy(book.publisher, token.c_str(), sizeof(book.publisher) - 1);
    book.publisher[sizeof(book.publisher) - 1] = '\0'; // Ensure null-termination
}
int main(){
    SequentialFile<BX_Books> * seq = new SequentialFile<BX_Books>();
    
    ifstream bx_books("../data/clean_BX_books_50k.csv");
    if (!bx_books) {throw runtime_error("No se pudo abrir el archivo");}
    string line;
    BX_Books book;

    getline(bx_books, line); // saltar header
    int i=2;
    while (getline(bx_books, line) && i<=20000) { // insertar cada linea del csv en sequential file
        parse_line(line, book);  
        seq->add(book);  
        if(i%1000 == 0){
            cout <<"insertado: " <<i << endl;
        }
        i+=1;
    }

    book = seq->search(152012966);
    book.showData();

    vector<BX_Books> v;
    v = seq->rangeSearch(689820000,689821166);
    for (auto i : v) i.showData();

    seq->remove("2005018");
    book = seq->search(2005018);
    book.showData();
    seq->remove("344244392");
    book = seq->search(344244392);
    book.showData();
   
    delete seq;
    return 0;
};