#include "Editor/editor.h"
#include <ncurses.h>
#include <filesystem>
#include <iostream>

void run_editor(const char *file_name)
{
    std::cout << "Running editor for file: " << file_name << std::endl;
    std::cout << "File exists: " << std::filesystem::exists(file_name) << std::endl;
    if (std::filesystem::exists(file_name))
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
            // Handle error opening file
            std::cerr << "Error opening file\n";
            return;
        }

        // Initialize Editor with file content
        Editor ed(content);

        int character;
        while (true)
        {
            character = getch();
            ed.process_keypress(character);
        }
    }
    else
    {
        // File does not exist, create it
        const std::vector<std::string> content;
        Editor e(content);

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
        // Provide feedback to the user that a filename argument is required
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1; // Exit with error status
    }
    return 0;
}
