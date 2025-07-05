#include "../include/load_patterns.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <filesystem>
#include <cctype>

using namespace std;
namespace fs = filesystem;

vector<string> load_patterns(const string& file_name) {
    fs::path directory = fs::path("../datasets/") / file_name;
    ifstream input(directory);
    if (!input) {
        std::cout << "Error: No se pudo abrir el archivo de patrones: " << directory << std::endl;
        exit(1);
    }

    string line;
    getline(input, line); // Lee toda la línea (ej: "ab, cd, abb")

    vector<string> patterns;
    istringstream iss(line);
    string pattern;

    // Separa por comas y elimina espacios alrededor
    while (getline(iss, pattern, ',')) {
        // Elimina espacios en blanco al inicio/final
        pattern.erase(remove_if(pattern.begin(), pattern.end(), ::isspace), pattern.end());
        // Convierte a minúsculas (opcional)
        transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
        if (!pattern.empty()) {
            patterns.push_back(pattern);
        }
    }

    return patterns;
}