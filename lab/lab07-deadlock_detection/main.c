/*
    Lab 07: Deadlock Detection
    COMP 173: Operating Systems (Fall 2020)
    Michael Kmak

    compilation: gcc main.c -o main
    run: ./main <input_file>

*/
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct {
    size_t process_count;
    size_t resource_count;
    uint32_t *available;
    uint32_t **max;
    uint32_t **have;
    uint32_t **need;
} system_state;

bool read_file(const char *path, system_state *s);
void alloc_sys_state(system_state *s);
void print_sys_state(const system_state *s);


int main() {
    system_state s;
    read_file("infile.txt", &s);
    print_sys_state(&s);
    return 0;
}

bool read_file(const char *path, system_state *s) {
    FILE *fp;

    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading file %s\n", path);
        return false;
    }

    fscanf(fp, "%zu", &s->process_count);
    fscanf(fp, "%zu", &s->resource_count);

    alloc_sys_state(s);

    // read s.available
    for(size_t i = 0; i < s->resource_count; i++) {
        fscanf(fp, "%"PRIu32, &s->available[i]);
    }

    // read s.have
    for(size_t p; p < s->process_count; p++) {
        for(size_t i = 0; i < s->resource_count; i++) {
            fscanf(fp, "%"PRIu32, &s->have[p][i]);
        }
    }

    // read s.need
    for(size_t p; p < s->process_count; p++) {
        for(size_t i = 0; i < s->resource_count; i++) {
            fscanf(fp, "%"PRIu32, &s->need[p][i]);
            // also fill s.max
            s->max[p][i] = s->have[p][i] + s->need[p][i];
        }
    }
    return true;
}

void alloc_sys_state(system_state *s) {
    printf("Allocing for %zu processes, %zu resources ...", s->process_count, s->resource_count);
    s->available = calloc(s->resource_count, sizeof(uint32_t));

    s->have = calloc(s->process_count, sizeof(uint32_t *));
    s->max = calloc(s->process_count, sizeof(uint32_t *));
    s->need = calloc(s->process_count, sizeof(uint32_t *));

    for(size_t p = 0; p < s->process_count; p++) {
        s->have[p] = calloc(s->resource_count, sizeof(uint32_t));
        s->max[p] = calloc(s->resource_count, sizeof(uint32_t));
        s->need[p] = calloc(s->resource_count, sizeof(uint32_t));
    }
    printf(" OK\n");
}

void print_sys_state(const system_state *s) {
    printf("=== System State ===\n");

    printf("Available:\n");
    printf("\t{");
    for(size_t i = 0; i < s->resource_count; i++) {
        printf(" %" PRIu32 " ", s->available[i]);
    }
    printf("}\n");

    printf("Have:\n");
    for(size_t p = 0; p < s->process_count; p++) {
        printf("\t{");
        for(size_t i = 0; i < s->resource_count; i++) {
            printf(" %" PRIu32 " ", s->have[p][i]);
        }
        printf("}\n");
    }

    printf("Max:\n");
    for(size_t p = 0; p < s->process_count; p++) {
        printf("\t{");
        for(size_t i = 0; i < s->resource_count; i++) {
            printf(" %" PRIu32 " ", s->max[p][i]);
        }
        printf("}\n");
    }

    printf("Need:\n");
    for(size_t p = 0; p < s->process_count; p++) {
        printf("\t{");
        for(size_t i = 0; i < s->resource_count; i++) {
            printf(" %" PRIu32 " ", s->need[p][i]);
        }
        printf("}\n");
    }

    printf("====================\n");
}

