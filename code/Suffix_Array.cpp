/**
 *  Compilar como: g++ -std=c++17 -I../include Suffix_Array.cpp load_file.cpp load_patterns.cpp -o suffixar
 *  Ejecutar: .\suffixar <nombre_del_archivo_concatenado> <archivo_de_patrones>
 * ************************************ 
 *  Programa en C++ para buscar un patrón en un texto dado 
 *  Suffix Array 
 * 
 * Implementación basada en la estructura Suffix Array de GeeksforGeeks.
 * Fuente original: https://www.geeksforgeeks.org/dsa/suffix-array-set-2-a-nlognlogn-algorithm/
 * Adaptado para:
 * 1. búsqueda en múltiples documentos con separador '$'.
 * 2. manejar el caracter '$' como separador entre documentos.
 * 3. recibir más de un patrón a buscar
 * Modificaciones principales: 
 * - Implementación de busqueda binaria por nuestra cuenta para encontrar las ocurrencias de los patrones
 */


#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include "../include/load_file.hpp"
#include "../include/load_patterns.hpp"
using namespace std;

// Estructura para almacenar la información de cada sufijo
struct suffix {
    int index;    // Guarda la posición inicial del sufijo en la cadena original
    int range[2];  // Guarda dos valores de range para comparación:
                   // range[0] = valor del primer carácter
                   // range[1] = valor del segundo carácter
};

// Función de comparación para ordenar los suffixes
int  compare_suffix(struct suffix a, struct suffix b) {
    // Primero comparamos el primer range
    if (a.range[0] == b.range[0]) {
        // Si los primeros ranges son iguales, comparamos los segundos ranges
        if (a.range[1] < b.range[1]) {
            return 1; // a debe ir antes que b
        } else {
            return 0; // b debe ir antes que a
        }
    } else {
        // Si los primeros ranges son diferentes
        if (a.range[0] < b.range[0]) {
            return 1; // a debe ir antes que b
        } else {
            return 0; // b debe ir antes que a
        }
    }
}

// Función principal que construye y retorna el Suffix Array
vector<int> build_suffix_array(const string &text) {
    // Paso 1: Crear un arreglo de estructuras suffix para guardar toda la información
    int n = text.length();
    vector<suffix> suffixes(n);

    // Paso 2: Inicializar los suffixes con sus primeros 2 caracteres
    for (int i = 0; i < n; i++) {
        suffixes[i].index = i; // Guardamos la posición original
        
        // Convertimos el primer carácter a su valor numérico (0 para 'a', 1 para 'b', etc.)
        suffixes[i].range[0] = text[i] - 'a';
        
        // Convertimos el segundo carácter (o -1 si no existe)
        if ((i+1) < n) {
            suffixes[i].range[1] = text[i + 1] - 'a';
        } else {
            suffixes[i].range[1] = -1; // No hay segundo carácter
        }
    }

    // Paso 3: Ordenar los suffixes basándose en los primeros 2 caracteres
    sort(suffixes.begin(), suffixes.end(), compare_suffix);

    // Necesitamos este arreglo para mapear desde el índice original a la posición ordenada
    // Esto nos ayudará a encontrar rápidamente el range de suffixes en pasos posteriores
    vector<int> mapIndex(n);

    // Paso 4: Ordenar considerando los primeros k caracteres (k = 4, 8, 16...)
    for (int k = 4; k < 2*n; k = k*2) {
        // Asignar nuevos ranges basados en la ordenación actual
        int nuevoRange = 0;
        int rangeAnterior = suffixes[0].range[0];
        suffixes[0].range[0] = nuevoRange;
        mapIndex[suffixes[0].index] = 0;

        // Paso 4.1: Actualizar los ranges para todos los suffixes
        for (int i = 1; i < n; i++) {
            // Si el sufijo actual tiene los mismos ranges que el anterior
            if (suffixes[i].range[0] == rangeAnterior &&
                suffixes[i].range[1] == suffixes[i-1].range[1]) {
                // Mantenemos el mismo range
                rangeAnterior = suffixes[i].range[0];
                suffixes[i].range[0] = nuevoRange;
            } else {
                // Incrementamos el range para este sufijo
                rangeAnterior = suffixes[i].range[0];
                suffixes[i].range[0] = ++nuevoRange;
            }
            // Actualizamos el mapeo de índices
            mapIndex[suffixes[i].index] = i;
        }

        // Paso 4.2: Asignar el siguiente range a cada sufijo
        // Esto nos permitirá comparar los siguientes k/2 caracteres
        for (int i = 0; i < n; i++) {
            int siguientePos = suffixes[i].index + k/2;
            if (siguientePos < n) {
                // Obtenemos el range de los siguientes k/2 caracteres
                suffixes[i].range[1] = suffixes[mapIndex[siguientePos]].range[0];
            } else {
                suffixes[i].range[1] = -1; // No hay suficientes caracteres
            }
        }

        // Paso 4.3: Re-ordenar los suffixes con los nuevos ranges
        sort(suffixes.begin(), suffixes.end(), compare_suffix);
    }

    // Paso 5: Construir el Suffix Array final
    vector<int> suffixArr(n);
    for (int i = 0; i < n; i++) {
        suffixArr[i] = suffixes[i].index;
    }

    return suffixArr;
}

