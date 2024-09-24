#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;

#define min_size_main 4 //cantidad minima de registros en el main para poder insertar en aux

struct Registro {
    int id;
    char nombre[20];
    char producto[20];

public:
    Registro(){}
    Registro(int id, string nombre, string producto){
        this->id = id;
        strncpy(this->nombre, nombre.c_str(), 20); 
        strncpy(this->producto, producto.c_str(), 20);
    }

    void setData(){
        cout<<"ID:";
        cin>>id;
        cout<<"Nombre:";
        cin>>nombre;
    }

    void showData(){
        cout<<"ID:"<<id
            <<"\tNombre:"<<nombre
            <<"\tProducto:"<<producto<<endl;
    }

    // sobrecarga de operador ==
    bool operator==(const Registro& other) const {
        return id == other.id && strcmp(nombre, other.nombre) == 0 && strcmp(producto, other.producto) == 0;
    }

    //TODO: destructor
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
        //std::remove("sequential_datos_old.dat");
        //std::remove("sequential_aux_old.dat");
        //std::remove("sequential_datos.dat");
        //std::remove("sequential_aux.dat");
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
            if (!inserted && registro.id < temp_reg.id) {
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
        cout << "filename: " << filename << endl;
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
        sort(aux.begin(), aux.end(), [](const T& a, const T& b) {return a.id < b.id;});

        // Empiezo el merge entre main antiguo y vector aux ordenado
        ifstream file_main_old("sequential_datos.dat", ios::in | ios::binary); //para leer main
        fstream merge_file("merge_file.dat", ios::out | ios::binary); //creo un nuevo archivo para insertar mergeados

        //insertar en orden creciente
        auto it = aux.begin();
        T reg_main;
        file_main_old.seekg(0,ios::beg);
        file_main_old.read((char*) &reg_main, sizeof(T));
        while(!file_main_old.eof() && it != aux.end()){
            if (reg_main.id < (*it).id){
                if (reg_main.id != -1){
                    merge_file.write((char*) &reg_main, sizeof(T));
                    file_main_old.read((char*) &reg_main, sizeof(T));
                }else{//ignoro los registros borrados (tag -1)
                    file_main_old.read((char*) &reg_main, sizeof(T));
                }
            }else if (reg_main.id >= (*it).id){
                if (reg_main.id != -1){
                    merge_file.write((char*) &(*it), sizeof(T));
                    ++it;
                }else{ //ignoro los registros borrados (tag -1)
                    ++it;
                }
            }
        }
        //terminar de insertar elementos faltantes del main antiguo
        while(!file_main_old.eof()){
            if (reg_main.id != -1){ 
                merge_file.write((char*) &reg_main, sizeof(T));
                file_main_old.read((char*) &reg_main, sizeof(T));
            }else{ //ignoro los registros borrados (tag -1)
                file_main_old.read((char*) &reg_main, sizeof(T));
            }
        }
        //terminar de insertar elementos faltandes del aux ordenado
        while(it != aux.end()){
            if ((*it).id != -1){ 
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

    vector<T> search(int key) {
        vector<T> result;
        // considerar
        // 1. Si el rango de busqueda esta en el main: Solo busco en el main
        // 2. Si el rango de busqueda esta en el aux: Busco en el aux
        // 3. Si el rango de busqueda esta en ambos:

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
            if (registro.id == key) {
                result.push_back(registro);
            }
        }

        // Busqueda en el aux
        auxFile.seekg(0, ios::end);
        int size = (int)auxFile.tellg() / sizeof(T);
        auxFile.seekg(0, ios::beg);

        for (int i = 0; i < size; i++) {
            auxFile.read((char*)&registro, sizeof(T));
            if (registro.id == key) {
                result.push_back(registro);
            }
        }

        mainFile.close();
        auxFile.close();

        return result;
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
            if (registro.id >= begin_id && registro.id <= end_id) {
                registros.push_back(registro);
            }
        }

        // Busqueda en el aux
        auxFile.seekg(0, ios::end);
        int size = (int)auxFile.tellg() / sizeof(T);
        auxFile.seekg(0, ios::beg);

        for (int i = 0; i < size; i++) {
            auxFile.read((char*)&registro, sizeof(T));
            if (registro.id >= begin_id && registro.id <= end_id) {
                registros.push_back(registro);
            }
        }

        mainFile.close();
        auxFile.close();

        return registros;
    }

    void remove(int id) { //borrar todos los registros con un mismo id
        remove(id, mainFilename);
        remove(id, auxFilename);
    }
    void remove(int id, string filename) { //borrar todos los registros con un mismo id
        fstream file(filename, ios::in | ios::out | ios::binary);  
        T reg; 
        while (file.read((char*)&reg, sizeof(T))) {
            if (reg.id == id) {
                int position = file.tellg() - sizeof(T); 
                file.seekp(position, ios::beg); 
                reg.id = -1;  
                file.write((char*)&reg, sizeof(T));  
            }
        }
        file.close();
    }


};
    
int main(){
    //Old tests
    
    Registro r1 = Registro(32,"davi","scooter");
    Registro r2 = Registro(12,"edizon","cuaderno");
    Registro r3 = Registro(8,"sergio","lapiz");
    Registro r4 = Registro(50,"napa","algoritmos");

    Registro r5 = Registro(5,"juancito","galletas");
    Registro r6 = Registro(18,"heider","DB");
    Registro r7 = Registro(2,"camila","botella");
    Registro r8 = Registro(3,"jorgito","plato");

    Registro r9 = Registro(23,"pedro","ceviche");
    Registro r10 = Registro(1,"juan","pescado");
    Registro r11 = Registro(4,"maria","arroz");
    Registro r12 = Registro(6,"luis","papa");

    Registro r13 = Registro(7,"lucas","carne");
    Registro r14 = Registro(9,"lucia","pollo");
    Registro r15 = Registro(10,"luciano","pasta");
    Registro r16 = Registro(11,"luciana","pizza");

    Registro r17 = Registro(13,"lucila","hamburguesa");
    Registro r18 = Registro(14,"lucio","tacos");

    SequentialFile<Registro> *seq = new SequentialFile<Registro>();
    seq->add(r1);
    seq->add(r2);
    seq->add(r3);
    seq->add(r4);
    seq->add(r5);

    seq->remove(5);
    seq->remove(50);

    seq->add(r6);
    seq->add(r7);
    seq->print_file("sequential_datos.dat");
    seq->print_file("sequential_aux.dat");

    seq->add(r8);
    seq->add(r9);
    seq->add(r10);
    seq->add(r11);
    seq->add(r12);

    seq->add(r13);
    seq->remove(4);
    seq->remove(6);

    seq->add(r14);
    seq->add(r15);
    seq->add(r16);
    seq->add(r17);
    seq->add(r18);


    seq->print_file("sequential_datos.dat");
    seq->print_file("sequential_aux.dat");

    //remove


    //search
    vector<Registro> reg = seq->search(13);
    for (auto r : reg) {
        r.showData();
    }
    cout << "Registros encontrados: " << endl;
    vector<Registro> regs = seq->rangeSearch(1, 22);
    for (auto r : regs) {
        r.showData();
    }
    cout << endl << endl;
    seq->print_file("sequential_datos.dat");
    seq->print_file("sequential_aux.dat");
    delete seq;
    
    return 0;
};