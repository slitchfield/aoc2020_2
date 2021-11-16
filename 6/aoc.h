#ifndef __AOC_H__
#define __AOC_H__

#include <stdint.h>

struct individual_answers {
    char* line;
    uint32_t bitmask;
};

struct input_elem {
    struct individual_answers* answers;
    uint32_t num_members;
    uint32_t union_bitmask;
    uint32_t inter_bitmask;
};

struct input_arr {
    struct input_elem* arr;
    int num_elems;
};

#define arraylen(arr) ( sizeof(arr) / sizeof( *(arr) ) )

unsigned int count_lines(const char* filename);

char** tokenize_elems(const char* filename, int* num_elems);
struct input_elem parse_elem(char* line);
void free_elem(struct input_elem in);
void print_elem(struct input_elem in);

struct input_arr read_inputfile(const char* filename);
void free_input(struct input_arr in);
void print_input(struct input_arr in);

char* trim_trailing_whitespace(char* str);
void replace_char(char* str, char find, char replace);
#endif