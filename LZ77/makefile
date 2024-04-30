CPP_FLAGS = -std=c++14 -Wall -Werror -Wpedantic -Weffc++

FILES = Screen/screen.cpp Editor/editor.cpp main.cpp

cvcs: ${FILES}
	g++ -O3 ${CPP_FLAGS} ${FILES} -o cvcs -lncurses

debug: ${FILES}
	g++ -ggdb ${CPP_FLAGS} ${FILES} -o cvcs -lncurses
