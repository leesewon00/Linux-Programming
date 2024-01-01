#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
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

//// Feature #1 ////
void custom_sigchld_handler(int sig)
{
  int status;
  pid_t pid;

  // 자식 프로세스의 종료 감지 및 블로킹 방지
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    // 자식 프로세스 종료 후, pid 출력
    printf("\n[%d] DONE\n", pid); 
  }
}
//// End of Feature #1 ////

int main(int argc, char **argv)
{
  int i = 0;
  pid_t pid;

  //// Feature #1 ////
  signal(SIGCHLD, custom_sigchld_handler); // 핸들러 등록
  //// End of Feature #1 ////

  //// Feature #2 ////
  // in shell
  signal(SIGINT, SIG_IGN); // SIGINT 무시
  signal(SIGQUIT, SIG_IGN); // SIGQUIT 무시
  signal(SIGTSTP, SIG_IGN); // SIGTSTP 무시
  //// End of Feature #2 ////

  while (1)
  {
    fputs(prompt, stdout);         // display prompt "myshell> "
    fgets(cmdline, BUFSIZ, stdin); // read commands from terminal

    if (!strcmp(cmdline, "\n"))
      continue;

    cmdline[strlen(cmdline) - 1] = '\0'; // replace '\n' with null character '\0'

    int tokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);

    if (!strcmp(cmdvector[0], "exit"))
    {
      printf("Shell Terminated...\n");
      exit(0);
    }

    else if (!strcmp(cmdvector[0], "cd"))
    {
      if (chdir(cmdvector[1]) == -1)
      { // error on chdir()
        fatal("main() - chdir()");
      }
    }

    else
    {
      if (!strcmp(cmdvector[0], "ll"))
      {
        cmdvector[0] = "ls";
        cmdvector[tokens] = "-als";
      }

      int background = 0;
      if (!strcmp(cmdvector[tokens - 1], "&"))
      {
        cmdvector[tokens - 1] = NULL;
        background = 1;
      }

      switch (pid = fork())
      {
      case 0:
        //// Feature #2 ////
        // in foreground process
        signal(SIGINT, SIG_DFL); // SIGINT 디폴트 동작
        signal(SIGQUIT, SIG_DFL); // SIGQUIT 디폴트 동작
        signal(SIGTSTP, SIG_DFL); // SIGTSTP 디폴트 동작
        //// End of Feature #2 ////
        execvp(cmdvector[0], cmdvector);        
        fatal("main()");
      case -1:
        fatal("main()");
      default:
        if (background == 0)
        {
          wait(NULL);
        }
      }
    }
  }
  return 0;
}
