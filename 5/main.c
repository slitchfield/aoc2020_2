
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

int part_one(struct input_arr input) {
    int max_seat_id = 0;
    for(int i = 0; i < input.num_elems; i++) {
        if (input.arr[i].seat_id > max_seat_id) max_seat_id = input.arr[i].seat_id;
    }
    return max_seat_id;
}

int comparison(const void* astar, const void* bstar) {
    struct input_elem* a = ((struct input_elem*)astar);
    struct input_elem* b = ((struct input_elem*)bstar);
    if(a->seat_id > b->seat_id) return 1;
    if(b->seat_id > a->seat_id) return -1;
    return 0;
}

int part_two(struct input_arr input) {
    unsigned int retval = 0; 
    
    // Sort the array
    qsort(input.arr, input.num_elems, sizeof(struct input_elem), comparison);
    
    // "Missing" seat is now wherever input.arr[i+1].seat_id - input.arr[i].seat_id == 2 
    for(int i = 0; i < input.num_elems - 1; i++) {
        if(input.arr[i+1].seat_id - input.arr[i].seat_id == 2) {
            retval = input.arr[i].seat_id + 1;
            break;
        }
    }
    return retval;
}