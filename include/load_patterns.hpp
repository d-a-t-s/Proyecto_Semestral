#ifndef LOAD_PATTERNS_HPP
#define LOAD_PATTERNS_HPP

#include <string>
#include <vector>

// Función para cargar patrones desde un archivo (formato: "ab, cd, abb")
std::vector<std::string> load_patterns(const std::string& file_name);

#endif 