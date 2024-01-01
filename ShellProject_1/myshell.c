#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char *cmdvector[MAX_CMD_ARG];
char cmdline[BUFSIZ];

void fatal(char *str)
{
  perror(str);
  exit(1);
}

int makelist(char *s, const char *delimiters, char **list, int MAX_LIST)
{
  int i = 0;
  int numtokens = 0;
  char *snew = NULL;

  if ((s == NULL) || (delimiters == NULL))
    return -1;

  snew = s + strspn(s, delimiters); /* Skip delimiters */
  if ((list[numtokens] = strtok(snew, delimiters)) == NULL)
    return numtokens;

  numtokens = 1;

  while (1)
  {
    if ((list[numtokens] = strtok(NULL, delimiters)) == NULL)
      break;
    if (numtokens == (MAX_LIST - 1))
      return -1;
    numtokens++;
  }
  return numtokens;
}

int main(int argc, char **argv)
{
  int i = 0;
  pid_t pid;
  int background = 0; // flag

  while (1)
  {
    fputs(prompt, stdout);
    fgets(cmdline, BUFSIZ, stdin);
    cmdline[strlen(cmdline) - 1] = '\0';

    if (strncmp(cmdline, "cd ", 3) == 0) // cd
    {
      char *cd_path = cmdline + 3;
      if (chdir(cd_path) != 0) // use chdir
      {
        perror("cd");
      }
    }
    if (strcmp(cmdline, "exit") == 0) // exit
    {
      printf("Shell Terminated...\n");
      exit(0); // use exit
    }
    
    else
    {
      if (strncmp(cmdline, "ll", 2) == 0)
      {
        char new_cmdline[BUFSIZ];
        sprintf(new_cmdline, "ls -als %s", cmdline + 2);
        strcpy(cmdline, new_cmdline);
      }
      
      if (cmdline[strlen(cmdline) - 1] == '&')
      {
        background = 1; // background flag set
        cmdline[strlen(cmdline) - 1] = '\0';
      }

      switch (pid = fork())
      {
      case 0:
        makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
        execvp(cmdvector[0], cmdvector);
        fatal("main()");
      case -1:
        fatal("main()");
      default:
        if (background)
        {
          // if background,, do not wait!!
          background = 0; // background flag reset
        }
        else
        {
          // foreground,, wait!!
          wait(NULL);
        }
      }
    }
  }
  return 0;
}