#include "LZS.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

LZS::LZS() : dictionary()
{
    inicializar_diccionario();
}

void LZS::inicializar_diccionario()
{
    // Inicializar el diccionario con los caracteres ASCII
    for (int i = 0; i < 256; ++i)
    {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
}

std::string juntar_vector(const std::vector<std::string> &v)
{
    std::string result;
    for (const std::string &line : v)
    {
        result += line + '\n';
    }
    return result;
}

void LZS::comprimir(std::vector<std::string> &content, const std::string &output_file)
{
    std::ofstream fout(output_file, std::ios::binary);

    if (!fout)
    {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    std::cout << "Buffer size:" << content.size() << std::endl;

    std::string text = juntar_vector(content);

    std::string current;
    for (char ch : text)
    {
        std::string temp = current + ch;

        if (dictionary.find(temp) != dictionary.end())
        {
            current = temp;
        }
        else
        {
            escribir_codigo(dictionary[current], fout);
            dictionary[temp] = dictionary.size();
            current = std::string(1, ch);
        }
    }

    if (!current.empty())
    {
        escribir_codigo(dictionary[current], fout);
    }

    fout.close(); // Close the output file stream
    std::cout << "Archivo comprimido con éxito!" << std::endl;
    descomprimir(output_file);
}

std::vector<std::string> LZS::descomprimir(const std::string &input_file)
{
    std::ifstream fin(input_file, std::ios::binary);
    std::vector<std::string> output_buffer;

    if (!fin)
    {
        std::cerr << "Error al abrir el archivo de entrada." << std::endl;
        return output_buffer; // Devuelve un buffer vacío en caso de error
    }

    std::vector<std::string> dictionaryReverse;
    for (int i = 0; i < 256; ++i)
    {
        std::string ch(1, char(i));
        dictionaryReverse.push_back(ch);
    }

    int code;
    std::string decompressed_text;
    std::string previous, entry;
    if (fin.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        if (code < static_cast<int>(dictionaryReverse.size()))
        {
            entry = dictionaryReverse[code];
            decompressed_text += entry;
            previous = entry;
        }
        else
        {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return output_buffer;
        }
    }

    while (fin.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        if (code < static_cast<int>(dictionaryReverse.size()))
        {
            entry = dictionaryReverse[code];
            decompressed_text += entry;
            dictionaryReverse.push_back(previous + entry[0]);
        }
        else if (code == static_cast<int>(dictionaryReverse.size()))
        {
            entry = previous + previous[0];
            decompressed_text += entry;
            dictionaryReverse.push_back(entry);
        }
        else
        {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return output_buffer;
        }
        previous = entry;

        // Verificar si se ha encontrado una nueva línea en el texto descomprimido
        size_t newline_pos;
        while ((newline_pos = decompressed_text.find('\n')) != std::string::npos)
        {
            output_buffer.push_back(decompressed_text.substr(0, newline_pos)); // Agregar la línea al buffer
            decompressed_text.erase(0, newline_pos + 1);                       // Eliminar la línea del texto descomprimido
        }
    }

    if (!decompressed_text.empty())
    {
        output_buffer.push_back(decompressed_text); // Agregar la última línea al buffer
    }

    std::cout << "Archivo descomprimido con éxito!" << std::endl;
    std::cout << "Tamaño del buffer:" << output_buffer.size() << std::endl;

    return output_buffer;
}

void LZS::escribir_codigo(int code, std::ofstream &fout)
{
    fout.write(reinterpret_cast<const char *>(&code), sizeof(code));
}

void LZS::mostrarDiccionario()
{
    std::cout << "Diccionario actual:" << std::endl;
    size_t i = 0;
    for (const auto &entry : dictionary)
    {
        std::cout << "[" << std::setw(3) << std::dec << i << "] -> '" << entry.first << "':{" << std::hex << entry.second << " HEX}" << std::endl;
        ++i;
    }
}
