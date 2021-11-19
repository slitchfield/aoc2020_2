
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
    // run_test(TEST_INPUT_2_FILENAME); // Test two not present for day 11
    run_test(INPUT_FILENAME);
    exit(EXIT_SUCCESS);
}

int normalize_heading(int heading) {
    // If positive and > 360, bring back to [0, 360]
    int new_heading = heading % 360;

    // If still negative, get to positive equivalent
    // ex, -90 + 360 = 270, -180 + 360 = 180, etc.
    if(new_heading < 0) {
        new_heading = new_heading + 360;
    }
    return new_heading;
}

int part_one(struct input_arr input) {
    for(auto instr: input.vec) {
        switch(instr.instr) {
            case NORTH: {
                // North is +y
                input.bstate.y += instr.value;
            } break;
            case SOUTH: {
                // South is -y
                input.bstate.y -= instr.value;
            } break;
            case EAST: {
                // East is +x 
                input.bstate.x += instr.value;
            } break;
            case WEST: {
                // East is -x
                input.bstate.x -= instr.value;
            } break;
            case LEFT: {
                // "Left" implies ccw rotation, convention will be pos deg
                input.bstate.heading += instr.value;
                input.bstate.heading = normalize_heading(input.bstate.heading);
            } break;
            case RIGHT: {
                // "Right" implies cw rotation, convention will be neg deg
                input.bstate.heading -= instr.value;
                input.bstate.heading = normalize_heading(input.bstate.heading);
            } break;
            case FWD: {
                switch(input.bstate.heading) {
                    case 0: {
                        input.bstate.x += instr.value;
                    } break;
                    case 90: {
                        input.bstate.y += instr.value;
                    } break;
                    case 180: {
                        input.bstate.x -= instr.value;
                    } break;
                    case 270: {
                        input.bstate.y -= instr.value;
                    } break;
                    default: {
                        printf("Non-right-angled heading! %d\n", input.bstate.heading);
                    } break;
                }
            } break;
            default: {
                printf("Unhandled instruction?\n");
            } break;
        }
    }
    int manhattan_distance = abs(input.bstate.x) + abs(input.bstate.y);
    return manhattan_distance;
}

void rotate_waypoint(instr_t dir, int value, struct boat_state* bstate) {
    int rot_degrees = value;

    // Translate all rotation into left rotation 
    if(dir == RIGHT) {
        rot_degrees = 360 - rot_degrees; 
    }

    while(rot_degrees > 0) {
        // One 90 deg left rotation involves:
        //   new y = x
        //   new x = -y
        int tmp = bstate->waypoint_delta.first;
        bstate->waypoint_delta.first = -bstate->waypoint_delta.second;
        bstate->waypoint_delta.second = tmp;
        rot_degrees -= 90;
    }
}

uint64_t part_two(struct input_arr input) {
    for(auto instr: input.vec) {
        switch(instr.instr) {
            case NORTH: {
                // North is +y
                input.bstate.waypoint_delta.second += instr.value;
            } break;
            case SOUTH: {
                // South is -y
                input.bstate.waypoint_delta.second -= instr.value;
            } break;
            case EAST: {
                // East is +x 
                input.bstate.waypoint_delta.first += instr.value;
            } break;
            case WEST: {
                // East is -x
                input.bstate.waypoint_delta.first -= instr.value;
            } break;
            case LEFT: {
                // "Left" implies ccw rotation, convention will be pos deg
                rotate_waypoint(instr.instr, instr.value, &input.bstate);
            } break;
            case RIGHT: {
                // "Right" implies cw rotation, convention will be neg deg
                rotate_waypoint(instr.instr, instr.value, &input.bstate);
            } break;
            case FWD: {
                for(int i = 0; i < instr.value; i++) {
                    input.bstate.x += input.bstate.waypoint_delta.first;
                    input.bstate.y += input.bstate.waypoint_delta.second;
                }
            } break;
            default: {
                printf("Unhandled instruction?\n");
            } break;
        }
    }
    int manhattan_distance = abs(input.bstate.x) + abs(input.bstate.y);
    return manhattan_distance;
    return 0;
}