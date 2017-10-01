#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

//function to return exit code for PID
static int ExitCode(pid_t pid)
{
    pid_t retpid;
    int status;

    retpid = waitpid(pid, &status, 0);
    if (pid != retpid) {
        fprintf(stderr, "Error waitpid\n");
    }

    return WEXITSTATUS(status);
}

int Fib(size_t n)
{
    pid_t pid1, pid2;
    int status1, status2;

    switch(n) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 1;
        default: break;
    }

    pid1 = fork();
    if (pid1 == 0) { // child process 1 (fib(n-1))
        exit(Fib(n - 1));
    } else if (pid1 < 0) {
        perror("fork");
    }

    pid2 = fork();
    if (pid2 == 0) { // child process 2 (fib(n-2))
        exit(Fib(n - 2));
    } else if(pid2 < 0) {
        perror("fork");
    }

    //get value from child process 1
    status1 = ExitCode(pid1);
    //get value from child process 2
    status2 = ExitCode(pid2);

    return status1 + status2;
}

int main(void)
{
    int n = 0;
    printf("Enter a sequence number: ");
    scanf("%d", &n);
    if (n < 0) {
        printf("Number must be > 0.\n");
    } else {
        printf("%d\n", Fib(n));
    }
    return 0;
}