#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#define DEC_SIZE 40
#define NUMBER_SIZE 32
#define FILE_SIZE 100

typedef unsigned __int128 int128_t;

void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

typedef struct _params {
    long long nc;
    int128_t localsum;
    unsigned int counter;
    off_t start_pos;
    long long cc; //thread count controller
} Params;

typedef struct _command {
    int threads_num;
    int memory_set;
} Command;

const char *file_name = "test.dat";
void init(Params *ptr, Command *command, long long num_count);
void generate();
void parse_commdand_line(int argc, char **argv, Command *command);
void print_int128(int128_t u128);


int main(int argc, char **argv)
{
    Command command;
    parse_commdand_line(argc, argv, &command);

    if (command.threads_num * sizeof(Params) + command.threads_num * sizeof(pthread_t) > command.memory_set) {
        fprintf(stderr, "%s\n", "Too much threads for this amount of memory");
        exit(EXIT_FAILURE);
    }
    pthread_t *thread_id = (pthread_t *) malloc(command.threads_num * sizeof(pthread_t));

    Params *params = (Params *) malloc(command.threads_num * sizeof(Params));

    int i, j;

    generate();
    int fd = open(file_name, O_RDWR| O_CREAT, 0666);
    long long size = lseek(fd, 0, SEEK_END);
    close(fd);
    long long num_count = size / (NUMBER_SIZE + 1); //+1 for /n
    init(params, &command, num_count);
    for (i = 0; i < command.threads_num; ++i) {
        pthread_create(&thread_id[i], NULL, thread_function, (void *) &params[i]); //last null is param
    }

    for (j = 0; j < command.threads_num; ++j) {
        pthread_join(thread_id[j], NULL); 
    }

    int128_t sum = 0;
    for (int i = 0; i < command.threads_num; ++i) {
        sum += params[i].localsum;
    }
    print_int128(sum);
    return 0;
}


void init(Params *ptr, Command *command, long long num_count)
{
    ptr[0].nc = num_count;
    ptr[0].localsum = 0;
    ptr[0].counter = 0;
    ptr[0].start_pos = 0;
    ptr[0].cc = (num_count / command->threads_num);
    for (int i = 1; i < command->threads_num; ++i) {
        ptr[i].nc = num_count;
        ptr[i].localsum = 0;
        ptr[i].counter = 0;
        ptr[i].start_pos = i * (ptr[i - 1].cc * (NUMBER_SIZE + 1));
        ptr[i].cc = ptr[i-1].cc;
    }
    ptr[command->threads_num - 1].cc += num_count % command->threads_num;
}

void generate()
{
    int fd = open(file_name, O_RDWR| O_CREAT, 0666);
    char buf[NUMBER_SIZE];
    srand(time(NULL));
    for (int i = 0; i < FILE_SIZE; ++i) {
        for (int i = 0; i < NUMBER_SIZE; ++i) {
            if (((int) rand()) % 2 == 0) {
                buf[i] = '0' + (((int) rand()) % 10);
            } else {
                buf[i] = 'A' + (((int) rand()) % 6);
            }
        }
        write(fd, &buf, NUMBER_SIZE);
        write(fd, "\n", 1);
    }
    close(fd);
}

void print_int128(int128_t u128)
{
    printf("DEC: \n");
    char buf[DEC_SIZE + 1];
    int i;
    for (i = 0; i < DEC_SIZE; ++i) {
        buf[i] = '0';
    }
    buf[DEC_SIZE] = '\0';
    for (i = DEC_SIZE - 1; u128 > 0; --i) {
        buf[i] = (int) (u128 % 10) + '0';
        u128 /= 10;
    }
    if (i == DEC_SIZE - 1) {
        printf("%d\n", 0);
    } else {
        printf("%s\n", &buf[i + 1]);
    }
}

int is_num(char *s)
{
    return (*s >= '0' && *s <= '9');
}

int hex_to_dec(char *s)
{
    if(*s == 'A')
        return 10;
    if(*s == 'B')
        return 11;
    if(*s == 'C')
        return 12;
    if(*s == 'D')
        return 13;
    if(*s == 'E')
        return 14;
    if(*s == 'F')
        return 15;
    return 0;
}

int128_t atobigint(char *str)
{
    int128_t res = 0;
    while (*str) {
        if(is_num(str))
            res = res * 16 + (*str - '0');
        else {
            int kek = hex_to_dec(str);
            res = res * 16 + kek;
        }
        ++str; 
    }
    return res;
}


void *thread_function(void *dummyPtr)
{
    Params *ptr = (Params *)dummyPtr;
    char buf[NUMBER_SIZE + 1];
    char c;
    int fd = open(file_name, O_RDWR| O_CREAT, 0666);

    lseek(fd, ptr->start_pos, SEEK_SET);
    for (int i = 0; i < ptr->cc; ++i)
    {
        read(fd,buf,NUMBER_SIZE);
        buf[NUMBER_SIZE] = '\0'; //important
        int128_t s;
        s = atobigint(buf);
        s /= ptr->nc;
        ptr->localsum += s;
        read(fd, &c, 1);
        if (c != '\n' && c != '\0') {
            fprintf(stderr, "%s %d\n","c = ", c);
            fprintf(stderr, "%s %ld\n","start pos = ", ptr->start_pos);
            fprintf(stderr, "%s %lld\n","cc  = ", ptr->cc);
            fprintf(stderr, "%s\n", "format error");
            exit(EXIT_FAILURE);
        }
    }
    close(fd);
    return 0; //0 == NULL
}


void parse_commdand_line(int argc, char **argv, Command *command)
{
    if (argc != 3) {
        fprintf(stderr, "%s\n","Usage: ThreadsNumber RAM");
        exit(EXIT_FAILURE);
    }
    command->threads_num = atoi(argv[1]);
    command->memory_set = atoi(argv[2]);
}
