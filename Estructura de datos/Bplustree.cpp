#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int ORDEN = 4;

struct Registro {
    int key;
    string value;

    Registro(int k = 0, string v = "") : key(k), value(v) {}

    bool operator<(const Registro& otro) const {
        return key < otro.key;
    }
};

class NodoBPlus {
public:
    bool hoja;
    vector<int> claves;
    vector<Registro> registros;  // Solo las hojas almacenan registros
    vector<NodoBPlus*> hijos;
    NodoBPlus* siguiente;  // Para conectar hojas adyacentes

    NodoBPlus(bool esHoja = true) : hoja(esHoja), siguiente(nullptr) {}

    bool estaLleno() const {
        return claves.size() >= ORDEN;
    }

    bool estaBajoMinimo() const {
        return claves.size() < (ORDEN + 1) / 2;
    }

    void insertarEnHoja(const Registro& registro) {
        // Insertar el registro en su posición correcta en el nodo hoja
        auto it = lower_bound(claves.begin(), claves.end(), registro.key);
        int pos = it - claves.begin();
        claves.insert(it, registro.key);
        registros.insert(registros.begin() + pos, registro);
    }

    bool eliminarDeHoja(int key) {
        auto it = find(claves.begin(), claves.end(), key);
        if (it == claves.end()) return false;  // No se encontró la clave
        int pos = it - claves.begin();
        claves.erase(it);
        registros.erase(registros.begin() + pos);
        return true;
    }

    NodoBPlus* buscarHijo(int key) {
        for (int i = 0; i < claves.size(); ++i) {
            if (key < claves[i]) return hijos[i];
        }
        return hijos.back();
    }

    void insertarClaveInterna(int clave, NodoBPlus* nuevoHijo) {
        auto it = lower_bound(claves.begin(), claves.end(), clave);
        int pos = it - claves.begin();
        claves.insert(it, clave);
        hijos.insert(hijos.begin() + pos + 1, nuevoHijo);
    }
};

// Clase BPlusTree
class BPlusTree {
private:
    NodoBPlus* raiz;

    void dividirNodo(NodoBPlus* nodo, NodoBPlus* padre = nullptr) {
        NodoBPlus* nuevo = new NodoBPlus(nodo->hoja);
        int mid = nodo->claves.size() / 2;

        // Copiar claves y registros (en caso de nodo hoja) o hijos (en caso de nodo interno)
        nuevo->claves.assign(nodo->claves.begin() + mid + !nodo->hoja, nodo->claves.end());
        nodo->claves.resize(mid);

        if (nodo->hoja) {
            nuevo->registros.assign(nodo->registros.begin() + mid, nodo->registros.end());
            nodo->registros.resize(mid);
            nuevo->siguiente = nodo->siguiente;
            nodo->siguiente = nuevo;
        } else {
            nuevo->hijos.assign(nodo->hijos.begin() + mid + 1, nodo->hijos.end());
            nodo->hijos.resize(mid + 1);
        }

        if (nodo == raiz) {
            // Si es la raíz, crear una nueva raíz
            NodoBPlus* nuevaRaiz = new NodoBPlus(false);
            nuevaRaiz->claves.push_back(nodo->claves[mid]);
            nuevaRaiz->hijos.push_back(nodo);
            nuevaRaiz->hijos.push_back(nuevo);
            raiz = nuevaRaiz;
        } else {
            // Insertar la clave central en el padre
            int claveMedia = nodo->claves[mid];
            nodo->claves.erase(nodo->claves.begin() + mid);
            padre->insertarClaveInterna(claveMedia, nuevo);

            // Si el padre está lleno, dividir nuevamente
            if (padre->estaLleno()) {
                NodoBPlus* abuelo = obtenerPadre(raiz, padre);
                dividirNodo(padre, abuelo);
            }
        }
    }

    NodoBPlus* obtenerPadre(NodoBPlus* nodoActual, NodoBPlus* nodoObjetivo) {
        // Función auxiliar para obtener el padre de un nodo
        if (nodoActual == nullptr || nodoActual->hoja) return nullptr;
        for (auto hijo : nodoActual->hijos) {
            if (hijo == nodoObjetivo) return nodoActual;
            NodoBPlus* padre = obtenerPadre(hijo, nodoObjetivo);
            if (padre != nullptr) return padre;
        }
        return nullptr;
    }

