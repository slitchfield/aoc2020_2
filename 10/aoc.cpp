
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
    ret.num = atoll(line);
    //print_elem(ret);
    return ret;
}

void print_elem(struct input_elem in) {
    printf("num: %lu\n", in.num);
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

bool comp_func(struct input_elem a, struct input_elem b) { return (a.num < b.num); }

struct input_arr read_inputfile(const char* filename) {
    struct input_arr ret;
    ret.num_elems = 0;

    char** token_list = tokenize_elems(filename, &ret.num_elems);

    //ret.arr = (struct input_elem*) calloc(ret.num_elems, sizeof(struct input_elem));

    for(int i = 0; i < ret.num_elems; i++) {
        //printf(">>Parsing element %d\n", i);
        //ret.arr[i] = parse_elem(token_list[i]);
        ret.vec.push_back(parse_elem(token_list[i]));
    }

    // Free temp token list
    for(int i = 0; i < ret.num_elems; i++) free(token_list[i]);
    free(token_list);

    // Do any post processing necessary
    
    // Add charging outlet
    uint64_t min = 0;
    struct input_elem outlet_elem;
    outlet_elem.num = 0;
    ret.vec.push_back(outlet_elem);

    // Sort ret.vec in place
    std::sort(ret.vec.begin(), ret.vec.end(), comp_func);

    // Add built-in adapter
    uint64_t max = ret.vec.back().num;
    struct input_elem new_elem;
    new_elem.num = max + 3;
    ret.vec.push_back(new_elem);

    for(int i = 0; i < ret.vec.size() - 1; i++) {
        uint64_t diff = ret.vec[i+1].num - ret.vec[i].num;
        ret.diffs.push_back(diff);
    }

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
}