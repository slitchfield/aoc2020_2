
#include "aoc.h"

#include <stdio.h>
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