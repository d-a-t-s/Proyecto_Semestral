import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Configuración estética
sns.set(style="whitegrid")
plt.figure(figsize=(12, 8))
palette = sns.color_palette("husl", 4)  # 4 colores para los 4 tamaños de patrones

# Leer los datos
df = pd.read_csv('resultados_boyermoore.csv')
#df = pd.read_csv('resultados_kmp.csv')
#df = pd.read_csv('resultados_robinkarp.csv')

# Filtrar solo datos de DNA
dna_df = df[df['Tipo_Archivo'] == 'DNA']

# Crear el gráfico principal
plt.figure(figsize=(14, 8))

# Gráfico de líneas
for i, num_patterns in enumerate([100, 200, 400, 800]):
    subset = dna_df[dna_df['Num_Patrones'] == num_patterns]
    plt.plot(subset['Num_Documentos'], 
             subset['Tiempo_Segundos'], 
             marker='o', 
             linestyle='-', 
             linewidth=2.5,
             markersize=10,
             label=f'{num_patterns} patrones',
             color=palette[i])

# Personalización
plt.title('Algoritmo Robin-Karp en Archivos de ADN', 
          fontsize=16, pad=20)
plt.xlabel('Número de Documentos Concatenados', fontsize=12)
plt.ylabel('Tiempo de Ejecución (segundos)', fontsize=12)
plt.xticks([10, 20, 30, 40], fontsize=11)
plt.yticks(fontsize=11)

# Añadir cuadrícula
plt.grid(True, which='both', linestyle='--', linewidth=0.5)

# Leyenda personalizada
legend = plt.legend(title='Número de Patrones', 
                    fontsize=11, 
                    title_fontsize=12,
                    framealpha=1,
                    shadow=True)
legend.get_frame().set_edgecolor('black')

# Anotar los valores máximos
max_time = dna_df['Tiempo_Segundos'].max()
plt.annotate(f'Máximo: {max_time:.2f}s', 
             xy=(40, max_time), 
             xytext=(30, max_time*0.9),
             arrowprops=dict(facecolor='black', shrink=0.05),
             fontsize=11)

# Gráfico secundario (escala logarítmica opcional)
plt.figure(figsize=(14, 6))
for num_patterns in [100, 200, 400, 800]:
    subset = dna_df[dna_df['Num_Patrones'] == num_patterns]
    plt.plot(subset['Num_Documentos'], 
             subset['Tiempo_Segundos'], 
             marker='s', 
             label=f'{num_patterns} patrones')
plt.yscale('log')
plt.title('Escala Logarítmica del Tiempo', fontsize=14)
plt.xlabel('Número de Documentos', fontsize=12)
plt.ylabel('Tiempo (log s)', fontsize=12)
plt.legend()
plt.grid(True, which='both', linestyle='--')

# Guardar los gráficos
plt.figure(1)
plt.savefig('tiempos_adn_lineal.png', dpi=300, bbox_inches='tight')
plt.figure(2)
plt.savefig('tiempos_adn_log.png', dpi=300, bbox_inches='tight')

# Mostrar ambos
plt.figure(1)
plt.show()
plt.figure(2)
plt.show()

# Gráfico adicional: Mapas de calor
# pivot_df = dna_df.pivot("Num_Documentos", "Num_Patrones", "Tiempo_Segundos")
# plt.figure(figsize=(10, 6))
# sns.heatmap(pivot_df, annot=True, fmt=".1f", cmap="YlOrRd", linewidths=0.5)
# plt.title('Mapa de Calor: Tiempo por Configuración', fontsize=14)
# plt.savefig('heatmap_tiempos.png', dpi=300, bbox_inches='tight')
# plt.show()