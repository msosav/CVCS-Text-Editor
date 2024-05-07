#include "LZS.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

LZS::LZS() : dictionary()
{
    initialize_dictionary();
}

void LZS::initialize_dictionary()
{
    for (int i = 0; i < 256; ++i)
    {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
}

std::string join_vector(const std::vector<std::string> &v)
{
    std::string result;
    for (const std::string &line : v)
    {
        result += line + '\n';
    }
    return result;
}

std::string LZS::rle_compress(const std::string &text)
{
    std::string encoded_text;
    char last_char = '\0';
    int count = 1;

    for (char ch : text)
    {
        if (ch == last_char)
        {
            count++;
        }
        else
        {
            if (count > 1)
            {
                encoded_text += std::to_string(count) + last_char;
            }
            else if (count == 1)
            {
                encoded_text += last_char;
            }
            count = 1;
            last_char = ch;
        }
    }

    if (count > 1)
    {
        encoded_text += std::to_string(count) + last_char;
    }
    else if (count == 1)
    {
        encoded_text += last_char;
    }

    return encoded_text;
}

std::string LZS::rle_decompress(const std::string &text)
{
    std::string decoded_text;
    std::string count_str;
    for (char ch : text)
    {
        if (std::isdigit(ch))
        {
            count_str += ch;
        }
        else
        {
            if (count_str.empty())
            {
                decoded_text += ch;
            }
            else
            {
                int count = std::stoi(count_str);
                decoded_text.append(count, ch);
                count_str.clear();
            }
        }
    }

    if (!count_str.empty())
    {
        std::cerr << "Error de descompresión: cadena no válida." << std::endl;
    }

    return decoded_text;
}

void::LZS::lzw_compress(const std::string &text, std::ofstream &fout)
{
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
            write_code(dictionary[current], fout);
            dictionary[temp] = dictionary.size();
            current = std::string(1, ch);
        }
    }

    if (!current.empty())
    {
        write_code(dictionary[current], fout);
    }
}

std::string LZS::lzw_decompress(std::ifstream &fin){
    std::vector<std::string> dictionary_reverse;
    for (int i = 0; i < 256; ++i)
    {
        std::string ch(1, char(i));
        dictionary_reverse.push_back(ch);
    }

    int code;
    std::string decompressed_text;
    std::string previous, entry;
    if (fin.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        if (code < static_cast<int>(dictionary_reverse.size()))
        {
            entry = dictionary_reverse[code];
            decompressed_text += entry;
            previous = entry;
        }
        else
        {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return decompressed_text;
        }
    }

    while (fin.read(reinterpret_cast<char *>(&code), sizeof(code)))
    {
        if (code < static_cast<int>(dictionary_reverse.size()))
        {
            entry = dictionary_reverse[code];
            decompressed_text += entry;
            dictionary_reverse.push_back(previous + entry[0]);
        }
        else if (code == static_cast<int>(dictionary_reverse.size()))
        {
            entry = previous + previous[0];
            decompressed_text += entry;
            dictionary_reverse.push_back(entry);
        }
        else
        {
            std::cerr << "Error de descompresión: código no válido." << std::endl;
            return decompressed_text;
        }
        previous = entry;
    }

    return decompressed_text;
}

void LZS::compress(std::vector<std::string> &content, const std::string &output_file)
{
    std::ofstream fout(output_file, std::ios::binary);

    if (!fout)
    {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }

    std::string text = join_vector(content);
    std::string rle_text = rle_compress(text);

    lzw_compress(rle_text, fout);

    fout.close();
    std::cout << "Archivo comprimido con éxito!" << std::endl;
}

std::vector<std::string> LZS::decompress(const std::string &input_file)
{
    std::ifstream fin(input_file, std::ios::binary);
    std::vector<std::string> output_buffer;

    if (!fin)
    {
        std::cerr << "Error al abrir el archivo de entrada." << std::endl;
        return output_buffer;
    }

    std::string decompressed_text = lzw_decompress(fin);

    std::string rle_text;
    if (!decompressed_text.empty())
    {
        rle_text = rle_decompress(decompressed_text);
    }

    std::istringstream iss(rle_text);
    std::string line;
    while (std::getline(iss, line))
    {
        output_buffer.push_back(line);
    }

    return output_buffer;
}

void LZS::write_code(int code, std::ofstream &fout)
{
    fout.write(reinterpret_cast<const char *>(&code), sizeof(code));
}
