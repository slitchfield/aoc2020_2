
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

struct input_elem parse_elem(char* line) {
    struct input_elem ret;
    ret.presence = 0;

    //printf("Parsing token: \"%s\"\n", line);

    const char sep[1] = " ";

    char* elem = strtok(line, sep);

    while (elem != NULL) {
        //printf("\tParsing element: \"%s\"\n", elem);

        char* val = elem + 4;

        switch(elem[0]) {
            case 'i':
                // iyr case
                ret.iyr = atoi(val);
                ret.presence |= IYR;
                break;

            case 'e':
                // ecl | eyr case
                if(elem[1] == 'c') {
                    ret.ecl[0] = val[0];
                    ret.ecl[1] = val[1];
                    ret.ecl[2] = val[2];
                    ret.ecl[3] = '\0';
                    ret.presence |= ECL;
                }
                else if (elem[1] == 'y') {
                    ret.eyr = atoi(val);
                    ret.presence |= EYR;
                }
                else printf("Unidentified case: %s\n", elem);
                break;

            case 'h':
                // hgt | hcl case
                if(elem[1] == 'c') {
                    strncpy(ret.hcl, val, 7);
                    ret.presence |= HCL;
                }
                else if (elem[1] == 'g') {
                    // TODO derive units
                    int num = atoi(val);
                    ret.hgt = num;
                    ret.presence |= HGT;
                    char* ptr = val;
                    while(isdigit(*++ptr)) {}
                    if ( ptr[0] == 'c' ) {
                        ret.hgt_unit = 'c';
                    }
                    else if ( ptr[0] == 'i' ) {
                        ret.hgt_unit = 'i';
                    }
                    else {
                        ret.hgt_unit = 'n';
                    }
                }
                else printf("Unidentified case: %s\n", elem);
                break;

            case 'p': {
                // pid case
                int num_chars = strlen(val);
                if (num_chars == 9) ret.pid_valid = 1;
                else ret.pid_valid = 0;
                ret.pid = atoll(val);
                ret.presence |= PID;
            } break;

            case 'b':
                // byr case
                ret.byr = atoi(val);
                ret.presence |= BYR;
                break;

            case 'c':
                // cid case
                ret.cid = atol(val);
                ret.presence |= CID;
                break;

            default:
                printf("Parseelem: unidentified case: %s\n", elem);
                break;
        }
        elem = strtok(NULL, sep);
    }

    return ret;
}

void print_elem(struct input_elem in) {
    // TODO:
}

char** tokenize_elems(const char* filename, int* num_elems) {
    char** elem_arr = NULL;
    FILE* fp;
    char* buf = NULL;
    char* curbuf = NULL;
    char* emptyline = NULL;
    const char sep[3] = "\n\n\0";
    char* token = NULL;
    unsigned int num_chars = 0;

    // Open and read entire file. 
    fp = fopen(filename, "r");
    if ( fp == NULL )
        exit(EXIT_FAILURE);

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    buf = calloc(fsize + 1, sizeof(char));
    fread(buf, sizeof(char), fsize, fp);
    buf[fsize] = '\0';
    fclose(fp);

    // Count how many tokens we have
    curbuf = buf;
    emptyline = strstr(curbuf, sep);
    int i = 1;

    while (emptyline != NULL) {
        i++;
        curbuf = emptyline + 2;
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

        curbuf = emptyline + 2;
        emptyline = strstr(curbuf, sep);
    }

    // Get last token
    num_chars = strlen(curbuf);
    elem_arr[i] = (char*) calloc(num_chars + 1, sizeof(char));
    strncpy(elem_arr[i], curbuf, num_chars);
    elem_arr[i][num_chars] = '\0';

    // Sanitize tokens
    trim_trailing_whitespace(elem_arr[i]);
    replace_char(elem_arr[i], '\n', ' ');

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
    //TODO
}

void free_input(struct input_arr in) {
    for(int i = 0; i < in.num_elems; i++) {
        free_elem(in.arr[i]); 
    }
    free(in.arr);
}

void print_input(struct input_arr in) {
}