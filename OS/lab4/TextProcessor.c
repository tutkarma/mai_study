#include "TextProcessor.h"

void handler(const int32_t derp)
{
    if (derp == 0) {
        printf("use '/PathToFile(if using command) RAMLimitInBytes Command(optional)'\n");
        printf("use '-help' to show this message\n");
        printf("use '-commands' to show commands and parameters\n");
    } else if (derp == 1) {
        printf("Unknown command.\n");
        printf("Use:\n");
        printf("-stats\n");
        printf("-print NumberString\n");
        printf("-search SubString prefix/suffix/part\n");
        printf("-replace OldSubString NewSubString (maybe empty)\n");
    } else if (derp == 2) {
        printf("Use:\n");
        printf("-stats\n");
        printf("-print NumberString\n");
        printf("-search SubString prefix/suffix/part\n");
        printf("-replace OldSubString NewSubString \n");
    }

    exit(0);
}

void menu(const int32_t act)
{
    switch (act) {
        case 0: {
            printf("Avaliable commands:\n");
            printf("0) Choose file\n");
            printf("1) Search substring\n");
            printf("2) Print string\n");
            printf("3) Replace substring\n");
            printf("4) Show stats\n");
            printf("5) Exit\n");
            break;
        }
        case 1: {
            printf("Enter substring to search: ");
            break;
        }
        case 2: {
            printf("Enter number string to show: ");
            break;
        }
        case 3: {
            break;
        }
        case 4: {
            printf("Enter filename: ");
        break;
        }
        case 5: {
            printf("Unknown command.\n");
            break;
        }
    }
}

void searchPart(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize)
{
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, index = 0, bytepos = 1;
    char *addr;

    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int i = 0; i < size; ++i, ++bytepos) {
            if (addr[i] == subString[index]) {
                ++index;
                for (int32_t j = i + 1; (index < strSize) && (j < size); ++index, ++j) {
                    if (subString[index] != addr[j] || isspace(addr[j])) {
                        index = 0;
                        break;
                    }
                }

                if (index == strSize) {
                    printf("First entry was found at %d byte\n", bytepos);
                    return;
                }
            }
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }
        offset += size;
    }
    printf("There is no entries in file\n");
}

void searchPrefix(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize)
{
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, index = 0, bytepos = 1;
    char *addr;

    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        int prev = '\n';
        for (int i = 0; i < size; ++i, ++bytepos) {
            if (addr[i] == subString[index] && isspace(prev)) {
                ++index;
                for (int32_t j = i + 1; (index < strSize) && (j < size); ++index, ++j) {
                    if (subString[index] != addr[j] || isspace(addr[j])) {
                        index = 0;
                        break;
                    }
                }

                if (index == strSize) {
                    printf("First entry prefix was found at %d byte\n", bytepos);
                    return;
                }
            }
            prev = addr[i];
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }
        offset += size;
    }
    printf("There is no entries in file\n");
}


void searchSuffix(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *subString, const int32_t strSize)
{
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, index = 0, bytepos = 1;
    char *addr;
    bool found = false;

    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        char next = '\n';

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            index = 0;
            if (addr[i] == subString[index]) {
                ++index;
                for (int32_t j = i + 1; (index < strSize) && (j < size); ++index, ++j) {
                    if (j + 1 < size) {
                        next = addr[j + 1];
                    } else {
                        next = '\n';
                    }
                    if (subString[index] != addr[j] || isspace(addr[j])) {
                        index = 0;
                        break;
                    }
                }

                if (index == strSize && isspace(next)) {
                    printf("First entry suffix was found at %d byte\n", bytepos);
                    found = true;
                    break;
                }
            }
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }
        offset += size;
    }
    if (!found) {
        printf("There is no entries in file\n");
    }
}

void checkLimit(int *userLimit)
{
    int32_t pageSize = sysconf(_SC_PAGESIZE);

    if (*userLimit < pageSize) {
        printf("Your limit is less than page size\n");
        printf("Mmap limit will be set to page size - %d\n", pageSize);
        *userLimit = pageSize;
    } else if ((*userLimit % pageSize) != 0) {
        int32_t count = 0;
        while (*userLimit > pageSize) {
            *userLimit -= pageSize;
            ++count;
        }

        *userLimit = pageSize * count;
    }
}

void print(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit, const int32_t line)
{
    if (line < 0) {
        printf("Number string must be > 0.\n");
        return;
    }

    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, lineCnt = 1;
    char *addr;
    bool printed = false;

    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i) {
            if (line == lineCnt) {
                putchar(addr[i]);
                printed = true;
            }
            if (addr[i] == '\n') {
                ++lineCnt;
            }
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }

        if (printed) {
            return;
        }

        offset += size;
    }
    if (!printed) {
        --lineCnt;
        printf("Wrong number string. Number of strings: %d\n", lineCnt);
        return;
    }
}


