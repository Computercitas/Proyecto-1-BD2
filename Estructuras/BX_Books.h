#include <iostream>
#include <string.h>
#include <string>
using namespace std;
//ejemplo de data
/*
ISBN;Book-Title;Book-Author;Year-Of-Publication;Publisher
195153448;Classical Mythology;Mark P. O. Morford;2002;Oxford University Press
2005018;Clara Callan;Richard Bruce Wright;2001;HarperFlamingo Canada
*/
           
/* ISBN (international standard book number) es un número identificador internacional
para libros, hasta 2006 tenian hasta 10 digitos, pero hoy tienen 13 digitos. La base de datos
tiene numeros de 10 digitos pero pondremos 13 caso se quiera expandirla futuramente con libros actuales*/
/*
Registro de mayor longitud en la columna 'Book-Title': 256 chars
Registro de mayor longitud en la columna 'Book-Author': 143 chars
Registro de mayor longitud en la columna 'Year-Of-Publication': 4 chars
Registro de mayor longitud en la columna 'Publisher': 134 chars
*/
struct BX_Books {
    unsigned long isbn;
    char book_title[256];
    char book_author[143];
    unsigned short year_of_publication;
    char publisher[134];

    BX_Books(){}
    BX_Books(string isbn, string book_title, string book_author, int year_of_publication, string publisher){
        this->isbn = stoul(isbn);
        strncpy(this->book_title, book_title.c_str(), 256);
        strncpy(this->book_author, book_author.c_str(), 143);
        this->year_of_publication = year_of_publication;
    }

    void showData() const {
        cout << "ISBN: " << isbn << endl;
        cout << "Book Title: " << book_title << endl;
        cout << "Book Author: " << book_author << endl;
        cout << "Year of Publication: " << year_of_publication << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "----------------------------------" << endl;
    }
};