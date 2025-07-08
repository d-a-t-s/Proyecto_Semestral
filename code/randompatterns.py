import random
import numpy as np

# Configuración
num_patterns = 800
min_len = 4
max_len = 8
output_file = "patterns_800.txt"

# Alfabeto de ADN + algunos caracteres imposibles
alphabet = ['A', 'T', 'C', 'G', 'Z', 'X', 'Y', 'S']

# Generar patrones
patterns = []
for _ in range(num_patterns):
    # Longitud aleatoria
    length = random.randint(min_len, max_len)
    
    # 20% de probabilidad de generar un patrón imposible
    if random.random() < 0.2:
        # Patrón con caracteres no existentes
        pattern = ''.join(random.choices(['Z', 'X', 'Y', 'S', 'W'], k=length))
    else:
        # Patrón normal de ADN
        pattern = ''.join(random.choices(['A', 'T', 'C', 'G'], k=length))
    
    patterns.append(pattern)

# Mezclar los patrones
random.shuffle(patterns)

# Guardar en archivo (formato: pat1,pat2,...,pat100)
with open(output_file, 'w') as f:
    f.write(','.join(patterns))

print(f"Se generaron {num_patterns} patrones en {output_file}")