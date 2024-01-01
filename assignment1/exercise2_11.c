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
    int userInput;

    // file open in main
    if ((fd = open("residents", O_RDWR)) == -1) {
        perror("Failed to open residents file");
        return 1;
    }

    while (1) {
        printf("\nfrontdesk start\n");
        printf("1. exit\n");
        printf("2. room list\n3. find fastest empty room\n");
        printf("4. checkIn\n5. checkout\n");
        printf("\nEnter number: ");

        char input[100];
        int i = 0;
        while (1) {
            char c = getchar();
            if (c == '\n') {
                input[i] = '\0';
                break;
            }

            input[i] = c;
            i++;

            if (i >= sizeof(input) - 1) {
                printf("invalid input. try next time.\n");
                return 1;
            }
        }
        userInput = atoi(input);   

        switch (userInput) {
            case 1:
                return 0;
            case 2:
                for(int j=1; j<=NROOMS; j++){
                    if(p=getoccupier(j)){
                        if (strcmp(p, empty) == 0) {
                            printf("Room %2d, is empty room.\n", j);
                        }
                        else{
                            printf("Room %2d, %s\n",j,p);
                        }            
                    }
                    else{
                        printf("Error on room %d", j);
                    }
                }
                break;
            case 3:
                if(findfree()==-1){
                    printf("no room left\n");
                }
                else{
                    printf("fastest room num : %d\n",findfree());
                }
                break;
            case 4:
                printf("Enter room number and guest name to check in: ");
                scanf("%d", &checkIn);
                scanf("%s", guestName);
                guestName[NAMELENGTH-2] = '\0';
                guestName[NAMELENGTH-1] = '\n';
                addguest(checkIn, guestName);
                getchar();
                break;
            case 5:
                printf("Enter room number to check out: ");
                scanf("%d", &checkOut);
                freeroom(checkOut);
                getchar();
                break;
            default:
                printf("invalid input. try again.\n");
        }
    }

    return 0;
}
