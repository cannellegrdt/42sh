/*
** EPITECH PROJECT, 2025
** B-PSU-200-LYN-2-1-42sh-hugo.arnal
** File description:
** backticks
*/

#include "mysh.h"

int return_backticks_str(char *line)
{
    int start = 0;
    int len = 0;

    for (int a = 0; a < strlen(line); a++) {
        if (line[a] == '`') {
            start++;
            continue;
        }
        if (start == 1)
            len++;
        if (start == 2)
            return len;
    }
    if (start == 1)
        return 0;
    return len;
}

char *create_command_line(char *line, int len)
{
    int start = 0;
    int b = 0;
    char *command_line;

    if (len == 0)
        return "yes";
    command_line = malloc(sizeof(char) * (len + 1));
    if (command_line == NULL)
        return NULL;

    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '`') {
            start++;
            continue;
        }
        if (start == 2) {
            command_line[b] = '\0';
            return command_line;
        }
        if (start == 1) {
            command_line[b] = line[i];
            b++;
        }
    }
    command_line[b] = '\0';
    return command_line;
}

char *read_file_to_string(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    char *buffer;
    long filesize = 0;

    if (!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    buffer = (char*)malloc(filesize + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, filesize, file);
    buffer[filesize] = '\0';
    fclose(file);
    return buffer;
}

int return_newsize(int line_len, int bet_len, char *line)
{
    int new_size = 0;

    for (int i = 0; i < line_len; i++) {
        if (line[i] == '`') {
            i++;
            while (i < line_len && line[i] != '`')
                i++;
            if (i < line_len)
                new_size += bet_len;
            continue;
        }
        if (line[i] == '\n') {
            new_size++; 
            continue;
        }
        new_size++;
    }
    return new_size;
}

void boucle(int bet_len, const char *between_backticks, char *replaced, int *j) {
    for (int k = 0; k < bet_len; k++) {
        if (between_backticks[k] == '\n') {
            replaced[(*j)++] = ' ';
        } else {
            replaced[(*j)++] = between_backticks[k];
        }
    }
}

int lopp(int i, int line_len, char *line)
{
    i++;
    while (i < line_len && line[i] != '`')
        i++;
    return i;
}


char *replace_backticks_zone(const char *line, const char *between_backticks)
{
    int j = 0;
    int line_len = strlen(line);
    int bet_len = strlen(between_backticks);
    int new_size = return_newsize(line_len, bet_len, line);
    char *replaced = malloc(new_size + 1);
    int i = 0;

    if (!replaced)
        return NULL;
    while (i < line_len && j < new_size) {
        if (line[i] == '`') {
            i = lopp(i, line_len, line);
            if (i < line_len) {
                i++;
                boucle(bet_len, between_backticks, replaced, &j);
                j--;
            }
            continue;
        }
        if (line[i] == '\n') {
            replaced[j++] = ' ';
            i++;
            continue;
        }
        replaced[j++] = line[i];
        i++;
    }
    replaced[j] = '\0';
    return replaced;
}

int backticks_numbers(char *line)
{
    int returned = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '`')
            returned++;
    }
    return returned;
}

char *backtiscks(char *buffer)
{
    char *replaced;
    int a = return_backticks_str(buffer);
    char *str = create_command_line(buffer, a);

    if (buffer == NULL) return NULL;
    if (str == NULL)
        return buffer;
    if (strcmp(str, "yes") == 0 && (backticks_numbers(buffer) != 2 &&
    backticks_numbers(buffer) != 0)) {
        printf("Unmatched '`'.\n");
        return "exit1";
    }
    if (strcmp(str, "yes") == 0)
        str = strdup("echo ");
    char *str1 = strdup(str);
    char *s2 = strdup(" > ali.txt");
    strcat(str, s2);
    main_execute_command(str);
    return replace_backticks_zone(buffer, read_file_to_string("ali.txt"));
}