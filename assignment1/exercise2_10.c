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
char *getoccupier(int roomNumber) {
    off_t offset;
    ssize_t nread;

    offset = (roomNumber - 1) * NAMELENGTH;
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

// 비어있는 방인가
int isEmpty(int roomNumber){
    char *occupier = getoccupier(roomNumber);
    if (occupier == NULL) {
        fprintf(stderr, "error\n");
        return -1;
    }

    if (strcmp(occupier, empty) == 0) {
        return 1; // 1 means empty room
    }else{
        return -1;
    }
}

// 체크아웃
void freeroom(int roomNumber) {
    off_t offset;
    ssize_t nwritten;

    // already empty
    if(isEmpty(roomNumber)==1){
        printf("room %d is already empty\n",roomNumber);
        return;
    }

    offset = (roomNumber - 1) * NAMELENGTH;
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek failed");
        return;
    }

    // 체크아웃
    if ((nwritten = write(fd, empty, NAMELENGTH)) != NAMELENGTH) {
        perror("write failed");
        return;
    }

    printf("check out success room : %d\n", roomNumber);
    return; // check out success then return 1
}

// 체크인
void addguest(int roomNumber, char *guestName) {
    off_t offset;
    ssize_t nwritten;

    // not empty room case
    if(isEmpty(roomNumber)!=1){
        printf("room %d is not empty\n",roomNumber);
        return;
    }

    offset = (roomNumber - 1) * NAMELENGTH;
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek failed");
        return;
    }

    // 체크인
    if ((nwritten = write(fd, guestName, strlen(guestName))) != strlen(guestName)) {
        perror("write failed");
        return;
    }

    printf("check in success room : %d name : %s\n", roomNumber, guestName);
    return;
}


int main() {
    char *getoccupier(int), *p;
    char guestName[NAMELENGTH];
    int checkOut, checkIn;

    // file open in main
    if ((fd = open("residents", O_RDWR)) == -1) {
        perror("Failed to open residents file");
        return 1;
    }

    printf("Enter room number to check out: ");
    scanf("%d", &checkOut);
    freeroom(checkOut);

    printf("Enter room number and your name to check in: ");
    scanf("%d", &checkIn);
    scanf("%s", guestName);
    guestName[NAMELENGTH-2] = '\0';
    guestName[NAMELENGTH-1] = '\n';
    addguest(checkIn, guestName);

    return 0;
}
