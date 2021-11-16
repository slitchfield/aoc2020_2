
#include "aoc.hpp"

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

std::pair<std::string, unsigned int> get_pair(std::string in) {
    if(in.back() == '.') in.pop_back();
    if(in.back() == 's') in.pop_back();

    size_t stop = in.find(' ');
    unsigned int count = atoi(in.substr(0, stop).c_str());
    std::string bag = in.substr(stop+1);

    return {bag, count};
}

struct input_elem parse_elem(char* line) {
    struct input_elem ret;
    std::string strline = line;
    size_t cur_end = strline.find(" contain");
    ret.container = new std::string(strline.substr(0, cur_end - 1));

    ret.contains = new std::vector<std::pair<std::string, unsigned int>>();

    cur_end = strline.find(" contain ");
    cur_end += strlen(" contain ");
    size_t next_end;
    std::string bag_num_pair;
    while( (next_end = strline.find(',', cur_end)) != std::string::npos ) {
        bag_num_pair = strline.substr(cur_end, next_end - cur_end);
        ret.contains->push_back(get_pair(bag_num_pair));
        cur_end = next_end + 2;
    }

    bag_num_pair = strline.substr(cur_end);
    ret.contains->push_back(get_pair(bag_num_pair));

    print_elem(ret);
    return ret;
}

void print_elem(struct input_elem in) {
    printf("%s contains:\n", in.container->c_str());
    for(auto iter: *in.contains) {
        printf("\t%d %s\n", iter.second, iter.first.c_str());
    }
    printf("\n");
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

        printf("Token %d:\n", i);
        printf("%s\n\n", elem_arr[i]);
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
    for(auto item: ret.vec) {
        for(auto subitem: *item.contains) {
            ret.k_contains_v[*item.container].push_back(subitem);
            ret.k_containedby_v[subitem.first].push_back({*item.container, subitem.second});
        }
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
    delete in.container;
    delete in.contains;
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