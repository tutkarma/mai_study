#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "structure.h"
#include "io.h"
#include "owner.h"

#define REFERENCE_FILE "reference.txt"

uint32_t compare(Computer *computer, Reference *reference)
{
    bool hdds_upgrade = false;
    for (uint32_t i = 0; i < computer->num_hdd; ++i) {
        if (computer->hdds[i].size < reference->hdd_size) {
            hdds_upgrade = true;
        }
    }
    return (computer->num_cpu < reference->num_cpu) ||
           (computer->cpu.freq < reference->cpu_freq) ||
           (computer->gpu.memory < reference->gpu_memory) ||
           hdds_upgrade ||
           (computer->ram < reference->ram);
}


uint32_t main(uint32_t argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    FILE *ref_txt = fopen(REFERENCE_FILE, "r");

    if (argc != 3) {
        fprintf(stderr, "Usage:\n\t./execute DB_FILE MINIMUM_COMPUTERS_NEED_UPGRADE\n");
        exit(1);
    }

    if (!in) {
        fprintf(stderr, "I/O Error: can't open file\n");
        exit(2);
    }

    if (!ref_txt) {
        fprintf(stderr, "I/O Error: can't open file\n");
        exit(2);
    }

    uint32_t upgrade = 0, need_upgrade = 0;
    Computer computer;
    Reference reference;
    reference_read_txt(&reference, ref_txt);

    ListOwner *owner_print = list_create();


    while (computer_read_bin(&computer, in)) {
        upgrade = compare(&computer, &reference);
        if (upgrade) {
            ++need_upgrade;
            list_add(&owner_print, &computer);
        }
    }

    if (need_upgrade < atoi(argv[2])) {
        fprintf(stderr, "Error: too few computers need upgrade\n");
        exit(4);
    } else {
        list_print(&owner_print);
    }

    list_destroy(&owner_print);
    fclose(in);
    fclose(ref_txt);

    return 0;
}

