#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>


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
    int currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
    Flags flags = {false, false, false, false, false, false};
    for (; currentFlag != -1; currentFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL))
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
        }    

    }
    return flags;
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

int main(int argc, char *argv[]){
    if (argc == 1) {
        CatNoArg(STDIN_FILENO);
    }else{
        Flags flags = CatReadFlags(argc, argv);
        if (flags.numberNonBlank)
            printf("number nonblank lines\n");
        if (flags.markEndl)
            printf("mark endl\n");
        if (flags.numberAll)
            printf("number all lines\n");
        if(flags.squeeze)
            printf("squeeze");
        if (flags.tab)
            printf("tab\n");
        if(flags.printNonPrintable)
            printf("printNonPrintable\n");
    }
    return 0;
}