//Compilar con g++ -std=c++17 -I../include boyer_moore.cpp load_file.cpp load_patterns.cpp -o boyermoore
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
#include <chrono>
#include "../include/load_file.hpp"
#include "../include/load_patterns.hpp"

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
    vector<string> pat=load_patterns(pattern);
    string corpus = load_file(file_name);

    //Ejecucion del algoritmo de Boyer-Moore
    //Preparacion de la funcion de busqueda
    auto boyer_moore = boyer_moore_searcher(pattern.begin(), pattern.end());

    auto start = chrono::high_resolution_clock::now();

    //Busqueda del patron en el corpus

    vector<int> matches;

    for(const auto& patt : pat){
        string patt_norm = patt;
        pattern_convert(patt_norm);

        //Preparacion de la funcion de busqueda
        auto boyer_moore = boyer_moore_searcher(patt_norm.begin(), patt_norm.end());

        auto search_start = corpus.begin();
        while(true){
            auto it = search(search_start, corpus.end(), boyer_moore);
            if(it == corpus.end()) {
                break; // No se encontraron más ocurrencias
            }
            matches.push_back(it - corpus.begin());
            search_start = it + 1; // Avanzar el iterador para buscar la siguiente ocurrencia
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    //Verificacion de la busqueda
    //if(it == corpus.end()){
    //    cout << "No se encontraron ocurrencias en el archivo";
    //}else{
    //    cout << "El patron se encuentra en la posicion: " << it - corpus.begin() << endl;
    //}

    running_time *= 1e-9; //Conversion a segundos
    cout << argv[0] << ";" << pat.size() << ";" << running_time << endl;
    return 0;
}