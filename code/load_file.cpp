#include "load_file.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <filesystem>
#include <cctype>

using namespace std;
namespace fs = filesystem;

//Funcion encargada de cargar el contenido de un archivo en un string
//Recibe como argumento el nombre del archivo y devuelve un string con el contenido del archivo
//Los espacion en blanco de tipo '\n', '\r' y '\t' se convierten en espacios simples

//Los espacios consecutivos se eliminan, dejando solo un espacio simple por lo que se debe de
//normalizar tambien el patron, problema pues si el patron Hola_mundo existe, entonces el patron 
//Hola____mundo tambien existe, por lo que se debe asumir que se ingresan patrones que si existen
//o en su defecto patrones que no existen derectamente, no se tomaran en cuenta patrones cuya 
//diferencia sea la cantidad de espacion en blanco que contienen

//Los caracteres se convierten a minúsculas

string load_file(const string& file_name){

    //Declaracion de la ruta del archivo que se va a cargar
    fs::path directory = fs::path("../datasets/Concatenated") / file_name; //¿Esta bien si se le pasa asi el nombre del archivo considerando que se le esta pasando por referencia a la funcion?

    //Verificacion de que el archivo existe
    ifstream input(directory, ios::binary);
    if(!input){
        cerr << "Error: No se puedo abrir el archivo: " << directory << " en load_file.cpp" << endl;
        exit(1);
    }

    //Copia del archivo a un string
    string result;
    bool last_space = false;
    char c;
    while(input.get(c)){
        if(c == '\n' || c == '\r' || c == '\t'){
            c = ' ';
        }

        //Verificacion para espacios consecutivos
        if(c == ' '){
            if(last_space){
                continue;
            }
            last_space = true;
        }else{
            last_space = false;
        }

        //Convertir a minúsculas
        result += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}