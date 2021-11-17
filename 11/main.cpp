
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

size_t get_num_neighbors(std::vector<std::vector<seat_state_t>>* seatmap, size_t row, size_t col) {
    size_t row_size = seatmap->size();
    size_t min_row = row > 0 ? row - 1 : 0;
    size_t max_row = row < row_size - 1 ? row + 1 : row_size - 1;

    size_t col_size = seatmap->at(0).size();
    size_t min_col = col > 0 ? col - 1 : 0;
    size_t max_col = col < col_size - 1 ? col + 1 : col_size - 1;

    size_t num_neighbors = 0;

    for(int r = min_row; r <= max_row; r++) {
        for(int c = min_col; c <= max_col; c++) {
            if(r == row && c == col) continue;
            if(seatmap->at(r)[c] == OCCUPIED) num_neighbors += 1;
        }
    }

    return num_neighbors;

}

void evolve_seatmap(struct input_arr* input) {
    size_t num_rows = input->cur_ptr->size();
    size_t num_cols = input->cur_ptr->at(0).size();

    for(int row = 0; row < num_rows; row++) {
        for(int col = 0; col < num_cols; col++) {
            seat_state_t cur_state = input->cur_ptr->at(row)[col];
            size_t num_neighbors = get_num_neighbors(input->cur_ptr, row, col);
            if( (cur_state == EMPTY) && 
                (num_neighbors == 0) ){
                input->nxt_ptr->at(row)[col] = OCCUPIED;
            }
            else if ( (cur_state == OCCUPIED) &&
                (num_neighbors >= 4) ) {
                input->nxt_ptr->at(row)[col] = EMPTY;
            } else {
                input->nxt_ptr->at(row)[col] = input->cur_ptr->at(row)[col];
            }
        }
    }
}

void swap_ptrs(struct input_arr* input) {
    std::vector<std::vector<seat_state_t>>* tmp = input->nxt_ptr;
    input->nxt_ptr = input->cur_ptr;
    input->cur_ptr = tmp;
}

int count_occupied(std::vector<std::vector<seat_state_t>>* m) {
    int retval = 0;
    for(auto row: *m) {
        for(auto col: row) {
            if(col == OCCUPIED) retval++;
        }
    }
    return retval;
}

int part_one(struct input_arr input) {
    // Iterate while evolve_seatmap creates differences 
    bool still_different = true;
    while(still_different) {
        evolve_seatmap(&input);
        //printf("Cur map:\n");
        //print_seatmap(input.cur_ptr);

        //printf("Nxt map:\n");
        //print_seatmap(input.nxt_ptr);

        still_different = diff_seatmaps(input.cur_ptr, input.nxt_ptr);
        swap_ptrs(&input);
        //print_input(input);
    }

    return count_occupied(input.cur_ptr);
}

size_t get_num_neighbors_2(std::vector<std::vector<seat_state_t>>* seatmap, size_t row, size_t col) {
    // TODO: change from looking at nearest 8 to casting rays in 8 dirs and
    //  finding closest non-floor
    size_t num_neighbors = 0;

    std::vector<std::pair<int, int>> dirs = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1}
    };
    size_t num_rows = seatmap->size();
    size_t num_cols = seatmap->at(0).size();

    for(auto dir: dirs) {
        int cur_row = row + dir.first;
        int cur_col = col + dir.second;
        while( (cur_row >= 0 && cur_row < num_rows) &&
               (cur_col >= 0 && cur_col < num_cols) ) {
            seat_state_t cur_state = seatmap->at(cur_row)[cur_col];

            // If this seat is occupied, we found a neighbor
            if(cur_state == OCCUPIED) {
                num_neighbors += 1;
                break;

            // If this seat is empty, no neighbor, but we're done with this direction
            } else if(cur_state == EMPTY) {
                break;

            // Otherwise keep moving along the direction
            } else {
                cur_row += dir.first;
                cur_col += dir.second; 
            }
        }
    }
    return num_neighbors;
}

void evolve_seatmap_2(struct input_arr* input) {
    // TODO: change rules to 5 or more neighbors
    size_t num_rows = input->cur_ptr->size();
    size_t num_cols = input->cur_ptr->at(0).size();

    for(int row = 0; row < num_rows; row++) {
        for(int col = 0; col < num_cols; col++) {
            seat_state_t cur_state = input->cur_ptr->at(row)[col];
            size_t num_neighbors = get_num_neighbors_2(input->cur_ptr, row, col);
            if      ( (cur_state == EMPTY) && 
                      (num_neighbors == 0) ) {
                input->nxt_ptr->at(row)[col] = OCCUPIED;
            }
            else if ( (cur_state == OCCUPIED) &&
                      (num_neighbors >= 5) ) {
                input->nxt_ptr->at(row)[col] = EMPTY;
            } else {
                input->nxt_ptr->at(row)[col] = input->cur_ptr->at(row)[col];
            }
        }
    }
}

uint64_t part_two(struct input_arr input) {

    // Reset the seat map
    for(int i = 0; i < input.seat_array_a.size(); i++) {
        for(int j = 0; j < input.seat_array_a[i].size(); j++) {
            if (input.seat_array_a[i][j] == OCCUPIED) {
                input.seat_array_a[i][j] = EMPTY;
                input.seat_array_b[i][j] = EMPTY;
            }
        }
    }
    // Passing input by value invalidates these pointers
    // This isn't great, tbh
    input.cur_ptr = &input.seat_array_a;
    input.nxt_ptr = &input.seat_array_b;

    // Iterate while evolve_seatmap creates differences 
    bool still_different = true;
    while(still_different) {
        evolve_seatmap_2(&input);
        //printf("Cur map:\n");
        //print_seatmap(input.cur_ptr);

        //printf("Nxt map:\n");
        //print_seatmap(input.nxt_ptr);

        still_different = diff_seatmaps(input.cur_ptr, input.nxt_ptr);
        swap_ptrs(&input);
        //print_input(input);
    }

    return count_occupied(input.cur_ptr);
}