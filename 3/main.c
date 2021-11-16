
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aoc.h"

#define INPUT_FILENAME "./input.txt"

unsigned int count_key_occurrences(char c, const char* str);

unsigned int part_one(struct input_arr);
unsigned long long int part_two(struct input_arr);

int main(int argc, char** argv) {

    struct input_arr input = read_inputfile(INPUT_FILENAME);
    print_input(input);

    unsigned int part_one_answer = part_one(input);
    unsigned long long int part_two_answer = part_two(input);

    printf("Part One Answer:\t%d\n", part_one_answer);
    printf("Part Two Answer:\t%llu\n", part_two_answer);

    free_input(input);

    exit(EXIT_SUCCESS);
}

unsigned int count_trees(struct input_arr input, unsigned int drow, unsigned int dcol) {
    unsigned int cur_row = 0;
    unsigned int cur_col = 0;
    unsigned int num_trees = 0;

    while(cur_row < input.num_rows) {
        if(input.array2[cur_row][cur_col] == '#')
            num_trees++;

        cur_row = cur_row + drow;
        cur_col = (cur_col + dcol) % input.num_cols;
    }
    return num_trees;
}

unsigned int part_one(struct input_arr input) {
    return count_trees(input, 1, 3);
}

unsigned long long int part_two(struct input_arr input) {
    unsigned long long int num_trees = count_trees(input, 1, 1);
    num_trees *= count_trees(input, 1, 3);
    num_trees *= count_trees(input, 1, 5);
    num_trees *= count_trees(input, 1, 7);
    num_trees *= count_trees(input, 2, 1);
    return num_trees;
}