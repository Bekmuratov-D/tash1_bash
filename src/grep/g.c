#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
// #include <regex.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    bool e; //?
    bool i; //-
    bool v; //+
    bool c; //+ +cl + несколько файлов
    bool l; //+
    bool n; //+
    bool h; //
    bool s; //
    bool o; //
    bool f; //
} Flags;
Flags GrepFlags(int argc, char *argv[]);
void Output(int argc, char *argv[], Flags flags);
void print_usage() { printf("Usage: grep [options] template [file_name]\n");}
int main(int argc, char *argv[]) {
    Flags flags = GrepFlags ( argc, argv);
    while (optind < argc) {
        Output(argc, argv, flags);
        optind++;  // сдвигаем указатель на следующий аргумент
    } 
}
Flags GrepFlags(int argc, char *argv[]){
    int currentFlag;
    Flags flags = {false, false, false, false, false, false};
    while ((currentFlag = getopt_long(argc, argv, "eivclnhsfo", NULL, 0)) != -1){
       switch (currentFlag) {
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
        case 'h':
            flags.h = true;
            break;
        case 's':
            flags.s = true;
            break;
        case 'o':
            flags.o = true;
            break;
        case 'f':
            flags.f = true;
            break;
        default:
            print_usage();
        } 
    }
    return flags;
}
void Output(int argc, char *argv[], Flags flags){
    if (optind >= argc) {
        printf("Missing template argument\n");
        print_usage();
    };
    char *template = argv[optind];
    optind++;
    if (optind < argc) {
        for(int i = optind; i < argc; i++){
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("Error opening file: %s\n", argv[i]);
                continue;
            }
            char line[MAX_LINE_LENGTH];
            int line_number = 1;
            int matches_count = 0;
            // regex_t regex;
            // int regex_compiled = 0;
            // if (flags.e) {
            //     regex_compiled = regcomp(&regex, template, REG_EXTENDED);
            //     if (regex_compiled) {
            //         char regex_error[100];
            //         regerror(regex_compiled, &regex, regex_error, sizeof(regex_error));
            //         fprintf(stderr, "Regex compilation failed: %s\n", regex_error);
            //         return;
            //     }
            // }      
            bool match = false;    
            while (fgets(line, sizeof(line), file)){
                
                if (flags.e) {
                    // if (regexec(&regex, line, 0, NULL, 0) == 0) {
                    //     match = true;
                    // }
                } else {
                    if (flags.i) {
                        if (strcasecmp(line, template) != 0) {
                            match = true;
                        }
                    } else {
                        if (strstr(line, template) != NULL) {
                            match = true;
                        }
                    }
                }
                if ((match && !flags.v) || (!match && flags.v)) {
                    if(flags.h){
                        // Print the file name only once
                        printf("%s:\n", argv[i]);
                        flags.h = false;
                    }
                    if(flags.s){
                        
                    }
                    if(flags.f){
                        // Print only the matched file names
                        printf("%s\n", argv[i]);
                        break;
                    }
                    if(flags.o){
                        // Print only the matched pattern
                        printf("%s\n", template);
                        break;
                    }
                    if (flags.l){
                        printf("%s\n", argv[i]);
                        break;
                    }
                    if(flags.c){
                        matches_count++;
                    }else{
                        if(flags.n){
                            printf("%d:", line_number);
                        }
                        printf("%s", line);
                    }
                }
                
                line_number++;
            }
            // if (flags.e) {
            //     regfree(&regex);  // Освобождение выделенной регулярным выражением памяти
            // }
            if (flags.c) {
                printf("%s: %d\n", argv[i], matches_count);
            }
            if (flags.i && match) {
                printf("%s", line);
            }

            fclose(file);
        }
    }
}