#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    const char *filename = argv[1];
    struct stat file_stat;
    time_t last_time = 0;

    while (1)
    {
        if (stat(filename, &file_stat) == -1)
        {
            usleep(1000000);
            continue;
        }

        if (file_stat.st_mtime != last_time)
        {
            last_time = file_stat.st_mtime;
            
            FILE *file = fopen(filename, "rb");
            if (file)
            {
                int ch;
                while ((ch = fgetc(file)) != EOF)
                {
                    putchar(ch);
                }
                fclose(file);
            }
            fflush(stdout);
        }

        usleep(1000000);
    }

    return 0;
}
