#include <stdio.h>
#include <string.h>

void print_nonblank_numbered(FILE *file) {
    int line_number = 1;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "\n") != 0) {
            printf("%6d\t%s", line_number++, line);
        } else {
            printf("%s", line);
        }
    }
}

void print_numbered(FILE *file) {
    int line_number = 1;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%6d\t%s", line_number++, line);
    }
}

void print_with_tabs_and_display_eol(FILE *file) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\t') {
            printf("^I");
        } else if (c == '\n') {
            printf("$\n");
        } else {
            putchar(c);
        }
    }
}

void squeeze_blank(FILE *file) {
    int previous_character = 0;
    int current_character;
    while ((current_character = fgetc(file)) != EOF) {
        if (current_character != '\n' || previous_character != '\n') {
            putchar(current_character);
        }
        previous_character = current_character;
    }
}

void cat_with_flags(char* file_name, int number_all_lines, int number_nonblank, int display_tabs, int display_nonprinting, int squeeze_blank) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "cat: %s: No such file or directory\n", file_name);
        return;
    }

    if (number_nonblank) {
        print_nonblank_numbered(file);
    } else if (number_all_lines) {
        print_numbered(file);
    } else if (display_tabs && display_nonprinting) {
        print_with_tabs_and_display_eol(file);
    } else if (squeeze_blank) {
        squeeze_blank(file);
    } else {
        int c;
        while ((c = fgetc(file)) != EOF) {
            putchar(c);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int i;
    int number_all_lines = 0;
    int number_nonblank = 0;
    int display_tabs = 0;
    int display_nonprinting = 0;
    int squeeze_blank = 0;

    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            number_all_lines = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            number_nonblank = 1;
        } else if (strcmp(argv[i], "-e") == 0) {
            display_nonprinting = 1;
            display_tabs = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            display_tabs = 1;
        } else if (strcmp(argv[i], "-s") == 0) {
            squeeze_blank = 1;
        }
    }

    for (; i < argc; i++) {
        cat_with_flags(argv[i], number_all_lines, number_nonblank, display_tabs, display_nonprinting, squeeze_blank);
    }

    return 0;
}