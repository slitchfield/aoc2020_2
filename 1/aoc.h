#ifndef __AOC_H__
#define __AOC_H__

struct input_data_ints {
    int* arr;
    unsigned int num_elems;
};

#define arraylen(arr) ( sizeof(arr) / sizeof( *(arr) ) )

unsigned int count_lines(const char* filename);
struct input_data_ints read_inputfile_ints(const char* filename);

#endif