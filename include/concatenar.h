#ifndef CONCATENAR_H // Guarda para evitar inclusiones múltiples
#define CONCATENAR_H

#include <string>
#include <vector>

/**
 * Concatena archivos DNA_00 a DNA_{n-1} en un solo string con separador '$'.
 * @param folderPath Ruta a la carpeta con los archivos (ej: "../include/DNA/").
 * @param n Número de archivos a leer.
 * @return Texto concatenado, o string vacío si hay errores.
 */
std::string concatenar(const std::string& folderPath, int n, const std::string& archivo);


#endif  // PATTERN_SEARCH_H