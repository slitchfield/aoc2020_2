#ifndef __AOC_H__
#define __AOC_H__

struct input_data_ints {
    int* arr;
    unsigned int num_elems;
};

struct input_elem {
    int lowerbound;
    int upperbound;
    char key;
    char* password;
};

struct input_arr {
    struct input_elem* arr;
    unsigned int num_elems;
};

#define arraylen(arr) ( sizeof(arr) / sizeof( *(arr) ) )

unsigned int count_lines(const char* filename);

struct input_data_ints read_inputfile_ints(const char* filename);

struct input_elem parse_line(char* line);
void free_elem(struct input_elem in);
void print_elem(struct input_elem in);

struct input_arr read_inputfile(const char* filename);
void free_input(struct input_arr in);

char* trim_trailing_whitespace(char* str);
#endif