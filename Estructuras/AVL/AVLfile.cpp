#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
using namespace std;

struct Record {
    int code;
    char nombre[12];
    char apellido[12];
    int ciclo;

    long left = -1;
    long right = -1;
    int height = 1;

    void showData() const {
        cout << "\nCodigo: " << code;
        cout << "\nNombre: " << nombre;
        cout << "\nApellido: " << apellido;
        cout << "\nCiclo : " << ciclo << "\n";
    }

    bool operator==(const Record& other) const {
        return code == other.code && strcmp(nombre, other.nombre) == 0 && strcmp(apellido, other.apellido) == 0 && ciclo == other.ciclo;
    }
};

template <typename TK>
class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename) : filename(filename), pos_root(-1) {
        ofstream createFile(this->filename, ios::binary | ios::app);
        createFile.close();
    }

    Record find(TK key) {
        return find(pos_root, key);
    }

    vector<Record> search(TK key) {
        vector<Record> results;
        search(pos_root, key, results);
        return results;
    }

    vector<Record> rangeSearch(TK begin_key, TK end_key) {
        vector<Record> results;
        rangeSearch(pos_root, begin_key, end_key, results);
        return results;
    }

    bool add(Record record) {
        long pos = getFileEndPosition();
        writeRecord(pos, record);
        pos_root = insert(pos_root, record);
        return pos != -1;
    }

    void insert(Record record) {
        pos_root = insert(pos_root, record);
    }

    bool remove(TK key) {
        bool removed = false;
        pos_root = remove(pos_root, key, removed);
        return removed;
    }

    vector<Record> inorder() {
        return inorder(pos_root);
    }

