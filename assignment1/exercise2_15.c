#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 512

int main(int argc, char *argv[]) {
    ssize_t nread;
    char buf[SIZE];

    if (argc > 1) {
        // 인자가 존재하는 경우
        for (int i = 1; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("open failed");
            }else{
                while ((nread = read(fd, buf, SIZE)) > 0) {
                    write(1, buf, nread); // screen
                }
                close(fd);
            }            
        }
    } else {
        // 인자가 없는 경우
        while ((nread = read(0, buf, SIZE)) > 0) {
            write(1, buf, nread);
        }
    }

    exit(0);
}
