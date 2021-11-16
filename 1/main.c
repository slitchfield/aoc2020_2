
#include <stdio.h>
#include <stdlib.h>

#include "aoc.h"

#define INPUT_FILENAME "./input.txt"


unsigned int part_one(struct input_data_ints);
unsigned int part_two(struct input_data_ints);

int main(int argc, char** argv) {

    struct input_data_ints input = read_inputfile_ints(INPUT_FILENAME);

    unsigned int part_one_answer = part_one(input);
    unsigned int part_two_answer = part_two(input);

    printf("Part One Answer:\t%d\n", part_one_answer);
    printf("Part Two Answer:\t%d\n", part_two_answer);

    if (input.arr)
        free(input.arr);

    exit(EXIT_SUCCESS);
}

unsigned int part_one(struct input_data_ints input) {
    for(int i = 0; i < input.num_elems - 1; i++ ) {
        for( int j = i + 1; j < input.num_elems; j++ ) {
            if ( input.arr[i] + input.arr[j] == 2020 ) {
                return input.arr[i] * input.arr[j];
            }
        }
    }
    return 0;
}

unsigned int part_two(struct input_data_ints input) {
    for(int i = 0; i < input.num_elems - 2; i++ ) {
        for(int j = i+1; j < input.num_elems - 1; j++ ) {
            for(int k = j+1; k < input.num_elems; k++) {
                if ( input.arr[i] + input.arr[j] + input.arr[k] == 2020 ) {
                    return input.arr[i] * input.arr[j] * input.arr[k];
                }
            }
        }
    }
}