private:
    int height(long pos_node) {
        if (pos_node == -1) return 0;
        Record node = readRecord(pos_node);
        return node.height;
    }

    int getBalance(long pos_node) {
        if (pos_node == -1) return 0;
        Record node = readRecord(pos_node);
        return height(node.left) - height(node.right);
    }

    Record readRecord(long pos) {
        ifstream file(filename, ios::binary);
        file.seekg(pos * sizeof(Record));
        Record record;
        file.read((char*)&record, sizeof(Record));
        file.close();
        return record;
    }

    void writeRecord(long pos, const Record& record) {
        ofstream file(filename, ios::binary | ios::in | ios::out);
        file.seekp(pos * sizeof(Record));
        file.write((char*)&record, sizeof(Record));
        file.close();
    }

    long insert(long pos_node, Record record) {
        if (pos_node == -1) {
            pos_node = getFileEndPosition();
            writeRecord(pos_node, record);
            return pos_node;
        }

        Record current = readRecord(pos_node);

        if (record.code < current.code) {
            current.left = insert(current.left, record);
        } else if (record.code > current.code) {
            current.right = insert(current.right, record);
        } else {
            return pos_node;
        }

        current.height = 1 + max(height(current.left), height(current.right));
        writeRecord(pos_node, current);

        int balance = getBalance(pos_node);

        if (balance > 1 && record.code < readRecord(current.left).code) {
            return rightRotate(pos_node);
        }

        if (balance < -1 && record.code > readRecord(current.right).code) {
            return leftRotate(pos_node);
        }

        if (balance > 1 && record.code > readRecord(current.left).code) {
            current.left = leftRotate(current.left);
            writeRecord(pos_node, current);
            return rightRotate(pos_node);
        }

        if (balance < -1 && record.code < readRecord(current.right).code) {
            current.right = rightRotate(current.right);
            writeRecord(pos_node, current);
            return leftRotate(pos_node);
        }

        writeRecord(pos_node, current);
        return pos_node;
    }

    long rightRotate(long y_pos) {
        Record y = readRecord(y_pos);
        long x_pos = y.left;
        Record x = readRecord(x_pos);
        long T2_pos = x.right;

        x.right = y_pos;
        y.left = T2_pos;

        y.height = max(height(y.left), height(y.right)) + 1;
        x.height = max(height(x.left), height(x.right)) + 1;

        writeRecord(y_pos, y);
        writeRecord(x_pos, x);

        return x_pos;
    }

    long leftRotate(long x_pos) {
        Record x = readRecord(x_pos);
        long y_pos = x.right;
        Record y = readRecord(y_pos);
        long T2_pos = y.left;

        y.left = x_pos;
        x.right = T2_pos;

        x.height = max(height(x.left), height(x.right)) + 1;
        y.height = max(height(y.left), height(y.right)) + 1;

        writeRecord(x_pos, x);
        writeRecord(y_pos, y);

        return y_pos;
    }

    long getFileEndPosition() {
        ifstream file(filename, ios::binary | ios::ate);
        return file.tellg() / sizeof(Record);
    }

    Record find(long pos_node, TK key) {
        if (pos_node == -1) {
            Record emptyRecord;
            emptyRecord.code = -1;
            return emptyRecord;
        }

        Record current = readRecord(pos_node);

        if (current.code == key) {
            return current;
        } else if (current.code > key) {
            return find(current.left, key);
        } else {
            return find(current.right, key);
        }
    }

    void search(long pos_node, TK key, vector<Record>& results) {
        if (pos_node == -1) return;

        Record current = readRecord(pos_node);

        if (current.code == key) {
            results.push_back(current);
        }

        if (key < current.code) {
            search(current.left, key, results);
        } else {
            search(current.right, key, results);
        }
    }

    void rangeSearch(long pos_node, TK begin_key, TK end_key, vector<Record>& results) {
        if (pos_node == -1) return;

        Record current = readRecord(pos_node);

        if (begin_key <= current.code && current.code <= end_key) {
            results.push_back(current);
        }

        if (begin_key < current.code) {
            rangeSearch(current.left, begin_key, end_key, results);
        }

        if (end_key > current.code) {
            rangeSearch(current.right, begin_key, end_key, results);
        }
    }
    //----------------------------------------------------------------
    vector<Record> inorder(long pos_node) {
        vector<Record> records;

        if (pos_node == -1) return records;

        Record current = readRecord(pos_node);

        vector<Record> leftRecords = inorder(current.left);
        records.insert(records.end(), leftRecords.begin(), leftRecords.end());

        records.push_back(current);

        vector<Record> rightRecords = inorder(current.right);
        records.insert(records.end(), rightRecords.begin(), rightRecords.end());

        return records;
    }
    //----------------------------------------------------------------

    long remove(long pos_node, TK key, bool &removed) {
        if (pos_node == -1) {
            removed = false;
            return pos_node;
        }

        Record current = readRecord(pos_node);

        if (key < current.code) {
            current.left = remove(current.left, key, removed);
        } else if (key > current.code) {
            current.right = remove(current.right, key, removed);
        } else {
            removed = true;

            if (current.left == -1 || current.right == -1) {
                long temp = (current.left != -1) ? current.left : current.right;

                if (temp == -1) {
                    temp = pos_node;
                    pos_node = -1;
                } else {
                    pos_node = temp;
                }
            } else {
                long temp = minValueNode(current.right);
                Record tempRecord = readRecord(temp);

                current.code = tempRecord.code;
                strncpy(current.nombre, tempRecord.nombre, sizeof(tempRecord.nombre));
                strncpy(current.apellido, tempRecord.apellido, sizeof(tempRecord.apellido));
                current.ciclo = tempRecord.ciclo;

                current.right = remove(current.right, tempRecord.code, removed);
            }
        }

        if (pos_node == -1) {
            return pos_node;
        }

        current.height = max(height(current.left), height(current.right)) + 1;
        writeRecord(pos_node, current);

        int balance = getBalance(pos_node);

        if (balance > 1 && getBalance(current.left) >= 0) {
            return rightRotate(pos_node);
        }

        if (balance > 1 && getBalance(current.left) < 0) {
            current.left = leftRotate(current.left);
            writeRecord(pos_node, current);
            return rightRotate(pos_node);
        }

        if (balance < -1 && getBalance(current.right) <= 0) {
            return leftRotate(pos_node);
        }

        if (balance < -1 && getBalance(current.right) > 0) {
            current.right = rightRotate(current.right);
            writeRecord(pos_node, current);
            return leftRotate(pos_node);
        }

        writeRecord(pos_node, current);
        return pos_node;
    }

    long minValueNode(long pos_node) {
        Record current = readRecord(pos_node);
        while (current.left != -1) {
            pos_node = current.left;
            current = readRecord(pos_node);
        }
        return pos_node;
    }
};



