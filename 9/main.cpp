
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <set>

#include "aoc.hpp"

#define INPUT_FILENAME "./input.txt"

int part_one(struct input_arr);
uint64_t part_two(struct input_arr);

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
    uint64_t part_two_answer = part_two(input);
    end = clock();
    double part_two_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Part One Answer (%.6fs):\t%d\n", part_one_time, part_one_answer);
    printf("Part Two Answer (%.6fs):\t%lu\n", part_two_time, part_two_answer);

    free_input(input);

    exit(EXIT_SUCCESS);
}

int part_one(struct input_arr input) {
    uint64_t retval = 0;
    for(int k = 25; k < input.vec.size(); k++) {
        bool found_valid = false;
        for(int i = k - 25; i < k - 1; i++) {
            for(int j = k - 24; j < k; j++) {
                if( (input.vec[i].num + input.vec[j].num) == input.vec[k].num) {
                    found_valid = true;
                }
            }
        }
        if(found_valid == false) {
            retval = input.vec[k].num;
            break;
        }
    }
    return retval;
}

uint64_t sum_set(struct input_arr in, uint32_t begin, uint32_t end) {
    uint64_t retval = 0;
    for(int i = begin; i < end; i++) {
        retval += in.vec[i].num;
    }
    return retval;
}

uint64_t part_two(struct input_arr input) {
    // First we need to refind the target and it's index
    uint64_t target = 0;
    uint32_t index = 0;
    for(int k = 25; k < input.vec.size(); k++) {
        bool found_valid = false;
        for(int i = k - 25; i < k - 1; i++) {
            for(int j = k - 24; j < k; j++) {
                if( (input.vec[i].num + input.vec[j].num) == input.vec[k].num) {
                    found_valid = true;
                }
            }
        }
        if(found_valid == false) {
            target = input.vec[k].num;
            index = k;
            break;
        }
    }

    int low_idx = 0;
    int high_idx = 0;
    bool found_valid = false;
    for(int i = 0; i < index; i++) {
        for(int j = i+1; j < index; j++) {
            uint64_t rangesum = sum_set(input, i, j);
            if (rangesum == target) {
                printf("Found a set! (%d, %d)\n", i, j);
                low_idx = i;
                high_idx = j;
                found_valid = true;
                break;
            }
            else if (rangesum > target) break;
        }
        if(found_valid) break;
    }

    uint64_t min = INT64_MAX;
    uint64_t max = 0;
    for(int i = low_idx; i <= high_idx; i++) {
        if(input.vec[i].num < min) min = input.vec[i].num;
        if(input.vec[i].num > max) max = input.vec[i].num;
    }

    return min + max;
}