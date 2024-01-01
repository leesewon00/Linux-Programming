#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "test";
    int fd;

    // creat without calling close
    fd = creat(filename, 0644);
    if (fd == -1) {
        printf("creat error");
        exit(1);
    }
    printf("successfully creat\n");

    // immediately open
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("open error");
        exit(1);
    }
    printf("successfully open\n");

    close(fd);
    return 0;
}

