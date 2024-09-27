#include <iostream>
#include <cstdio> 
#include <fstream>
#include <cmath> 
#include <cstring>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std; 

#define MAX_FB 4 //max elements en un bucket
#define MAX_GLOBAL_DEPTH 3 //2^MAX_GLOBAL_DEPTH hash indexes

struct Registro{
    int codigo;
    char nombre[12];
    char apellido[12];
    int ciclo;

    void setData(int _codigo, string _nombre, string _apellido, int _ciclo){
        this->codigo = _codigo;
        strncpy(nombre, _nombre.c_str(), 12); 
        strncpy(apellido, _apellido.c_str(), 12); 
        this->ciclo = _ciclo; 
    } 
        
    void showData() {
        cout << "Codigo: " << codigo
        << "\tNombre: " << nombre
        << "\tApellido: " << apellido
        << "\t\tCiclo : " << ciclo << endl;
    }

    bool operator==(const Registro& other) const { //para verificar igualdad entre objetos
        return codigo == other.codigo && strcmp(nombre, other.nombre) == 0 && strcmp(apellido, other.apellido) == 0 && ciclo == other.ciclo;
    }
};

string intToBinaryString(int number) {
    string binary;
    while (number) {
        binary = to_string(number % 2) + binary;
        number /= 2;
    }
    return binary.empty() ? "0" : binary;
}

template <typename T>
struct Bucket{
    int local_depth = 1;  
    string filename; //nombre del archivo del bucket
    Bucket* linked_bucket = nullptr; //referencia al siguiente bucket

    Bucket(int bucket_number){ //crear primer bucket de un index
        this->filename = "binary" + to_string(bucket_number) +"_0.bin";
        fstream file(filename, ios::out | ios::binary); //crear archivo
        cout << "Created new bucket file: " << filename << endl;
        file.close();
    }
    Bucket(string new_filename){ //para crear buckets enlazados
        this->local_depth = MAX_GLOBAL_DEPTH; //Como el bucket anterior tiene local_depth=MAX_GLOBAL_DEPTH, los enlazados tambien
        this->filename = new_filename; 
        fstream file(filename, ios::out | ios::binary); //crear archivo
        cout << "Created new linked bucket: " << filename << endl;
        file.close();
    }

    void write_file(Registro reg){ //escribir en archivo
        fstream file(filename, ios::app | ios::out | ios::binary);
        if (!file) {throw runtime_error("Bucket insert: Unable to open the file.");}
        file.write((char*) &reg, sizeof(Registro));
        file.close();
    }

    int bucket_size() { //cantidad de elementos en un bucket
        fstream file(filename, ios::in | ios::binary); 
        if (!file) {throw runtime_error("Unable to open the file.");}
        file.seekg(0, ios::end); 
        int size = file.tellg() / sizeof(Registro);  
        file.close(); 
        return size;
    }

    bool isFull(){
        if (bucket_size() >= MAX_FB) return true; 
        return false;
    }

    void print_contents(){ //imprimo el bucket con los enlazados si es que hay
        fstream file(filename, ios::in | ios::binary);
        if (!file) {throw runtime_error("Unable to open the file.");}
        file.seekg(0,ios::beg);
        Registro reg;
        cout << filename << " contents:" << endl;
        while(file.peek() != EOF){
            file.read((char*) &reg, sizeof(Registro));
            reg.showData();  
        }
        file.close();
        cout << "\t\t|\n\t\tv" << endl; //flechita para abajo xd
        if(linked_bucket != nullptr){
            linked_bucket->print_contents();
        }
    }
    
    ~Bucket() {
        if (linked_bucket != nullptr) { 
            delete linked_bucket; //llamo al destructor recursivamente
        }
        std::remove(filename.c_str()); //borro el archivo del bucket (solo para probar el main rapido y no tener que borrar manualmente)
    }
};

template <typename T>
class HashIndex{ 
private: 
    int global_depth = 1; //profundidad global inicial (aumenta conforme hago splits)
    unordered_map<string,Bucket<Registro>*> indexes; //("1001", Bucket*)
public: 
    HashIndex(){}

    Registro search(T key);

