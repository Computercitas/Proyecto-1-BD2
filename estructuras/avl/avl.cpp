#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <vector>
#include <string>
using namespace std;

// Definición de las estructuras de datos para los datasets
struct AdultRecord {
    string DNI;
    string Nombre;
    string Ocupacion;
    string EstadoCivil;
    double IngresoMensual;
    int Edad;
    string Sexo;

    bool operator<(const AdultRecord& other) const {
        return DNI < other.DNI;  // Comparación por DNI
    }
    /*
    bool operator>(const AdultRecord& other) const {
        return DNI > other.DNI;
    }*/

    bool operator==(const AdultRecord& other) const {
        return DNI == other.DNI;
    }
};

struct SuperstoreRecord {
    int RowID;
    string OrderID;
    string OrderDate;
    string ShipDate;
    string ShipMode;
    string CustomerID;
    string CustomerName;
    string Segment;
    string Country;
    string City;
    string State;
    string PostalCode;
    string Region;
    string ProductID;
    string Category;
    string SubCategory;
    string ProductName;
    double Sales;
    int Quantity;
    double Discount;
    double Profit;

    bool operator<(const SuperstoreRecord& other) const {
        return OrderID < other.OrderID;  // Comparación por OrderID
    }
    /*
    bool operator>(const SuperstoreRecord& other) const {
        return OrderID > other.OrderID;
    }*/

    bool operator==(const SuperstoreRecord& other) const {
        return OrderID == other.OrderID;
    }
};

// Cargar el dataset AdultDataset.csv:
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

// Cargar el dataset Superstore.csv:
void loadSuperstoreDataset(const string& filename, AVLTree<SuperstoreRecord>& tree) {
    ifstream file(filename);
    string line;
    
    // Ignorar la primera línea (encabezado)
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        SuperstoreRecord record;
        
        // Leer los campos
        string temp;
        getline(ss, temp, ','); // RowID
        record.RowID = stoi(temp);
        getline(ss, record.OrderID, ',');
        getline(ss, record.OrderDate, ',');
        getline(ss, record.ShipDate, ',');
        getline(ss, record.ShipMode, ',');
        getline(ss, record.CustomerID, ',');
        getline(ss, record.CustomerName, ',');
        getline(ss, record.Segment, ',');
        getline(ss, record.Country, ',');
        getline(ss, record.City, ',');
        getline(ss, record.State, ',');
        getline(ss, record.PostalCode, ',');
        getline(ss, record.Region, ',');
        getline(ss, record.ProductID, ',');
        getline(ss, record.Category, ',');
        getline(ss, record.SubCategory, ',');
        getline(ss, record.ProductName, ',');
        ss >> record.Sales;
        ss.ignore();
        ss >> record.Quantity;
        ss.ignore();
        ss >> record.Discount;
        ss.ignore();
        ss >> record.Profit;
        
        // Insertar en el árbol AVL
        tree.insert(record);
    }

    file.close();
}


// Clase que implementa un nodo del árbol AVL.
template <typename T>
class Node {
public:
    T value; // Valor almacenado en el nodo.
    int height; // Altura del nodo.
    int balanceFactor; // Factor de balance del nodo.
    Node* left; // Hijo izquierdo.
    Node* right; // Hijo derecho.

    Node(T val) : value(val), height(0), balanceFactor(0), left(nullptr), right(nullptr) {}
};

/// Clase que implementa el árbol AVL
template <typename T>
class AVLTree {
private:
    Node<T>* root;
    int nodeCount;

    void update(Node<T>* node) {
        int leftHeight = (node->left == nullptr) ? -1 : node->left->height;
        int rightHeight = (node->right == nullptr) ? -1 : node->right->height;

        node->height = 1 + max(leftHeight, rightHeight);
        node->balanceFactor = rightHeight - leftHeight;
    }

    Node<T>* leftRotation(Node<T>* node) {
        Node<T>* newParent = node->right;
        node->right = newParent->left;
        newParent->left = node;

        update(node);
        update(newParent);

        return newParent;
    }

    Node<T>* rightRotation(Node<T>* node) {
        Node<T>* newParent = node->left;
        node->left = newParent->right;
        newParent->right = node;

        update(node);
        update(newParent);

        return newParent;
    }

    Node<T>* balance(Node<T>* node) {//-1, 0, 1
        if (node->balanceFactor < -1) {
            if (node->left->balanceFactor <= 0) {//LL
                return rightRotation(node);
            } else {
                node->left = leftRotation(node->left);//LR
                return rightRotation(node);
            }
        } else if (node->balanceFactor > 1) {
            if (node->right->balanceFactor >= 0) {//RR
                return leftRotation(node);
            } else {
                node->right = rightRotation(node->right);//RL
                return leftRotation(node);
            }
        }
        return node;
    }

