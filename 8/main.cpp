
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
    unsigned int part_two_answer = part_two(input);
    end = clock();
    double part_two_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Part One Answer (%.6fs):\t%d\n", part_one_time, part_one_answer);
    printf("Part Two Answer (%.6fs):\t%d\n", part_two_time, part_two_answer);

    free_input(input);

    exit(EXIT_SUCCESS);
}

int run_code(struct input_arr* input, bool* to_completion) {
    for(int i = 0; i < input->vec.size(); i++) {
        input->vec[i].times_visited = 0;
    }
    input->pstate.pc = 0;
    input->pstate.accum = 0;
    while( (input->pstate.pc < input->vec.size()) &&
           (input->vec[input->pstate.pc].times_visited == 0) ) {
        struct input_elem curinstr = input->vec[input->pstate.pc];
        switch(curinstr.type) {
            case NOP: {
                input->vec[input->pstate.pc].accum_when_visited = input->pstate.accum;
                input->vec[input->pstate.pc].times_visited++;
                input->pstate.pc += 1;
            } break;
            case ACC: {
                input->vec[input->pstate.pc].accum_when_visited = input->pstate.accum;
                input->pstate.accum += curinstr.value;
                input->vec[input->pstate.pc].times_visited++;
                input->pstate.pc += 1;
            } break;
            case JMP: {
                input->vec[input->pstate.pc].accum_when_visited = input->pstate.accum;
                input->vec[input->pstate.pc].times_visited++;
                input->pstate.pc = input->pstate.pc + curinstr.value;
            } break;
            default: {
                printf("Unrecognized instruction!\n");
                return 0;
            } break;
        }
    }
    if(input->pstate.pc >= input->vec.size()) {
        *to_completion = true;
    }
    return input->pstate.accum;
}

int part_one(struct input_arr input) {
    uint32_t maxpc = 0;
    while(input.vec[input.pstate.pc].times_visited == 0) {
        struct input_elem curinstr = input.vec[input.pstate.pc];
        switch(curinstr.type) {
            case NOP: {
                input.vec[input.pstate.pc].accum_when_visited = input.pstate.accum;
                input.vec[input.pstate.pc].times_visited++;
                input.pstate.pc += 1;
            } break;
            case ACC: {
                input.vec[input.pstate.pc].accum_when_visited = input.pstate.accum;
                input.pstate.accum += curinstr.value;
                input.vec[input.pstate.pc].times_visited++;
                input.pstate.pc += 1;
            } break;
            case JMP: {
                input.vec[input.pstate.pc].accum_when_visited = input.pstate.accum;
                input.vec[input.pstate.pc].times_visited++;
                input.pstate.pc = input.pstate.pc + curinstr.value;
            } break;
            default: {
                printf("Unrecognized instruction!\n");
                return 0;
            } break;
        }
        if (input.pstate.pc > maxpc) maxpc = input.pstate.pc;
    }
    return input.pstate.accum;
}

uint64_t part_two(struct input_arr input) {
    bool to_completion = false;
    int accum = 0;
    for(int i = 0; i < input.vec.size(); i++) {
        if(input.vec[i].type == NOP) input.vec[i].type = JMP;
        else if(input.vec[i].type == JMP) input.vec[i].type = NOP;
        else continue;

        accum = run_code(&input, &to_completion);

        if(input.vec[i].type == NOP) input.vec[i].type = JMP;
        else if(input.vec[i].type == JMP) input.vec[i].type = NOP;

        if(to_completion) {
            printf("Done!\n");
            return accum;
        }
    }
    return 0;
}