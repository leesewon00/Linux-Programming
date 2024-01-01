#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int fileopen(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);

    if (file == NULL) {
        perror("open failed");
        return -1; // error code -1
    }

    int fd = fileno(file);
    if (fd == -1) {
        perror("fd error");
        fclose(file);
        return -1; // error code -1
    }

    return fd; // return fd
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("need fileName and mode.\n");
        exit(1);
    }

    const char *filename = argv[1];
    const char *mode = argv[2];

    int fd = fileopen(filename, mode);
    if (fd != -1) {
        printf("successfully opened\nfile descriptor: %d\nmode: %s\n", fd, mode);
    } else {
        printf("open failed\nfile descriptor: %d\n", fd);
    }

    return 0;
}

