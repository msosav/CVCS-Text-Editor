#include "Editor/editor.h"
#include <filesystem>
#include <iostream>

void run_editor(const char *file_name)
{
	std::cout << "Running editor for file: " << file_name << std::endl;
	std::cout << "File exists: " << std::filesystem::exists(file_name) << std::endl;
	if (std::filesystem::exists(file_name))
	{
		// File exists, open it in read/write mode
		Editor e(file_name, "r+");

		int character;
		while (true)
		{
			character = getch();
			e.process_keypress(character);
		}
	}
	else
	{
		// File does not exist, create it
		Editor e(file_name, "w+");

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
	if (argc > 1) // Changed from argc > 0 to argc > 1 to ensure at least one argument is provided (the file name)
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
