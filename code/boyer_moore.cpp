//Compilar con g++ -std=c++17 -o boyer_moore boyer_moore.cpp load_file.cpp
// Ejecutar con ./boyer_moore <nombre_del_archivo> <patron_a_buscar>
//Se utiliza -std=c++17 para asegurar que se pueda utilizar la funcion de busqueda Boyer-Moore

// El archivo debe estar en el directorio ../datasets/Concatenated
// El patron a buscar debe ser pasado por consola, cambiando los espacios por guiones bajos


#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include "load_file.h"

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

    //Ejecucion del algoritmo de Boyer-Moore
    //Preparacion de la funcion de busqueda
    auto boyer_moore = boyer_moore_searcher(pattern.begin(), pattern.end());

    //Busqueda del patron en el corpus
    auto it = search(corpus.begin(), corpus.end(), boyer_moore);

    //Verificacion de la busqueda
    if(it == corpus.end()){
        cout << "No se encontraron ocurrencias en el archivo";
    }else{
        cout << "El patron se encuentra en la posicion: " << it - corpus.begin() << endl;
    }

    //Imprimimos el trozo del archivo donde se encuentra el patron
    for(int i = 0; i < pattern.size(); i++){
        cout << *(it + i);
    }
    cout << endl;
    return 0;
}