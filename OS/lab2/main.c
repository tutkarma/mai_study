#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


//Parent: reads from P1_READ, writes on P1_WRITE
//Child: reads from P2_READ, writes on P2_WRITE
#define P1_READ    0
#define P2_WRITE   1

int fd[2];

int Fib(int n)
{
    pid_t pid1, pid2;
    int buf1, buf2, res, status, bufread1, bufread2;

    if (n == 0) {
        return 0;
    } else if (n == 1 || n == 2) {
        return 1;
    }

    pid1 = fork();
    if (pid1 == 0) { // child process 1 (fib(n-1))
        buf1 = Fib(n - 1);
        close(fd[P1_READ]);
        if(write(fd[P2_WRITE], &buf1, sizeof(buf1)) == -1) {
            perror("write");
        }
        exit(0);
    } else if (pid1 < 0) {
        perror("fork");
    } else if (pid1 > 0) {
        pid2 = fork();
        if (pid2 == 0) { // child process 2 (fib(n-2))
            buf2 = Fib(n - 2);
            close(fd[P1_READ]);
            if(write(fd[P2_WRITE], &buf2, sizeof(buf2)) == -1) {
                perror("write");
            }
            exit(0);
        } else if (pid2 < 0) {
            perror("fork");
        }
    }

    if (waitpid(pid1, &status, 0) == -1) {
        perror("waitpid");
    }
    if (waitpid(pid2, &status, 0) == -1) {
        perror("waitpid");
    }

    if(read(fd[P1_READ], &bufread1, sizeof(bufread1)) == -1) {
        perror("read");
    }
    if(read(fd[P1_READ], &bufread2, sizeof(bufread2)) == -1) {
        perror("read");
    }
    res = bufread1 + bufread2;

    return res;
}

int main(void)
{
    int n = 0;

    if (pipe(fd) == -1) {
        perror("pipe");
    }

    printf("Enter a sequence number: ");
    scanf("%d", &n);
    if (n < 0) {
        printf("Number must be > 0.\n");
    } else {
        printf("%d\n", Fib(n));
    }

    return 0;
}