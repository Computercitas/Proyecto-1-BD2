#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

// Definición de la estructura de datos para AdultRecord
struct AdultRecord {
    string DNI;
    string Nombre;
    string Ocupacion;
    string EstadoCivil;
    double IngresoMensual;
    int Edad;
    string Sexo;    // Sexo ("M" o "F")

    // Constructor por defecto
    AdultRecord() {}

    // Constructor para inicializar un registro
    AdultRecord(string DNI, const string &  Nombre, const string &Ocupacion, const string &Estado_Civil,
                double  Ingreso_Mensual, int Edad, string Sexo)
        : DNI(DNI), Nombre(Nombre), Ocupacion(Ocupacion), EstadoCivil(Estado_Civil),
          IngresoMensual(Ingreso_Mensual), Edad(Edad), Sexo(Sexo) {}



    bool operator>(const AdultRecord& other) const {
        return DNI > other.DNI;
    }
    // Sobrecarga del operador < para comparación en el AVL
    bool operator<(const AdultRecord& other) const {
        return DNI < other.DNI; // Comparación por DNI
    }
    // Sobrecarga del operador == para búsqueda específica
    bool operator==(const AdultRecord& other) const {
        return DNI == other.DNI;
    }
};

// Clase que implementa un nodo del árbol AVL
template <typename T>
class Node {
public:
    T value; // Valor almacenado en el nodo
    int height; // Altura del nodo
    int balanceFactor; // Factor de balance del nodo
    Node* left; // Hijo izquierdo
    Node* right; // Hijo derecho

    Node(T val) : value(val), height(0), balanceFactor(0), left(nullptr), right(nullptr) {}
};

// Clase que implementa el árbol AVL
template <typename T>
class AVLTree {
private:
    Node<T>* root;
    int nodeCount;

    // Actualiza la altura y el factor de balanceo de un nodo
    void update(Node<T>* node) {
        int leftHeight = (node->left == nullptr) ? -1 : node->left->height;
        int rightHeight = (node->right == nullptr) ? -1 : node->right->height;

        node->height = 1 + max(leftHeight, rightHeight);
        node->balanceFactor = rightHeight - leftHeight;
    }

    // Rotación a la izquierda
    Node<T>* leftRotation(Node<T>* node) {
        Node<T>* newParent = node->right;
        node->right = newParent->left;
        newParent->left = node;

        update(node);
        update(newParent);

        return newParent;
    }
    // Rotación a la derecha
    Node<T>* rightRotation(Node<T>* node) {
        Node<T>* newParent = node->left;
        node->left = newParent->right;
        newParent->right = node;

        update(node);
        update(newParent);

        return newParent;
    }
    // Balancear un nodo
    Node<T>* balance(Node<T>* node) {
        if (node->balanceFactor < -1) {
            if (node->left->balanceFactor <= 0) { // LL
                return rightRotation(node);
            } else {
                node->left = leftRotation(node->left); // LR
                return rightRotation(node);
            }
        } else if (node->balanceFactor > 1) {
            if (node->right->balanceFactor >= 0) { // RR
                return leftRotation(node);
            } else {
                node->right = rightRotation(node->right); // RL
                return leftRotation(node);
            }
        }
        return node; // El árbol ya está balanceado
    }
    // Insertar un nodo en el árbol
    Node<T>* insert(Node<T>* node, T value) {
        if (node == nullptr) return new Node<T>(value);

        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node; // Valor duplicado, no se permite insertar
        }

