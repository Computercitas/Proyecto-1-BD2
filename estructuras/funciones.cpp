#include "funciones.h"
#include <fstream>
#include <sstream>
#include <iostream>

void cargarDatosAdultDataset(const string &filename, vector<AdultRecord> &records)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Omitir la primera línea (encabezados)
    while (getline(file, line))
    {
        stringstream ss(line);
        string dni, nombre, ocupacion, estado_civil, ingreso_mensual, edad, sexo;

        getline(ss, dni, ',');
        getline(ss, nombre, ',');
        getline(ss, ocupacion, ',');
        getline(ss, estado_civil, ',');
        getline(ss, ingreso_mensual, ',');
        getline(ss, edad, ',');
        getline(ss, sexo, ',');

        records.emplace_back(stoi(dni), nombre, ocupacion, estado_civil, stof(ingreso_mensual), stoi(edad), sexo[0]);
    }
    file.close();
}

void cargarDatosSuperstore(const string &filename, vector<SuperstoreRecord> &records)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Omitir la primera línea (encabezados)
    while (getline(file, line))
    {
        stringstream ss(line);
        string row_id, order_id, order_date, ship_date, ship_mode, customer_id, customer_name, segment, city, state, product_id, category, sub_category, sales, quantity, discount, profit;

        getline(ss, row_id, ',');
        getline(ss, order_id, ',');
        getline(ss, order_date, ',');
        getline(ss, ship_date, ',');
        getline(ss, ship_mode, ',');
        getline(ss, customer_id, ',');
        getline(ss, customer_name, ',');
        getline(ss, segment, ',');
        getline(ss, city, ',');
        getline(ss, state, ',');
        getline(ss, product_id, ',');
        getline(ss, category, ',');
        getline(ss, sub_category, ',');
        getline(ss, sales, ',');
        getline(ss, quantity, ',');
        getline(ss, discount, ',');
        getline(ss, profit, ',');

        try
        {
            records.emplace_back(stoi(row_id), order_id, order_date, ship_date, ship_mode, customer_id, customer_name, segment, city, state, product_id, category, sub_category,
                                 stof(sales), stoi(quantity), stof(discount), stof(profit));
        }
        catch (const invalid_argument &e)
        {
            cerr << "Error: Datos inválidos en la línea: " << line << endl;
            continue; // Continuar con el siguiente registro en caso de error
        }
        catch (const out_of_range &e)
        {
            cerr << "Error: Datos fuera de rango en la línea: " << line << endl;
            continue; // Continuar con el siguiente registro en caso de error
        }
    }
    file.close();
}
