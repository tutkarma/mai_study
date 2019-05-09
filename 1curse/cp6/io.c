#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "structure.h"
#include "io.h"

uint32_t computer_read_txt(Computer *computer, FILE *in)
{
    fscanf(in, "%s", computer->owner);
    fscanf(in, "%s", computer->name_OS);
    fscanf(in, "%u", &computer->num_cpu);
    fscanf(in, "%s", computer->cpu.name);
    fscanf(in, "%u", &computer->cpu.freq);
    fscanf(in, "%s", computer->gpu.name);
    fscanf(in, "%u", &computer->gpu.memory);
    fscanf(in, "%u", &computer->num_hdd);

    computer->hdds = (HDD *) malloc(computer->num_hdd * sizeof(HDD));
    for (int i = 0; i < computer->num_hdd; ++i) {
        fscanf(in, "%s", computer->hdds[i].type);
        fscanf(in, "%u", &computer->hdds[i].size);
    }

    //fscanf(in, "%s", computer->hdd.type);
    //fscanf(in, "%u", &computer->hdd.size);
    fscanf(in, "%u", &computer->ram);

    return !feof(in);
}

uint32_t computer_read_bin(Computer *computer, FILE *in)
{
    fread(computer->owner, sizeof(char), STR_SIZE, in);
    fread(computer->name_OS, sizeof(char), STR_SIZE, in);
    fread(&computer->num_cpu, sizeof(uint32_t), 1, in);
    fread(computer->cpu.name, sizeof(char), STR_SIZE, in);
    fread(&computer->cpu.freq, sizeof(uint32_t), 1, in);
    fread(computer->gpu.name, sizeof(char), STR_SIZE, in);
    fread(&computer->gpu.memory, sizeof(uint32_t), 1, in);
    fread(&computer->num_hdd, sizeof(uint32_t), 1, in);

    computer->hdds = (HDD *) malloc(computer->num_hdd * sizeof(HDD));
    for (uint32_t i = 0; i < computer->num_hdd; ++i) {
        fread(computer->hdds[i].type, sizeof(char), STR_SIZE, in);
        fread(&computer->hdds[i].size, sizeof(uint32_t), 1, in);
    }

    //fread(computer->hdd.type, sizeof(char), STR_SIZE, in);
    //fread(&computer->hdd.size, sizeof(uint32_t), 1, in);
    fread(&computer->ram, sizeof(uint32_t), 1, in);

    return !feof(in);
}

void computer_write_bin(Computer *computer, FILE *out)
{
    fwrite(computer->owner, sizeof(char), STR_SIZE, out);
    fwrite(computer->name_OS,  sizeof(char), STR_SIZE, out);
    fwrite(&computer->num_cpu, sizeof(uint32_t), 1, out);
    fwrite(computer->cpu.name, sizeof(char), STR_SIZE, out);
    fwrite(&computer->cpu.freq, sizeof(uint32_t), 1, out);
    fwrite(computer->gpu.name, sizeof(char), STR_SIZE, out);
    fwrite(&computer->gpu.memory, sizeof(uint32_t), 1, out);
    fwrite(&computer->num_hdd, sizeof(uint32_t), 1, out);

    for (int i = 0; i < computer->num_hdd; ++i) {
        fwrite(computer->hdds[i].type, sizeof(char), STR_SIZE, out);
        fwrite(&computer->hdds[i].size, sizeof(uint32_t), 1, out);
    }

    //fwrite(computer->hdd.type, sizeof(char), STR_SIZE, out);
    //fwrite(&computer->hdd.size, sizeof(uint32_t), 1, out);
    fwrite(&computer->ram, sizeof(uint32_t), 1, out);
}

void computer_print(Computer *computer) 
{
    printf("%s\n", computer->owner);
    printf("%s\n", computer->name_OS);
    printf("%u\n", computer->num_cpu);
    printf("%s\n", computer->cpu.name);
    printf("%u\n", computer->cpu.freq);
    printf("%s\n", computer->gpu.name);
    printf("%u\n", computer->gpu.memory);
    printf("%u\n", computer->num_hdd);

    for (int i = 0; i < computer->num_hdd; ++i) {
        printf("%s\n", computer->hdds[i].type);
        printf("%u\n", computer->hdds[i].size);
    }

    //printf("%s\n", computer->hdd.type);
    //printf("%u\n", computer->hdd.size);
    printf("%u\n", computer->ram);
    printf("\n");
}


uint32_t reference_read_txt(Reference *reference, FILE *in)
{
    char spec[STR_SIZE];
    for(int i = 0; i < 5; ++i) {
        fscanf(in, "%s", spec);
        if(!strcmp(spec, "num_cpu:")) {
            fscanf(in, "%lf", &reference->num_cpu);
        } else if(!strcmp(spec, "cpu_freq:")) {
            fscanf(in, "%lf", &reference->cpu_freq);
        } else if(!strcmp(spec, "gpu_memory:")) {
            fscanf(in, "%lf", &reference->gpu_memory);
        } else if(!strcmp(spec, "hdd_size:")) {
            fscanf(in, "%lf", &reference->hdd_size);
        } else if(!strcmp(spec, "ram:")) {
            fscanf(in, "%lf", &reference->ram);
        }
    }
    return !feof(in);
}