
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>


void Usage();

typedef struct {
    bool numberNonBlank;
    bool markEndl;
    bool numberAll;
    bool squeeze;
    bool tab;
    bool printNonPrintable;
} Flags;

Flags CatReadFlags(int argc, char *argv[]){
    struct option longOptions[] = {
        {"number-nonblank", 0, NULL, 'b'},
        {"number", 0, NULL, 'n'},
        {"squeeze-blank", 0, NULL, 's'},
        {NULL, 0, NULL, 0},
    };
    int currentFlag;
    Flags flags = {false, false, false, false, false, false};
    while ((currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) != -1)
    {
        switch (currentFlag)
        {
        case 'b':
            flags.numberNonBlank = true;
            break;
        case 'e':
        case 'v':
            flags.printNonPrintable = true;
            break;
        case 'E':
            flags.markEndl = true;
            break;
        case 'n':
            flags.numberAll = true;
            break;
        case 's':
            flags.squeeze = true;
            break;
        case 't':
            flags.tab = true;
            break;
        case 'T':
            flags.tab = true;
            break;
        default:
            Usage();
        }
    }
    return flags;
}

void Usage() {
    printf("usage: cat [-benstuv] [file ...]");
}

void CatFile(int fd, Flags flags) {
    char buf[4096];
    ssize_t bytes_read;
    
    int line_number = 1;  // начальный номер строки
    bool prev_blank = true;  // Маркер для сжатия пустых строк

    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (flags.squeeze && prev_blank) {
                if (buf[i] == '\n') {
                    continue;  // пропускаем символы новой строки, чтобы сжать пустые строки
                }
                prev_blank = false;  // сбрасываем маркер, когда встречаем непустой символ
            }
            
            if (flags.tab && buf[i] == '\t') {
                printf("^I");  // заменяем символ табуляции на ^I
            } else if (flags.markEndl && buf[i] == '\n') {
                printf("$ \n");  // выводим конец строки как $
            } else if (flags.numberAll && line_number == 1) {
                printf("%d  ", line_number++);
            } else if (flags.numberAll && buf[i] == '\n' && !prev_blank) {
                printf("%d  ", line_number++);
            } else if (flags.numberAll && buf[i] == '\n' && !prev_blank) {
                printf("%d  ", line_number++);  // номеруем все строки
            } else if (flags.numberNonBlank && buf[i] != '\n') {
                printf("%6d  ", line_number++);  // номеруем только непустые строки
            } else {
                putchar(buf[i]);  // выводим символ как есть
            }

            if (buf[i] == '\n') {
                prev_blank = true;  // устанавливаем маркер, если встречаем символ новой строки
            }
        }
    }
}

int main(int argc, char *argv[]){
    Flags flags = CatReadFlags(argc, argv);

    if (optind == argc) {
        // Если отсутствуют аргументы, считывайте ввод с stdin
        CatFile(STDIN_FILENO, flags);
    } else {
        // Иначе, читайте каждый указанный файл
        for (int i = optind; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror(argv[i]);
                continue;
            }
            CatFile(fd, flags);
            close(fd);
        }
    }
    
    return 0;
}
