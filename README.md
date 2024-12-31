# Proyecto 1

# Autores

|                                                            | Nombre                                                                   | GitHub                                                     |
| ---------------------------------------------------------- | ------------------------------------------------------------------------ | ---------------------------------------------------------- |
| ![Mariel](https://github.com/MarielUTEC.png?size=50)       | [Mariel Carolina Tovar Tolentino](https://github.com/MarielUTEC)         | [@MarielUTEC](https://github.com/MarielUTEC)               |
| ![Noemi](https://github.com/NoemiHuarino-utec.png?size=50) | [Noemi Alejandra Huarino Anchillo](https://github.com/NoemiHuarino-utec) | [@NoemiHuarino-utec](https://github.com/NoemiHuarino-utec) |

# Introducción

## Objetivo

El proyecto Data Fusion DB busca diseñar e implementar un mini gestor de bases de datos que integre técnicas avanzadas de organización e indexación de archivos. Estas técnicas incluyen **Sequential File**, **AVL File** y **Extendible Hashing**, permitiendo una administración eficiente de datos estructurados. El sistema está orientado a optimizar operaciones críticas como inserción, eliminación y búsqueda, asegurando un manejo eficaz y escalable de grandes volúmenes de datos.

Como parte del proyecto, se desarrollará también un parser de código SQL que permita interpretar y ejecutar sentencias clave, como la creación de tablas, consultas, inserciones y eliminaciones.

## Dominio de datos

Para el desarrollo del proyecto, se utilizó un dominio de datos basado en un conjunto sintético generado con la librería `Faker` en Python. Este conjunto representa información demográfica y económica de individuos, incluyendo los siguientes atributos:

- **DNI**: Número único de 8 dígitos que identifica a cada individuo.
- **Nombre**: Nombre completo del individuo, generado en español.
- **Ocupación**: Profesión o trabajo, seleccionada de un conjunto predefinido.
- **Estado Civil**: Clasificación del estado marital (soltero/a, casado/a, divorciado/a, viudo/a).
- **Ingreso Mensual**: Salario estimado en un rango entre 1,000 y 20,000 soles.
- **Edad**: Edad del individuo, comprendida entre 18 y 80 años.
- **Sexo**: Género del individuo (F o M).

En nuestro código se visualiza de la siguiente manera:

```c++
struct Record
{
  long dni;                // Documento Nacional de Identidad
  char nombre[200];        // Nombre completo
  char ocupacion[200];     // Ocupación
  char estado_civil[200];  // Estado civil
  long ingreso_mensual;    // Ingreso mensual
  int edad;                // Edad
  char sexo[2];            // Sexo (M/F)
}
```

El conjunto de datos consta de **20,000 registros** almacenados en un archivo plano llamado `AdultDataset.csv`, preparado para evaluar las operaciones de organización e indexación implementadas. La elección de este dominio permite simular un caso práctico realista, común en aplicaciones de sistemas de gestión de bases de datos.

## Resultados esperados

Se espera implementar un sistema eficiente que permita minimizar los accesos a memoria secundaria durante las operaciones de manipulación de datos. Esto incluye garantizar tiempos de respuesta óptimos y mantener la escalabilidad en la ejecución de tareas críticas como búsqueda, inserción y eliminación. Además, los resultados incluirán un análisis comparativo del desempeño de las técnicas empleadas, destacando su efectividad en diferentes escenarios.

# Técnicas Utilizadas

En este proyecto se implementaron tres técnicas avanzadas de organización e indexación de archivos en memoria secundaria. A continuación, se describen las técnicas seleccionadas, sus principios básicos y su implementación en el contexto del proyecto.

## AVL

El uso de AVL proporciona una mayor eficiencia en la búsqueda y gestión de registros individuales debido a su naturaleza balanceada. Es una técnica ideal para aplicaciones que requieren búsquedas frecuentes y eficientes de registros o consultas por rangos.

El árbol AVL en este proyecto se implementa como un archivo indexado en memoria secundaria, donde cada nodo corresponde a un registro almacenado físicamente. Las operaciones principales incluyen inserción, búsqueda, búsqueda por rango y eliminación, garantizando el equilibrio del árbol mediante rotaciones simples y dobles.

Cada nodo del árbol (implementado en la clase `Nodo`) contiene:

- **Llave primaria (`key`)**: En este caso, corresponde al campo `dni` del registro.
- **Información del registro (`record`)**: Los datos asociados al nodo.
- **Posición en el archivo (`pos`)**: Localización física del nodo en el archivo.
- **Punteros a hijos (`left` y `right`)**: Punteros a los nodos hijo izquierdo y derecho.
- **Altura del nodo (`height`)**: Utilizada para calcular el factor de balanceo.
- **Siguiente nodo eliminado (`next_del`)**: Referencia para reutilizar posiciones en el archivo.

### Inserción (`add`)

El proceso de inserción sigue estos pasos:

1. **Verificación de posición vacía**:

   - Si el árbol está vacío, el registro se inserta como raíz.
   - Si hay nodos eliminados disponibles, se reutiliza su posición.

2. **Descenso por el árbol**:

   - Si la clave es menor que la del nodo actual, se desciende por el subárbol izquierdo.
   - Si es mayor, por el subárbol derecho.

3. **Inserción del nodo**:

   - El nuevo nodo se inserta como hoja.

4. **Balanceo del árbol**:
   - Se actualiza la altura del nodo.
   - Se calcula el factor de balanceo y, si es necesario, se realizan rotaciones (izquierda, derecha, o combinadas).

### Rotaciones

Las rotaciones se utilizan para mantener el árbol balanceado. Existen dos tipos principales de rotaciones simples y sus variantes dobles:

1. **Rotación simple a la izquierda**:

   - Se utiliza cuando el subárbol derecho es más alto que el izquierdo en dos niveles, y la clave está en el subárbol derecho.

2. **Rotación simple a la derecha**:

   - Se utiliza cuando el subárbol izquierdo es más alto que el derecho en dos niveles, y la clave está en el subárbol izquierdo.

3. **Rotación doble izquierda-derecha**:

   - Se realiza una rotación simple a la izquierda en el hijo izquierdo, seguida de una rotación simple a la derecha en el nodo actual.

4. **Rotación doble derecha-izquierda**:
   - Se realiza una rotación simple a la derecha en el hijo derecho, seguida de una rotación simple a la izquierda en el nodo actual.

#### Ejemplo gráfico:

<img src="./images/AVL_Tree_Example.gif" alt="rotations" width="400" height="300">

#### Búsqueda (`search`)

La búsqueda en un árbol AVL se realiza de forma binaria, asegurando eficiencia:

1. **Descenso por el árbol**:
   - Si la clave buscada es menor que la clave del nodo actual, se busca en el subárbol izquierdo.
   - Si es mayor, se busca en el subárbol derecho.
   - Si la clave coincide, el registro se añade al vector de resultados.
2. **Continuación recursiva**: La búsqueda continúa en ambos subárboles para identificar posibles duplicados.

### Búsqueda por rango (`rangeSearch`)

La búsqueda por rango en un árbol AVL en este proyecto se centra en el atributo `ingreso_mensual` de los registros almacenados. Su objetivo es encontrar todos los registros cuyos valores de ingreso mensual estén dentro de un rango específico definido por los límites `begin_key` (valor mínimo) y `end_key` (valor máximo). Esta búsqueda aprovecha la naturaleza ordenada del árbol AVL para optimizar el recorrido, limitando el acceso a nodos irrelevantes.

El funcionamiento es el siguiente:

1. **Comparación con el nodo actual:**

   - Si el atributo `ingreso_mensual` del nodo actual es **mayor o igual a `begin_key`**, significa que podrían existir valores relevantes en el subárbol izquierdo, por lo que se desciende recursivamente.
   - Si el atributo `ingreso_mensual` del nodo actual es **menor o igual a `end_key`**, significa que podrían existir valores relevantes en el subárbol derecho, por lo que se desciende recursivamente.

2. **Incluir el nodo actual:**

   - Si el valor de `ingreso_mensual` del nodo actual está dentro del rango [`begin_key`, `end_key`], el registro asociado al nodo se añade a la lista de resultados.

3. **Finalización de la búsqueda:**
   - La búsqueda se detiene cuando se alcanzan nodos nulos o cuando no hay más nodos que cumplan con las condiciones del rango.

**Ejemplo:**

Supongamos que tenemos un árbol AVL que indexa registros con los siguientes valores de `ingreso_mensual`: `1000`, `3000`, `5000`, `7000`, `9000`. Queremos buscar todos los registros cuyos valores de ingreso mensual estén en el rango `[3000, 7000]`.

- Nodo raíz (`5000`):

  - `5000` está dentro del rango `[3000, 7000]`, por lo que se incluye en los resultados.
  - Se busca recursivamente en el subárbol izquierdo para posibles valores menores.
  - Se busca en el subárbol derecho para posibles valores mayores.

- Subárbol izquierdo (`3000`):

  - `3000` está dentro del rango, por lo que se incluye en los resultados.
  - No se necesita buscar más a la izquierda porque todas las claves serán menores que `3000`.

- Subárbol derecho (`7000`):
  - `7000` está dentro del rango, por lo que se incluye en los resultados.
  - No se necesita buscar más a la derecha porque todas las claves serán mayores que `7000`.

**Ventajas de esta implementación:**

- **Eficiencia**: Al utilizar la estructura balanceada del árbol AVL, la búsqueda por rango solo explora nodos relevantes, evitando recorridos innecesarios.
- **Orden**: Los resultados se devuelven automáticamente en orden ascendente gracias a la naturaleza del árbol AVL.

### Eliminación (`remove`)

El proceso de eliminación en un árbol AVL considera tres casos principales:

1. **Nodo hoja**: Si el nodo no tiene hijos, se elimina directamente.
2. **Nodo con un hijo**: Si el nodo tiene solo un hijo, se reemplaza por su hijo.
3. **Nodo con dos hijos**:

   - Se encuentra el sucesor en el subárbol derecho (el nodo con el valor mínimo).
   - Se reemplaza el nodo actual por el sucesor.
   - Se elimina el sucesor del subárbol derecho.

4. **Balanceo después de la eliminación**: Después de eliminar un nodo, el árbol verifica si sigue equilibrado y aplica rotaciones si es necesario. Esto asegura que el tiempo de las operaciones permanezca eficiente.

Ejemplo de eliminación:  
<img src="./images/avlremove1.png" alt="AVL Remove Step 1" width="450" height="300">  
<img src="./images/avlremove2.png" alt="AVL Remove Step 2" width="450" height="300">  
<img src="./images/avlremove3.png" alt="AVL Remove Step 3" width="450" height="300">

### Complejidad de las operaciones

| Operación         | Descripción                                                            | Complejidad                                                  |
| ----------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------ |
| **add()**         | Inserción en las hojas. Con rotación en caso necesario.                | O(log n)                                                     |
| **search()**      | Búsqueda binaria descendiendo por los nodos.                           | O(log n)                                                     |
| **rangeSearch()** | Búsqueda de todos los registros dentro de un rango específico.         | O(log n + k), donde k es el número de elementos en el rango. |
| **remove()**      | Eliminación de un nodo, seguida de balanceo del árbol si es necesario. | O(log n)                                                     |

## Sequential File

Este método emplea dos archivos distintos para gestionar los datos: un archivo principal `main` y un archivo auxiliar `aux`. En el archivo principal los registros se mantienen ordenados. Insertamos nuevos registros en el archivo auxiliar y luego lo juntamos con el archivo principal de forma ordenada.

<img src="./images/SequentialFile.png" alt="SequentialFile" width="700" height="450">

### Inserción (add)

Insertamos nuevos registros al final del archivo auxiliar hasta que su tamaño sea mayor o igual al logaritmo de la cantidad de registros en el archivo main. Entonces hacemos `merge()` para poder insertar los registros del archivo auxiliar al archivo principal.

### Búsqueda

Realizamos una búsqueda binaria en el archivo principal, ya que los índices están ordenados por la llave primaria. Luego buscamos secuencialmente en los registros del archivo auxiliar.

### Merge

Ordenamos los registros ubicados en el archivo auxiliar y creamos un nuevo archivo para hacer la reconstrucción. Luego vamos insertando los registros antiguos del archivo y los registros del archivo auxiliar ordenado en el nuevo archivo, uno por uno, de forma ordenada. Por último borramos los archivos antiguos y renombramos los archivos nuevos. Al final tenemos un nuevo archivo auxiliar vacio y un archivo main ordenado.

Tenemos las siguientes complejidades en base a nuestra implementación:
| Función | Descripción | Complejidad |
|--------------------|-------------------------------------------------------------------------------------------------------|----------------------------------|
| **add()** | Inserción constante al final del archivo auxiliar. En caso de merge ordenamos archivo auxiliar y reconstruimos archivo principal insertando todos los registros nuevamente | Sin merge: O(1), Con merge: O(m log m + n) |
| **search()** | Búsqueda binaria en el archivo main, si no encuentra busca linealmente en el archivo aux | Principal: O(lg n), Auxiliar: O(m) |
| **rangeSearch()** | Búsqueda lineal en ambos archivos | Principal: O(n), Auxiliar: O(m) |
| **remove()** | Busco linealmente y sobreescribo el registro, si no encuentra busco en aux | Principal: O(n), Auxiliar: O(m) |

Cabe destacar que el archivo aux tiene maximo `lg(n)` elementos, por lo tanto la complejidad `O(m)` técnicamente es equivalente a `O(lg(n))`. Si implementamos el sequential file utilizando punteros se podría diminuir la complejidad de la inserción, ya que podríamos aprovechar los espacios vacíos de los registros borrados para poder insertar.

### Eliminación

Buscamos el key del registro por eliminarse, en ambos archivos. Luego sobreescribimos ese registro, con una llave `-1` para indicar que es un registro que fue borrado.

## Extendible Hashing

El **Extendible Hashing** es una técnica de organización de archivos que permite manejar eficientemente las operaciones de búsqueda, inserción y eliminación en estructuras dinámicas. A diferencia de métodos tradicionales como los árboles o el hashing estático, este enfoque se adapta automáticamente al crecimiento de los datos, evitando problemas de colisiones y reorganización masiva.

### Fundamentos teóricos

El Extendible Hashing utiliza una tabla de índices basada en un **hash binario**. Cada clave se transforma en un valor binario que determina la ubicación en la tabla. Este método permite manejar eficientemente colisiones y reorganizar dinámicamente la estructura cuando los datos aumentan. Los conceptos clave son:

1. **Profundidad global (`globalDepth`)**:

   - Representa el número de bits usados del hash para indexar los buckets.
   - Define la capacidad máxima actual de la tabla de índices.
   - Si se llena un bucket y no hay suficiente espacio, se incrementa esta profundidad.

2. **Profundidad local (`localDepth`)**:

   - Representa los bits usados por un bucket específico.
   - Si la profundidad local de un bucket lleno es menor que la global, se realiza una división del bucket (split), duplicando su espacio de almacenamiento.

3. **Hash dinámico**:

   - La clave se procesa con una función hash que genera un identificador binario.
   - Este identificador se trunca según la profundidad global para ubicar el bucket correspondiente.

4. **Split de buckets**:
   - Ocurre cuando un bucket se llena y su profundidad local es menor que la global.
   - Los datos se redistribuyen entre el bucket original y uno nuevo, ajustando los bits relevantes del hash.

### Implementación en este proyecto

En este proyecto, el sistema de Extendible Hashing está implementado mediante las clases `Bucket` y `ExtendibleHashing`, donde cada una desempeña roles específicos:

#### Buckets

Los buckets son la unidad básica de almacenamiento. Sus características principales incluyen:

- **\_localDepth**: Define la profundidad local del bucket, usada para distinguir datos al realizar un split.
- **\_values**: Arreglo de pares clave-valor que almacena los datos.
- **\_next**: Apuntador al siguiente bucket en caso de encadenamiento (chaining).

Las operaciones básicas en los buckets incluyen:

- **Búsqueda (`search`)**: Recorre los datos del bucket buscando una clave específica.
- **Eliminación (`remove`)**: Elimina un par clave-valor del bucket.
- **Impresión (`print`)**: Permite visualizar el contenido del bucket, útil para depuración.

#### Clase ExtendibleHashing

Esta clase gestiona la relación entre las claves y los buckets mediante una tabla de índices. Algunos aspectos clave de su funcionamiento son:

- **Tabla de índices**:

  - La tabla de índices almacena referencias a los buckets basadas en los valores truncados del hash.
  - Cada entrada en la tabla apunta a un bucket.

- **Inserción (`insert`)**:

  - Calcula el hash de la clave y localiza el bucket correspondiente.
  - Si el bucket tiene espacio, agrega el nuevo par clave-valor.
  - Si está lleno:
    - Realiza un split si la profundidad local es menor que la global.
    - Incrementa la profundidad global y ajusta la tabla de índices si la profundidad local ya coincide con la global.

- **Búsqueda (`search`)**:

  - Localiza la posición inicial del bucket correspondiente.
  - Busca la clave en dicho bucket y en los buckets encadenados si es necesario.

- **Eliminación (`remove`)**:
  - Localiza la clave en el bucket correspondiente y la elimina, ajustando el contenido del bucket.

#### Beneficios de esta implementación

- **Crecimiento dinámico**: La estructura se adapta automáticamente al volumen de datos, evitando colisiones y reorganización masiva.
- **Eficiencia en tiempo**: Tanto la búsqueda como la inserción tienen un costo constante en promedio (O(1)).
- **Manejo de colisiones**: Se emplea encadenamiento o duplicación controlada de buckets según el caso.

### Complejidades de las funciones

Tenemos las siguientes complejidades en base a nuestra implementación:

| Función                    | Descripción                                                                                   | Complejidad                                                                 |
| -------------------------- | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| `insert`                   | Inserta un nuevo par clave-valor. Si el bucket está lleno, realiza un split o encadenamiento. | O(1) promedio, O(n) en el peor caso (split masivo)                          |
| `search`                   | Busca una clave en el bucket correspondiente y en los buckets encadenados si es necesario.    | O(1) promedio, O(log n) en el peor caso (por profundidad de encadenamiento) |
| `remove`                   | Elimina un par clave-valor del bucket y ajusta el contenido si es necesario.                  | O(1) promedio                                                               |
| `splitBucket`              | Divide un bucket en dos cuando se llena, redistribuyendo las claves según los bits del hash.  | O(n) en el peor caso                                                        |
| `getInitialBucketPosition` | Calcula la posición inicial del bucket correspondiente usando la tabla de índices.            | O(1)                                                                        |
| `getLastBucket`            | Encuentra el último bucket encadenado a partir de una posición inicial.                       | O(log(globalDepth)) en el peor caso                                         |

### Búsquedas por rango

El **Extendible Hashing** no soporta nativamente las búsquedas por rango debido a que las claves se distribuyen según su valor hash, lo que rompe el orden natural de los datos. Por esta razón, no se ha implementado una función de búsqueda por rango en este proyecto, ya que no es una operación compatible con las características del hashing.

## SQL Parser

El SQL Parser fue diseñado para interpretar y ejecutar consultas SQL básicas como `CREATE`, `SELECT`, `INSERT`, y `DELETE`, permitiendo operar sobre tres estructuras de datos principales: **AVL File**, **Extendible Hashing**, y **Sequential File**. Estas operaciones son realizadas sobre tablas definidas en archivos y cargadas en memoria.

El parser funciona mediante un flujo que convierte una entrada en texto (consulta SQL) en tokens, los interpreta según una gramática definida y ejecuta las operaciones correspondientes sobre las estructuras de datos.

## ParserSQL.h

El archivo `ParserSQL.h` contiene la implementación del parser que interpreta y ejecuta las consultas SQL. Es el componente principal que traduce los comandos SQL en acciones sobre las estructuras de datos.

### Clases Principales

1. **`Parser`**:

   - **Responsabilidad**: Interpretar tokens generados por el `Scanner` y realizar operaciones como `CREATE`, `SELECT`, `INSERT`, y `DELETE`.
   - **Métodos Principales**:
     - `parseCreateTable`: Interpreta el comando `CREATE TABLE` y carga los datos en la estructura especificada.
     - `parseSelect`: Maneja consultas de selección, incluyendo búsquedas por igualdad (`=`) y rango (`BETWEEN`).
     - `parseInsert`: Agrega un nuevo registro a la estructura asociada.
     - `parseDelete`: Elimina registros según condiciones.

2. **`Table`**:

   - **Responsabilidad**: Representar la configuración de una tabla.
   - **Atributos**:
     - `name`: Nombre de la tabla.
     - `file`: Ruta del archivo asociado a los datos.
     - `index`: Tipo de estructura de índice usado (e.g., `AVL`, `EXTENDIBLE`, `SEQUENTIAL`).

3. **`Condition`**:
   - **Responsabilidad**: Representar condiciones de las consultas SQL (e.g., `WHERE field = value`).

### Ejemplo de Implementación

```cpp
void parseSelect() {
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    Table* table = findTable(tableName);
    if (!table) {
        error("Table not found: " + tableName);
        return;
    }

    if (table->index == "AVL") {
        // Lógica para AVL
    } else if (table->index == "EXTENDIBLE") {
        // Lógica para Hash Extensible
    } else if (table->index == "SEQUENTIAL") {
        // Lógica para Archivo Secuencial
    }
}
```

## tokensSQL.h

El archivo `tokensSQL.h` define los tokens reconocidos por el parser y el escáner. Estos tokens representan las palabras clave, operadores y estructuras reconocidas del lenguaje SQL.

### Componentes Principales

1. **Clase `Token`**:

   - Representa un token generado por el escáner.
   - **Atributos**:
     - `type`: Tipo de token (e.g., `SELECT`, `CREATE`, `ID`).
     - `lexema`: El valor asociado al token.

2. **Clase `Scanner`**:

   - **Responsabilidad**: Leer la entrada en texto y generar una secuencia de tokens.
   - **Métodos Clave**:
     - `nextToken`: Devuelve el siguiente token en la entrada.

3. **Tabla de Tokens**:
   | Token | Descripción |
   |---------------|--------------------------------------|
   | `CREATE` | Crear una nueva tabla |
   | `SELECT` | Consultar registros |
   | `INSERT` | Insertar un nuevo registro |
   | `DELETE` | Eliminar registros |
   | `BETWEEN` | Consultar un rango de valores |
   | `EQUAL` (`=`) | Condición de igualdad |
   | `ID` | Nombre de la tabla |
   | `VALUE` | Valores para registros o condición |
   | `FILE` | Especifica un archivo de datos |
   | `INDEX` | Define el tipo de índice |
   | `LPARENT` | Paréntesis izquierdo `(` |
   | `RPARENT` | Paréntesis derecho `)` |
   | `COLON` (`:`) | Separador en listas de valores |
   | `ALL` | Indicador de selección completa |

## Gramática del Parser

```ebnf
<statement> ::= <create_table> | <select> | <insert> | <delete>

<create_table> ::= "CREATE" "TABLE" <table_name> "FROM" "FILE" <value> "USING" "INDEX" <index_type> "(" <field_name> ")"
<index_type> ::= "AVL" | "EXTENDIBLE" | "SEQUENTIAL"

<select> ::= "SELECT" "ALL" "FROM" <table_name> "WHERE" <condition>

<insert> ::= "INSERT" "INTO" <table_name> "VALUES" "(" <value_list> ")"
<value_list> ::= <value> | <value> ":" <value_list>

<delete> ::= "DELETE" "FROM" <table_name> "WHERE" <condition>

<condition> ::= <field_name> "=" <value> | <field_name> "BETWEEN" <value> "AND" <value>

<table_name> ::= /* Nombre de la tabla */
<field_name> ::= /* Nombre del campo */
<value> ::= /* strings o números */
```

### Desglose por Comandos

1. **`CREATE TABLE`**:

   - Crea una nueva tabla basada en un archivo y especifica la estructura de índice utilizada.

   ```sql
   CREATE TABLE Adult FROM FILE 'D:/bd2/P1 maybe last/Proyecto-1-BD2/data/AdultDataset.csv' USING INDEX AVL('DNI');
   ```

2. **`SELECT`**:

   - Permite consultar registros por igualdad o rango.

   ```sql
   SELECT * FROM Adult WHERE DNI = '12345678';
   SELECT * FROM Adult WHERE SUELDO BETWEEN '4000' AND '5000';
   ```

3. **`INSERT`**:
   - Inserta un nuevo registro en una tabla.
   ```sql
   INSERT INTO Adult VALUES ('12345678', 'Mariel Tovar Tolentino', 'CS', 'Soltero/a', '5000', '21', 'F');
   ```
4. **`DELETE`**:
   - Elimina registros de una tabla según una condición.
   ```sql
   DELETE FROM Adult WHERE DNI='12345678';
   ```

# Pruebas de uso

A continuación mostramos ejemplos para usar el programa:

### Paso 1: Acceder al Proyecto

1. Abre una terminal y navega a la carpeta `parser` del proyecto.
   ```bash
   cd path/to/Proyecto-1-BD2/parser
   ```

### Paso 2: Ejecutar el Programa

Ejecuta el programa compilado desde la terminal:

```bash
./programa
```

### Paso 3: Realizar Consultas

Introduce consultas SQL según el formato soportado. En el archivo `comandos.txt` se pueden encontrar ejemplos adicionales de consultas SQL para cada estructura soportada. Para utilizarlos, asegúrate de ajustar las rutas de los archivos mencionados en los comandos para que sean funcionales en tu entorno.

### Paso 4: Salir del Programa

Para salir, escribe `exit` en la terminal:

```bash
exit
```

Al salir, las tablas se guardan automáticamente en `tables.csv` para ser cargadas en la siguiente ejecución.

# Resultados experimentales

## Inserción

<img src="./images/InsertP1.PNG" width="700"/>

### Análisis

- Sequential File:
  - Para 1000 y 2000 registros, el tiempo es casi constante, ya que la inserción es O(1).
  - A partir de 4000 registros, el tiempo de inserción aumenta significativamente debido al merge, lo que lleva a una complejidad de O(m log m + n).
- AVL File:
  - El tiempo sigue una crecimiento logarítmico (O(log n)) de manera constante, incluso a tamaños grandes (hasta 16000 registros). No hay un cambio brusco en la complejidad, ya que siempre se mantiene eficiente.
- Extendible Hashing:
  - En el caso promedio, la inserción es O(1), y los tiempos son bajos hasta 4000 registros.
  - Sin embargo, a partir de 8000 registros, el tiempo crece de manera más pronunciada debido a los splits masivos en el peor caso, alcanzando una complejidad de O(n).

## Búsqueda

<img src="./images/SEARCHP1.PNG" width="700"/>

| Tamaño de Datos (N) | AVL File (ms) | Secuencial File (ms) | Extendible Hash (ms) |
| ------------------- | ------------- | -------------------- | -------------------- |
| 1000                | 12            | 3                    | 1                    |
| 2000                | 24            | 7                    | 2                    |
| 4000                | 48            | 20                   | 5                    |
| 8000                | 96            | 45                   | 10                   |
| 16000               | 192           | 100                  | 20                   |

### Análisis:

- Extendible Hashing ofrece un excelente rendimiento en términos de tiempo de búsqueda. Cumple con la complejidad O(1) en promedio para los tamaños más pequeños de datos (hasta 4000 registros), y aunque su tiempo crece ligeramente a partir de 8000 registros debido al peor caso (O(log n)), sigue siendo una opción muy eficiente comparada con los otros métodos.
- Sequential File sigue O(log n) en el archivo principal, pero a medida que el tamaño aumenta, su comportamiento lineal en el archivo auxiliar (O(m)) comienza a influir más, haciendo que la curva se haga más pronunciada a partir de los 8000 registros.
- AVL mantiene una O(log n) constante y eficiente para todos los tamaños de datos. Esto lo convierte en una opción sólida para búsquedas rápidas, aunque no tan rápida como el Extendible Hashing en los primeros tamaños de datos.

# Video funcional

A continuación se adjunta el video donde explicamos el funcionamiento del proyecto:

[Video Proyecto 1 en Drive](https://drive.google.com/drive/folders/1PICXK1EqS86VyYQRm-APzpn1PcWLwjj7?usp=sharing)
