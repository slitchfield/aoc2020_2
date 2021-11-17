
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

int part_one(struct input_arr input) {
    std::set<std::string> cancontaingoldbags;
    for(auto item: input.k_containedby_v["shiny gold bag"]) {
        cancontaingoldbags.insert(item.first);
    }
    size_t maxiters = 10000;
    size_t curiter = 0;
    size_t begin_size = cancontaingoldbags.size();
    size_t end_size = 0;
    while( (curiter++ < maxiters) && (begin_size != end_size) ) {
        begin_size = cancontaingoldbags.size();
        for(auto item: cancontaingoldbags) {
            for(auto subitem: input.k_containedby_v[item]) {
                cancontaingoldbags.insert(subitem.first);
            }
        }
        end_size = cancontaingoldbags.size();
    }
    return cancontaingoldbags.size();
}

uint64_t count_sub_bags(struct input_arr* input, std::string bag_name) {
    uint64_t bag_count = 0;
    // "other bag" indicates a leaf node, one that contains "no other bags"
    if(bag_name.compare("other bag") == 0)
        return 0;

    for(auto subbag: input->k_contains_v[bag_name]) {
        bag_count += subbag.second * count_sub_bags(input, subbag.first);
    }
    return 1 + bag_count;
}
uint64_t part_two(struct input_arr input) {
    uint64_t bag_count = count_sub_bags(&input, "shiny gold bag");
    return bag_count;
}