#!/bin/bash

# Configuración
BASE_DIR="$(dirname "$0")/.."  # Asume que el script está en code/
DATASET_DIR="${BASE_DIR}/datasets"
CONCATENATED_DIR="${DATASET_DIR}/Concatenated"
PATTERNS_DIR="${DATASET_DIR}"
OUTPUT_FILE="${BASE_DIR}/code/resultados_boyermoore.csv"
PATTERNS_COUNTS=(100 200 400 800)
DNA_FILES=(10 20 30 40)

# Encabezado del CSV
echo "Tipo_Archivo,Num_Documentos,Num_Patrones,Tiempo_Segundos" > "${OUTPUT_FILE}"

# Función para ejecutar pruebas
run_test() {
    local file="$1"
    local patterns_file="$2"
    local type="$3"
    local num_docs="$4"
    local num_patterns="$5"
    
    # Ejecutar y capturar SOLO el tiempo (última línea)
    tiempo=$(./boyermoore "$file" "$patterns_file" | tail -n 1)
    
    # Verificar si es numérico
    if [[ "$tiempo" =~ ^[0-9.]+$ ]]; then
        echo "$type,$num_docs,$num_patterns,$tiempo" >> "${OUTPUT_FILE}"
        echo "✓ $type $num_docs docs con $num_patterns patterns: ${tiempo}s"
    else
        echo "✗ Error en $file con $patterns_file (output: '$tiempo')"
    fi
}

# Verificación de directorios
if [[ ! -d "$CONCATENATED_DIR" ]]; then
    echo "Error: No se encontró $CONCATENATED_DIR"
    exit 1
fi

if [[ ! -d "$PATTERNS_DIR" ]]; then
    echo "Error: No se encontró $PATTERNS_DIR"
    exit 1
fi

# Ejecutar todas las combinaciones
for count in "${PATTERNS_COUNTS[@]}"; do
    patterns_file="${PATTERNS_DIR}/patterns_${count}.txt"
    
    if [[ ! -f "$patterns_file" ]]; then
        echo "Advertencia: No existe $patterns_file - saltando..."
        continue
    fi
    
    for num in "${DNA_FILES[@]}"; do
        file="${CONCATENATED_DIR}/concatenated_DNA_${num}"
        
        if [[ -f "$file" ]]; then
            run_test "$file" "$patterns_file" "DNA" "$num" "$count"
        else
            echo "Advertencia: No existe $file"
        fi
    done
done

echo -e "\nResultados guardados en: ${OUTPUT_FILE}"
echo "=== Resumen ==="
column -s, -t < "${OUTPUT_FILE}"