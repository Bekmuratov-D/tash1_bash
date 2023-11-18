#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
typedef struct catOptions {
  int b;
  int e;
  int n;
  int s;
  int t;
} flags;

void parser(int args, char *argv[], flags *options);
void catFile(int argc, char *argv[], flags *options);
void CatNoArg(int fd);
// void func_n(FILE *file);
// void func_b(FILE *file);
// void func_s(FILE *file);
// void func_e(FILE *file);
// void func_t(FILE *file);

int main(int argc, char *argv[]) {
    flags options = {0};
    parser(argc, argv, &options);
    if(optind == argc){
        CatNoArg(STDIN_FILENO);
    }else{
        catFile(argc, argv, &options);
    }
    return (0);
}

void CatNoArg(int fd) {
    char buf[4096];
    int bytes_read;

    bytes_read = read(fd, buf, 4096); // Функция read() возвращает количество фактически прочитанных байт или -1, если произошла ошибка.
    while (bytes_read >= 0) {
        printf("%.*s", bytes_read, buf); // Формат %.*s позволяет указать максимальное количество символов, которые нужно вывести
        bytes_read = read(fd, buf, 4096);
    }
}

void parser(int argc, char *argv[], flags *options) {
  int buff;
  struct option longOption[] = {
    {"number", 0, 0, 'n'},
    {"number-nonblank", 0, 0, 'b'},
    {"squeeze-blank", 0, 0, 's'},
    {0, 0, 0, 0}
    };
  while ((buff = getopt_long(argc, argv, "benvstET", longOption, 0)) != -1) {
    switch (buff) {
      case ('b'):
        options->b = 1;
        break;
      case ('e'):
        options->e = 1;
        break;
      case ('n'):
        options->n = 1;
        break;
      case ('s'):
        options->s = 1;
        break;
      case ('t'):
        options->t = 1;
        break;
      case ('E'):
        options->e = 1;
        break;
      case ('T'):
        options->t = 1;
        break;
    }
  }
}

void catFile(int argc, char *argv[], flags *options) {
    for (int i = optind; i < argc; i++){
        FILE *file = fopen(argv[i], "r+");
        int buff = fgetc(file);
        if (options->n == 1) {
            int counter = 1;
            if (buff != EOF) {
                printf("%6d\t", counter);
                counter += 1;
            }
            while (buff != EOF) {
                printf("%c", buff);
                if (buff == '\n') {
                printf("%6d\t", counter);
                counter += 1;
                }
                buff = fgetc(file);
            }
            rewind(file);
        } else if (options->b == 1){
            char buff = fgetc(file);
            int counter = 1;
            char buff_of_buff = buff;

            if (buff != EOF && buff != '\n') {
                printf("%6d\t", counter);
                counter += 1;
            }
            while (buff != EOF) {
                if (buff != '\n' && buff_of_buff == '\n') {
                printf("%6d\t", counter);
                counter += 1;
                }
                printf("%c", buff);
                buff_of_buff = buff;
                buff = fgetc(file);
            }
            rewind(file);
        } else if (options->s == 1){
             char buff_of_buff = ' ';
            char buff_of_buff_of_buff = ' ';
            while (buff != EOF) {
                if (!(buff_of_buff == '\n' && buff == '\n' &&
                    buff_of_buff_of_buff == '\n')) {
                printf("%c", buff);
                }
                buff_of_buff_of_buff = buff_of_buff;
                buff_of_buff = buff;
                buff = fgetc(file);
            }
            rewind(file);
            
        } else if (options->e == 1){
            while (buff != EOF) {
                if ((buff < 32 && buff != '\n' && buff != '\t') || buff == 127) printf("^");
                if (buff > 127 && buff < 160) printf("M-^");
                if (buff > 159 && buff < 255) {
                printf("M-");
                buff = buff + 128;
                } else if ((buff < 32 || (buff > 126 && buff < 255)) && buff != '\n' &&
                        buff != '\t') {
                if (buff > 126 && buff < 160)
                    buff = buff - 64;
                else if (buff < 126)
                    buff = buff + 64;
                }
                if (buff == '\n') {
                printf("$");
                }
                printf("%c", buff);
                buff = fgetc(file);
            }
            rewind(file);
            
        } else if (options->t == 1){
             while (buff != EOF) {
                if ((buff < 32 && buff != '\n' && buff != '\t') || buff == 127) printf("^");
                if (buff > 127 && buff < 160) printf("M-^");
                if (buff > 159 && buff < 255) {
                printf("M-");
                buff = buff + 128;
                } else if ((buff < 32 || (buff > 126 && buff < 255)) && buff != '\n' &&
                        buff != '\t') {
                if (buff > 126 && buff < 160)
                    buff = buff - 64;
                else if (buff < 126)
                    buff = buff + 64;
                }
                if (buff == '\t') {
                printf("^|");
                } else {
                printf("%c", buff);
                }
                buff = fgetc(file);
            }
            rewind(file);
        } else {
            while (buff != EOF)
            {
                putc(buff, stdout);
                buff = fgetc(file);
            }
        }
        fclose(file);
    }
}