    /* 
    int hash_function(T key){ //por definir
        return (key % pow(2,MAX_GLOBAL_DEPTH));
    }
    */

    void split(){
        cout << "gaa" << endl;
    }

    //retorna el nombre del archivo del siguiente bucket enlazado e.g (bucket001_0.bin --> bucket001_1.bin)
    string name_bucket(string previous_bucket){
        size_t underscore_pos = previous_bucket.find('_');
        char ch = previous_bucket[underscore_pos + 1];
        int digit = (ch - '0') +1;  
        string new_filename = previous_bucket;
        new_filename[underscore_pos + 1] = '0' + digit;
        return new_filename;
    }
    
    int hash(int key){ //no se por que si le pongo pow() no funciona
        return key % (2^MAX_GLOBAL_DEPTH);
    }

    //halla una posicion binaria en el umap, de mayor hasta menor find(1001)->find(001)->find(01)->find(1)
    //si no encuentra retorna el iterador final it.end()
    auto find_pos_bucket(string &pos_bucket){
        auto it = indexes.find(pos_bucket);
        if (it != indexes.end()){
            return it;
        }else if (it == indexes.end() && pos_bucket.length()>1){
            pos_bucket = pos_bucket.substr(pos_bucket.size()-1);
            return (find_pos_bucket(pos_bucket));
        }else{
            return indexes.end();
        }
    }

    bool add(Registro registro){
        int key = registro.codigo;
        string pos_bucket = intToBinaryString(hash(key)); //Binary(key % 2^D)
        
        auto it = find_pos_bucket(pos_bucket);
        if (it != indexes.end()) {//si se encontró un bucket 
            if (!it->second->isFull()){ //verifico si el bucket asociado no está lleno
               it->second->write_file(registro);
            }else{
                if (it->second->local_depth < MAX_GLOBAL_DEPTH){ //si todavia puedo hacer splits
                    it->second->write_file(registro);
                    split();
                }else{ //ya no puedo hacer split, inserto en el ultimo bucket enlazado
                    Bucket<Registro> *temp = it->second;
                    while (temp->linked_bucket != nullptr){
                        temp = temp->linked_bucket;
                    }
                    if (!temp->isFull()){ //si todavia hay espacio en el ultimo bucket enlazado
                        temp->write_file(registro);
                    }else{ //tengo que enlazar un bucket nuevo y insertar ahi
                        string new_filename = name_bucket(temp->filename);
                        Bucket<Registro> *new_bucket = new Bucket<Registro>(new_filename);
                        temp->linked_bucket = new_bucket; //linkeo el bucket anterior con el nuevo
                        new_bucket->write_file(registro);
                    }
                }   
            }
        } else {
            //creo bucket con profundidad local minima (osea bucket 0 o bucket 1)
            string new_filename = "binary" + pos_bucket + "_0.bin";
            Bucket<Registro> *new_bucket = new Bucket<Registro>(new_filename);
            new_bucket->write_file(registro);
            indexes[pos_bucket] = new_bucket;
        }
    }  
    void print_buckets(string pos_bucket){ //imprime todos los buckets enlazados de un index binario
        auto it = find_pos_bucket(pos_bucket);
        if (it != indexes.end()){
            it->second->print_contents();
        }else{
            cout << "no se encontró un bucket con el índice " << pos_bucket << endl;
        }
    }

    bool remove(T key);
    ~HashIndex() {
        for (auto& entry : indexes) {
            delete entry.second; //llama al destructor de Bucket
        }
        global_depth = 1;
        indexes.clear(); 
    }
};

int main(){
    Registro r1;
    Registro r2;
    r1.setData(2023,"juan","perez",8);
    r2.setData(2021,"jorgito","gonzalez",3);

    /*por si acaso estoy creando los archivos de los buckets, pero los borro en el destructor, para poder probar mi main
    varias veces sin tener que borrar los archivos manualmente*/

    HashIndex<Registro> *h = new HashIndex<Registro>();
    h->add(r1);
    h->add(r2);
    h->print_buckets("0"); //imprimir todo lo que hay en bucket 0

    delete h;
    return 0; 
    
};