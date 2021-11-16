
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

int part_one_validate(struct input_elem elem) {
    int full_passport = IYR | BYR | ECL | EYR | HCL | CID | PID | HGT;
    int missing_cid   = IYR | BYR | ECL | EYR | HCL | PID | HGT;

    int ret_val = 0; 
    switch(elem.presence) {
        case IYR | BYR | ECL | EYR | HCL | CID | PID | HGT :
        case IYR | BYR | ECL | EYR | HCL | PID | HGT:
            ret_val = 1;
            break;
        default:
            ret_val = 0;
            break;
    }
    return ret_val;
}

int part_one(struct input_arr input) {
    int valid_passports = 0;
    for (int i = 0; i < input.num_elems; i++) {
        valid_passports += part_one_validate(input.arr[i]);
    }
    return valid_passports;
}

int part_two_validate(struct input_elem input) {
    // Check for all required fields
    if ( part_one_validate(input) == 0) return 0;

    // Validate byr
    if (input.byr < 1920 || input.byr > 2002) return 0;

    // Validate iyr
    if (input.iyr < 2010 || input.iyr > 2020) return 0;

    if (input.eyr < 2020 || input.eyr > 2030) return 0;

    if ( input.hgt_unit == 'c' ) {
        if (input.hgt < 150 || input.hgt > 193) return 0;
    } else if ( input.hgt_unit == 'i' ) {
        if (input.hgt < 59 || input.hgt > 76) return 0;
    } else return 0;

    if ( input.hcl[0] != '#') return 0;
    for(int i = 1; i < 7; i++) {
        if ( isxdigit(input.hcl[i]) == 0) return 0;
    }

    int val = strcmp("amb", input.ecl) == 0;
    val += strcmp("blu", input.ecl) == 0;
    val += strcmp("brn", input.ecl) == 0;
    val += strcmp("gry", input.ecl) == 0;
    val += strcmp("grn", input.ecl) == 0;
    val += strcmp("hzl", input.ecl) == 0;
    val += strcmp("oth", input.ecl) == 0;
    if (val == 0) return 0;

    if (input.pid_valid == 0) return 0;

    return 1;
}

int part_two(struct input_arr input) {
    int valid_passports = 0;
    for (int i = 0; i < input.num_elems; i++) {
        valid_passports += part_two_validate(input.arr[i]);
    }
    return valid_passports;
}