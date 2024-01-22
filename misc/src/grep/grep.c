#include "grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parser(int argc, char *argv[], flags *options);
void output(int argc, char *argv[], flags *options, int optind);
void grep_output(FILE *file, char *search_str, char *name_file, flags *options);

int main(int argc, char *argv[]) {
  flags options = {0};
  options.regcomp_flag = REG_EXTENDED;
  output(argc, argv, &options, parser(argc, argv, &options));
  return 0;
}

int parser(int argc, char *argv[], flags *options) {
  int currentFlag;
  while ((currentFlag = getopt_long(argc, argv, "e:f:vicohsln", NULL, 0)) !=
         -1) {
    switch (currentFlag) {
      case ('e'):
        options->e = 1;
        strcat(options->regular, optarg);
        strcat(options->regular, "|");
        break;
      case ('i'):
        options->i = 1;
        break;
      case ('v'):
        options->v = 1;
        break;
      case ('c'):
        options->c = 1;
        break;
      case ('l'):
        options->l = 1;
        break;
      case ('n'):
        options->n = 1;
        break;
      case ('h'):
        options->h = 1;
        break;
      case ('s'):
        options->s = 1;
        break;
      case ('f'):
        options->f = 1;
        char buff[10000];
        size_t buff_size = 10000;
        FILE *file = fopen(optarg, "r+");
        while (fgets(buff, buff_size, file) != NULL) {
          if (buff[strlen(buff) - 1] == '\n') {
            buff[strlen(buff) - 1] = '\0';
          }
          strcat(options->regular, buff);
          strcat(options->regular, "|");
        }
        printf("%s", options->regular_f);  // не понято зачем
        fclose(file);
        break;
      case ('o'):
        options->o = 1;
        break;
    }
  }
  return (optind);
}

void output(int argc, char *argv[], flags *options, int optind) {
  char *search_str;
  if (options->e == 0 && options->f == 0) {
    search_str = argv[optind];
    optind++;
  } else {
    search_str = options->regular;
    search_str[strlen(search_str) - 1] = '\0';
  }
  if (argc - optind <= 1) {
    options->h = 1;
  }
  for (int j = optind; j < argc; j++) {
    FILE *file;
    char *name_file = argv[j];
    file = fopen(name_file, "r+");
    if ((file != NULL) || (options->s != 1)) {
      grep_output(file, search_str, argv[j], options);
    }
    fclose(file);
  }
}

void grep_output(FILE *file, char *search_str, char *name_file,
                 flags *options) {
  char buffer[10000];
  size_t buffer_size = 10000;
  int count_str = 0;
  int file_check = 0;
  int counter = 1;
  regex_t preg;
  regmatch_t pmatch;
  char last_of_chars = '\n';
  if (options->i == 1) {
    options->regcomp_flag += REG_ICASE;
  }
  regcomp(&preg, search_str, options->regcomp_flag);
  while (fgets(buffer, buffer_size, file) != NULL) {
    if (((regexec(&preg, buffer, 1, &pmatch, 0) == 0 && options->v == 0) ||
         (regexec(&preg, buffer, 1, &pmatch, 0) != 0 && options->v == 1))) {
      count_str++;
      last_of_chars = buffer[strlen(buffer) - 1];
      if (options->l == 0 && options->c == 0 && options->o == 0) {
        if (options->h == 0) {
          printf("%s:", name_file);
        }
        if (options->n == 1) {
          printf("%d:", counter);
        }
        printf("%s", buffer);
      } else if (options->c == 0 && options->o == 0) {
        file_check = 1;
      } else if (options->c == 0) {
        if (options->n == 1) {
          printf("%d:", counter);
        }
        char *o_string = &buffer[0];
        while (regexec(&preg, o_string, 1, &pmatch, 0) == 0) {
          printf("%.*s\n", (int)(pmatch.rm_eo - pmatch.rm_so),
                 &o_string[pmatch.rm_so]);
          o_string += (int)pmatch.rm_eo;
        }
      }
    }
    counter++;
  }
  if (file_check == 1) {
    printf("%s\n", name_file);
  }
  if (options->c == 1) {
    if (options->h == 0) {
      printf("%s:", name_file);
    }
    printf("%d\n", count_str);
  }
  if (last_of_chars != '\n' && options->c == 0 && options->l == 0 &&
      options->o == 0) {
    printf("\n");
  }
  regfree(&preg);
}