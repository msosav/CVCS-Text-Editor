CPP_FLAGS = -std=c++17 -Wall -Werror -Wpedantic -Weffc++

FILES = Screen/screen.cpp Editor/editor.cpp main.cpp Compressing/LZW.cpp

cvcs: ${FILES}
	g++ -O3 ${CPP_FLAGS} ${FILES} -o cvcs -lncurses

debug: ${FILES}
	g++ -ggdb ${CPP_FLAGS} ${FILES} -o cvcs -lncurses
