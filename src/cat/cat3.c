#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void CatNoArg(int fd) {
    char buf[4096];
    int bytes_read;

    bytes_read = read(fd, buf, 4096); // Функция read() возвращает количество фактически прочитанных байт или -1, если произошла ошибка.
    while (bytes_read >= 0) {
        printf("%.*s", bytes_read, buf); // Формат %.*s позволяет указать максимальное количество символов, которые нужно вывести
        bytes_read = read(fd, buf, 4096);
    }
}

void CatArgs(int argc, char *argv[]) {
    CatNoArg(open(argv[1], O_RDONLY));
}


int main(int argc, char *argv[]) {
    if (argc == 1)
        CatNoArg(STDIN_FILENO);
    else
        CatArgs(argc, argv);
    return 0;
}