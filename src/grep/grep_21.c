
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
} Flags;

Flags GrepReadFlags(int argc, char *argv[]) {

    int currentFlag;
    Flags flags = {false, false, false, false, false, false};

    while ((currentFlag = getopt_long(argc, argv, "eivcln", NULL, 0)) != -1)
    {
        switch (currentFlag)
        {
        case 'e':
            flags.e = true;
            break;
        case 'i':
            flags.i = true;
            break;
        case 'v':
            flags.v = true;
            break;
        case 'c':
            flags.c = true;
            break;
        case 'l':
            flags.l = true;
            break;
        case 'n':
            flags.n = true;
            break;
        default:
            break;;
        }
        return flags;
    }
}

void output(char *argv[], Flags *flags) {
    FILE *file;
    char line[1024];
    int lineNumber = 1;
    int matches = 0;

    if (flags->e) {
        // Логика для обработки флага '-e'
        if (flags->i) {
            // Логика для регистронезависимого поиска
        }
        // Добавьте код, который использует флаг '-e'
    }

    if (flags->c) {
        // Логика для обработки флага '-c'
        // Сюда вставьте логику для подсчета количества соответствующих строк
    }

    if (flags->l) {
        // Логика для обработки флага '-l'
        // Сюда вставьте логику для вывода только имен файлов, содержимое которых соответствует заданному шаблону
    }

    if (flags->n) {
        printf("Файл: %s\n", argv[optind]);
        file = fopen(argv[optind], "r");
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, "your_pattern") != NULL) {
                printf("%d: %s", lineNumber, line);
                matches++;
            }
            lineNumber++;
        }
        printf("Общее количество соответствий: %d\n", matches);
        fclose(file);
    } else {
        file = fopen(argv[optind], "r");
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, "your_pattern") != NULL) {
                printf("%s", line);
                matches++;
            }
        }
        fclose(file);
    }
}




int main(int argc, char *argv[]) {

    Flags flags = GrepReadFlags(argc, argv);
    while (optind < argc) {
        output(argv, &flags);
        optind++;  // сдвигаем указатель на следующий аргумент
    }
    return 0;
}