void LZW::comprimir(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream fin(inputFile, std::ios::binary);
    std::ofstream fout(outputFile, std::ios::binary);

    if (!fin.is_open() || !fout.is_open())
    {
        std::cerr << "Error al abrir los archivos." << std::endl;
        return;
    }

    std::string current;
    char ch;
    int i = 0;
    while (fin.get(ch))
    {

        //---------------
        std::string temp = current + ch;
        std::cout << "Caracter Archivo: (" << i << ") " << temp << std::endl;
        ++i;

        if (dictionary.find(temp) != dictionary.end())
        {
            current = temp;
        }
        else
        {
            escribirCodigo(dictionary[current], fout);
            if (dictionary.size() < static_cast<size_t>(MAX_DICT_SIZE))
            {
                dictionary[temp] = static_cast<int>(dictionary.size());
            }
            current = std::string(1, ch);
        }
    }

    if (!current.empty())
    {
        escribirCodigo(dictionary[current], fout);
    }

    fin.close();
    fout.close();
    std::cout << "Archivo comprimido con éxito!" << std::endl;
}