    void ajustarEliminacion(NodoBPlus* nodo) {
        if (nodo == raiz) {
            // Si es la raíz y está vacía, hacer que el primer hijo sea la nueva raíz
            if (nodo->claves.empty() && !nodo->hoja) {
                raiz = nodo->hijos[0];
                delete nodo;
            }
            return;
        }

        NodoBPlus* padre = obtenerPadre(raiz, nodo);
        int indiceEnPadre = 0;
        while (indiceEnPadre < padre->hijos.size() && padre->hijos[indiceEnPadre] != nodo) {
            indiceEnPadre++;
        }

        NodoBPlus* hermanoIzq = (indiceEnPadre > 0) ? padre->hijos[indiceEnPadre - 1] : nullptr;
        NodoBPlus* hermanoDer = (indiceEnPadre < padre->hijos.size() - 1) ? padre->hijos[indiceEnPadre + 1] : nullptr;

        if (hermanoIzq && hermanoIzq->claves.size() > (ORDEN + 1) / 2) {
            // Redistribuir desde el hermano izquierdo
            nodo->claves.insert(nodo->claves.begin(), padre->claves[indiceEnPadre - 1]);
            padre->claves[indiceEnPadre - 1] = hermanoIzq->claves.back();
            hermanoIzq->claves.pop_back();

            if (!nodo->hoja) {
                nodo->hijos.insert(nodo->hijos.begin(), hermanoIzq->hijos.back());
                hermanoIzq->hijos.pop_back();
            }
        } else if (hermanoDer && hermanoDer->claves.size() > (ORDEN + 1) / 2) {
            // Redistribuir desde el hermano derecho
            nodo->claves.push_back(padre->claves[indiceEnPadre]);
            padre->claves[indiceEnPadre] = hermanoDer->claves.front();
            hermanoDer->claves.erase(hermanoDer->claves.begin());

            if (!nodo->hoja) {
                nodo->hijos.push_back(hermanoDer->hijos.front());
                hermanoDer->hijos.erase(hermanoDer->hijos.begin());
            }
        } else {
            // Fusionar con un hermano
            if (hermanoIzq) {
                // Fusionar con el hermano izquierdo
                hermanoIzq->claves.push_back(padre->claves[indiceEnPadre - 1]);
                hermanoIzq->claves.insert(hermanoIzq->claves.end(), nodo->claves.begin(), nodo->claves.end());
                if (!nodo->hoja) {
                    hermanoIzq->hijos.insert(hermanoIzq->hijos.end(), nodo->hijos.begin(), nodo->hijos.end());
                }
                padre->claves.erase(padre->claves.begin() + indiceEnPadre - 1);
                padre->hijos.erase(padre->hijos.begin() + indiceEnPadre);
                delete nodo;
            } else if (hermanoDer) {
                // Fusionar con el hermano derecho
                nodo->claves.push_back(padre->claves[indiceEnPadre]);
                nodo->claves.insert(nodo->claves.end(), hermanoDer->claves.begin(), hermanoDer->claves.end());
                if (!nodo->hoja) {
                    nodo->hijos.insert(nodo->hijos.end(), hermanoDer->hijos.begin(), hermanoDer->hijos.end());
                }
                padre->claves.erase(padre->claves.begin() + indiceEnPadre);
                padre->hijos.erase(padre->hijos.begin() + indiceEnPadre + 1);
                delete hermanoDer;
            }

            // Si el padre quedó con menos del mínimo de claves, ajustar también
            if (padre->estaBajoMinimo()) {
                ajustarEliminacion(padre);
            }
        }
    }

public:
    BPlusTree() : raiz(new NodoBPlus()) {}

    Registro search(int key) {
        NodoBPlus* nodo = raiz;
        while (!nodo->hoja) {
            nodo = nodo->buscarHijo(key);
        }

        for (int i = 0; i < nodo->claves.size(); ++i) {
            if (nodo->claves[i] == key) {
                return nodo->registros[i];
            }
        }

        return Registro();  // No encontrado
    }

    vector<Registro> rangeSearch(int beginKey, int endKey) {
        vector<Registro> resultado;
        NodoBPlus* nodo = raiz;

        // Navegar hasta el nodo hoja adecuado
        while (!nodo->hoja) {
            nodo = nodo->buscarHijo(beginKey);
        }

        // Iterar por las hojas hasta que se sobrepase el endKey
        while (nodo) {
            for (int i = 0; i < nodo->claves.size(); ++i) {
                if (nodo->claves[i] >= beginKey && nodo->claves[i] <= endKey) {
                    resultado.push_back(nodo->registros[i]);
                }
            }
            if (!nodo->siguiente || nodo->claves.back() > endKey) {
                break;
            }
            nodo = nodo->siguiente;
        }

        return resultado;
    }

    bool add(Registro registro) {
        NodoBPlus* nodo = raiz;

        // Navegar hasta el nodo hoja adecuado
        while (!nodo->hoja) {
            nodo = nodo->buscarHijo(registro.key);
        }

        nodo->insertarEnHoja(registro);

        // Si el nodo hoja se llena, dividir
        if (nodo->estaLleno()) {
            NodoBPlus* padre = obtenerPadre(raiz, nodo);
            dividirNodo(nodo, padre);
        }

        return true;
    }

    bool remove(int key) {
        NodoBPlus* nodo = raiz;

        // Navegar hasta el nodo hoja adecuado
        while (!nodo->hoja) {
            nodo = nodo->buscarHijo(key);
        }

        // Intentar eliminar el registro de la hoja
        if (!nodo->eliminarDeHoja(key)) return false;

        // Si el nodo está por debajo del mínimo, ajustar
        if (nodo->estaBajoMinimo()) {
            ajustarEliminacion(nodo);
        }

        return true;
    }
};

int main() {
    BPlusTree arbol;

    arbol.add(Registro(10, "A"));
    arbol.add(Registro(20, "B"));
    arbol.add(Registro(30, "C"));
    arbol.add(Registro(40, "D"));
    arbol.add(Registro(50, "E"));

    arbol.remove(30);

    vector<Registro> resultados = arbol.rangeSearch(15, 45);
    for (const Registro& reg : resultados) {
        cout << reg.key << ": " << reg.value << endl;
    }

    return 0;
}
