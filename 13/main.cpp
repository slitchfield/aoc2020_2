
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
    run_test(TEST_INPUT_2_FILENAME); // Test two not present for day 11
    run_test(INPUT_FILENAME);
    exit(EXIT_SUCCESS);
}

bool compare(std::pair<uint64_t, uint64_t> a, std::pair<uint64_t, uint64_t> b) {
    return a.second < b.second; 
}

int part_one(struct input_arr input) {
    uint64_t min_id = 0;
    uint64_t min_diff = UINT64_MAX;
    for(auto bus: input.vec) {
        if(!bus.is_valid) continue;
        uint64_t next_departure = ((input.departure_time / bus.bus_id) + 1) * bus.bus_id;
        uint64_t time_diff = next_departure - input.departure_time;
        if(time_diff < min_diff) {
            min_diff = time_diff;
            min_id = bus.bus_id;
        }
    }

    return min_diff * min_id;
    return 0;
}

uint64_t next_departure_from_timestamp(uint64_t timestamp, uint64_t bus_id) {
    if(timestamp % bus_id == 0) return 0;
    uint64_t next_departure = ((timestamp / bus_id) + 1) * bus_id;
    return next_departure - timestamp;
}

typedef struct {
    uint64_t busid;
    uint64_t offset;
    uint64_t bi;
    uint64_t ni;
    uint64_t xi;
} crt_elem;

uint64_t mod_inverse(uint64_t val, uint64_t base) {
    uint64_t ret = 1;
    while( ( (ret * val) % base ) != 1)
        ret++;

    return ret;
}

void print_crt_elem(crt_elem a) {
    printf("[%lu/%lu] {%lu, %lu, %lu}\n", a.busid, a.offset, a.bi, a.ni, a.xi);
}

uint64_t math_mod(int64_t val, int64_t base) {
    if(val > 0) return val % base;
    while(val < 0) val += base;
    return val % base;
}

uint64_t chinese_remainder_theorem(struct input_arr input) {
    // Because all bus id's are relatively prime, CRT applies
    // Cast problem as solving for set of eqns where
    //   x = n mod m, where x is target timestamp, n is offset, and m is busid

    // Pretty standard CRT implementation, but checked details against:
    // https://github.com/mariothedog/Advent-of-Code-2020/blob/main/Day%2013/day_13.py

    // NOTE: c/c++ implementation of mod for negative numbers gave very wrong
    //   answer, hence math_mod() impl above.
    std::vector<crt_elem> crt_vec;
    uint64_t N = 1;
    for(auto bus: input.vec) {
        if(!bus.is_valid) continue;
        N *= bus.bus_id;
    }

    uint64_t return_val = 0;
    for(int i = 0; i < input.vec.size(); i++) {
        
        if(!input.vec[i].is_valid) continue;
        crt_elem cur;
        cur.busid = input.vec[i].bus_id;
        cur.offset = i;

        // Allow for negative differences here, and account for
        //  c/c++ weird mod behavior
        int64_t diff = (int64_t)input.vec[i].bus_id - i;
        cur.bi = math_mod(diff, input.vec[i].bus_id);

        cur.ni = N / input.vec[i].bus_id;
        cur.xi = mod_inverse(cur.ni % input.vec[i].bus_id, input.vec[i].bus_id);

        crt_vec.push_back(cur);
        return_val += cur.bi * cur.ni * cur.xi;
    }

#if 0
    for(auto a: crt_vec) {
        print_crt_elem(a);
    }
#endif

    return return_val % N;
}

uint64_t part_two(struct input_arr input) {
    return chinese_remainder_theorem(input);
}