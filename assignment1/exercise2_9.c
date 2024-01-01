#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define NAMELENGTH 41
#define NROOMS 10

char namebuf[NAMELENGTH];
int fd = -1;
char* empty = "                                       \0\n";

// 투숙객 확인
char *getoccupier(int roomno) {
    off_t offset;
    ssize_t nread;

    if (fd == -1 && (fd = open("residents", O_RDONLY)) == -1) {
        return NULL;
    }
    offset = (roomno - 1) * NAMELENGTH;

    if (lseek(fd, offset, SEEK_SET) == -1) {
        return NULL;
    }

    if ((nread = read(fd, namebuf, NAMELENGTH)) <= 0) {
        return NULL;
    }

    namebuf[nread-2] = '\0';
    namebuf[nread-1] = '\n';
    return namebuf;
}

// 빈 방 찾기 (lowest)
int findfree() {
    for (int i = 1; i <= NROOMS; i++) {
        char *occupier = getoccupier(i);

        if (occupier == NULL) {
            fprintf(stderr, "error\n");
            continue;
        }

        if (strcmp(occupier, empty) == 0) {
            return i; // lowest
        }
    }
    return -1; // 빈 방 없음
}

int main() {
    int j;
    char *getoccupier(int), *p;

    for(j=1; j<=NROOMS; j++){
        if(p=getoccupier(j)){
            if (strcmp(p, empty) == 0) {
                 printf("Room %2d, is empty room.\n", j);
            }else{
                printf("Room %2d, %s\n",j,p);
            }            
        }else{
            printf("Error on room %d", j);
        }
    }

    int freeNum = findfree();
    if(freeNum==-1){
        printf("There are no rooms left.\n");
    }else{
        printf("lowest freeRoom number: %d\n", freeNum);
    }    

    return 0;
}
