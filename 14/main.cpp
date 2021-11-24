
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
    // Day 14 P2 means the initial test input runs through 2^34 iterations. Skip for now
    //run_test(TEST_INPUT_1_FILENAME);
    run_test(TEST_INPUT_2_FILENAME); 
    run_test(INPUT_FILENAME);
    exit(EXIT_SUCCESS);
}

uint64_t part_one(struct input_arr input) {
    std::map<uint64_t, uint64_t> memory;
    uint64_t set_mask;
    uint64_t reset_mask;

    for(auto instr: input.vec) {
        if(instr.type == MASK_UPDATE) {
            set_mask = instr.set_mask;
            reset_mask = instr.reset_mask;
            //printf("New masks:\n\t%016lx\n\t%016lx\n", set_mask, ~reset_mask);
        }
        else if(instr.type == MEM_UPDATE) {
            uint64_t temp = instr.value;
            temp = temp & reset_mask;
            temp = temp | set_mask;
            memory[instr.address] = temp;
        }
        else {
            //printf("Undefined instruction!\n");
        }
    }
    uint64_t accum = 0;
    for(auto p: memory) {
        accum += p.second;
    }
    return accum;
}

void print36bitsinbinary(uint64_t val) {
    for(int i = 35; i >= 0; i--) {
        bool bit = (val & ( (uint64_t)1 << i)) > 0;
        printf("%c", bit ? '1' : '0' );
    }
    
}
uint64_t get_idx_of_nth_set_bit(uint64_t mask, uint64_t n) {
    uint64_t sets_found = 0;
    for(int i = 0; i < 64; i++) {
        if( mask & ((uint64_t)1 << i) ) {
            if(sets_found == n) return i;
            sets_found++;
            continue; 
        }
    }
    return 0;
}

uint64_t generate_next_address(uint64_t address, uint64_t float_mask, uint64_t num_floats, uint64_t curval) {
    // Split curval as binary to positions of float_mask
    for(int i = 0; i < num_floats; i++) {
        // Is this bit of the current val set?
        bool bit_set = (curval & ((uint64_t)1 << i)) > 0;

        // What position should this bit take in the final address?
        uint64_t idx = get_idx_of_nth_set_bit(float_mask, i);

        // If the bit is set, set the corresponding idx in the final address
        if(bit_set) {
            address |= ((uint64_t)1 << idx);
        }
        // If the bit is not set, clear the corresponding idx in the final address 
        else {
            uint64_t mask = ~((uint64_t)1 << idx);
            address &= mask;
        }
    }
    return address;
}

uint64_t part_two(struct input_arr input) {

    std::map<uint64_t, uint64_t> memory;
    uint64_t set_mask;
    uint64_t reset_mask;
    uint64_t float_mask;
    uint64_t num_floats;

    for(auto instr: input.vec) {
        if(instr.type == MASK_UPDATE) {
            set_mask = instr.set_mask;
            float_mask = instr.float_mask;
            num_floats = instr.num_floats;
            //printf("New mask:\n\t%016lx\n", float_mask);
        }
        else if(instr.type == MEM_UPDATE) {
            uint64_t address = instr.address | set_mask;
            uint64_t num_values = (uint64_t)1 << num_floats;
            for(int i = 0; i < num_values; i++) {
                uint64_t cur_address = generate_next_address(address, float_mask, num_floats, i); 

                //printf("Writing to address: "); print36bitsinbinary(cur_address); printf("\n");
                memory[cur_address] = instr.value;
            }
        }
        else {
            printf("Undefined instruction!\n");
        }
    }
    uint64_t accum = 0;
    for(auto p: memory) {
        accum += p.second;
    }
    return accum;
    return 0; 
}