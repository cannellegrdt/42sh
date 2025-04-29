/*
** EPITECH PROJECT, 2025
** B-PSU-200-LYN-2-1-42sh-hugo.arnal
** File description:
** Functions for parsing and replacing backticks in a command line.
*/

#include "mysh.h"

static int get_backticks_length(char *line)
{
    int start = 0;
    int len = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '`') {
            start++;
            continue;
        }
        if (start == 1)
            len++;
        if (start == 2)
            return len;
    }
    return (start == 1) ? 0 : len;
}

static char *extract_command(char *line, int len, int j)
{
    int start = 0;
    char *command_line;

    if (len == 0)
        return strdup("echo");
    command_line = malloc(sizeof(char) * (len + 1));
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '`') {
            start++;
            continue;
        }
        if (start == 2)
            break;
        if (start == 1) {
            command_line[j] = line[i];
            j++;
        }
    }
    command_line[j] = '\0';
    return command_line;
}

static char *read_file_content(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    char *buffer;
    long filesize;

    if (file == NULL)
        return NULL;
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = malloc(filesize + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';
    fclose(file);
    return buffer;
}

static int count_between_backticks(int line_len, char *line, int i)
{
    i++;
    while (i < line_len && line[i] != '`')
        i++;
    return i;
}

static int adjust_new_size(int i, int line_len, int new_size, int cmd_len)
{
    if (i < line_len)
        new_size += cmd_len;
    return new_size;
}

static int calculate_new_size(int line_len, int cmd_len, const char *line)
{
    int new_size = 0;

    for (int i = 0; i < line_len; i++) {
        if (line[i] == '`') {
            i = count_between_backticks(line_len, (char *)line, i);
            new_size = adjust_new_size(i, line_len, new_size, cmd_len);
            continue;
        }
        new_size++;
    }
    return new_size;
}

static void copy_command(const char *cmd, char *replaced, int *j)
{
    for (int k = 0; cmd[k] != '\0'; k++) {
        if (cmd[k] == '\n') {
            replaced[*j] = ' ';
            (*j)++;
            continue;
        }
        replaced[*j] = cmd[k];
        (*j)++;
    }
}

void cond1(int *i, int line_len)
{
    if (*i < line_len) {
        *(i)++;
    } 
}

static char *replace_backticks(const char *line, const char *cmd)
{
    int line_len = strlen(line);
    int cmd_len = strlen(cmd);
    int new_size = calculate_new_size(line_len, cmd_len, line);
    char *replaced = malloc(new_size + 1);
    int i = 0;
    int j = 0;

    while (i < line_len) {
        if (line[i] == '`') {
            i = count_between_backticks(line_len, (char *)line, i);
            if (i < line_len) {
                i++;
                copy_command(cmd, replaced, &j);
            }
            continue;
        }
        if (line[i] == '\n') {
            replaced[j] = ' ';
            j++;
            i++;
            continue;
        }
        replaced[j] = line[i];
        j++;
        i++;
    }
    replaced[j] = '\0';
    return replaced;
}

static int count_backticks(const char *line)
{
    int count = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '`')
            count++;
    }
    return count;
}

static char *check_unmatched_backticks(char *buffer, char *cmd)
{
    if (strcmp(cmd, "echo") == 0 && (count_backticks(buffer) != 2 && count_backticks(buffer) != 0)) {
        printf("Unmatched '`'.\n");
        free(cmd);
        return strdup("exit1");
    }
    return NULL;
}

static char *create_redirect_command(char *cmd)
{
    char *cmd_with_redirect;
    int size_cmd = strlen(cmd);
    int size_redirect = strlen(" > ali.txt");

    cmd_with_redirect = malloc(size_cmd + size_redirect + 1);
    if (cmd_with_redirect == NULL)
        return NULL;
    strcpy(cmd_with_redirect, cmd);
    strcat(cmd_with_redirect, " > ali.txt");
    return cmd_with_redirect;
}

char *backtiscks(char *buffer)
{
    int len;
    char *cmd;
    char *cmd_with_redirect;
    char *file_content;
    char *result;

    len = get_backticks_length(buffer);
    cmd = extract_command(buffer, len, 0);
    if (cmd == NULL)
        return strdup(buffer);
    result = check_unmatched_backticks(buffer, cmd);
    if (result != NULL)
        return result;
    cmd_with_redirect = create_redirect_command(cmd);
    if (cmd_with_redirect == NULL)
        return NULL;
    main_execute_command(cmd_with_redirect);
    file_content = read_file_content("ali.txt");
    result = replace_backticks(buffer, file_content);
    return result;
}
