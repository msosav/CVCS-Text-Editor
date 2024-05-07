#ifndef LZW_H
#define LZW_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

class LZW {
private:
    std::unordered_map<std::string, int> dictionary;
    static const int MAX_DICT_SIZE = 16384; // Declare MAX_DICT_SIZE as static const

public:
    LZW();

    void comprimir(std::vector<std::string> &content, const std::string &output_file);
    std::vector<std::string> descomprimir(const std::string &input_file);
    void mostrarDiccionario();

private:
    void inicializar_diccionario();
    void escribir_codigo(int code, std::ofstream &fout);
};

#endif // LZW_H
