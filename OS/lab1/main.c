#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> // options flags
#include <inttypes.h>
#include <errno.h>

#define BUFSIZE 256
#define STRINGCUT 4

int main(void)
{
    const char *testSting = "easyeasyeasy";
    char buf[BUFSIZE];
    char bufTrunc[BUFSIZE];
    int fd;
    ssize_t rd, wr;

    //create file "test"
    fd = creat("test", O_RDWR); //S_IREAD | S_IWRITE
    if (fd == -1) {
        perror("Error creat");
    } else {
        fprintf(stdout, "test opened for read/write access\n");
        fprintf(stdout, "test opened is empty\n");
    }

    if (fsync(fd) == -1) {
        perror("Error fsync");
    }

    // write test string in file "test"
    if (wr = write(fd, testSting, strlen(testSting)) == -1) {
        perror("Error write");
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error lseek");
    }

    if (close(fd) == -1) {
        perror("Error close");
    }

    // read from file in buffer
    fd = open("test", O_RDWR);
    if (read(fd, buf, strlen(testSting)) == strlen(testSting)) {
        fprintf(stdout, "%s", buf);
        fprintf(stdout, " was written to test\n");
    } else {
        perror("Error read"); 
    }

    if (close(fd) == -1) {
        perror("Error close");
    }
    if (truncate("test", STRINGCUT) == -1) {
        perror("Error truncate");
    }

    // open file for reading
    fd = open("test", O_RDONLY);
    rd = read(fd, bufTrunc, BUFSIZE);
    if (rd == -1 && errno == EINTR) {
        perror("Error read");
    }

    fprintf(stdout, "String after trunc: ");
    fprintf(stdout, "%s\n", bufTrunc);
    if (close(fd) == -1) {
        perror("Error close");
    }


    //create new directory, called newdir, using fork() and exec()
    if (fork() !=  0) {
        wait(NULL);
    } else {
        execl("/bin/mkdir", "mkdir", "newdir", (char *) NULL);
        perror("Error exec");
    }

    // use new directory
    const char *msg = "I'm alive...\n";
    if ((fd = open("newdir/newfile", O_RDWR | O_CREAT, 0644)) == -1) { /* открыть файл в режиме чтения и записи,
                                                                         и автоматически создать файл, если такого не существует,
                                                                         если создается новый файл, то задать права доступа
                                                                         (0664 аналогично S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH 
                                                                         -- доступен в режиме чтение-запсь для пользователя и группы
                                                                         и только чтения для остальных пользователей) */
        perror("Error opening");
    }
    write(fd, msg, strlen(msg));
    close(fd);

    pid_t pid = fork();
    if (pid == 0) {
        fprintf(stdout, "It's child process, pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getpid());
        fprintf(stdout, "Parent's pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getppid());
    } else if (pid > 0) {
        fprintf(stdout, "It's parent process, pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getpid());
        fprintf(stdout, "Parent's pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getppid());
        wait(NULL);
    } else if (pid == -1) {
        perror("Error fork");
    }
    return 0;
}