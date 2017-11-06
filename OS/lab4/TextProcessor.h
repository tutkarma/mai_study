#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

#define handle_error(msg)                             \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void handler(const int32_t derp);
void menu(const int32_t act);
void searchPrefix(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize);
void searchSuffix(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize);
void searchPart(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize);
void checkLimit(int *userLimit);
void print(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit, const int32_t line);
void replace(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *oldString, const int32_t sizeOldStr, char *newString, const int32_t sizeNewStr);
void getStats(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit);

#endif