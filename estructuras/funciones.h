#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <vector>
#include "Record.h"
using namespace std;

// Declaraciones de funciones para leer datos
void cargarDatosAdultDataset(const string &filename, vector<AdultRecord> &records);
void cargarDatosSuperstore(const string &filename, vector<SuperstoreRecord> &records);

#endif // FUNCIONES_H