void getStats(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit)
{
    char *addr;
    int32_t offset = 0, bytepos = 1, lines = 1;
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;

    while(offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if (addr[i] == '\n') {
                ++lines;
            }
        }

        if (munmap(addr, RAMLimit) == -1) {
            handle_error("munmap");
        }

        offset += RAMLimit;
    }

    if (lines > 1) {
        --lines;
    }

    printf("Amount of symbols - %d\nAmount of lines - %d\n", fileSize, lines);
}

int32_t removeOldString(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit, const int32_t from, const int32_t to)
{
    if ((from < 0) || (to < 0) || (from > to) || (to - 1 > fileSize)) {
        printf("Incorrect bounds.\n");
        return 0;
    }
    int32_t diff = (fileSize == to - 1) ? 1 : fileSize - to;
    char *old = (char *) malloc(sizeof(char) * diff);
    if (old == NULL) {
        handle_error("malloc-");
    }
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, bytepos = 1, k = 0;
    char *addr;

    while (offset < fileSize) {
        addr = (char *) mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if (bytepos >= to) {
                old[k] = addr[i];
                ++k;
            }
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }

        offset += size;
    }

    int32_t newFileSize = fileSize - to + from;
    if (ftruncate(fd, newFileSize)) {
        handle_error("ftruncate-");
    }

    bytepos = 1, offset = 0, k = 0;
    size = (RAMLimit > newFileSize) ? newFileSize : RAMLimit;

    while(offset < newFileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if (bytepos >= from) {
                addr[i] = old[k];
                ++k;
            }
        }

        if (msync(addr, size, MS_SYNC) == -1) {
            handle_error("msync");
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }

        offset += size;
    }

    free(old);
    return newFileSize;
}

void insertNewString(const int32_t fd, const int32_t fileSize, const int32_t RAMLimit, char *string, const int32_t from, const int32_t to)
{
    if ((from < 0) || (to < 0) || (from > to)) {
        printf("Incorrect bounds.\n");
        return;
    }

    char *old = (char*) malloc(sizeof(char) * (fileSize - from));
    if (old == NULL) {
        handle_error("malloc+");
    }
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, bytepos = 1, k = 0;
    char *addr;

    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if (bytepos >= from) {
                old[k] = addr[i];
                ++k;
            }
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }

        offset += size;
    }

    int32_t newFileSize = fileSize + to - from;
    if (ftruncate(fd, newFileSize)) {
        handle_error("ftruncate+");
    }

    int32_t l = 0;
    bytepos = 1, offset = 0, k = 0;
    size = (RAMLimit > newFileSize) ? newFileSize : RAMLimit;

    while (offset < newFileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if ((bytepos >= from) && (bytepos < to)) {
                addr[i] = string[l];
                ++l;
            }

            if (bytepos >= to) {
                addr[i] = old[k];
                ++k;
            }
        }

        if (msync(addr, size, MS_SYNC) == -1) {
            handle_error("msync");
        }

        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }

        offset += size;
    }

    free(old);
}



void replace(const int32_t fd, const int32_t fileSize,  const int32_t RAMLimit, char *oldString,
             const int32_t sizeOldStr, char *newString, const int32_t sizeNewStr)
{
    int32_t size = (RAMLimit > fileSize) ? fileSize : RAMLimit;
    int32_t offset = 0, index = 0, bytepos = 1, from = 0, to = 0;
    char *addr;
    bool found = false;


    while (offset < fileSize) {
        addr = (char *)mmap(NULL, (size_t)size, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }

        for (int32_t i = 0; i < size; ++i, ++bytepos) {
            if (addr[i] == oldString[index]) {
                ++index;
                for (int32_t j = i + 1; (index < sizeOldStr) && (j < size); ++index, ++j) {
                    if (oldString[index] != addr[j] || addr[j] == '\n') {
                        index = 0;
                        break;
                    }
                }

                if (index == sizeOldStr) {
                    from = bytepos;
                    found = true;
                    index = 0;
                    break;
                }
            }
        }
        if (munmap(addr, size) == -1) {
            handle_error("munmap");
        }
        offset += size;
    }


    if (!found) {
        printf("This substring no entries in file.\n");
        return;
    }

    int32_t newFileSize = removeOldString(fd, fileSize, RAMLimit, from, from + sizeOldStr);
    insertNewString(fd, newFileSize, RAMLimit, newString, from, from + sizeNewStr);
}