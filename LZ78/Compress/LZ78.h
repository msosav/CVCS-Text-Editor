#ifndef LZ78_H
#define LZ78_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

class LZ78 {
private:
    std::unordered_map<std::string, int> dictionary;
    static const int MAX_DICT_SIZE = 16384; // Declare MAX_DICT_SIZE as static const
    
public:
    LZ78();
    //void comprimir(const std::string& archivo_fuente, const std::string& archivo_comprimido);
    void comprimir(std::vector<std::string> &content, const std::string& archivo_comprimido);
    //void descomprimir(const std::string& archivo_comprimido, const std::string& archivo_descomprimido);
    std::vector<std::string> descomprimir(const std::string& archivo_comprimido);

private:
    void inicializar_diccionario();
    void escribir_codigo(int code, std::ofstream &fout);
    void mostrarDiccionario();
};

#endif // LZ78_H
