## DataFusionDB
Proyecto 1 del curso de Base de Datos 2, relacionado a organización de archivos. Integración de diferentes modelos de datos dentro de una sola BD.

### Autores

- [Mariel Carolina Tovar Tolentino](https://github.com/MarielUTEC)  
- [Noemi Alejandra Huarino Anchillo](https://github.com/NoemiHuarino-utec)

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

El uso de AVL proporciona una mayor eficiencia en la búsqueda y gestión de registros individuales debido a su naturaleza completamente dinámica. Esto lo convierte en una opción ideal para aplicaciones que requieren búsquedas frecuentes y eficientes de registros. Además, es especialmente útil para aplicaciones que necesitan realizar búsquedas por rangos de valores en la clave de índice.

#### Inserción (add)

El proceso de inserción en un archivo AVL comienza verificando si el árbol está vacío. Si es así, se establece la raíz y se escribe el registro en la posición inicial del archivo. Si el árbol no está vacío, se lee el nodo actual y se comparan las claves. Si la clave del nuevo registro es menor, se desciende por el subárbol izquierdo; si es mayor, por el subárbol derecho. El nuevo nodo se inserta como una hoja, y durante este proceso, se verifica la altura del árbol para determinar si es necesario realizar rotaciones. Finalmente, se llama a la función de balanceo para mantener el árbol equilibrado, asegurando así la eficiencia en las operaciones de búsqueda.
| Cantidad de elementos | Tiempo de ejecución    |                                                                               
|-----------------------|------------------------|
|     10k               | 126ms                  |
|     20k               | 290ms                  |
|     50k               | 600ms                  |

Rotaciones:

<img src="./images/AVL_Tree_Example.gif" alt="rotations" width="400" height="300">



#### Búsqueda

El proceso de búsqueda en un archivo AVL comienza verificando si el nodo actual es nulo. Si no lo es, se lee el registro correspondiente desde el archivo. Luego, se compara la clave buscada con la clave del nodo actual. Si la clave buscada es menor, la búsqueda continúa recursivamente en el subárbol izquierdo. Si es mayor, se procede con el subárbol derecho. Si la clave coincide, se añade el registro al vector de resultados y se continúa la búsqueda en ambos subárboles para encontrar posibles duplicados. Este método asegura que se recorran todos los nodos relevantes, manteniendo la eficiencia del árbol AVL en las operaciones de búsqueda.

#### Búsqueda por rango

La búsqueda por rango en un archivo AVL localiza registros dentro de un rango específico comparando la clave del nodo con los límites del rango. Si la clave es mayor o igual a `beginKey`, se busca en el subárbol izquierdo; y si es menor o igual a `endKey`, se busca en el subárbol derecho. Este método garantiza la eficiencia del árbol AVL al encontrar todos los nodos relevantes.

#### Remove

El proceso de eliminación en un archivo AVL, sigue varios pasos para asegurar que el árbol se mantenga balanceado después de la eliminación de un nodo. Se lee el nodo actual y se compara la clave del nodo a eliminar con la clave del nodo actual. Si la clave es menor, se llama recursivamente a la función de eliminación en el subárbol izquierdo; si es mayor, en el subárbol derecho. Si la clave coincide, se manejan tres casos:

1. *Nodo Hoja*: Si el nodo es una hoja (sin hijos), se elimina directamente y se actualiza el puntero del nodo padre.
2. *Nodo con un Hijo* : Si el nodo tiene solo un hijo, se reemplaza el nodo por su hijo.
3. *Nodo con Dos Hijos*: Si el nodo tiene dos hijos, se encuentra el sucesor en el subárbol derecho (el nodo con el valor mínimo), se reemplaza el nodo actual por el sucesor y se elimina el sucesor del subárbol derecho.

Ilustración:

<img src="./images/avlremove1.png" alt="SequentialFile" width="450" height="300">

<img src="./images/avlremove2.png" alt="SequentialFile" width="450" height="300">

<img src="./images/avlremove3.png" alt="SequentialFile" width="450" height="300">

#### Búsqueda
Búsqueda binaria, bajamos por los nodos.

#### Remove


| Función            | Descripción                                                                                           | Complejidad                       |
|--------------------|-------------------------------------------------------------------------------------------------------|----------------------------------|
| **insert()**         | Inserción en las hojas. Con rotación caso necesario. | O(lg n) |
| **search()**         | Búsqueda binaria bajando por los nodos | O(lg n) |
| **rangeSearch()**    | 	Búsqueda de todos los registros cuyas claves están dentro de un rango específico. | O(log n + k) donde k es el número de elementos en el rango |
| **remove()**         | Eliminación de un nodo, seguido de balanceo del árbol. | O(log n) |


### Sequential File
Este método emplea dos archivos distintos para gestionar los datos: un archivo principal `main` y un archivo auxiliar `aux`. En el archivo principal los registros se mantienen ordenados. Insertamos nuevos registros en el archivo auxiliar y luego lo juntamos con el archivo principal de forma ordenada.
<img src="./images/SequentialFile.png" alt="SequentialFile" width="700" height="450">


#### Inserción (add)
Insertamos nuevos registros al final del archivo auxiliar hasta que su tamaño sea mayor o igual al logaritmo de la cantidad de registros en el archivo main. Entonces hacemos `merge()` para poder insertar los registros del archivo auxiliar al archivo principal.
| Cantidad de elementos | Tiempo de ejecución    |                                                                               
|-----------------------|------------------------|
|     1k                | 1547ms                 |
|     10k               | 82895ms                |
|     20k               | 274662ms               |

#### Búsqueda
Realizamos una búsqueda binaria en el archivo principal, ya que los índices están ordenados por la llave primaria. Luego buscamos secuencialmente en los registros del archivo auxiliar. 

#### Merge
Ordenamos los registros ubicados en el archivo auxiliar y creamos un nuevo archivo para hacer la reconstrucción. Luego vamos insertando  los registros antiguos del archivo y los registros del archivo auxiliar ordenado en el nuevo archivo, uno por uno, de forma ordenada. Por último borramos los archivos antiguos y renombramos los archivos nuevos. Al final tenemos un nuevo archivo auxiliar vacio y un archivo main ordenado.

Tenemos las siguientes complejidades en base a nuestra implementación: 
| Función            | Descripción                                                                                           | Complejidad                       |
|--------------------|-------------------------------------------------------------------------------------------------------|----------------------------------|
| **add()**         | Inserción constante al final del archivo auxiliar. En caso de merge ordenamos archivo auxiliar y reconstruimos archivo principal insertando todos los registros nuevamente | Sin merge: O(1), Con merge: O(m log m + n)  |
| **search()**      | Búsqueda binaria en el archivo main, si no encuentra busca linealmente en el archivo aux | Principal: O(lg n), Auxiliar: O(m)  |
| **rangeSearch()** | Búsqueda lineal en ambos archivos | Principal: O(n), Auxiliar: O(m)  |
| **remove()**      | Busco linealmente y sobreescribo el registro, si no encuentra busco en aux | Principal: O(n), Auxiliar: O(m)  |

Cabe destacar que el archivo aux tiene maximo `lg(n)` elementos, por lo tanto la complejidad `O(m)` técnicamente es equivalente a `O(lg(n))`. Si implementamos el sequential file utilizando punteros se podría diminuir la complejidad de la inserción, ya que podríamos aprovechar los espacios vacíos de los registros borrados para poder insertar.

#### Eliminación
Buscamos el key del registro por eliminarse, en ambos archivos. Luego sobreescribimos ese registro, con una llave `-1` para indicar que es un registro que fue borrado.

### Extendible Hashing (Se intentó)
Se implementó algunas funcionalidades básicas del Extendible Hashing con indexación binaria, utilizando buckets de archivos. Se implementó la inserción, split y el sistema para nombrar los archivos de cada bucket.

### B+ tree (Se intentó)
Funcionan las operaciones pero para una cantidad pequeña de registros.

## SQL Parser

### Descripción
Este parser permite la interpretación de instrucciones SQL comunes, como `CREATE`, `INSERT`, `DELETE`, y `SELECT`, facilitando la integración de consultas en aplicaciones que requieren manipulación de bases de datos.

### Características
- Análisis de Consultas: Capaz de procesar sentencias SQL complejas y desglosarlas en componentes estructurales, permitiendo su interpretación y ejecución.
- Soporte para Múltiples Comandos: Maneja una variedad de comandos SQL, incluyendo:
  -  `CREATE TABLE`: Crea nuevas tablas en la base de datos con la posibilidad de especificar rutas de archivo y columnas clave.
  -  `INSERT`: Permite la inserción de nuevos registros en las tablas existentes.
  -  `DELETE`: Facilita la eliminación de registros basados en condiciones específicas.
  -  `SELECT`: Realiza consultas para recuperar datos de las tablas, incluyendo la capacidad de realizar búsquedas por rango.

### Gramática del Parser
```plaintext
Program := CREATE | INSERT | DELETE | SELECT

CREATE ::= "create" "table" TABLE_ID "from" "file" filename "using" "index" structure "(" Key ")";

INSERT ::= "insert" "into" TABLE_ID "values" (unsigned long, char, char, unsigned short, char);

DELETE ::= "delete" "from" TABLE_ID "where" KEY "=" value;

SELECT ::= "select" "*" "from" TABLE_ID "where" KEY "between" value1 "and" value2 
          | "select" "*" "from" TABLE_ID "where" KEY "=" value;
```

### Estructura del Parser
- Token: Representa los tokens individuales generados por el analizador léxico, incluyendo tipos como identificadores, números, palabras reservadas y símbolos.
- Scanner: Responsable de leer la entrada de texto y generar tokens. Utiliza un enfoque basado en la máquina de estados para identificar diferentes tipos de lexemas.
- Parser: La clase principal que analiza los tokens generados por el scanner y construye una representación estructural de las consultas SQL, devolviendo objetos de declaración (como `CreateTableStatement`).
- Declaraciones: Clases que representan las diversas instrucciones SQL, como `CreateTableStatement`, `InsertStatement`, y `DeleteStatement`.

### Ejemplos de uso
    // Parser para leer una consulta
    const char* input = "create table Books from file \"data.csv\" using index sequential(ISBN);";
    const char* input2 = "insert into Books values (\"9783161484100\", \"The Catcher in the Rye\", \"J D Salinger\", 1951, \"Brown and Company\");";
    const char* input3 = "select * from Books where ISBN = 9783161484100;";
    const char* input4 = "delete from Books where ISBN = 9783161484100;";
    const char* input5 = "select * from Books where ISBN between 11 and 50;";

    // Crea un scanner con la consulta
    Scanner* scanner = new Scanner(input); // Cambiar `input1` a `input2`, `input3` o `input4` para otras consultas
    Parser* parser = new Parser(scanner);

### Discusión y análisis
Algunas estructuras pueden ser optimizadas
- Sequential file: Utilizar la implementación con punteros para poder reutilizar espacios borrados al momento de insertar. También se puede optimizar la operación de rangeSearch aprovechando que el archivo main está ordenado, para que busque en una complejidad más cercana a logaritmica en vez de lineal.
- Futuramente sería idel hacer una prueba con más elementos (1M+) para probar la escalabilidad de las estructuras.

## Pruebas de uso
La implementación de una GUI utilizando librerías queda pendiente.

