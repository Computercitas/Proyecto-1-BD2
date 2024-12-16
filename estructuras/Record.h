// Record.h
#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <vector>
using namespace std;

// Estructura para AdultDataset
struct AdultRecord
{
    int dni;               // Documento Nacional de Identidad
    string nombre;         // Nombre completo
    string ocupacion;      // Ocupación
    string estado_civil;   // Estado civil
    float ingreso_mensual; // Ingreso mensual
    int edad;              // Edad
    char sexo;             // Sexo (M/F)

    // Constructor por defecto
    AdultRecord() {}

    // Constructor para inicializar un registro
    AdultRecord(int dni, const string &nombre, const string &ocupacion, const string &estado_civil,
                float ingreso_mensual, int edad, char sexo)
        : dni(dni), nombre(nombre), ocupacion(ocupacion), estado_civil(estado_civil),
          ingreso_mensual(ingreso_mensual), edad(edad), sexo(sexo) {}
};

// Estructura para Superstore
struct SuperstoreRecord
{
    int row_id;           // Identificador de fila
    string order_id;      // Identificador de orden
    string order_date;    // Fecha de orden
    string ship_date;     // Fecha de envío
    string ship_mode;     // Modo de envío
    string customer_id;   // Identificador del cliente
    string customer_name; // Nombre del cliente
    string segment;       // Segmento de cliente
    string city;          // Ciudad
    string state;         // Estado
    string product_id;    // Identificador del producto
    string category;      // Categoría del producto
    string sub_category;  // Subcategoría del producto
    float sales;          // Ventas
    int quantity;         // Cantidad vendida
    float discount;       // Descuento
    float profit;         // Ganancias

    // Constructor por defecto
    SuperstoreRecord() {}

    // Constructor para inicializar un registro
    SuperstoreRecord(int row_id, const string &order_id, const string &order_date, const string &ship_date,
                     const string &ship_mode, const string &customer_id, const string &customer_name,
                     const string &segment, const string &city, const string &state, const string &product_id,
                     const string &category, const string &sub_category, float sales, int quantity, float discount, float profit)
        : row_id(row_id), order_id(order_id), order_date(order_date), ship_date(ship_date),
          ship_mode(ship_mode), customer_id(customer_id), customer_name(customer_name), segment(segment),
          city(city), state(state), product_id(product_id), category(category), sub_category(sub_category),
          sales(sales), quantity(quantity), discount(discount), profit(profit) {}
};

#endif // RECORD_H
