#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000

void grep(const char* pattern, const char* file, bool ignoreCase, bool invertMatch, bool countMatch, bool printFileNames, bool printLineNumbers) {
    FILE* fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Cannot open file: %s\n", file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNum = 0;
    int matchCount = 0;
    bool matchFound = false;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNum++;

        if (ignoreCase) {
            char* lowercaseLine = strdup(line);
            strlwr(lowercaseLine);
            if (strstr(lowercaseLine, pattern) != NULL) {
                matchFound = true;
            }
            free(lowercaseLine);
        } else {
            if (strstr(line, pattern) != NULL) {
                matchFound = true;
            }
        }

        if (invertMatch) {
            matchFound = !matchFound;
        }

        if (matchFound) {
            matchCount++;

            if (printFileNames || printLineNumbers) {
                if (printFileNames) {
                    printf("%s: ", file);
                }
                if (printLineNumbers) {
                    printf("%d: ", lineNum);
                }
            }

            printf("%s", line);
        }
        
        matchFound = false;
    }

    if (countMatch) {
        printf("%s: %d\n", file, matchCount);
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    bool ignoreCase = false;
    bool invertMatch = false;
    bool countMatch = false;
    bool printFileNames = false;
    bool printLineNumbers = false;
    
    char* pattern = NULL;

    if (argc < 3) {
        printf("Usage: grep <pattern> <file> [<options>]\n");
        return 1;
    }

    pattern = argv[1];

    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            pattern = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0) {
            ignoreCase = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            invertMatch = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            countMatch = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            printFileNames = true;
        } else if (strcmp(argv[i], "-n") == 0) {
            printLineNumbers = true;
        }
        else {
            printf("Invalid option: %s\n", argv[i]);
            return 1;
        }
    }
    
    grep(pattern, argv[2], ignoreCase, invertMatch, countMatch, printFileNames, printLineNumbers);

    return 0;
}