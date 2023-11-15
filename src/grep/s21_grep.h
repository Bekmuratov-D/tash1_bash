#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 10000

typedef struct {
    int e; //+
    int i; //+
    int v; //+
    int c; //+ +cl + несколько файлов
    int l; //+
    int n; //+ +несколько файлов
    int h; //+
    int s; //+
    int o; //+
    int f; //+
    int c_flag; // if file>1
    char str[BUFFER];
    char str_o[BUFFER]; // строка шаблонов for o
    char filename[BUFFER];
    char str_regexec[BUFFER]; // строка шаблонов 
} flags;

void parser(int argc, char *argv[], flags *A);
void output(char *argv[], flags *A);
void f_flag(flags *A);

#endif //S21_GREP_H_  // S21_GREP_H_