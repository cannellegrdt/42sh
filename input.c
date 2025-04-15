/*
** EPITECH PROJECT, 2025
** B-PSU-200-LYN-2-1-42sh-hugo.arnal
** File description:
** test
*/

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LEN 100

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

static void display_prompt_and_buffer(const char *buffer, int pos)
{
    if (isatty(0)) {
        printf("\r$>%s", buffer);
        for (int i = pos; i < strlen(buffer); i++) {
            printf("\b");
        }
        fflush(stdout);
    }
}

static switch_cse(int ch, char *buffer, int *pos)
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

static void handle_special_key(int *pos, char *buffer)
{
    int ch = getchar();

    if (ch == '[') {
        ch = getchar();
        switch_cse(ch, buffer, pos);
    }
}

static void handle_backspace(char *buffer, int *pos)
{
    if (*pos > 0) {
        memmove(&buffer[*pos - 1], &buffer[*pos], strlen(buffer) - *pos + 1);
        (*pos)--;
    }
}

void handle_printable_char(char *buffer, int *pos, int ch)
{
    memmove(&buffer[*pos + 1], &buffer[*pos], strlen(buffer) - *pos + 1);
    buffer[*pos] = ch;
    (*pos)++;
}

void read_input(char *buffer)
{
    int pos = 0;
    int ch;

    while (1) {
        display_prompt_and_buffer(buffer, pos);
        ch = getchar();
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


char *input_handler(void)
{
    char *input = malloc(sizeof(char)* MAX_LEN);
    struct termios orig_termios;

    set_raw_mode(&orig_termios);
    read_input(input);
    reset_mode(&orig_termios);
    if (isatty(0))
        printf("\n");
    if ((input)[strlen(input) - 1] == '\n')
        (input)[strlen(input) - 1] = '\0';
    return input;
}
