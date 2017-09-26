#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h> // S_IREAD, S_IWRITE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> // options flags
#include <inttypes.h>
#include <errno.h>


int main(void)
{
    const int BUFSIZ = 256;
    const char *testSting = "easyeasyeasy";
    char buf[BUFSIZ];
    char bufTrunc[BUFSIZ];
    int fd;
    ssize_t rd, wr;


    //create file "test"
    fd = creat("test", S_IREAD | S_IWRITE);
    if (fd == -1) {
        perror("Error in opening test\n");
    } else {
        fprintf(stdout, "test opened for read/write access\n");
        fprintf(stdout, "test opened is empty\n");
    }

    if (fsync(fd) == -1) { //синхронизирует состояние файла в памяти с его состоянием на диске
        perror("Error: fsync\n");
    }

    // write test string in file "test"
    if (wr = write(fd, testSting, strlen(testSting)) == -1) { /* записывает количество байтов в 3 аргументе из буфера в файл с дискриптором fd,
                                                                  возвращает количество записанных байтов или -1 в случае ошибки */
        perror("Error: write\n");
    }
    lseek(fd, 0L, 0); // вернуться к началу файла

    // read from file in buffer
    if (read(fd, buf, sizeof(testSting)) == sizeof(testSting)) {
        fprintf(stdout, "%s", buf);
    } else {
        perror("Error reading test\n");
    }

    //close file
    if (close(fd) == -1) {
        perror("Error: close\n");
    }
    if (truncate("test", 2) == -1) { // укорачивает файл до указанной длины, отсеченные данные теряются
        perror("Error: truncate\n");
    }

    // open file for reading
    fd = open("test", O_RDONLY);
    rd = read(fd, bufTrunc, BUFSIZ);
    if (rd == -1) {
        if (errno == EINTR) { // системный вызов прерван сигналом
            fprintf(stdout, "EINTR\n");
        }
        perror("Error read\n");
    }

    fprintf(stdout, "String after trunc\n");
    fprintf(stdout, "%s", bufTrunc);
    if (close(fd) == -1) {
        perror("Error close\n");
    }


    //create new directory, called newdir, using fork() and exec()
    if (fork() !=  0) {
        wait((int *) 0); // нулевой указатель -- возвращаемое значение не сохранено
    } else {
        execl("./mkdir", "mkdir", "newdir", (char *) NULL); // передаем аргументы в командную строку (когда их количество известно)
        perror("Error exec\n");
    }

    // use new directory
    if ((fd = open("newdir/newfile", O_RDWR | O_CREAT, 0644)) == -1) { /* первый аргумент -- путь до файла, 
                                                                         второй -- флаги открытия (здесь открыть файл в режиме чтения и записи,
                                                                         и автоматически создать файл, если такого не существует),
                                                                         третий -- если создается новый файл, то задать права доступа
                                                                         (0664 аналогично S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH 
                                                                         -- доступен в режиме чтение-запсь для пользователя и группы
                                                                         и только чтения для остальных пользователей) */
        perror("Error open\n");
    }
    write(fd, "Hello, world\n", 14);
    close(fd);

    pid_t pid = fork();
    if (pid == 0) {
        fprintf(stdout, "It's child process, pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getpid());
        fprintf(stdout, "Parent's pid = \n");
        fprintf(stdout, "%ld\n", (intmax_t)getppid());
    } else if (pid > 0) {
        fprintf(stdout, "It's parent process, pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getpid());
        fprintf(stdout, "Parent's pid = ");
        fprintf(stdout, "%ld\n", (intmax_t)getppid());
        wait(NULL);
    } else if (pid == -1) {
        perror("Error: fork.\n");
    }


    return 0;
}