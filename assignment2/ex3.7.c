#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int lsoct(char *perm)
{
    int tmp1, tmp2, tmp3;
    int oct = 0;

    if (perm[0] == 'r')
        oct += 400;
    if (perm[1] == 'w')
        oct += 200;
    if (perm[2] == 'x')
        oct += 100;
    if (perm[3] == 'r')
        oct += 40;
    if (perm[4] == 'w')
        oct += 20;
    if (perm[5] == 'x')
        oct += 10;
    if (perm[6] == 'r')
        oct += 4;
    if (perm[7] == 'w')
        oct += 2;
    if (perm[8] == 'x')
        oct += 1;

    tmp1 = oct % 10;
    oct /= 10;
    tmp2 = oct % 10;
    oct /= 10;
    tmp3 = oct % 10;

    int result = tmp3 * 8 * 8 + tmp2 * 8 + tmp1;
    return result; // %o
}

char *octls(int oct)
{
    char *perm = (char *)malloc(10 * sizeof(char));
    int i = 6;
    int left;

    while (i >= 0)
    {
        left = oct % 8;
        oct /= 8;

        switch (left)
        {
        case 0:
            perm[i] = perm[i + 1] = perm[i + 2] = '-';
            break;
        case 1:
            perm[i] = perm[i + 1] = '-'; perm[i + 2] = 'x';
            break;
        case 2:
            perm[i] = perm[i + 2] = '-'; perm[i + 1] = 'w';
            break;

        case 3:
            perm[i] = '-'; perm[i + 1] = 'w'; perm[i + 2] = 'x'; 
            break;
        case 4:
            perm[i] = 'r'; perm[i + 1] = perm[i + 2] = '-';
            break;
        case 5:
            perm[i] = 'r'; perm[i + 1] = '-'; perm[i + 2] = 'x';
            break;

        case 6:
            perm[i] = 'r'; perm[i + 1] = 'w'; perm[i + 2] = '-';
            break;
        case 7:
            perm[i] = 'r'; perm[i + 1] = 'w'; perm[i + 2] = 'x';
            break;

        default:
            return NULL;
        }
        
        i -= 3;
    }
    return perm;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return 1;
    }

    char *filename = argv[1];
    char *input_perm = argv[2];
    int perm_oct;

    if (input_perm[0] >= '0' && input_perm[0] <= '7')
    {
        perm_oct = strtol(input_perm, NULL, 8);
    }
    else
    {
        perm_oct = lsoct(input_perm);
    }

    struct stat file_stat;
    if (stat(filename, &file_stat) == -1)
    {
        perror("stat");
        return 1;
    }

    if (chmod(filename, perm_oct) == -1)
    {
        perror("chmod");
        return 1;
    }

    return 0;
}