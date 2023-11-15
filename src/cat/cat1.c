#include <stdio.h>

void print_file(char *file);
void parser(int argc, char **argv);

int main(int argc, char **argv)
{
    
    parser(argc, **argv);

    for(int i = 1; argc > i; i++){
        print_file(argv[i]);
        printf("\n");
    }
}

void print_file(char *file)
{
    FILE *f = fopen(file, "rt");
    if(f != NULL){
        int c = fgetc(f);
        while (c != EOF)
        {
            putc(c, stdout);
            c = fgetc(f);
        }
        fclose(f);
    }
}
