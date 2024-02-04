#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIFO_PATH "/tmp/hello_fifo"
#define BUFFER_SIZE 100

int main(void) {
    char str[] = "Hello world";
    char str1[BUFFER_SIZE];  // Буфер для чтения строки
    int arr[2];  // Массив для хранения дескрипторов канала
    int a;

    // Создаем именованный канал (FIFO)
    mkfifo(FIFO_PATH, 0666);

    // Создаем новый процесс
    a = fork();

    if (a > 0) {
        // Родительский процесс
        int fd_write = open(FIFO_PATH, O_WRONLY);
        if (fd_write == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Пишем данные в канал
        write(fd_write, str, sizeof(str));

        close(fd_write);  // Закрываем дескриптор записи
    }
    else {
        // Дочерний процесс
        int fd_read = open(FIFO_PATH, O_RDONLY);
        if (fd_read == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Читаем данные из канала
        ssize_t bytesRead = read(fd_read, str1, sizeof(str1));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(fd_read);  // Закрываем дескриптор чтения

        // Выводим прочитанную строку
        printf("\nReceived message in child process: %.*s\n", (int)bytesRead, str1);
    }

    // Удаляем FIFO
    unlink(FIFO_PATH);

    return 0;
}
