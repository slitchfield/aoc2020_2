
#include "aoc.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int count_lines(const char* filename) {

    FILE* fp;
    char* line = NULL;
    size_t len = 0;

    fp = fopen(filename, "r");
    if ( fp == NULL )
        return 0;

    unsigned int lines = 0;
    while ( getline(&line, &len, fp) != -1 ) {
        lines++;
    }

    if (line)
        free(line);

    fclose(fp);

    return lines;    
}

struct input_elem parse_line(char* line) {
    struct input_elem ret;

    char dash[2] = "-"; 
    char space[2] = " ";
    char col[3] = ": ";
    char* token = strtok(line, dash);

    ret.lowerbound = atoi(token);
    token = strtok(NULL, space);
    ret.upperbound = atoi(token);
    token = strtok(NULL, col);
    ret.key = *token;
    token = strtok(NULL, space);
    ret.password = calloc(strlen(token), sizeof(char));
    strncpy(ret.password, token, strlen(token));
    ret.password = trim_trailing_whitespace(ret.password);

    return ret;
}

void print_elem(struct input_elem in) {
    printf("%d-%d %c: %s\n", in.lowerbound, in.upperbound, in.key, in.password);
}

struct input_arr read_inputfile(const char* filename) {
    struct input_arr ret; 
    FILE* fp;
    char* line = NULL;
    size_t len = 0;

    ssize_t read;
    int idx = 0;

    ret.num_cols = 0;
    ret.num_rows = count_lines(filename);
    ret.array2 = (char**) calloc(ret.num_rows, sizeof(char*));
    if (ret.array2 == NULL) // TODO: have better error reporting
        exit(EXIT_FAILURE);

    fp = fopen(filename, "r");
    if ( fp == NULL )
        exit(EXIT_FAILURE);

    while ( (read = getline(&line, &len, fp)) != -1) {
        size_t linelen = strlen(line);
        ret.array2[idx] = (char*) calloc(linelen, sizeof(char));
        strncpy(ret.array2[idx], line, linelen);
        trim_trailing_whitespace(ret.array2[idx]);
        linelen = strlen(ret.array2[idx]);
        if (ret.num_cols == 0) ret.num_cols = linelen;
        else if (ret.num_cols != linelen) {
            printf("Got lines of differing size?\n");
        }
        idx++;
    }

    fclose(fp);
    if (line) free(line);

    return ret;
}

struct input_data_ints read_inputfile_ints(const char* filename) {
    struct input_data_ints ret;
    FILE* fp;
    char* line = NULL;
    size_t len = 0;

    ssize_t read;
    int idx = 0;

    ret.num_elems = count_lines(filename);
    ret.arr = (unsigned int*)calloc(ret.num_elems, sizeof(int));
    if (ret.arr == NULL) // TODO: have better error reporting
        exit(EXIT_FAILURE);

    fp = fopen(filename, "r");
    if ( fp == NULL )
        exit(EXIT_FAILURE);

    while ( (read = getline(&line, &len, fp)) != -1) {
        ret.arr[idx++] = atoi(line);
    }

    fclose(fp);
    if (line) free(line);

    return ret;
}

char* trim_trailing_whitespace(char* str) {

    char* end = str + strlen(str) - 1;
    while (end > str && isspace( (unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

void free_elem(struct input_elem in) {
    free(in.password);
}

void free_input(struct input_arr in) {
    for(int i = 0; i < in.num_rows; i++) {
        free(in.array2[i]);
    }
    free(in.array2);
}

void print_input(struct input_arr in) {
    for(int i = 0; i < in.num_rows; i++) {
        for(int j = 0; j < in.num_cols; j++) {
            printf("%c", in.array2[i][j]);
        }
        printf("\n");
    }
}