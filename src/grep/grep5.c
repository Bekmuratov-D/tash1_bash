#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void print_usage() {
    printf("Usage: grep [options] template [file_name]\n");
    printf("Options:\n");
    printf("  -e  Use extended regular expression\n");
    printf("  -i  Ignore case distinctions\n");
    printf("  -v  Invert the match\n");
    printf("  -c  Print only a count of selected lines per FILE\n");
    printf("  -l  Print only the names of FILEs containing selected lines\n");
    printf("  -n  Print line number with output lines\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    bool use_regexp = false;
    bool ignore_case = false;
    bool invert_match = false;
    bool print_count = false;
    bool print_files = false;
    bool print_line_number = false;

    // Определение доступных флагов и их аргументов с помощью getopt_long
    // static struct option long_options[] = {
    //     {"regexp", no_argument, 0, 'e'},
    //     {"ignorecase", no_argument, 0, 'i'},
    //     {"invert", no_argument, 0, 'v'},
    //     {"count", no_argument, 0, 'c'},
    //     {"files", no_argument, 0, 'l'},
    //     {"line", no_argument, 0, 'n'},
    //     {0, 0, 0, 0}
    // };

    while ((opt = getopt_long(argc, argv, "eivcln", NULL, 0)) != -1) {
        switch (opt) {
            case 'e':
                use_regexp = true;
                break;
            case 'i':
                ignore_case = true;
                break;
            case 'v':
                invert_match = true;
                break;
            case 'c':
                print_count = true;
                break;
            case 'l':
                print_files = true;
                break;
            case 'n':
                print_line_number = true;
                break;
            default:
                print_usage();
                return 1;
        }
    }

    // Проверка наличия обязательного аргумента шаблона
    if (optind >= argc) {
        printf("Missing template argument\n");
        print_usage();
        return 1;
    }

    // Получение шаблона из аргументов командной строки
    char *template = argv[optind];
    optind++;

    // Чтение и обработка файлов, если они указаны
    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("Error opening file: %s\n", argv[i]);
                continue;
            }

            char line[MAX_LINE_LENGTH];
            int line_number = 1;
            int matches_count = 0;

            // Чтение каждой строки файла и проверка на соответствие шаблону
            while (fgets(line, sizeof(line), file)) {
                // Проверка на соответствие шаблону с учетом флагов
                bool match = false;
                if (use_regexp) {
                    // Реализация регулярных выражений
                    // (не входит в объем данной задачи)
                } else {
                    if (ignore_case) {
                        if (strcasecmp(line, template) != NULL) {
                            match = true;
                        }
                    } else {
                        if (strstr(line, template) != NULL) {
                            match = true;
                        }
                    }
                }

                // Обработка найденного соответствия
                if ((match && !invert_match) || (!match && invert_match)) {
                    if (print_files) {
                        printf("%s\n", argv[i]);
                        break;  // Прекращаем поиск соответствий в текущем файле, если нужно только имена файлов
                    }

                    if (print_count) {
                        matches_count++;  // Увеличиваем счетчик найденных соответствий
                    } else {
                        if (print_line_number) {
                            printf("%d:", line_number);  // Выводим номер строки, если нужно
                        }
                        printf("%s", line);
                    }
                }

                line_number++;
            }

            // Вывод количества найденных соответствий, если нужно
            if (print_count) {
                printf("%s: %d\n", argv[i], matches_count);
            }

            fclose(file);
        }
    } else {
        // Чтение стандартного ввода и обработка каждой строки
        char line[MAX_LINE_LENGTH];
        int line_number = 1;

        while (fgets(line, sizeof(line), stdin)) {
            // Проверка на соответствие шаблону с учетом флагов
            bool match = false;
            if (use_regexp) {
                // Реализация регулярных выражений
                // (не входит в объем данной задачи)
            } else {
                if (ignore_case) {
                    if (strcasestr(line, template) != NULL) {
                        match = true;
                    }
                } else {
                    if (strstr(line, template) != NULL) {
                        match = true;
                    }
                }
            }

            // Обработка найденного соответствия
            if ((match && !invert_match) || (!match && invert_match)) {
                if (print_line_number) {
                    printf("%d:", line_number);  // Выводим номер строки, если нужно
                }
                printf("%s", line);
            }

            line_number++;
        }
    }

    return 0;
}