// Función para encontrar el range de suffixes que empiezan con el patrón
pair<int, int> find_pattern_range(const string &text, const vector<int> &suffixArr, const string &pattern) {
    int n = suffixArr.size();
    int m = pattern.size();
    int left = 0, right = n - 1;
    int start = -1, end = -1;

    // Búsqueda binaria para encontrar la primera ocurrencia
    while (left <= right) {
        int mid = (left + right) / 2;
        string suffix = text.substr(suffixArr[mid], m);
        if (suffix == pattern) {
            start = mid;
            right = mid - 1;
        } else if (suffix < pattern) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (start == -1) return {-1, -1}; // No se encontró el patrón

    // Búsqueda binaria para encontrar la última ocurrencia
    left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        string suffix = text.substr(suffixArr[mid], min(m, n - suffixArr[mid]));
        if (suffix == pattern) {
            end = mid;
            left = mid + 1;
        } else if (suffix < pattern) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return {start, end};
}

// Función para contar ocurrencias del patrón
int count_pattern_occurrences(const string &text, const string &pattern, const vector<int>& suffixArr) {
    auto range = find_pattern_range(text, suffixArr, pattern);
    
    if (range.first == -1) return 0; // No hay ocurrencias
    return range.second - range.first + 1;
}



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
    
    //Carga del archivo de texto y patrones
    vector<string> pat=load_patterns(pattern);
    string corpus = load_file(file_name);

    //Creacion del suffix array del texto
    auto start_sa = chrono::high_resolution_clock::now();
    vector<int> suffixArr = build_suffix_array(corpus);
    auto end_sa = chrono::high_resolution_clock::now();
    double running_time_sa = chrono::duration_cast<chrono::nanoseconds>(end_sa - start_sa).count();

    size_t suffixArraySize = suffixArr.size() * sizeof(int);

    auto start = chrono::high_resolution_clock::now();
    for (auto &pattern : pat){
        pattern_convert(pattern);
        int matches = count_pattern_occurrences(corpus, pattern, suffixArr);
        //Verificacion de la busqueda
        if(matches == 0) {
            cout << "No se encontraron ocurrencias del patrón en el archivo." << endl;
        } else {
            cout << "Se encontraron " << matches << " ocurrencias." << endl;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    running_time_sa *= 1e-9; //Conversion a segundos
    running_time *= 1e-9; //Conversion a segundos
    cout << argv[0] << ";" << suffixArr.size() << ";" << running_time_sa << ";" << (suffixArraySize / 1024) << "KB" << ";" << pat.size() << ";" << running_time << endl;
    return 0;
}