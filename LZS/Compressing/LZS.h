#ifndef LZS_H
#define LZS_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iomanip>

class LZS {
private:
    std::unordered_map<std::string, int> dictionary;

public:
    LZS();
    void compress(std::vector<std::string> &buffer, const std::string &output_file);
    std::vector<std::string> decompress(const std::string &input_file);
    std::string rle_compress(const std::string &text);
    std::string rle_decompress(const std::string &text);
    void lzw_compress(const std::string &text, std::ofstream &fout);
    std::string lzw_decompress(std::ifstream &fin);

private:
    void initialize_dictionary();
    void write_code(int code, std::ofstream &fout);
};

#endif // LZS_H
