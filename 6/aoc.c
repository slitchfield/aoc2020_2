
#include "aoc.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    char* subptr = strstr(input, sub);
    while(subptr != NULL) {
        subptr += strlen(sub);
        i++;
        subptr = strstr(subptr, sub);
    }
    return i;
}

uint32_t get_bitmask(char* line) {
    uint32_t bitmask = 0;

    for(int i = 0; i < strlen(line); i++) {
        char c = line[i];
        c = c - 'a';
        bitmask |= (1 << c);
    }

    return bitmask;
}

struct input_elem parse_elem(char* line) {
    struct input_elem ret;
    ret.num_members = 0;
    ret.union_bitmask = 0;
    ret.inter_bitmask = 0xffffffff;

    // Count number of spaces, number of members = this + 1
    const char* sep = " ";
    ret.num_members = count_substr(line, sep) + 1;

    ret.answers = (struct individual_answers*) calloc(ret.num_members, sizeof(struct individual_answers));
    char* cur_answer = line;
    for(int i = 0; i < ret.num_members; i++) {

        char* next_answer = strstr(cur_answer, sep);
        int num_chars = next_answer - cur_answer;
        if (next_answer == NULL) num_chars = strlen(cur_answer);


        ret.answers[i].line = (char*)calloc(num_chars + 1, sizeof(char));
        strncpy(ret.answers[i].line, cur_answer, num_chars);
        ret.answers[i].line[num_chars] = '\0';
        ret.answers[i].bitmask = get_bitmask(ret.answers[i].line);

        cur_answer = next_answer + strlen(sep);

        ret.union_bitmask |= ret.answers[i].bitmask;
        ret.inter_bitmask &= ret.answers[i].bitmask;
    }
    print_elem(ret);
    return ret;
}

void print_elem(struct input_elem in) {
    printf("Individual Answers:\n");
    for(int i = 0; i < in.num_members; i++) {
        printf("\t0x%08x\n", in.answers[i].bitmask); 
    }
    printf("Union:\t0x%08x\n", in.union_bitmask);
    printf("Inter:\t0x%08x\n", in.inter_bitmask);
}

char** tokenize_elems(const char* filename, int* num_elems) {
    char** elem_arr = NULL;
    FILE* fp;
    char* buf = NULL;
    char* curbuf = NULL;
    char* emptyline = NULL;
    const char sep[3] = "\n\n";
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

    ret.arr = NULL;
    ret.num_elems = 0;

    char** token_list = tokenize_elems(filename, &ret.num_elems);

    ret.arr = (struct input_elem*) calloc(ret.num_elems, sizeof(struct input_elem));

    for(int i = 0; i < ret.num_elems; i++) {
        //printf(">>Parsing element %d\n", i);
        ret.arr[i] = parse_elem(token_list[i]);
    }

    // Free temp token list
    for(int i = 0; i < ret.num_elems; i++) free(token_list[i]);
    free(token_list);

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
    for(int i = 0; i < in.num_members; i++) {
        free(in.answers[i].line);
    }
    free(in.answers);
}

void free_input(struct input_arr in) {
    for(int i = 0; i < in.num_elems; i++) {
        free_elem(in.arr[i]); 
    }
    free(in.arr);
}

void print_input(struct input_arr in) {
}