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

    // Sobrecarga del operador < para comparación en el AVL
    bool operator<(const AdultRecord& other) const {
        return DNI < other.DNI; // Comparación por DNI
    }
    // Sobrecarga del operador == para búsqueda específica
    bool operator==(const AdultRecord& other) const {
        return DNI == other.DNI;
    }
};

// Cargar el dataset AdultDataset.csv
void loadAdultDataset(const string& filename, AVLTree<AdultRecord>& tree) {
    ifstream file(filename);
    string line;

    // Ignorar la primera línea (encabezado)
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        AdultRecord record;

        // Leer los campos
        getline(ss, record.DNI, ',');
        getline(ss, record.Nombre, ',');
        getline(ss, record.Ocupacion, ',');
        getline(ss, record.EstadoCivil, ',');
        ss >> record.IngresoMensual;
        ss.ignore();
        ss >> record.Edad;
        ss.ignore();
        getline(ss, record.Sexo);

        // Insertar en el árbol AVL
        tree.insert(record);
    }

    file.close();
}

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

        update(node);
        return balance(node);// Balancear el árbol después de la inserción
    }

    Node<T>* findMin(Node<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

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
    }
//--
    bool contains(Node<T>* node, T value) {
        if (node == nullptr) return false;
        if (value < node->value) return contains(node->left, value);
        if (value > node->value) return contains(node->right, value);
        return true; // Valor encontrado
    }

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
    void remove(T value) {
        if (contains(root, value)) {
            root = remove(root, value);
            nodeCount--;
        }
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

    bool contains(T value) {
        return contains(root, value);
    }

    void display() {
        vector<T> result;
        inOrderTraversal(root, result);
        for (const auto& val : result) cout << val.DNI << " ";
        cout << endl;
    }

    int size() {
        return nodeCount;
    }

    // Búsqueda específica por DNI
    vector<AdultRecord> searchByDNI(string key) {
        vector<AdultRecord> result;
        if (contains(root, AdultRecord{key, "", "", "", 0, 0, ""})) {
            result.push_back(AdultRecord{key, "", "", "", 0, 0, ""});
        }
        return result;
    }
};
