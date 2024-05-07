#include "Editor/editor.h"
#include "Compressing/LZS.h"
#include <ncurses.h>
#include <filesystem>
#include <iostream>

void run_editor(const char *file_name)
{
    if (std::filesystem::exists(file_name))
    {
        if (std::filesystem::path(file_name).extension() == ".bin")
        {
            LZS lzs;
            std::vector<std::string> content = lzs.descomprimir(file_name);

            Editor e(file_name, content);

            int character;
            while (true)
            {
                character = getch();
                e.process_keypress(character);
            }
        }
        else
        {
            FILE *file;
            file = fopen(file_name, "r+");
            std::vector<std::string> content;
            Editor e;

            if (file != nullptr)
            {
                content = e.create_file_contents(file);
                fclose(file);
            }
            else
            {
                std::cerr << "Error opening file\n";
                return;
            }

            Editor ed(file_name, content);

            int character;
            while (true)
            {
                character = getch();
                ed.process_keypress(character);
            }
        }
    }
    else
    {
        Editor e(file_name);

        int character;

        while (true)
        {
            character = getch();
            e.process_keypress(character);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        run_editor(argv[1]);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
    }
    return 0;
}
