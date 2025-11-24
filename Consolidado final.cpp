#include <iostream>
#include <string>
using namespace std;

// NODO DEL ÁRBOL
// Cada nodo es una persona
struct Nodo {
    string codigo;        // código de la persona
    string nombre;        // nombre completo
    string parentesco;    // alumno, padre, madre, etc.
    string codigoAlumno;  // código del alumno al que pertenece

    Nodo* izquierda;
    Nodo* derecha;
};

// Crear un nuevo nodo

Nodo* crearNodo(string codigo,
                string nombre,
                string parentesco,
                string codigoAlumno)
{
    Nodo* nuevo = new Nodo;
    nuevo->codigo       = codigo;
    nuevo->nombre       = nombre;
    nuevo->parentesco   = parentesco;
    nuevo->codigoAlumno = codigoAlumno;
    nuevo->izquierda    = NULL;
    nuevo->derecha      = NULL;
    return nuevo;
}


// Insertar en el ABB (ordenado por codigo)

Nodo* insertar(Nodo* raiz,
               string codigo,
               string nombre,
               string parentesco,
               string codigoAlumno)
{
    // si el árbol está vacío, este nodo será la raíz
    if (raiz == NULL)
        return crearNodo(codigo, nombre, parentesco, codigoAlumno);

    // si el código es menor, va al lado izquierdo
    if (codigo < raiz->codigo) {
        raiz->izquierda = insertar(raiz->izquierda,
                                   codigo, nombre, parentesco, codigoAlumno);
    }
    // si el código es mayor, va al lado derecho
    else if (codigo > raiz->codigo) {
        raiz->derecha = insertar(raiz->derecha,
                                 codigo, nombre, parentesco, codigoAlumno);
    }
    // si el código ya existe, no lo insertamos
    else {
        cout << "El codigo ya existe. No se puede registrar de nuevo." << endl;
    }

    return raiz;
}


// Buscar persona por codigo

Nodo* buscar(Nodo* raiz, string codigo)
{
    if (raiz == NULL)
        return NULL;

    if (codigo == raiz->codigo)
        return raiz;

    if (codigo < raiz->codigo)
        return buscar(raiz->izquierda, codigo);
    else
        return buscar(raiz->derecha, codigo);
}


// Mostrar todas las personas (inorden)

void mostrarEnOrden(Nodo* raiz)
{
    if (raiz != NULL) {
        mostrarEnOrden(raiz->izquierda);

        cout << "Codigo: " << raiz->codigo
             << " | Nombre: " << raiz->nombre
             << " | Parentesco: " << raiz->parentesco
             << " | Alumno: " << raiz->codigoAlumno << endl;

        mostrarEnOrden(raiz->derecha);
    }
}


// Mostrar solo familia / contactos de un alumno
// (no muestra al alumno)

void mostrarFamilia(Nodo* raiz, string codAlumno)
{
    if (raiz != NULL) {
        mostrarFamilia(raiz->izquierda, codAlumno);

        // mostramos SOLO a los familiares / contactos, NO al alumno
        if (raiz->codigoAlumno == codAlumno && raiz->parentesco != "alumno") {
            cout << raiz->nombre
                 << " (" << raiz->parentesco << ")" << endl;
        }

        mostrarFamilia(raiz->derecha, codAlumno);
    }
}


// Funciones para eliminar nodos


// busca el nodo con el menor codigo (para el caso de 2 hijos)
Nodo* minimo(Nodo* nodo)
{
    Nodo* actual = nodo;
    while (actual != NULL && actual->izquierda != NULL) {
        actual = actual->izquierda;
    }
    return actual;
}

