#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[]){

    //Verificacion de el numero de argumentos, queremos [nombre_ejecutable, numero de archivos, nombre del archivo]
    if(argc < 3 || argc > 3){
        cerr << "Usage: " << argv[0] << " <numero_de_archivos>" << " <nombre_del_archivo>"<< endl;
        return 1;
    }

    //Conversion y verificacion del numero de archivos
    int num_files = atoi(argv[1]);
    if(num_files <= 0 || num_files > 40){
        cerr << "Error: El numero de archivos de estar entre 1 y 40" << endl;
        return 1;
    }

    //Guardado de la ruta del directorio donde se encuentran los archivos particionados
    fs::path directory = fs::path("../datasets") / argv[2];

    //Se guardan las rutas de los archivos en un vector de manera ordenada
    vector<fs::path> files;
    for(const auto& entry : fs::directory_iterator(directory)){
        files.push_back(entry.path());
    }

    sort(files.begin(), files.end());

    //Inicio de la concatenacion para los archivos particionados
    //Creamos la ruta del archivo de salida
    fs::path output_path = fs::path("../datasets/Concatenated") / ("concatenated_" + string(argv[2]) + "_" + to_string(num_files));
    
    //Creacion del archivo de salida
    ofstream output(output_path, ios::out);

    //Verificacion de que se haya creado el archivo de salida
    if(!output){
        cerr << "No se pudo crear el archivo de salida: " << output_path << endl;
        return 1;
    }

    //Lectura de los archivos y concatenacion del contenido del buffer en el archivo de salida
    for(int i = 1; i <= num_files; i++){
        ifstream input(files[i-1], ios::in);
        if(!input){
            cerr << "Error: No se puedo abrir el archivo: " << files[i-1] << endl;
            return 1;
        }
        output << input.rdbuf();
    }
    return 0;
}