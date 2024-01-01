#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    const char *filename = argv[1];

    if (access(filename, W_OK) != -1)
    {
        printf("You have write permission\n");
        return 0;
    }
    else
    {
        printf("You don't have write permission\n");

        char input;
        printf("Do you want to unlink this file?\n(yes = 1, / No = 0)\n");
        scanf(" %c", &input);

        if (input == '1')
        {
            if (unlink(filename) == -1)
            {
                perror("unlink");
                return 1;
            }
            return 0;
        }
        else
        {
            return 0;
        }
    }
}
