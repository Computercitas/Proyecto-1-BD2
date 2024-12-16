#include <iostream>
#include "Record.h"
#include "funciones.cpp"
int main()
{
    vector<SuperstoreRecord> superstoreRecord;

    // Cargar datos desde el archivo
    cargarDatosSuperstore("../data/Superstore.csv", superstoreRecord);

    // Mostrar los primeros registros para verificar
    for (size_t i = 0; i < min(superstoreRecord.size(), (size_t)5); ++i)
    {
        cout << "row_id: " << superstoreRecord[i].row_id
             << ", order_id: " << superstoreRecord[i].order_id
             << ", order_date: " << superstoreRecord[i].order_date
             << ", ship_date: " << superstoreRecord[i].ship_date
             << ", ship_mode: " << superstoreRecord[i].ship_mode
             << ", customer_id: " << superstoreRecord[i].customer_id
             << ", customer_name: " << superstoreRecord[i].customer_name
             << ", segment: " << superstoreRecord[i].segment
             << ", city: " << superstoreRecord[i].city
             << ", state " << superstoreRecord[i].state
             << ", product_id: " << superstoreRecord[i].product_id
             << ", category: " << superstoreRecord[i].category
             << ", sub_category: " << superstoreRecord[i].sub_category
             << ", sales: " << superstoreRecord[i].sales
             << ", quantity: " << superstoreRecord[i].quantity
             << ", discount: " << superstoreRecord[i].discount
             << ", profit: " << superstoreRecord[i].profit << endl;
    }

    return 0;
}
