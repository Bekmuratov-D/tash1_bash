#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

int search_file(const char *filename, const char *pattern, int ignore_case, int invert_match, int count_match, int print_lines, int line_numbers) {
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    int match_count = 0;
    int line_number = 0;

    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 1;
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        line_number++;
        int match = 0;
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (ignore_case) {
            char *ptr1 = line;
            char *ptr2 = pattern;
            while (*ptr1 && *ptr2) {
                if (tolower(*ptr1) != tolower(*ptr2)) {
                    break;
                }
                ptr1++;
                ptr2++;
            }
            if (!(*ptr2))
                match = 1;
        } else {
            if (strstr(line, pattern) != NULL)
                match = 1;
        }

        if (invert_match)
            match = !match;

        if (match) {
            match_count++;
            if (count_match)
                continue;
            
            if (print_lines) {
                if (line_numbers)
                    printf("%s:%d:%s\n", filename, line_number, line);
                else
                    printf("%s:%s\n", filename, line);
            }
        }
    }

    fclose(file);
    
    if (count_match)
        printf("%s: %d\n", filename, match_count);
    
    return 0;
}

int main(int argc, char *argv[]) {
    int ignore_case = 0;
    int invert_match = 0;
    int count_match = 0;
    int print_lines = 1;
    int line_numbers = 0;
    const char *pattern = NULL;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            if ((i + 1) < argc) {
                pattern = argv[i + 1];
                i++;
            } else {
                printf("Invalid usage: -e requires an argument\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            ignore_case = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            invert_match = 1;
        } else if (strcmp(argv[i], "-c") == 0) {
            count_match = 1;
            print_lines = 0;
        } else if (strcmp(argv[i], "-l") == 0) {
            print_lines = 0;
        } else if (strcmp(argv[i], "-n") == 0) {
            line_numbers = 1;
        } else {
            search_file(argv[i], pattern, ignore_case, invert_match, count_match, print_lines, line_numbers);
        }
    }

    return 0;
}
