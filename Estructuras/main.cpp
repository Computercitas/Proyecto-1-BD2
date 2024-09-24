
#include "SequentialFile.cpp"

int main(){

    
    SequentialFile<BX_Books> * seq = new SequentialFile<BX_Books>();
    
    ifstream bx_books("../data/BX_Books_p.csv");
    if (!bx_books) {throw runtime_error("No se pudo abrir el archivo");}
    string line;
    BX_Books book;

    getline(bx_books, line); // saltar header
    int i=2;
    while (getline(bx_books, line)) { // insertar cada linea del csv en sequential file
        parse_line(line, book);  
        seq->add(book);  
        if(i%500 == 0){
            cout <<"insertado: " <<i << endl;
        }
        i+=1;
        
    }

    // Buscar un registro
    vector<BX_Books> result = seq->search(1853262404);

    // busqueda en rango 
    vector<BX_Books> result2 = seq->rangeSearch(2005018, 802116825);

    // Imprimir resultados
    result[0].showData();

    cout << "Resultados de busqueda en rango:" << endl;
    for (const BX_Books& book_s : result2) {
        book_s.showData();
    }
   
    delete seq;
    return 0;
};




