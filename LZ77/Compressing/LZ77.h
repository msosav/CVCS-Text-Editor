#ifndef LZ77_H
#define LZ77_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

class LZ77 {
private:
    std::unordered_map<std::string, int> dictionary;
    static const int MAX_DICT_SIZE = 16384; 
public:
    LZ77();
    void comprimir(std::vector<std::string> &content, const std::string& archivo_comprimido);
    std::vector<std::string> descomprimir(const std::string& archivo_comprimido);

private:
    void inicializar_diccionario();
    void escribir_codigo(int code, std::ofstream &fout);
    void mostrarDiccionario();};

#endif // LZ77_H
