#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define ON 1
#define OFF 0
#define PAGELEN 23
#define LINELEN 512

void DoMore(FILE*);
int SeeMore();
int SetTcFlag(int, int, FILE*);

int main(int argc, char *argv[])
{
  int fd_tty;
  FILE *fp;

  if (argc == 1)
  {
    DoMore(stdin);
  }
  else
  {
    fp = fopen(*++argv, "r");
    if (fp != NULL)
    {
      DoMore(fp);
      fclose(fp);
    }
    else
    {
      perror("Could not open file %s\n");
      exit(1);
    }
  }
}

void DoMore(FILE* fp)
{
  char line[LINELEN];
  int numLines = 0;
  int reply;

  while(fgets(line, LINELEN, fp))
  {
    if (numLines == PAGELEN)
    {
      reply = SeeMore();
      if (reply == 0)
      {
        break;
      }
      numLines -= reply;
    }
    
    if (fputs(line, stdout) == EOF)
    {
      exit(1);
    }
    numLines++;
  }
}

int SeeMore()
{
  int fd;
  char c;


  while((c = getchar()) != EOF)
  {
    if (c == 'q')
    {
      return 0;
    }
    if (c == ' ')
    {
      return PAGELEN;
    }
    if (c == '\n')
    {
      return 1;
    }
  }
  return 0;
}

int SetTcFlag(int flag, int onOff, FILE *fp)
{
  struct termios oldAttr, newAttr;
  if (tcgetattr(fileno(fp), &oldAttr) != 0)
    return -1;

  newAttr = oldAttr;
  if (onOff == ON)
  {
    newAttr.c_lflag |= ECHO;
  }
  else
  {
    newAttr.c_lflag &= ~(ICANON | ECHO);
  }

  if (tcsetattr(fileno(fp), TCSAFLUSH, &newAttr) != 0)
    return -1;
}
