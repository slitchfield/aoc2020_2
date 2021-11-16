#ifndef __AOC_H__
#define __AOC_H__

#include <stdint.h>

#define BYR 0b10000000
#define IYR 0b01000000
#define EYR 0b00100000
#define HGT 0b00010000
#define HCL 0b00001000
#define ECL 0b00000100
#define PID 0b00000010
#define CID 0b00000001

struct input_elem {
    int byr; // Pure int
    int iyr; // Pure int
    int eyr; // Pure int

    int hgt; // [int]{cm|in}
    char hgt_unit;

    char hcl[7]; // hex int
    char ecl[4]; // 3 chars

    long long int pid; // Pure int
    char pid_valid;
    long cid; // Pure int
    uint16_t presence;
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