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

#define BUFSI 256
#define STRINGCUT 4

int main(void)
{
    //const int BUFSIZ = 256;
    const char *testSting = "easyeasyeasy";
    char buf[BUFSI];
    char bufTrunc[BUFSI];
    int fd;
    ssize_t rd, wr;

    //create file "test"
    fd = creat("test", O_RDWR); //S_IREAD | S_IWRITE
    if (fd == -1) {
        perror("Error opening test");
    } else {
        fprintf(stdout, "test opened for read/write access\n");
        fprintf(stdout, "test opened is empty\n");
    }

    if (fsync(fd) == -1) { //синхронизирует состояние файла в памяти с его состоянием на диске
        perror("Error fsync");
    }

    // write test string in file "test"
    if (wr = write(fd, testSting, strlen(testSting)) == -1) { /* записывает количество байтов в 3 аргументе из буфера в файл с дискриптором fd,
                                                                  возвращает количество записанных байтов или -1 в случае ошибки */
        perror("Error writing");
    }
    if (lseek(fd, 0, SEEK_SET) == -1) { // вернуться к началу файла
        perror("Error lseek");
    }

    if (close(fd) == -1) {
        perror("Error closing");
    }

    // read from file in buffer
    fd = open("test", O_RDWR);
    if (read(fd, buf, strlen(testSting)) == strlen(testSting)) {
        fprintf(stdout, "%s", buf);
        fprintf(stdout, " was written to test\n");
    } else {
        perror("Error reading"); 
    }

    if (close(fd) == -1) {
        perror("Error closing");
    }
    if (truncate("test", STRINGCUT) == -1) { // укорачивает файл до указанной длины, отсеченные данные теряются
        perror("Error truncate");
    }

    // open file for reading
    fd = open("test", O_RDONLY);
    rd = read(fd, bufTrunc, BUFSI);
    if (rd == -1) {
        if (errno == EINTR) { // системный вызов прерван сигналом
            fprintf(stdout, "EINTR\n");
        }
        perror("Error reading");
    }

    fprintf(stdout, "String after trunc: ");
    fprintf(stdout, "%s\n", bufTrunc);
    if (close(fd) == -1) {
        perror("Error closing");
    }


    //create new directory, called newdir, using fork() and exec()
    if (fork() !=  0) {
        wait((int *) 0); // нулевой указатель -- возвращаемое значение не сохранено
    } else {
        execl("/bin/mkdir", "mkdir", "newdir", (char *) NULL); // передаем аргументы в командную строку (когда их количество известно)
        perror("Error exec");
    }

    // use new directory
    if ((fd = open("newdir/newfile", O_RDWR | O_CREAT, 0644)) == -1) { /* первый аргумент -- путь до файла, 
                                                                         второй -- флаги открытия (здесь открыть файл в режиме чтения и записи,
                                                                         и автоматически создать файл, если такого не существует),
                                                                         третий -- если создается новый файл, то задать права доступа
                                                                         (0664 аналогично S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH 
                                                                         -- доступен в режиме чтение-запсь для пользователя и группы
                                                                         и только чтения для остальных пользователей) */
        perror("Error opening");
    }
    write(fd, "Hello, world\n", 14);
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