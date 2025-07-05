/**
 *  Compilar como: g++ -std=c++17 -I../include AlgoritmoKMP.cpp load_file.cpp load_patterns.cpp -o AlgoritmoKMP
 *  Ejecutar: .\algoritmokmp.exe <nombre_del_archivo_concatenado> <archivo_de_patrones>
 * ************************************ 
 *  Programa en C++ para buscar un patrón en un texto dado 
 *  KMP Algorithm
 * 
 * Implementación basada en el algoritmo Knuth-Morris-Pratt (KMP) de GeeksforGeeks.
 * Fuente original: https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
 * Adaptado para:
 * 1. búsqueda en múltiples documentos con separador '$'.
 * 2. manejar el caracter '$' como separador entre documentos.
 * 3. recibir más de un patrón a buscar
 * Modificaciones principales: 
 * - uso de contenedor map <clave, valor> -> <patron, indice> (indice desde donde se encontró el patron en el texto)
 * - Función search recibe un vector de string (patrones) en vez de sólo un string
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <chrono>
#include "../include/load_file.hpp"
#include "../include/load_patterns.hpp"
/**
 * @param pat corresponde al patron 
 * @param lps es el prefijo propio más largo (aquel que no incluye la cadena completa)
 *            Ejemplo: de "abc" -> "a" y "ab" 
 *            Asi que lps[i] es la longitud del prefijo en pat, que tambien longitud de pat[0...i]
 * En sintesis, se buscan patrones con lps, enfocandonos en subcadenas Para el patrón “AABAACAABAA”, 
 * lps[] es [0, 1, 0, 1, 2, 0, 1, 2, 3, 4, 5] (cada numero es el tamaño de lps en cada cadena pat[i])
 */

void constructLps(std::string &pat, std::vector<int> &lps) {

    // len almacena la longitud del prefijo más largo que es también un sufijo para el índice anterior
    int len = 0;

    // lps[0] siempre será 0, obviamente, por ende empezamos a comparar desde i=1
    lps[0] = 0;
    int i = 1;
    while (i < pat.length()) {

        // If characters match, increment the size of lps
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        }

        // If there is a mismatch
        else {
            if (len != 0) {

                // Update len to the previous lps value
                // to avoid reduntant comparisons
                len = lps[len - 1];
            }
            else {

                // If no matching prefix found, set lps[i] to 0
                lps[i] = 0;
                i++;
            }
        }
    }
}
/** 
 * cambio en la función "search" original que recibe los patrones a buscar 
 * ahora search es una función que recibe un vector de patrones + el texto concatenado con &
 * @param n largo de la cadena de texto "txt"
 * @param m largo del patron
 * @param results será el mapa de los resultados optenidos usando el algoritmo
 * */
std::map<std::string, std::vector<int>> search(std::vector<std::string> &patrones, std::string &txt) {
    std::map<std::string, std::vector<int>> results;
    for (auto &patron : patrones) {
        int m = patron.length();
        int n = txt.length(); 
        std::vector<int> lps(m);
        constructLps(patron, lps);

        // indices para txt y patron
        int i = 0, j = 0;
        // mientras el indice a recorrer sea menor que n
        while (i < n) {
            // Si se encuentra el patron en el txt, i y j aumentan
            if (txt[i] == patron[j]) {
                i++;
                j++;
                // se almacena el indice de inicio si todo el patron coincide
                if (j == m) {
                    results[patron].push_back(i-j);
                    // j se remplaza por el lps del sufijo anterior (evita comparaciones innecesarias)
                    j = lps[j - 1];
                }
            }
            // en caso de que no exista coincidencia se usa el lps de j anterior
            else {
                // se evita compraraciones redundantes
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
    }
    return results;
}

int main(int argc, char* argv[]){
    //Verificacion del numero de agumentos, queremos [nombre_ejecutable, nombre_del_archivo, patron a buscar]
    if(argc < 3 || argc > 3){
        std::cout << "Uso: " << argv[0] << " <nombre_del_archivo> <archivo_de_patrones>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string archivotxt=argv[1];
    std::string patron= argv[2];
    std::string txt=load_file("datasets/Concatenated/"+ archivotxt);
    std::vector<std::string> pat=load_patterns("datasets/" + patron);
    
    double tiempo_total = 0;

    auto inicio = std::chrono::high_resolution_clock::now();
    std::map<std::string, std::vector<int>> res = search(pat, txt);
    auto fin = std::chrono::high_resolution_clock::now();

    double duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio).count() * 1e-9;
    
    if (res.empty()) {
    std::cout << "No se encontraron coincidencias para los patrones." << std::endl;
    } else {
        for (auto &inicio : res) {
            std::cout << "Patrón " << inicio.first << " encontrado en posiciones: ";
            for (int pos : inicio.second) {
                std::cout << pos << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Tiempo de busqueda: " << duracion << "s" << std::endl;
    }
    return 0;
}