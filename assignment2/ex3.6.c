#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char* filename = "afile";

    // Check read access
    if (access(filename, R_OK) == -1) {
        perror("Read access error");
        exit(1);
    }
    printf("%s read proceeding\n", filename);

    // Check write access
    if (access(filename, W_OK) == -1) {
        perror("Write access error");
        exit(1);
    }
    printf("%s write proceeding\n", filename);

    // Check execute access
    if (access(filename, X_OK) == -1) {
        perror("Execute access error");
        exit(1);
    }
    printf("%s execute proceeding\n", filename);

    return 0;
}
