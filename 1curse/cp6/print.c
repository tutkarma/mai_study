#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "structure.h"
#include "io.h"

void main(uint32_t argc, char *argv[]) 
{
    FILE *in = fopen(argv[1], "r");

    if (argc != 2) {
        fprintf(stdin, "Usage:\n\t./print DB_FILE\n");
        exit(1);
    }
    if (!in) {
        fprintf(stderr, "I/O Error: can't open file.\n");
        exit(2);
    }

    Computer computer;

    while (computer_read_bin(&computer, in)) {
        computer_print(&computer);
    }

    fclose(in);
}