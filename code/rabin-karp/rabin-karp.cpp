#include <vector>
#include <string>
#include <limits>

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







