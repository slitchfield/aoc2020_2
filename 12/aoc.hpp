#ifndef __AOC_H__
#define __AOC_H__
#include <stdint.h>

#include <string>
#include <vector>
#include <map>

using std::string;

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    LEFT,
    RIGHT,
    FWD,
    UNDEF
} instr_t;

// In this case, an element represents a 'rule'
struct input_elem {
    instr_t instr;
    int value;
};

struct boat_state {
    int heading;
    std::pair<int, int> waypoint_delta;
    int x;
    int y;
};

struct input_arr {
    std::vector<struct input_elem> vec;
    struct boat_state bstate;
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