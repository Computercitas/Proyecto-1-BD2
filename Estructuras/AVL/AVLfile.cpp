#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;


struct Record {

    long left = -1;
    long right = -1;
    int height = 1;

    unsigned long long code;
    char book_title[256];
    char book_author[143];
    unsigned short year_of_publication;
    char publisher[134];

    Record(){};
    Record(string isbn, string book_title, string book_author, int year_of_publication, string publisher){
        this->code = stoull(isbn);
        strncpy(this->book_title, book_title.c_str(), 256);
        strncpy(this->book_author, book_author.c_str(), 143);
        this->year_of_publication = year_of_publication;
    }

    void showData() const {
        cout << "ISBN: " << code << endl;
        cout << "Book Title: " << book_title << endl;
        cout << "Book Author: " << book_author << endl;
        cout << "Year of Publication: " << year_of_publication << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "----------------------------------" << endl;
    }

    void showNode(){
        cout << code << "\t" << left << "\t" << right << "\t" << height << endl;
    }
};

template <typename TK>
class AVLFile {
private:
    string filename;
    long pos_root;

public:
    AVLFile(string filename, bool create) { //especificar si es para crear archivo no
        if (create){
            ofstream createFile(this->filename, ios::binary | ios::app);
            createFile.close();
        }else{
            this->filename = filename; //abrir archivo .dat con registros ya insertados
        }
        this->pos_root = -1;
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
            string empty_string = "";
            cout << "No se encontro registro con key: " << key << endl;
            //solo para imprimirlo como "vacio"
            emptyRecord.code = 0;
            strncpy(emptyRecord.book_author, empty_string.c_str(),sizeof(emptyRecord.book_author));
            strncpy(emptyRecord.book_title, empty_string.c_str(),sizeof(emptyRecord.book_title));     
            strncpy(emptyRecord.publisher, empty_string.c_str(),sizeof(emptyRecord.publisher));   
            emptyRecord.year_of_publication = 0; 
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
                strncpy(current.book_title, tempRecord.book_title, sizeof(tempRecord.book_title));
                strncpy(current.book_author, tempRecord.book_author, sizeof(tempRecord.book_author));
                current.year_of_publication = tempRecord.year_of_publication;
                strncpy(current.publisher, tempRecord.publisher, sizeof(tempRecord.publisher));

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

void parse_line(const string& line, Record& book) {
    stringstream ss(line);
    string token;

    // Parse ISBN
    getline(ss, token, ';');
    try {
        book.code = stoull(token);  // Convert to long
    } catch (const invalid_argument& e) {
        throw runtime_error("Invalid ISBN: " + token);
    } catch (const out_of_range& e) {
        throw runtime_error("ISBN out of range: " + token);
    }
    book.code = stoull(token);

    // Parse Book Title
    getline(ss, token, ';');
    strncpy(book.book_title, token.c_str(), sizeof(book.book_title) - 1);
    book.book_title[sizeof(book.book_title) - 1] = '\0'; // Ensure null-termination

    // Parse Book Author
    getline(ss, token, ';');
    strncpy(book.book_author, token.c_str(), sizeof(book.book_author) - 1);
    book.book_author[sizeof(book.book_author) - 1] = '\0'; // Ensure null-termination

    // Parse Year of Publication
    getline(ss, token, ';');
    if (token.empty() || !all_of(token.begin(), token.end(), ::isdigit)) {
        throw runtime_error("Invalid year of publication: " + token);
    }
    book.year_of_publication = static_cast<unsigned short>(stoi(token));

    // Parse Publisher
    getline(ss, token, ';');
    strncpy(book.publisher, token.c_str(), sizeof(book.publisher) - 1);
    book.publisher[sizeof(book.publisher) - 1] = '\0'; // Ensure null-termination
}

void print_nodes(string filename) {
    ifstream file(filename, ios::binary);
    Record record;
    int i=0;
    cout << "code\tleft\tright\theight" << endl;
    while (file.read((char*)&record, sizeof(Record)) && i<=20) {
        record.showNode();
        i++;
    }
    file.close();
}


int main() {
    string filename = "avl_data.dat";
    AVLFile<int> avlFile(filename, true);
    Record rec;
    vector<Record> v;
    ifstream bx_books("C:/Users/davie/VSCode_projects/DataFusionDB/data/clean_BX_Books_50k.csv");
    if (!bx_books) {throw runtime_error("No se pudo abrir el archivo");}
    string line;
    Record book;

    getline(bx_books, line); // saltar header

    int i=1;
    unsigned long max_code = 0;
    while (getline(bx_books, line)) { // insertar cada linea del csv en sequential file
        parse_line(line, book);  
        //cout << "Max code: " << max_code << endl;
        //book.showData(); cout << endl;
        if(book.code > max_code) max_code = book.code;
        avlFile.insert(book); 
        if (i%100 == 0) cout << "Insertado: " << i << endl;
        i++;
    }

    //print_nodes(filename);

    //find key invalido
    //rec = avlFile.find(123124);
    //rec.showData();

    //find key valido
    //rec = avlFile.find(3453209419);
    //rec.showData();

    //search key valido
    //v = avlFile.search(399135790);
    //for (auto elem : v) elem.showData();

    //search key invalido
    //v = avlFile.search(1231234);
    //for (auto elem : v) elem.showData();

    //range search donde ningun key en el rango es valido
    //v = avlFile.rangeSearch(0,10);
    //for (auto elem : v) elem.showData();

    //range search con keys validos en el rango
    //v = avlFile.rangeSearch(2005018,80652121);
    //for (auto elem : v) elem.showData();


        /*
    //AVLFile<int> avlFile("avl_data.dat");
    */                                  

    delete &avlFile;
    return 0; 
}
