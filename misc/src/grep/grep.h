#ifndef HEADER
#define HEADER

typedef struct grep_options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char regular_f[10000];
  int regcomp_flag;
  char regular[10000];
} flags;

#endif