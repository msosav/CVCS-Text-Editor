#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <string>
#include <cctype>

#include "../Screen/screen.h"
#include "../Cursor/cursor.h"
#include "../Compressing/LZS.h"

enum class Mode
{
    NORMAL = 0,
    INSERT,
    REPLACE
};

static const int ESCAPE_KEY = '\x1B';
static const int BACKSPACE_KEY = '\x7F';
static const int SPACES_FOR_TAB = 8;
static const int ENTER_KEY = '\xA';

class Editor
{
    Screen screen;
    LZS lsz;
    Mode current_mode{Mode::NORMAL};
    std::vector<std::string> file_buffer;
    Cursor cursor{};
    std::size_t file_buffer_index = 0;
    std::size_t top_of_screen_index = 0;

    void move_cursor_down() noexcept;
    void move_cursor_up() noexcept;
    void move_cursor_left() noexcept;
    void move_cursor_right() noexcept;
    void move_cursor_x_considering_lines(
        const std::string &line_considered,
        const std::string &current_line) noexcept;

    void normal_mode_action(int character) noexcept;
    void insert_mode_action(int character) noexcept;
    void replace_mode_action(int character) noexcept;

    void do_w_motion() noexcept;
    void do_b_motion() noexcept;

    void insert_char(int character) noexcept;
    void replace_char(int character) noexcept;
    void delete_char() noexcept;
    void add_new_line() noexcept;

public:
    Editor(const char *file_name = "", const std::vector<std::string> &buffer = {}, const char *output_file_name = "prueba") noexcept;
    Editor(const Editor &e) = delete;
    Editor &operator=(const Editor &e) = delete;

    void process_keypress(int character) noexcept;
    std::vector<std::string> create_file_buffer(FILE *file) const noexcept;
    std::vector<std::string> get_file_buffer() const noexcept;
};

#endif