// eliminar persona por codigo
Nodo* eliminarNodo(Nodo* raiz, string codigo)
{
    if (raiz == NULL)
        return raiz;

    if (codigo < raiz->codigo) {
        raiz->izquierda = eliminarNodo(raiz->izquierda, codigo);
    }
    else if (codigo > raiz->codigo) {
        raiz->derecha = eliminarNodo(raiz->derecha, codigo);
    }
    else {
        // aquí encontramos el nodo que queremos borrar

        // caso 1: sin hijos
        if (raiz->izquierda == NULL && raiz->derecha == NULL) {
            delete raiz;
            return NULL;
        }
        // caso 2: solo hijo derecho
        else if (raiz->izquierda == NULL) {
            Nodo* temp = raiz->derecha;
            delete raiz;
            return temp;
        }
        // caso 2: solo hijo izquierdo
        else if (raiz->derecha == NULL) {
            Nodo* temp = raiz->izquierda;
            delete raiz;
            return temp;
        }
        // caso 3: dos hijos
        else {
            // tomamos el menor del subarbol derecho (sucesor)
            Nodo* temp = minimo(raiz->derecha);

            // copiamos sus datos al nodo actual
            raiz->codigo       = temp->codigo;
            raiz->nombre       = temp->nombre;
            raiz->parentesco   = temp->parentesco;
            raiz->codigoAlumno = temp->codigoAlumno;

            // eliminamos el sucesor
            raiz->derecha = eliminarNodo(raiz->derecha, temp->codigo);
        }
    }

    return raiz;
}


// Liberar memoria del árbol

void liberarArbol(Nodo* raiz)
{
    if (raiz != NULL) {
        liberarArbol(raiz->izquierda);
        liberarArbol(raiz->derecha);
        delete raiz;
    }
}


// Programa principal

int main()
{
    Nodo* raiz = NULL;
    int opcion;
    string codigo, nombre, parentesco, codAlumno;

    do {
        cout << "\n=========== MENU ===========" << endl;
        cout << "1. Registrar persona" << endl;
        cout << "2. Buscar persona por codigo" << endl;
        cout << "3. Mostrar todas las personas" << endl;
        cout << "4. Mostrar familia / contactos de un alumno" << endl;
        cout << "5. Eliminar persona por codigo" << endl;
        cout << "6. Salir" << endl;
        cout << "Seleccionar opcion: ";
        cin >> opcion;
        cin.ignore(); // limpiar el ENTER que queda

        switch (opcion) {

        case 1:
            cout << "\n--- REGISTRAR PERSONA ---" << endl;

            cout << "Codigo de la persona: ";
            getline(cin, codigo);

            cout << "Nombre completo: ";
            getline(cin, nombre);

            cout << "Parentesco (alumno, padre, madre, hermano, apoderado, etc.): ";
            getline(cin, parentesco);

            cout << "Codigo del alumno (si es alumno, repita su mismo codigo): ";
            getline(cin, codAlumno);

            raiz = insertar(raiz, codigo, nombre, parentesco, codAlumno);
            break;

        case 2:
            cout << "\n--- BUSCAR PERSONA ---" << endl;
            cout << "Codigo a buscar: ";
            getline(cin, codigo);

            {
                Nodo* p = buscar(raiz, codigo);
                if (p == NULL) {
                    cout << "No se encontro una persona con ese codigo." << endl;
                } else {
                    cout << "Codigo: " << p->codigo << endl;
                    cout << "Nombre: " << p->nombre << endl;
                    cout << "Parentesco: " << p->parentesco << endl;
                    cout << "Alumno asociado: " << p->codigoAlumno << endl;
                }
            }
            break;

        case 3:
            cout << "\n--- LISTADO COMPLETO (INORDEN) ---" << endl;
            mostrarEnOrden(raiz);
            break;

        case 4:
            cout << "\n--- FAMILIA / CONTACTOS POR ALUMNO ---" << endl;
            cout << "Codigo del alumno: ";
            getline(cin, codAlumno);

            // primero mostramos al alumno (si existe)
            {
                Nodo* alum = buscar(raiz, codAlumno);
                if (alum != NULL && alum->parentesco == "alumno") {
                    cout << "Alumno: " << alum->nombre
                         << " (codigo: " << alum->codigo << ")" << endl;
                } else {
                    cout << "No se encontro al alumno con ese codigo." << endl;
                }
            }

            cout << "Familia y contactos del alumno " << codAlumno << ":" << endl;
            mostrarFamilia(raiz, codAlumno);
            break;

        case 5:
            cout << "\n--- ELIMINAR PERSONA ---" << endl;
            cout << "Codigo de la persona a eliminar: ";
            getline(cin, codigo);

            raiz = eliminarNodo(raiz, codigo);
            cout << "Si el codigo existia, la persona ha sido eliminada." << endl;
            break;

        case 6:
            cout << "Saliendo del sistema..." << endl;
            break;

        default:
            cout << "Opcion invalida. Intente de nuevo." << endl;
        }

    } while (opcion != 6);

    liberarArbol(raiz);
    return 0;
}

