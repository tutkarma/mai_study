#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_
#include <inttypes.h>

#define STR_SIZE 32



typedef struct {
    char name[STR_SIZE];
    uint32_t freq;
} CPU;

typedef struct {
    char name[STR_SIZE];
    uint32_t memory;
} GPU;

typedef struct {
    char type[STR_SIZE];
    uint32_t size;
} HDD;

typedef struct {
    char owner[STR_SIZE];
    char name_OS[STR_SIZE];
    uint32_t num_cpu;
    CPU cpu;
    GPU gpu;
    uint32_t num_hdd;
    HDD *hdds;
    uint32_t ram;
} Computer;

typedef struct {
    double num_cpu;
    double cpu_freq;
    double gpu_memory;
    double hdd_size;
    double ram;
} Reference;

#endif