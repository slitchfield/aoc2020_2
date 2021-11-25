
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

uint64_t part_one(struct input_arr);
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
    uint64_t part_one_answer = part_one(input);
    end = clock();
    double part_one_time = (double)(end - begin) / CLOCKS_PER_SEC;

    begin = clock();
    uint64_t part_two_answer = part_two(input);
    end = clock();
    double part_two_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\tPart One Answer (%.6fs):\t%lu\n", part_one_time, part_one_answer);
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

uint64_t part_one(struct input_arr input) {

    // Keep a history of when numbers were spoken
    std::map<int, std::vector<int>> history;
    // First, ingest whatever turns we start with
    int turns = 1;
    int last_number = 0;
    int next_number = 0;
    for(auto cur: input.vec) {
        history[cur.num] = {};
        history[cur.num].push_back(turns);
        //printf("Saying %d on turn %d\n", cur.num, turns);
        last_number = cur.num;
        turns += 1;
    }

    for(; turns <= 2020; turns++) {
        // Consider last_number. Either:
        //   never spoken before, and this number is 0
        //     implies that history entry only has one value
        //   spoken before, and this number is diff in last two times
        //     implies that history entry has more than one value
        if(history[last_number].size() == 1) {
            next_number = 0;
        } else {
            int last_time = history[last_number].rbegin()[0];
            int prev_time = history[last_number].rbegin()[1];
            next_number = last_time - prev_time;
        }
        //printf("Saying %d on turn %d\n", next_number, turns);
        if(history.count(next_number) == 0) {
            history[next_number] = {};
        }
        history[next_number].push_back(turns);
        last_number = next_number;
    }

    return last_number;
}

uint64_t part_two(struct input_arr input) {

    // Keep a history of when numbers were spoken
    // NOTE: vector is fine for now, but in prod code we could use a deque to
    //   minimize reallocations as vectors grow
    std::map<long int, std::vector<long int>> history;
    // First, ingest whatever turns we start with
    long int turns = 1;
    long int last_number = 0;
    long int next_number = 0;
    for(auto cur: input.vec) {
        history[cur.num] = {};
        history[cur.num].push_back(turns);
        //printf("Saying %ld on turn %ld\n", cur.num, turns);
        last_number = cur.num;
        turns += 1;
    }

    for(; turns <= 30000000; turns++) {
        // Consider last_number. Either:
        //   never spoken before, and this number is 0
        //     implies that history entry only has one value
        //   spoken before, and this number is diff in last two times
        //     implies that history entry has more than one value
        if(history[last_number].size() == 1) {
            next_number = 0;
        } else {
            long int last_time = history[last_number].rbegin()[0];
            long int prev_time = history[last_number].rbegin()[1];
            next_number = last_time - prev_time;
        }

        if(turns % 1000000 == 0) {
            printf("Saying %ld on turn %ld\n", next_number, turns);
        }

        if(history.count(next_number) == 0) {
            history[next_number] = {};
        }
        history[next_number].push_back(turns);
        last_number = next_number;
    }

    return last_number;
    return 0; 
}