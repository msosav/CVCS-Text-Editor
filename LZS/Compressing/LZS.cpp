#include "LZS.h"
#include <fstream>
#include <ostream>
#include <string>

LZS::LZS() : dictionary() {
    inicializar_diccionario();
}

void LZS::inicializar_diccionario() {
    // Inicializar el diccionario con los caracteres ASCII
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
}

void LZS::comprimir(std::vector<std::string> &content, const std::string &output_file) {
    std::ofstream fout(output_file, std::ios::binary);

    if (!fout) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    std::string current;
    for (const std::string &line : content) {
        for (char ch : line) {
            std::string temp = current + ch;
            if (dictionary.find(temp) != dictionary.end()) {
                current = temp;
            } else {
                escribir_codigo(dictionary[current], fout);
                if (dictionary.size() < static_cast<size_t>(MAX_DICT_SIZE)) {
                    dictionary[temp] = static_cast<int>(dictionary.size());
                }
                current = ch;  // Reset current to the current character
            }
        }
        // Add a newline character to the compressed output after each line
        escribir_codigo(dictionary[current], fout);
        current.clear(); // Clear current to start a new line
    }

    fout.close(); // Close the output file stream
    std::cout << "Archivo comprimido con éxito!" << std::endl;
}


std::vector<std::string> LZS::descomprimir(const std::string &input_file) {
    std::ifstream fin(input_file, std::ios::binary);
    std::vector<std::string> output_buffer;

    if (!fin) {
        std::cerr << "Error opening input file." << std::endl;
        return output_buffer; // Devuelve un buffer vacío en caso de error
    }

    std::vector<std::string> dictionaryReverse(static_cast<size_t>(MAX_DICT_SIZE));
    for (auto entry : dictionary) {
        dictionaryReverse[entry.second] = entry.first;
    }

    int code;
    std::string current, previous;
    while (fin.read(reinterpret_cast<char*>(&code), sizeof(code))) {
        std::string entry;
        if (code < static_cast<int>(dictionaryReverse.size())) {
            entry = dictionaryReverse[code];
        } else if (code == static_cast<int>(dictionaryReverse.size())) {
            entry = previous + previous[0];
        } else {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return output_buffer; // Devuelve un buffer vacío en caso de error
        }

        output_buffer.push_back(entry);

        if (!previous.empty() && dictionaryReverse.size() < static_cast<size_t>(MAX_DICT_SIZE)) {
            dictionaryReverse.push_back(previous + entry[0]);
        }
        previous = entry;
    }

    std::cout << "Archivo descomprimido con éxito!" << std::endl;
    return output_buffer;
}

void LZS::escribir_codigo(int code, std::ofstream &fout) {
    fout.write(reinterpret_cast<const char*>(&code), sizeof(code));
}

void LZS::mostrarDiccionario() {
    std::cout << "Diccionario actual:" << std::endl;
    size_t i = 0;
    for (const auto &entry : dictionary) {
        std::cout <<"["<<std::setw(3)<<std::dec<<i <<"] -> '"<< entry.first << "':{"<< std::hex <<entry.second<<" HEX}"<<std::endl;
        ++i;
    }
}
