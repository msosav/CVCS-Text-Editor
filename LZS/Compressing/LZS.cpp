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

    dictionary[std::string(1, '\n')] = 256; // Añadir el salto de línea al diccionario
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

    std::string current;
    for (const std::string &line : content)
    {
        for (char ch : line)
        {
            std::string temp = current + ch;
            if (dictionary.find(temp) != dictionary.end())
            {
                current = temp;
            }
            else
            {
                escribir_codigo(dictionary[current], fout);
                if (dictionary.size() < static_cast<size_t>(MAX_DICT_SIZE))
                {
                    dictionary[temp] = static_cast<int>(dictionary.size());
                }
                current = std::string(1, ch);
            }
        }
        // Agregar un salto de línea al final de cada línea
        escribir_codigo(dictionary[current], fout);
        current.clear(); // Limpiar el buffer de caracteres
        // Escribir el código del salto de línea
        escribir_codigo(dictionary[std::string(1, '\n')], fout);
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
            if (entry == "\n")
            {
                std::cout << "Línea descomprimida: " << decompressed_text << std::endl;
                output_buffer.push_back(decompressed_text); // Agregar la línea al buffer
                decompressed_text.clear();                  // Limpiar el texto descomprimido para la siguiente línea
            }
            else
            {
                decompressed_text += entry;
            }
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
