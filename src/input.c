/*
** EPITECH PROJECT, 2025
** B-PSU-200-LYN-2-1-42sh-hugo.arnal
** File description:
** test
*/

#include "mysh.h"

void set_raw_mode(struct termios *orig_termios)
{
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, orig_termios);
    new_termios = *orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

static void reset_mode(struct termios *orig_termios)
{
    tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

static void display_prompt_and_buffer(const char *buffer, size_t pos)
{
    if (isatty(0)) {
        printf("\r$>%s", buffer);
        for (size_t i = pos; i < strlen(buffer); i++) {
            printf("\b");
        }
        fflush(stdout);
    }
}

static void switch_cse(int ch, char *buffer, size_t *pos)
{
    switch (ch) {
        case 'D':
            if (*pos > 0)
                (*pos)--;
            break;
        case 'C':
            if (*pos < strlen(buffer))
                (*pos)++;
            break;
    }
}

static void handle_special_key(size_t *pos, char *buffer)
{
    int ch = getchar();

    if (ch == '[') {
        ch = getchar();
        switch_cse(ch, buffer, pos);
    }
}

static void handle_backspace(char *buffer, size_t *pos)
{
    if (*pos > 0) {
        memmove(&buffer[*pos - 1], &buffer[*pos], strlen(buffer) - *pos + 1);
        (*pos)--;
    }
}

void handle_printable_char(char *buffer, size_t *pos, int ch)
{
    memmove(&buffer[*pos + 1], &buffer[*pos], strlen(buffer) - *pos + 1);
    buffer[*pos] = ch;
    (*pos)++;
}

/**
 * @brief Handles EOF (Ctrl+D) condition by exiting the shell.
 *
 * @param line : The line buffer to free (if allocated).
 * @param last_status : The status of the last executed command.
 */
static void handle_eof(char *line, int last_status)
{
    if (isatty(STDIN_FILENO))
        write(1, "exit\n", 5);
    if (line)
        free(line);
    exit(last_status);
}

void read_input(char *buffer, int exit_status)
{
    size_t pos = 0;
    int ch;

    while (1) {
        display_prompt_and_buffer(buffer, pos);
        ch = getchar();
        if (ch == 4 || ch == EOF)
            handle_eof(buffer, exit_status);
        if (ch == 27)
            handle_special_key(&pos, buffer);
        if (ch == '\n')
            break;
        if (ch == 127 || ch == 8)
            handle_backspace(buffer, &pos);
        if (pos < MAX_LEN - 1 && isprint(ch))
            handle_printable_char(buffer, &pos, ch);
    }
}

char *input_handler(int exit_status)
{
    char *input = malloc(sizeof(char)* MAX_LEN);
    struct termios orig_termios;

    set_raw_mode(&orig_termios);
    read_input(input, exit_status);
    reset_mode(&orig_termios);
    if (isatty(0))
        printf("\n");
    if ((input)[strlen(input) - 1] == '\n')
        (input)[strlen(input) - 1] = '\0';
    return input;
}
