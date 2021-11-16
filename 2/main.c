
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aoc.h"

#define INPUT_FILENAME "./input.txt"

unsigned int count_key_occurrences(char c, const char* str);

unsigned int part_one(struct input_arr);
unsigned int part_two(struct input_arr);

int main(int argc, char** argv) {

    struct input_arr input = read_inputfile(INPUT_FILENAME);
    //for(int i = 0; i < input.num_elems; i++)
    //    print_elem(input.arr[i]);

    unsigned int part_one_answer = part_one(input);
    unsigned int part_two_answer = part_two(input);

    printf("Part One Answer:\t%d\n", part_one_answer);
    printf("Part Two Answer:\t%d\n", part_two_answer);

    free_input(input);

    exit(EXIT_SUCCESS);
}

unsigned int count_key_occurrences(char c, const char* str) {
    unsigned int count = 0;

    for(int i = 0; i < strlen(str); i++) {
        if (str[i] == c) count++;
    }
    return count;
}

int p1_isvalid(struct input_elem input) {

    unsigned int key_count = count_key_occurrences(input.key, input.password);
    if (key_count >= input.lowerbound && key_count <= input.upperbound)
        return 1;
    else
        return 0;
}

unsigned int part_one(struct input_arr input) {
    unsigned int num_valid = 0;
    for(int i = 0; i < input.num_elems; i++) {
        if (p1_isvalid(input.arr[i]) == 1)
            num_valid++;
    }
    return num_valid;
}

int p2_isvalid(struct input_elem input) {
    char pos_one = input.password[input.lowerbound - 1];
    char pos_two = input.password[input.upperbound - 1];

    if (pos_one == input.key && pos_two != input.key)
        return 1;
    if (pos_one != input.key && pos_two == input.key)
        return 1;
    return 0;
}

unsigned int part_two(struct input_arr input) {
    unsigned int num_valid = 0;
    for(int i = 0; i < input.num_elems; i++) {
        if (p2_isvalid(input.arr[i]) == 1)
            num_valid++;
    }
    return num_valid;
}