vector<Record> leerCSV(const string& filename) {
    ifstream file(filename);
    vector<Record> records;
    string line;

    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo.\n";
        return records;
    }

    while (getline(file, line)) {
        if (line.empty()) continue;  // Ignorar líneas vacías

        stringstream ss(line);
        string token;
        Record record;

        // Leer el código
        getline(ss, token, ',');
        try {
            record.code = stoi(token);
        } catch (const invalid_argument& e) {
            cerr << "Error al convertir el código: " << token << endl;
            continue;  // Saltar esta línea si hay un error
        }

        // Leer el nombre
        getline(ss, token, ',');
        strncpy(record.nombre, token.c_str(), sizeof(record.nombre));
        record.nombre[sizeof(record.nombre) - 1] = '\0';  // Asegurar el null-termination

        // Leer el apellido
        getline(ss, token, ',');
        strncpy(record.apellido, token.c_str(), sizeof(record.apellido));
        record.apellido[sizeof(record.apellido) - 1] = '\0';  // Asegurar el null-termination

        // Leer el ciclo
        getline(ss, token, ',');
        try {
            record.ciclo = stoi(token);
        } catch (const invalid_argument& e) {
            cerr << "Error al convertir el ciclo: " << token << endl;
            continue;  // Saltar esta línea si hay un error
        }

        records.push_back(record);
    }

    file.close();
    return records;
}
//

void menu() { cout << "\nSeleccione una opción:\n";
    cout << "1. Insertar un registro\n";
    cout << "2. Buscar un registro\n";
    cout << "3. Eliminar un registro\n";
    cout << "4. Mostrar todos los registros en orden\n";
    cout << "5. Buscar registros por clave\n";
    cout << "6. Buscar registros por rango\n"; 
    cout << "7. Agregar un registro al archivo\n"; 
    cout << "8. Salir\n"; }

void testAVL() {
    string filename = "avl_data.dat"; 
    AVLFile<int> avlFile(filename);

    // Leer registros del archivo CSV
    vector<Record> records = leerCSV("datos.csv");

    // Insertar registros en el árbol AVL
    for (const auto& record : records) {
        avlFile.insert(record);
    }

    int option;
    do {
    menu();
    cin >> option;

    switch (option) {

        case 1: {
            Record record;
            cout << "Ingrese código: ";
            cin >> record.code;
                
            // Verificar si el código ya está registrado
            Record existingRecord = avlFile.find(record.code);
            if (existingRecord.code != -1) {
                cout << "Código ya registrado. No se puede agregar nuevamente\n";
            } else {
                cout << "Ingrese nombre: ";
                cin >> record.nombre;
                cout << "Ingrese apellido: ";
                cin >> record.apellido;
                cout << "Ingrese ciclo: ";
                cin >> record.ciclo;
                avlFile.insert(record);
                cout << "Registro insertado.\n";
            }
            break;
        }
        case 2: {
            int code;
            cout << "Ingrese el código del registro a buscar: ";
            cin >> code;
            vector<Record> results = avlFile.search(code);
            if (!results.empty()) {
                for (const auto& result : results) {
                    result.showData();
                }
            } else {
                cout << "Registro no encontrado.\n";
            }
            break;
        }
        case 3: {
            int code;
            cout << "Ingrese el código del registro a eliminar: ";
            cin >> code;
            bool removed = avlFile.remove(code);
            if (removed) {
                cout << "Registro eliminado.\n";
            } else {
                cout << "Registro no encontrado.\n";
            }
            break;
        }
        case 4: {
            cout << "Registros en orden:\n";
            vector<Record> avlRecords = avlFile.inorder();
            for (const auto& record : avlRecords) {
                record.showData();
            }
            break;
        }
        case 5: {
            int code;
            cout << "Ingrese el código del registro a buscar: ";
            cin >> code;
            vector<Record> results = avlFile.search(code);
            if (!results.empty()) {
                for (const auto& result : results) {
                    result.showData();
                }
            } else {
                cout << "Registro no encontrado.\n";
            }
            break;
        }
        case 6: {
            int start, end;
            cout << "Ingrese el código de inicio del rango: ";
            cin >> start;
            cout << "Ingrese el código de fin del rango: ";
            cin >> end;
            vector<Record> results = avlFile.rangeSearch(start, end);
            if (!results.empty()) {
                for (const auto& result : results) {
                    result.showData();
                }
            } else {
                cout << "No se encontraron registros en el rango dado.\n";
            }
            break;
        }
        case 7: {
            Record record;
            cout << "Ingrese código: ";
            cin >> record.code;
                
            // Verificar si el código ya está registrado
            Record existingRecord = avlFile.find(record.code);
            if (existingRecord.code != -1) {
                cout << "Código ya registrado. No se puede agregar nuevamente\n";
            } else {
                cout << "Ingrese nombre: ";
                cin >> record.nombre;
                cout << "Ingrese apellido: ";
                cin >> record.apellido;
                cout << "Ingrese ciclo: ";
                cin >> record.ciclo;
                avlFile.add(record);
                cout << "Registro agregado al archivo.\n";
            }
            break;
        }
        case 8:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opción inválida. Inténtelo de nuevo.\n";
    }
} while (option != 8);

}

int main() {
    testAVL(); 
    return 0; 
}