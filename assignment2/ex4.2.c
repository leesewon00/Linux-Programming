#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <sys/param.h>

int my_double_ls(const char *name, int flag)
{
    struct dirent *d;
    DIR *dp;
    struct stat buf;
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
                char path[PATH_MAX];
                snprintf(path, sizeof(path), "%s/%s", name, d->d_name);

                if (stat(path, &buf) == 0)
                {
                    printf("%s%s ", d->d_name, S_ISDIR(buf.st_mode) ? "*" : "");
                }
                else
                {
                    perror("stat");
                }
            }
        }
        printf("\n");
        rewinddir(dp);
    }
    closedir(dp);
    return (0);
}

int main(int argc, char *argv[])
{
    const char *dir_name = argc > 1 ? argv[1] : "./";
    int flag = 0;

    if (my_double_ls(dir_name, flag) == -1)
    {
        return 1;
    }
    return 0;
}
