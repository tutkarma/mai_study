#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>

#include "TextProcessor.h"

int main(int argc, char *argv[])
{
    int32_t size, fd = 0;
    struct stat fileStat;

    if (argc < 2 || argc > 6) {
        handler(0);
    } else if (argc == 2) {
        if (!strcmp(argv[1], "-help")) {
            handler(0);
        } else if (!strcmp(argv[1], "-commands")) {
            handler(2); 
        } else {
            int32_t command, numStr;
            char opt[7], strToSearch[256], strToAdd[256], filename[256], oldStr[256], newStr[256];
            filename[0] = '\0';

            size = atoi(argv[1]);
            checkLimit(&size);

            do {
                menu(0);
                scanf("%d", &command);

                switch (command) {
                    case 0: {
                        menu(4);
                        if (fd) {
                            if (flock(fd, LOCK_UN)) {
                                handle_error("flock");
                            }
                            close(fd);
                        }

                        scanf("%255s", filename);
                        fd = open(filename, O_RDWR);
                        if (fd == -1) {
                            handle_error("open");
                        }
                        if (fstat(fd, &fileStat) == -1) {
                            handle_error("fstat");
                        }
                        if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                            if (errno == EWOULDBLOCK) {
                                printf("File is in usage\n");
                                exit(1);
                            } else {
                                handle_error("flock");
                            }
                        }
                        break;
                    }

                    case 1: {
                        menu(1);
                        scanf("%255s", strToSearch);
                        printf("Prefix/suffix/part?\n");
                        scanf("%7s", opt);
                        if (!strcmp(opt, "prefix")) {
                            searchPrefix(fd, fileStat.st_size, size, strToSearch, strlen(strToSearch));
                        } else if (!strcmp(opt, "suffix")) {
                            searchSuffix(fd, fileStat.st_size, size, strToSearch, strlen(strToSearch));
                        } else if (!strcmp(opt, "part")) {
                            searchPart(fd, fileStat.st_size, size, strToSearch, strlen(strToSearch));
                        } else {
                            printf("Unknown command.\n");
                        }
                        break;
                    }

                    case 2: {
                        menu(2);
                        scanf("%d", &numStr);
                        print(fd, fileStat.st_size, size, numStr);
                        break;
                    }

                    case 3: {
                        menu(3);
                        printf("Enter substring to replace: ");
                        scanf("%255s", oldStr);
                        printf("Enter substring which it will be replaced: ");
                        scanf("%255s", newStr);
                        replace(fd, fileStat.st_size, size, oldStr, strlen(oldStr), newStr, strlen(newStr));
                        if (fstat(fd, &fileStat) == -1) {
                            handle_error("fstat");
                        }
                        break;
                    }

                    case 4: {
                        getStats(fd, fileStat.st_size, size);
                        break;
                    }

                    case 5:
                        break;

                    default: {
                        menu(5);
                        break;
                    }
                }
            } while (command != 5);
        }
    } else if (argc == 4) {
        if (!strcmp(argv[3], "-stats")) {
            fd = open(argv[1], O_RDWR);
            if (fd == -1) {
                handle_error("open");
            }
            if (fstat(fd, &fileStat) == -1) {
                handle_error("fstat");
            }
            if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("File is in usage\n");
                    exit(1);
                } else {
                    handle_error("flock");
                }
            }

            size = atoi(argv[2]);
            checkLimit(&size);
            getStats(fd, fileStat.st_size, size);

            flock(fd, LOCK_UN);
        } else {
            printf("Unknown command.\n");
        }
    } else if (argc == 5) {
        if (!strcmp(argv[3], "-print")) {
            fd = open(argv[1], O_RDWR);
            if (fd == -1) {
                handle_error("open");
            }
            if (fstat(fd, &fileStat) == -1) {
                handle_error("fstat");
            }
            if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("File is in usage\n");
                    exit(1);
                } else {
                    handle_error("flock");
                }
            }

            int32_t numStr = atoi(argv[4]);
            size = atoi(argv[2]);
            checkLimit(&size);
            print(fd, fileStat.st_size, size, numStr);
            if (flock(fd, LOCK_UN)) {
                handle_error("flock");
            }
            close(fd);
        } else if (!strcmp(argv[3], "-replace")) {
            printf("File name: %s\n", argv[1]);
            fd = open(argv[1], O_RDWR);
            if (fd == -1) {
                handle_error("open");
            }
            if (fstat(fd, &fileStat) == -1) {
                handle_error("fstat");
            }
            if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("File is in usage\n");
                    exit(1);
                } else {
                    handle_error("flock");
                }
            }

            size = atoi(argv[2]);
            checkLimit(&size);
            char empty[0];
            replace(fd, fileStat.st_size, size, argv[4], strlen(argv[4]), empty, strlen(empty));
            if (flock(fd, LOCK_UN)) {
                handle_error("flock");
            }
            close(fd);
        } else {
            printf("Unknown command\n");
        }
    } else if (argc >= 6) {
        if (!strcmp(argv[3], "-search")) {
            fd = open(argv[1], O_RDWR);
            if (fd == -1) {
                handle_error("open");
            }
            if (fstat(fd, &fileStat) == -1) {
                handle_error("fstat");
            }
            if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("File is in usage\n");
                    exit(1);
                } else {
                    handle_error("flock");
                }
            }

            printf("File name: %s\n", argv[1]);
            printf("Search substring: %s\n", argv[4]);
            size = atoi(argv[2]);
            checkLimit(&size);
            if (!strcmp(argv[5], "part")) {
                searchPart(fd, fileStat.st_size, size, argv[4], strlen(argv[4]));
            } else if (!strcmp(argv[5], "prefix")) {
                searchPrefix(fd, fileStat.st_size, size, argv[4], strlen(argv[4]));
            } else if (!strcmp(argv[5], "suffix")) {
                searchSuffix(fd, fileStat.st_size, size, argv[4], strlen(argv[4]));
            } else {
                printf("Unknown command.\n");;
            }
            if (flock(fd, LOCK_UN)) {
                handle_error("flock");
            }
            close(fd);

        } else if (!strcmp(argv[3], "-replace")) {
            printf("File name: %s\n", argv[1]);
            fd = open(argv[1], O_RDWR);
            if (fd == -1) {
                handle_error("open");
            }
            if (fstat(fd, &fileStat) == -1) {
                handle_error("fstat");
            }
            if (flock(fd, LOCK_EX|LOCK_NB) == -1) {
                if (errno == EWOULDBLOCK) {
                    printf("File is in usage\n");
                    exit(1);
                } else {
                    handle_error("flock");
                }
            }

            size = atoi(argv[2]);
            checkLimit(&size);
            replace(fd, fileStat.st_size, size, argv[4], strlen(argv[4]), argv[5], strlen(argv[5]));
            if (flock(fd, LOCK_UN)) {
                handle_error("flock");
            }
            close(fd);
        } else {
            printf("Unknown command.\n");
        }

    } else {
        handler(1);
    }

    return 0;
}
