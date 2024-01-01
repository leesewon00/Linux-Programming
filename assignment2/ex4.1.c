#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int my_double_ls(const char *name, int flag)
{
    struct dirent *d;
    DIR *dp;
    if ((dp = opendir(name)) == NULL)
    {
        return (-1);
    }
    for (int i = 0; i < 2; i++)
    {
        while ((d = readdir(dp)))
        {
            if (d->d_ino != 0)
            {   
                if (flag == 1 && d->d_name[0] == '.')
                {
                    continue;
                }
                printf("%s\n", d->d_name);
            }
        }
        rewinddir(dp);
    }
    closedir(dp);
    return (0);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return 1;
    }
    const char *dir_name = argv[1];
    int flag = atoi(argv[2]);

    if (my_double_ls(dir_name, flag) == -1)
    {
        return 1;
    }
    return 0;
}