    Node<T>* insert(Node<T>* node, T value) {
        if (node == nullptr) return new Node<T>(value);

        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node; // Valor duplicado, no se permite insertar.
        }

        update(node);
        return balance(node);
    }

    Node<T>* findMin(Node<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node<T>* remove(Node<T>* node, T value) {
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

    bool contains(Node<T>* node, T value) {
        if (node == nullptr) return false;
        if (value < node->value) return contains(node->left, value);
        if (value > node->value) return contains(node->right, value);
        return true;  // Valor encontrado.
    }

    void inOrderTraversal(Node<T>* node, vector<T>& result) {
        if (node == nullptr) return;
        inOrderTraversal(node->left, result);
        result.push_back(node->value);
        inOrderTraversal(node->right, result);
    }

    // Operación de búsqueda específica
    vector<T> search(Node<T>* node, T key) {
        vector<T> result;
        if (node == nullptr) return result;
        if (node->value == key) result.push_back(node->value);
        if (key < node->value) result = search(node->left, key);
        if (key > node->value) result = search(node->right, key);
        return result;
    }

    // Búsqueda por rango
    vector<T> rangeSearch(Node<T>* node, T begin_key, T end_key) {
        vector<T> result;
        if (node == nullptr) return result;
        if (node->value >= begin_key && node->value <= end_key) result.push_back(node->value);
        if (begin_key < node->value) result = rangeSearch(node->left, begin_key, end_key);
        if (end_key > node->value) result = rangeSearch(node->right, begin_key, end_key);
        return result;
    }

public:
    AVLTree() : root(nullptr), nodeCount(0) {}

    void insert(T value) {
        root = insert(root, value);
        nodeCount++;
    }

    void remove(T value) {
        if (contains(root, value)) {
            root = remove(root, value);
            nodeCount--;
        }
    }

    bool contains(T value) {
        return contains(root, value);
    }

    void display() {
        vector<T> result;
        inOrderTraversal(root, result);
        for (const auto& val : result) cout << val << " ";
        cout << endl;
    }

    int size() {
        return nodeCount;
    }

    // Implementación de operaciones adicionales para los datasets
    vector<AdultRecord> searchByDNI(string key) {
        return search(root, AdultRecord{key, "", "", "", 0, 0, ""});
    }

    vector<SuperstoreRecord> searchByOrderID(string key) {
        return search(root, SuperstoreRecord{0, key, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 0.0, 0, 0.0, 0.0});
    }

    // Búsqueda por rango
    //Búsqueda por Rango en IngresoMensual (para AdultRecord)
    vector<AdultRecord> rangeSearchByIncome(double begin, double end) {
        return rangeSearch(root, AdultRecord{"", "", "", "", begin, 0, ""}, AdultRecord{"", "", "", "", end, 0, ""});
    }
    //Búsqueda por Rango en OrderDate (para SuperstoreRecord)
    vector<SuperstoreRecord> rangeSearchByOrderDate(string begin, string end) {
        return rangeSearch(root, SuperstoreRecord{0, "", begin, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 0.0, 0, 0.0, 0.0}, 
                          SuperstoreRecord{0, "", end, "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 0.0, 0, 0.0, 0.0});
    }
};


int main() {
    AVLTree<AdultRecord> adultTree;
    AVLTree<SuperstoreRecord> superstoreTree;
    
    // Cargar los datasets
    loadAdultDataset("AdultDataset.csv", adultTree);
    loadSuperstoreDataset("Superstore.csv", superstoreTree);

    // Ejemplo de búsqueda específica en AdultDataset por DNI
    string searchDNI = "12345678";
    vector<AdultRecord> resultsAdult = adultTree.search(searchDNI);
    cout << "Búsqueda por DNI (" << searchDNI << "):\n";
    for (const auto& record : resultsAdult) {
        cout << record.Nombre << ", " << record.Ocupacion << endl;
    }

    // Ejemplo de búsqueda por rango en Superstore por OrderDate
    string beginDate = "2023-01-01", endDate = "2023-12-31";
    vector<SuperstoreRecord> rangeResults = superstoreTree.rangeSearch(beginDate, endDate);
    cout << "Búsqueda por rango en OrderDate (" << beginDate << " a " << endDate << "):\n";
    for (const auto& record : rangeResults) {
        cout << record.OrderID << ", " << record.OrderDate << endl;
    }

    // Ejemplo de eliminación en AdultDataset por DNI
    bool removed = adultTree.remove("12345678");
    if (removed) {
        cout << "Registro con DNI 12345678 eliminado\n";
    } else {
        cout << "No se encontró el registro con DNI 12345678\n";
    }

    // Ejemplo de eliminación en Superstore por OrderID
    bool removedOrder = superstoreTree.remove("SO123456");
    if (removedOrder) {
        cout << "Registro con OrderID SO123456 eliminado\n";
    } else {
        cout << "No se encontró el registro con OrderID SO123456\n";
    }

    return 0;
}

