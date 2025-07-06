/**
 *  Compilar como: g++ -std=c++17 -I../include Rabin-Karp.cpp load_file.cpp load_patterns.cpp -o algoritmork
 *  Ejecutar: .\algoritmork <nombre_del_archivo_concatenado> <archivo_de_patrones>
 * ************************************ 
 *  Programa en C++ para buscar un patrón en un texto dado 
 *  Rabin-Karp Algorithm
 * 
 * Implementación basada en el algoritmo Rabin-Karp de GeeksforGeeks.
 * Fuente original: https://www.geeksforgeeks.org/dsa/rabin-karp-algorithm-for-pattern-searching/
 * Adaptado para:
 * 1. búsqueda en múltiples documentos con separador '$'.
 * 2. manejar el caracter '$' como separador entre documentos.
 * 3. recibir más de un patrón a buscar
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

//Función que implementa el algoritmo Rabin-Karp para buscar patrón en un texto
vector<int> rabinKarpSearch(const string &text, const string &pattern) {
    
    vector<int> matches; // Vector que almacena las posiciones donde se encuentra el patrón 
    
    // Parámetro del algoritmo
    const int d = 256;          // Base para el cálculo del hash (tamaño del alfabeto ASCII)
    const int q = 1e9 + 7;      // Número primo grande (1,000,000,007) para evitar colisiones 
    int text_len = text.length(); // Largo del texto
    int pat_len = pattern.length(); // Largo del patrón
    
    // Patrón vacío o texto más corto que el patrón
    if (pat_len == 0 || text_len < pat_len) return matches;
    
    long pat_hash = 0;  // Hash del patrón
    long text_hash = 0; // Hash de la ventana actual del texto
    long h = 1;         // Almacenará base^(longitud_patron-1) para el rolling hash
    
    // Calcular potencia = base^(longitud_patron-1) % primo
    for (int i = 0; i < pat_len - 1; i++)
        h = (h * d) % q;

    // Calcular hash inicial para el patrón y la primera ventana del texto
    for (int i = 0; i < pat_len; i++) {
        pat_hash = (d * pat_hash + pattern[i]) % q;
        text_hash = (d * text_hash + text[i]) % q;
    }

    // Deslizar el patrón sobre el texto
    for (int i = 0; i <= text_len - pat_len; i++) {
        if (pat_hash == text_hash) {
            bool match = true;
            // Si los hashes coinciden, verificar carácter por carácter
            for (int j = 0; j < pat_len; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) matches.push_back(i); // Guardar posición de inicio de la coincidencia
        }

        // Calcular hash para la siguiente ventana
        if (i < text_len - pat_len) {

            // Eliminar el carácter que sale y agregar el nuevo carácter
            text_hash = (d * (text_hash - text[i] * h) + text[i + pat_len]) % q;

            // Asegurar que el hash no sea negativo
            if (text_hash < 0) text_hash += q;
        }
    }

    return matches;
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

    pattern_convert(pattern);

    //Carga del archivo de texto y patrones
    vector<string> pat=load_patterns(pattern);
    string corpus = load_file(file_name);


    // Ejecución del algoritmo de Rabin-Karp
    auto start = chrono::high_resolution_clock::now();
    for (auto &pattern : pat){
        vector<int> matches = rabinKarpSearch(corpus, pattern);

        //Verificacion de la busqueda
    if(matches.empty()) {
            cout << "No se encontraron ocurrencias del patrón en el archivo." << endl;
        } else {
            cout << "Se encontraron " << matches.size() << " ocurrencias." << endl;
            cout << "Primera ocurrencia en la posición: " << matches[0] << endl;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double running_time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    running_time *= 1e-9; //Conversion a segundos
    cout << argv[0] << ";" << pat.size() << ";" << running_time << endl;
    return 0;
}




