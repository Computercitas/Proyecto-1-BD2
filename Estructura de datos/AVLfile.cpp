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
};