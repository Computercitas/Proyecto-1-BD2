## DataFusionDB
Proyecto 1 del curso de Base de Datos 2, relacionado a organización de archivos. Integración de diferentes modelos de datos dentro de una sola BD.

### Autores

- Mariel Carolina Tovar Tolentino
- Noemi Alejandra Huarino Anchillo
- Sergio Sebastian Sotil Lozada
- Davi Magalhaes Eler
- Jose Eddison Pinedo Espinoza

## Introducción

### Objetivo
El proyecto tiene como objetivo implementar técnicas de organización e indexación de archivos, manejando diversas estructuras de datos para ello.

### Dominio de datos
Utilizamos una base de datos de libros con sus atributos y reviews. La base de datos consiste de dos archivos: 
- BX_Books.csv
- BX-Book-Ratings.csv 

#### BX_Books
Tiene los siguientes campos:
- ISBN: Código único de un libro, máximo 13 dígitos.
- Book-Title: Título del libro.
- Book-Author: Autor del libro.
- Year-Of-Publication: Año de publicación de un libro, hasta 2006.
- Publisher: Editora del libro.

#### BX-Book-Ratings
Tiene los siguientes campos: 
- User-ID: Identificador del usuario que calificó el libro, no es único.
- ISBN: Código único de un libro, máximo 13 digitos.
- Rating: Calificación del libro, entre 0 y 10.

### Resultados esperados
Indexar archivos para poder minimizar los accesos a la memoria principal y poder realizar operaciones eficientemente, de forma escalable.


## Técnicas Utilizadas

### AVL


### Sequential File
Este método emplea dos archivos distintos para gestionar los datos: un archivo principal (main) y un archivo auxiliar (aux). En el archivo principal los registros se mantienen ordenados lo que nos permite optimizar ciertas operaciones. Insertamos nuevos registros en el archivo auxiliar y luego lo juntamos con el archivo principal de forma ordenada.

#### Inserción (add)
Insertamos nuevos registros al final del archivo auxiliar hasta que su tamaño sea mayor o igual al logaritmo de la cantidad de registros en el archivo main. Entonces hacemos merge() para poder insertar los registros del archivo auxiliar al archivo principal.

#### Busqueda
Realizamos una búsqueda binaria en el archivo principal, ya que los índices están ordenados por la llave primaria. Luego buscamos secuencialmente en los registros del archivo auxiliar. 

#### Merge
Ordenamos los registros ubicados en el archivo auxiliar y creamos un nuevo archivo para hacer la reconstrucción. Luego vamos insertando  los registros antiguos del archivo y los registros del archivo auxiliar ordenado en el nuevo archivo, uno por uno, de forma ordenada. Por último borramos los archivos antiguos y renombramos los archivos nuevos. Al final tenemos un nuevo archivo auxiliar vacio y un archivo main ordenado.

#### Eliminación
Buscamos el key del registro por eliminarse, en ambos archivos. Luego sobreescribimos ese registro, con una llave -1 para indicar que es un registro que fue borrado.

### B+ tree (Se intentó)

### Extendible Hashing (Se intentó)
Se implementó algunas funcionalidades básicas del Extendible Hashing con indexación binaria, utilizando buckets de archivos. Se implementó la inserción, split y el sistema para nombrar los archivos de cada bucket.

## Resultados Experimentales


## Pruebas de uso
La implementación de una GUI utilizando librerías queda pendiente.

