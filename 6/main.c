
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "aoc.h"

#define INPUT_FILENAME "./input.txt"

int part_one(struct input_arr);
int part_two(struct input_arr);

int main(int argc, char** argv) {


    clock_t begin = clock();
    struct input_arr input = read_inputfile(INPUT_FILENAME);
    clock_t end = clock();
    print_input(input);
    double input_parse_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Parsed input in  %.6fs\n", input_parse_time);

    begin = clock();
    unsigned int part_one_answer = part_one(input);
    end = clock();
    double part_one_time = (double)(end - begin) / CLOCKS_PER_SEC;

    begin = clock();
    unsigned int part_two_answer = part_two(input);
    end = clock();
    double part_two_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Part One Answer (%.6fs):\t%d\n", part_one_time, part_one_answer);
    printf("Part Two Answer (%.6fs):\t%llu\n", part_two_time, part_two_answer);

    free_input(input);

    exit(EXIT_SUCCESS);
}

unsigned int count_set_bits(uint32_t input) {
    unsigned int ret = 0;

    for(int i = 0; i < 32; i++) {
        if(input & 0x1) ret++;
        input = input >> 1;
    }

    return ret;
}

int part_one(struct input_arr input) {
    int answer_count = 0;
    for(int i = 0; i < input.num_elems; i++) {
        answer_count += count_set_bits(input.arr[i].union_bitmask);
    }
    return answer_count;
}

int part_two(struct input_arr input) {
    int answer_count = 0;
    for(int i = 0; i < input.num_elems; i++) {
        answer_count += count_set_bits(input.arr[i].inter_bitmask);
    }
    return answer_count;
}