        update(node);// Actualizar altura y factor de balanceo
        return balance(node);// Balancear el árbol después de la inserción
    }

    Node<T>* findMin(Node<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    // Eliminar un nodo del árbol (busca y elimina el nodo por el DNI)
    Node<T>* remove(Node<T>* node, const string& key) { // Ahora acepta un string (DNI)
        if (node == nullptr) return nullptr;

        if (key < node->value.DNI) { // Compara el DNI
            node->left = remove(node->left, key);
        } else if (key > node->value.DNI) {
            node->right = remove(node->right, key);
        } else {
            // Nodo encontrado, realizar eliminación
            if (node->left == nullptr) {
                Node<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (node->right == nullptr) {
                Node<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                Node<T>* successor = findMin(node->right);
                node->value = successor->value;
                node->right = remove(node->right, successor->value.DNI); // Usamos DNI del sucesor para eliminar
            }
        }

        update(node); // Actualizamos altura y balance del nodo
        return balance(node); // Balanceamos el árbol después de la eliminación
    }

    /*
    Node<T>* remove(Node<T>* node, T value) { //value=key=DNI
        if (node == nullptr) return nullptr;

        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {
            if (node->left == nullptr) {
                Node<T>* rightChild = node->right;
                delete node;
                return rightChild;
            } else if (node->right == nullptr) {
                Node<T>* leftChild = node->left;
                delete node;
                return leftChild;
            } else {
                Node<T>* successor = findMin(node->right);
                node->value = successor->value;
                node->right = remove(node->right, successor->value);
            }
        }

        update(node);
        return balance(node);
    }*/
//--

    void inOrderTraversal(Node<T>* node, vector<T>& result) {
        if (node == nullptr) return;
        inOrderTraversal(node->left, result);
        result.push_back(node->value);
        inOrderTraversal(node->right, result);
    }
//--

    // Recorrer el árbol en orden para encontrar valores en un rango
    void rangeSearch(Node<T>* node, const string& beginKey, const string& endKey, vector<T>& result) {
        if (!node)
            return;

        if (beginKey < node->value.DNI)
            rangeSearch(node->left, beginKey, endKey, result);

        if (node->value.DNI >= beginKey && node->value.DNI <= endKey)
            result.push_back(node->value);

        if (endKey > node->value.DNI)
            rangeSearch(node->right, beginKey, endKey, result);
    }

    // Búsqueda específica (recorrido en orden)
    void searchByDNI(Node<T>* node, const string& key, vector<T>& result) {
        if (!node)
            return;

        searchByDNI(node->left, key, result);

        if (node->value.DNI == key)
            result.push_back(node->value);

        searchByDNI(node->right, key, result);
    }


public:
    AVLTree() : root(nullptr), nodeCount(0) {}

    // Insertar un nuevo valor en el árbol
    void insert(T value) {
        root = insert(root, value);
        nodeCount++;
    }

    // Eliminar un valor por clave
    bool remove(const string& key) {
        if (!root)
            return false;
        root = remove(root, key);
        return true;
    }

    // Buscar por clave específica
    vector<T> searchByDNI(const string& key) {
        vector<T> result;
        searchByDNI(root, key, result);
        return result;
    }

    // Buscar por rango de claves
    vector<T> rangeSearch(const string& beginKey, const string& endKey) {
        vector<T> result;
        rangeSearch(root, beginKey, endKey, result);
        return result;
    }

    void display() {
        vector<T> result;
        inOrderTraversal(root, result);
        cout << "DNI       | Nombre         | Ocupacion     | Estado Civil | Ingreso Mensual| Edad | Sexo" << endl;
        cout << "---------------------------------------------------------------------------------" << endl;
        for (const auto& val : result) {
            cout << val.DNI << "     | " << val.Nombre << "        | " << val.Ocupacion 
                << "    |  " << val.EstadoCivil << "  |   " << val.IngresoMensual 
                << "   | " << val.Edad << " | " << val.Sexo << endl;
        }
    }
/*
    void display() {
        vector<T> result;
        inOrderTraversal(root, result);
        for (const auto& val : result) {
            cout << "DNI: " << val.DNI << ", Nombre: " << val.Nombre 
                << ", Ocupacion: " << val.Ocupacion << ", Estado Civil: " << val.EstadoCivil
                << ", Ingreso Mensual: " << val.IngresoMensual << ", Edad: " << val.Edad << ", Sexo: " << val.Sexo << endl;
        }
    }
*/
    // Agregar un nuevo registro
    bool add(const T& record) {
        if (!searchByDNI(record.DNI).empty()){
            cout << "Registro duplicado: " << record.DNI << endl;
            return false;
        }
        insert(record);
        cout << "Registro agregado exitosamente: " << record.DNI << endl;
        return true;
    }

    int size() {
        return nodeCount;
    }

};

// Cargar el dataset AdultDataset.csv
void loadAdultDataset(const string& filename, AVLTree<AdultRecord>& tree) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        AdultRecord record;
        getline(ss, record.DNI, ',');
        getline(ss, record.Nombre, ',');
        getline(ss, record.Ocupacion, ',');
        getline(ss, record.EstadoCivil, ',');
        ss >> record.IngresoMensual; ss.ignore();
        ss >> record.Edad; ss.ignore();
        getline(ss, record.Sexo, ',');

        cout << "Cargando: " << record.DNI << " - " << record.Nombre << endl;
        tree.add(record);
    }
}

int main() {
    AVLTree<AdultRecord> tree;
    cout << "Antes de la carga inicial: " << endl;
    tree.display();

    // Cargar registros desde el archivo CSV
    string filename = "../data/Adultprueba.csv";
    cout << "Cargando registros desde " << filename << "..." << endl;
    loadAdultDataset(filename, tree);

    // Mostrar los registros cargados
    cout << "\nRegistros cargados en el arbol (en orden):" << endl;
    tree.display();
    cout << "\nTamanio del arbol despues de la carga: " << tree.size() << endl;

    // Agregar registros al árbol
    cout << "\nAgregando registros al arbol..." << endl;
    tree.insert({"12345", "Alice", "Ingeniera", "Soltero/a", 5000.00, 29, "F"});
    tree.insert({"23456", "Bob", "Profesora", "Casado/a", 4000.00, 35, "M"});
    tree.insert({"34567", "Charlie", "Doctor", "Soltero/a", 6000.00, 40, "M"});
    tree.insert({"12345", "Alicia", "Cientifica", "Soltero/a", 7000.00, 31, "F"}); // DNI repetido

    tree.display();
    cout << "Tamanio del arbol despues de la insercion: " << tree.size() << endl;

    // Búsqueda específica
    cout << "\nBusqueda especifica por DNI '12345':" << endl;
    auto searchResults = tree.searchByDNI("12345");
    if (!searchResults.empty()) {
        for (const auto& record : searchResults)
            cout << "DNI: " << record.DNI << ", Nombre: " << record.Nombre << endl;
    } else {
        cout << "No se encontraron registros con ese DNI." << endl;
    }

    // Búsqueda por rango
    cout << "\nBusqueda por rango de DNI entre '12344' y '34568':" << endl;
    auto rangeResults = tree.rangeSearch("12344", "34568");
    for (const auto& record : rangeResults)
        cout << "DNI: " << record.DNI << ", Nombre: " << record.Nombre << endl;

    // Eliminar un nodo
    cout << "\nEliminando el registro con DNI '23456'..." << endl;
    tree.remove("23456");
    tree.display();

    cout << "Tamanio del arbol despues de la eliminacion: " << tree.size() << endl;

    return 0;


};