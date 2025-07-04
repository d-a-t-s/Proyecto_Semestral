//Compilar con g++ -std=c++17 -o rk rabin-karp.cpp load_file.cpp
// Ejecutar con ./rk <nombre_del_archivo> <patron_a_buscar>
//Se utiliza -std=c++17 para asegurar que se pueda utilizar la funcion de busqueda Boyer-Moore

// El archivo debe estar en el directorio ../../datasets/Concatenated
// El patron a buscar debe ser pasado por consola, cambiando los espacios por guiones bajos


#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include "../load_file.h"
#include "rabin-karp.h"

using namespace std;

//Por comodidad, los patrones se pasan por consola cambiando los espacios por un guion bajo
void pattern_convert(string& pattern){
    for(auto& c : pattern){
        if( c == '_'){
            c = ' ';
        }
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
}

//Se hace la busqueda del patron sin distincion de mayusculas y minusculas normalizando el corpus y el patron
int main(int argc, char* argv[]){

    //Verificacion del numero de agumentos, queremos [nombre_ejecutable, nombre_del_archivo, patron_a_buscar]
    if(argc < 3 || argc > 3){
        cerr << "Uso: " << argv[0] << " <nombre_del_archivo> <patron_a_buscar>" << endl;
        return 1;
    }

    //Guardado del nombre del archivo y del patron a buscar
    string file_name = argv[1];
    string pattern = argv[2];
    pattern_convert(pattern);

    //Carga del archivo
    string corpus = load_file(file_name);

    // Ejecución del algoritmo de Rabin-Karp
    vector<int> matches = rabinKarpSearch(corpus, pattern);

    //Verificacion de la busqueda
   if(matches.empty()) {
        cout << "No se encontraron ocurrencias del patrón en el archivo." << endl;
    } else {
        cout << "Se encontraron " << matches.size() << " ocurrencias." << endl;
        cout << "Primera ocurrencia en la posición: " << matches[0] << endl;
    }
    
    return 0;
}