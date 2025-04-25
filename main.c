/*
** EPITECH PROJECT, 2024
** B-PSU-200-LYN-2-1-minishell1-cannelle.gourdet
** File description:
** main.c
*/

#include "mysh.h"

/**
 * @brief Processes special commands like help and exit.
 *
 * @param line : The command line to check.
 * @param last_status : The status of the last executed command.
 * @return : 1 if a special command was processed, 0 otherwise.
 */
static int process_special_commands(char *line, int last_status)
{
    if (strcmp(line, "-h") == 0 || strcmp(line, "--help") == 0) {
        print_help();
        return 1;
    }
    if (strcmp(line, EXIT) == 0) {
        free(line);
        exit(last_status);
    }
    return 0;
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

/**
 * @brief Reads a command line from standard input.
 *
 * @param line : Pointer to the line buffer.
 * @param len : Pointer to the buffer size.
 * @param read_size : Pointer to store the number of bytes read.
 * @return : The line read, or NULL on EOF.
 */
static char *read_command_line(char **line, size_t *len, ssize_t *read_size)
{
    *read_size = getline(line, len, stdin);
    if (*read_size == -1)
        return NULL;
    if (*read_size > 0 && (*line)[*read_size - 1] == '\n')
        (*line)[*read_size - 1] = '\0';
    return *line;
}

int main(void)
{
    char *line = NULL;
    int last_status = 0;
    char quote_type = 0;

    setup_environment();
    setup_signal_handlers();
    setup_bindkeys();
    setup_config_files();
    while (1) {
        display_prompt();
        if (!read_command_line(&line, &len, &read))
            handle_eof(line, last_status);
        if (process_special_commands(line, last_status))
            continue;
        last_status = main_execute_command(line);
    }
    return 0;
}
