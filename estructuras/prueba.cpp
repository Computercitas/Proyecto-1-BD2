#include <iostream>
#include "Record.h"
#include "funciones.cpp"
int main()
{
    vector<AdultRecord> adultRecords;

    // Cargar datos desde el archivo
    cargarDatosAdultDataset("../data/AdultDataset.csv", adultRecords);

    // Mostrar los primeros registros para verificar
    for (size_t i = 0; i < min(adultRecords.size(), (size_t)5); ++i)
    {
        cout << "DNI: " << adultRecords[i].dni
             << ", Nombre: " << adultRecords[i].nombre
             << ", Ocupación: " << adultRecords[i].ocupacion
             << ", Estado Civil: " << adultRecords[i].estado_civil
             << ", Ingreso: " << adultRecords[i].ingreso_mensual
             << ", Edad: " << adultRecords[i].edad
             << ", Sexo: " << adultRecords[i].sexo << endl;
    }

    return 0;
}
