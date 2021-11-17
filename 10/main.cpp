
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <set>

#include "aoc.hpp"

#define TEST_INPUT_1_FILENAME "./test1.txt"
#define TEST_INPUT_2_FILENAME "./test2.txt"
#define INPUT_FILENAME "./input.txt"

int part_one(struct input_arr);
uint64_t part_two(struct input_arr);

void run_test(const char* filename) {
    clock_t test_begin = clock();
    printf("==============================\n");
    printf("Running test file \"%s\"\n", filename);
    
    clock_t begin = clock();
    struct input_arr input = read_inputfile(filename);
    clock_t end = clock();
    print_input(input);
    double input_parse_time = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\tParsed input in  %.6fs\n", input_parse_time);

    begin = clock();
    unsigned int part_one_answer = part_one(input);
    end = clock();
    double part_one_time = (double)(end - begin) / CLOCKS_PER_SEC;

    begin = clock();
    uint64_t part_two_answer = part_two(input);
    end = clock();
    double part_two_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\tPart One Answer (%.6fs):\t%d\n", part_one_time, part_one_answer);
    printf("\tPart Two Answer (%.6fs):\t%lu\n", part_two_time, part_two_answer);

    free_input(input);

    clock_t test_end = clock();
    double test_time = (double)(test_end - test_begin) / CLOCKS_PER_SEC;
    printf("Ran entire test in %.6fs\n", test_time);
    printf("==============================\n");
}

int main(int argc, char** argv) {
    run_test(TEST_INPUT_1_FILENAME);
    run_test(TEST_INPUT_2_FILENAME);
    run_test(INPUT_FILENAME);
    exit(EXIT_SUCCESS);
}

int part_one(struct input_arr input) {
    uint64_t num_ones = 0;
    uint64_t num_threes = 0;
    for(auto diff: input.diffs) {
        if(diff == 1) num_ones++;
        else if (diff == 3) num_threes++;
        else printf("Found unrecognized diff! %d\n", diff);
    }

    return num_ones * num_threes;
}

uint64_t part_two(struct input_arr input) {
    std::vector<uint64_t> possibilities(input.vec.size(), 0);
    possibilities[possibilities.size() - 1] = 1;

    // Start calculating backwards...
    for(int i = input.vec.size() - 2; i >= 0; i--) {

        // Number of possibilities for this number is the sum of possibilities
        //   from every number reachable from this number

        // For everything going forward...
        for(int j = i+1; j < input.vec.size(); j++) {

            // If we can reach this number, accumulate possibilities
            if(input.vec[j].num - input.vec[i].num <= 3) {
                possibilities[i] += possibilities[j];
            } else {
                // Otherwise we can't reach anything past this either
                break;
            }
        }
    }

    // Answer is the number of possibilities from '0'
    return possibilities[0];
}