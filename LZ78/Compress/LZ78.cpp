#include "LZ78.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <cstdint>

LZ78::LZ78() : dictionary() {
    inicializar_diccionario();
}

void LZ78::inicializar_diccionario() {
    // Inicializar el diccionario con los caracteres ASCII
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
}

std::string juntar_vector(const std::vector<std::string> &v) {
    std::string result;
    for (const std::string &line : v) {
        result += line + '\n';
    }
    return result;
}

void LZ78::comprimir(std::vector<std::string> &content, const std::string& archivo_comprimido) {
    //std::ifstream entrada(archivo_fuente, std::ios::binary);
    std::ofstream salida(archivo_comprimido, std::ios::binary);

    if (!salida) {
        std::cerr << "Error al abrir archivo de salida." << std::endl;
        return;
    }

    std::cout << "Buffer size:" << content.size() << std::endl;
    std::string text = juntar_vector(content);
    std::string current;
    for (char ch : text) {
        std::string temp = current + ch;
        if (dictionary.find(temp) != dictionary.end()) {
            current = temp;
        } else {
            escribir_codigo(dictionary[current], salida);
            dictionary[temp] = dictionary.size();
            current = std::string(1, ch);
        }
    }

    if (!current.empty()) {
        escribir_codigo(dictionary[current], salida);
    }

    salida.close(); // Close the output file stream
    std::cout << "Archivo comprimido con éxito!" << std::endl;
    descomprimir(archivo_comprimido);
}

std::vector<std::string> LZ78::descomprimir(const std::string& archivo_comprimido) {
    std::ifstream entrada(archivo_comprimido, std::ios::binary);
    //std::ofstream salida(archivo_descomprimido, std::ios::binary);
    std::vector<std::string> output_buffer;

    if (!entrada) {
        std::cerr << "Error al abrir archivo." << std::endl;
        return output_buffer; // Devuelve un buffer vacío en caso de error;
    }

    //std::unordered_map<int, std::string> dictionary;
    //int nextCode = 1;

    //uint16_t code;
    //std::string buffer;
    //while (entrada.read(reinterpret_cast<char*>(&code), sizeof(code))) {
      //  if (code == 0) { // Manejar el caso especial para el primer caracter
        //    entrada.read(reinterpret_cast<char*>(&code), sizeof(code));
          //  buffer = static_cast<char>(code);
           // output_buffer.push_back(buffer);
    //    } else {
      //      buffer = dictionary[code - 1];
        //    output_buffer.push_back(buffer);
          //  entrada.read(reinterpret_cast<char*>(&code), sizeof(code));
       //     buffer = static_cast<char>(code);
         //   output_buffer.push_back(buffer);
    //    }
      //  dictionary[nextCode++] = buffer;
   // }
    std::vector<std::string> dictionaryReverse;
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionaryReverse.push_back(ch);
    }

    int code;
    std::string decompressed_text;
    std::string previous, entry;
    if (entrada.read(reinterpret_cast<char *>(&code), sizeof(code))) {
        if (code < static_cast<int>(dictionaryReverse.size())) {
            entry = dictionaryReverse[code];
            decompressed_text += entry;
            previous = entry;
        } else {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return output_buffer;
        }
    }

    while (entrada.read(reinterpret_cast<char *>(&code), sizeof(code))) {
        if (code < static_cast<int>(dictionaryReverse.size())) {
            entry = dictionaryReverse[code];
            decompressed_text += entry;
            dictionaryReverse.push_back(previous + entry[0]);
        } else if (code == static_cast<int>(dictionaryReverse.size())) {
            entry = previous + previous[0];
            decompressed_text += entry;
            dictionaryReverse.push_back(entry);
        } else {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return output_buffer;
        }
        previous = entry;

        // Verificar si se ha encontrado una nueva línea en el texto descomprimido
        size_t newline_pos;
        while ((newline_pos = decompressed_text.find('\n')) != std::string::npos) {
            output_buffer.push_back(decompressed_text.substr(0, newline_pos)); // Agregar la línea al buffer
            decompressed_text.erase(0, newline_pos + 1);                       // Eliminar la línea del texto descomprimido
        }
    }

    if (!decompressed_text.empty()) {
        output_buffer.push_back(decompressed_text); // Agregar la última línea al buffer
    }

    std::cout << "Archivo descomprimido con éxito!" << std::endl;
    std::cout << "Tamaño del buffer:" << output_buffer.size() << std::endl;
    
    return output_buffer;
}

void LZ78::escribir_codigo(int code, std::ofstream &fout) {
    fout.write(reinterpret_cast<const char *>(&code), sizeof(code));
}

void LZ78::mostrarDiccionario()
{
    std::cout << "Diccionario actual:" << std::endl;
    size_t i = 0;
    for (const auto &entry : dictionary)
    {
        std::cout << "[" << std::setw(3) << std::dec << i << "] -> '" << entry.first << "':{" << std::hex << entry.second << " HEX}" << std::endl;
        ++i;
    }
}