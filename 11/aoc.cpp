
#include "aoc.hpp"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>

unsigned int count_lines(const char* filename) {

    FILE* fp;
    char* line = NULL;
    size_t len = 0;

    fp = fopen(filename, "r");
    if ( fp == NULL )
        return 0;

    unsigned int lines = 0;
    while ( getline(&line, &len, fp) != -1 ) {
        lines++;
    }

    if (line)
        free(line);

    fclose(fp);

    return lines;    
}

unsigned int count_substr(const char* input, const char* sub) {
    unsigned int i = 0;
    const char* subptr = strstr(input, sub);
    while(subptr != NULL) {
        subptr += strlen(sub);
        i++;
        subptr = strstr(subptr, sub);
    }
    return i;
}

struct input_elem parse_elem(char* line) {
    struct input_elem ret;
    //print_elem(ret);
    return ret;
}

std::vector<seat_state_t> parse_to_vec(char* line) {
    std::vector<seat_state_t> ret;
    for(int i = 0; i < strlen(line); i++) {
        if(line[i] == 'L') {
            ret.push_back(EMPTY);
        }
        else if (line[i] == '.') {
            ret.push_back(FLOOR);
        }
        else {
            ret.push_back(UNDEF);
        }
    }
    return ret;
}

void print_elem(struct input_elem in) {
}

char** tokenize_elems(const char* filename, int* num_elems) {
    char** elem_arr = NULL;
    FILE* fp;
    char* buf = NULL;
    char* curbuf = NULL;
    char* emptyline = NULL;
    const char sep[3] = "\n";
    char* token = NULL;
    unsigned int num_chars = 0;

    // Open and read entire file. 
    fp = fopen(filename, "r");
    if ( fp == NULL )
        exit(EXIT_FAILURE);

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buf = (char*)calloc(fsize + 1, sizeof(char));
    fread(buf, sizeof(char), fsize, fp);
    buf[fsize] = '\0';
    fclose(fp);

    // Count how many tokens we have
    curbuf = buf;
    emptyline = strstr(curbuf, sep);
    int i = 1;

    while (emptyline != NULL) {
        i++;
        curbuf = emptyline + strlen(sep);
        emptyline = strstr(curbuf, sep);
    }

    *num_elems = i;

    // Allocate ultimate array and pull out tokens
    elem_arr = (char**) calloc(*num_elems, sizeof(char*));

    i = 0;
    curbuf = buf;
    emptyline = strstr(curbuf, sep);

    while (emptyline != NULL) {
        num_chars = emptyline - curbuf;
        elem_arr[i] = (char*) calloc(num_chars + 1, sizeof(char));
        strncpy(elem_arr[i], curbuf, num_chars);
        elem_arr[i][num_chars] = '\0';

        // Sanitize tokens
        trim_trailing_whitespace(elem_arr[i]);
        replace_char(elem_arr[i], '\n', ' ');

        //printf("Token %d:\n", i);
        //printf("%s\n\n", elem_arr[i]);
        i += 1;

        curbuf = emptyline + strlen(sep);
        emptyline = strstr(curbuf, sep);
    }

    // Get last token, accounting for mistaking last delimiter as token
    // Technically wastes 1x char*, but no problems with needing to
    //   free the last slot, since we never allocate to it
    num_chars = strlen(curbuf);
    if (num_chars == 0) {
        *num_elems -= 1;
    } else {
        elem_arr[i] = (char*) calloc(num_chars + 1, sizeof(char));
        strncpy(elem_arr[i], curbuf, num_chars);
        elem_arr[i][num_chars] = '\0';
        // Sanitize
        trim_trailing_whitespace(elem_arr[i]);
        replace_char(elem_arr[i], '\n', ' ');
    }


    //printf("Token %d:\n", i);
    //printf("%s\n\n", elem_arr[i]);

    free(buf);
    return elem_arr;
}

struct input_arr read_inputfile(const char* filename) {
    struct input_arr ret;
    ret.num_elems = 0;

    char** token_list = tokenize_elems(filename, &ret.num_elems);

    //ret.arr = (struct input_elem*) calloc(ret.num_elems, sizeof(struct input_elem));

    for(int i = 0; i < ret.num_elems; i++) {
        //printf(">>Parsing element %d\n", i);
        //ret.arr[i] = parse_elem(token_list[i]);
        //ret.vec.push_back(parse_elem(token_list[i])); // Parse a single element at a time
        ret.seat_array_a.push_back(parse_to_vec(token_list[i])); // Parse a line of a whole input
        ret.seat_array_b.push_back(parse_to_vec(token_list[i]));
    }
    ret.cur_ptr = &ret.seat_array_a;
    ret.nxt_ptr = &ret.seat_array_b;

    // Free temp token list
    for(int i = 0; i < ret.num_elems; i++) free(token_list[i]);
    free(token_list);

    // Do any post processing necessary

    return ret;
}

char* trim_trailing_whitespace(char* str) {

    char* end = str + strlen(str) - 1;
    while (end > str && isspace( (unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

void replace_char(char* str, char find, char replace) {
    char* cur = str;
    while (*cur != '\0') {
        if ( *cur == find ) *cur = replace;
        cur++;
    }
}

void free_elem(struct input_elem in) {
    // UPDATE: Free any internal allocations
}

void free_input(struct input_arr in) {
    //for(int i = 0; i < in.num_elems; i++) {
    //    free_elem(in.arr[i]); 
    //}
    for(auto item: in.vec) {
        free_elem(item);
    }
    in.vec.clear();
    //free(in.arr);
}

void print_input(struct input_arr in) {
    printf("Input array:\n");
    print_seatmap(in.cur_ptr);
}

bool diff_seatmaps(std::vector<std::vector<seat_state_t>>* a,
                   std::vector<std::vector<seat_state_t>>* b) {
    // Returns true if maps are different

    // First check sizes
    if(a->size() != b->size()) return true;
    if(a[0].size() != b[0].size()) return true;

    // Check seat contents
    for(int r = 0; r < a->size(); r++) {
        for(int c = 0; c < a->at(r).size(); c++) {
            if(a->at(r)[c] != b->at(r)[c]) {
                printf("Found difference @ %d,%d\n", r, c);
                return true;
            }
        }
    }

    // We didn't find any differences!
    return false;
}

void print_seatmap(std::vector<std::vector<seat_state_t>>* m) {
    for(auto row: *m) {
        for(auto col: row) {
            switch(col) {
                case FLOOR: {
                    printf(" ");
                } break;
                case EMPTY: {
                    printf("L");
                } break;
                case OCCUPIED: {
                    printf("#");
                } break;
                case UNDEF: {
                    printf("U");
                } break;
            }
        }
        printf("\n");
    }
}